/** *************************************************************************
 *  @file          main_libua.c
 *  @brief	  FAST平台用户应用（UA）编程支持库
 * 
 *  FAST平台可以支持多用户UA同时编程操作，每个不同用户UA可以对硬件进行不同流表配置，
 *  UA根据自己业务特点从硬件提取自己感兴趣的特征流进行处理，处理完成后可以丢弃或转发。
 * 转发操作既可以支持发送到硬件设备不同端口，也可以转发到其他注册的UA程序。\n
 * UA程序在注册时，要配合流表进行，将自己的特征流打上相应模块ID的标记，并向内核注册
 * 自己进程有处理此模块ID的报文的能力。内核接收到不同MID值的报文，根据不同用户UA的注册
 * 进行，通过NetLink的方式转发给不同的UA程序。
 *  @date		2017/01/05 13:20:17 星期四
 *  @author		XDL(Copyright  2017  XuDongLai)
 *  @email		<XuDongLai0923@163.com>
 *  @verion		0.2.0
 ****************************************************************************/
/*
 * main_libua.c
 *
 * Copyright (C) 2017 - XuDongLai
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../include/fast.h"

#define FAST_UA_FILE 1

int recv_poll = 1;
int nlsk_recv = -1;
int cur_mid = -1;
fast_ua_recv_callback ua_recv_callback = NULL;
struct msghdr netlink_msg;
struct nlmsghdr *nlh = NULL;
struct fast_ua_kernel_msg register_ua;
struct fast_ua_kernel_msg unregister_ua;
struct iovec iov;
struct sockaddr_nl src_addr, dest_addr;	
int uaf = 0;
void fast_ua_destroy(void);
int seq = 0;
/**
* @brief UA程序非正常退出时的最后工作
*
* 程序非正常退出时一定要进行清理工作(注销此进程的UA注册消息)
* 否则，内核依然给此进程的PID发送消息
* @param 中断信号（捕获Ctrl + C操作）
*
*/
void ua_improper_quit(int argc)
{
	recv_poll = 0;
	nlh->nlmsg_len = FAST_UA_REG_LEN;
	/*构建UA的注销报文*/
	memcpy(NLMSG_DATA(nlh),(char *)&unregister_ua,FAST_UA_REG_LEN);
	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;
	dest_addr.nl_family = AF_NETLINK;
	netlink_msg.msg_name = (void *)&dest_addr;
	netlink_msg.msg_namelen = sizeof(dest_addr);
	netlink_msg.msg_iov = &iov;
	netlink_msg.msg_iovlen = 1;

	/*向系统发送注销信息*/
	sendmsg(nlsk_recv, &netlink_msg, 0);	
	close(nlsk_recv);
	printf("fast_ua_destroy\n");
	exit(0);//结束程序运行
}

/**
* @brief UA向内核注册操作函数
*
* @param mid	UA程序注册的模块ID号
*
* @return   0：表示UA注册成功\n
 *			-E_UA_NLSK_CREATE：创建NetLink类型SOCKET失败\n
 *			-E_UA_NLSK_BIND：绑定NetLink失败\n
 *			-E_UA_NLSK_REG_ERR：注册UA失败
*/
/*
 * FAST的UA进程向内核发起注册请求，并等待注册回应 
 * 消息，如果回应消息为注册成功，则设置信号处理并返回成功 
 * 否则返回失败
 */
static int fast_ua_register(int mid)
{

	int rc = 0,cur_pid = 0;
	
	cur_pid = getpid();
	//创建NetLink类型SOCKET
	nlsk_recv = socket(PF_NETLINK, SOCK_RAW, FAST_UA_NETLINK);
	if(nlsk_recv < 0)
	{
		FAST_ERR("Create FAST NetLink Socket Error!\n");
		return -E_UA_NLSK_CREATE;
	}
	//初始化数据为零
	memset(&netlink_msg, 0, sizeof(netlink_msg));
	memset(&src_addr, 0, sizeof(src_addr));
	//设置通信源端节点信息
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = cur_pid; 
	src_addr.nl_groups = 0;
	//SOCKET绑定本地节点
	rc = bind(nlsk_recv, (struct sockaddr*)&src_addr, sizeof(src_addr));
	if(rc < 0)
	{
		FAST_ERR("Bind FAST NetLink Socket Error!\n");
		return -E_UA_NLSK_BIND;
	}
	//设置通信目的节点信息，pid为0表示目的为：内核
	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0;
	dest_addr.nl_groups = 0;
	//初始化通信消息
	nlh=(struct nlmsghdr *)malloc(NLMSG_SPACE(FAST_UA_PKT_MAX_LEN));
	nlh->nlmsg_len = FAST_UA_REG_LEN;
	nlh->nlmsg_pid = cur_pid;
	nlh->nlmsg_flags = 0;
	//初始化通信注册数据(通信消息载体)
	register_ua.mid = mid;
	register_ua.pid = getpid();
	register_ua.state = UA_REG;
	//注销数据
	unregister_ua.mid = mid;
	unregister_ua.pid = register_ua.pid;
	unregister_ua.state = UA_UNREG;

	//设置通信参数
	memcpy(NLMSG_DATA(nlh),(char *)&register_ua,sizeof(struct fast_ua_kernel_msg));
	memset(&register_ua,0,sizeof(struct fast_ua_kernel_msg));
	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;
	netlink_msg.msg_name = (void *)&dest_addr;
	netlink_msg.msg_namelen = sizeof(dest_addr);
	netlink_msg.msg_iov = &iov;
	netlink_msg.msg_iovlen = 1;

	//向内核发送NetLink消息，消息内容为注册数据结构
	sendmsg(nlsk_recv, &netlink_msg, 0);
	FAST_DBG("Register UA to FAST Kernel! Wait Reply......\n");
	
	//清空数据后准备接收
	memset(nlh, 0, NLMSG_SPACE(FAST_UA_PKT_MAX_LEN));
	//等待内核返回消息
	rc = recvmsg(nlsk_recv, &netlink_msg, 0);
	memcpy((char *)&register_ua,NLMSG_DATA(nlh),sizeof(register_ua));//FAST_UA_REG_LEN
	//判断返回消息是否注册成功
	if(register_ua.mid == mid && register_ua.pid == cur_pid && register_ua.state == UA_OK)//返回2说明注册成功
	{		
		FAST_DBG("UA->pid:%d,mid:%d,Register OK!\n",register_ua.pid,register_ua.mid);
		signal(SIGTERM,ua_improper_quit);//非法结束时，调用注销函数
		signal(SIGKILL,ua_improper_quit);//非法结束时，调用注销函数
		signal(SIGTSTP,ua_improper_quit);//非法结束时，调用注销函数
		signal(SIGINT,ua_improper_quit);//非法结束时，调用注销函数
		cur_mid = mid;
		return 0;
	}
	else
	{
		FAST_DBG("Register ERROR!return->pid:%d,mid:%d,state:%d\n",register_ua.pid,register_ua.mid,register_ua.state);
		return -E_UA_NLSK_REG_ERR;
	}	
}

/**
* @brief UA发送报文操作
*
* @param pkt		：发送的报文指针
* @param pkt_len	：发送的报文长度
*
* @return   -1：表示发送失败\n
 *			返回报文长度：表示发送成功
*/
/*
 * 提供给UA程序向内核发送消息，此消息可以是转发给其他UA进程，也可以是直接转发至硬件
 * 其中UM中的dstmid的取值为[1-10]时表示转发到其他UA进程，[11-254]表示转发到硬件，取值范围可再商定
*/
int fast_ua_send(struct fast_packet *pkt,int pkt_len)
{
	if(pkt->um.len != pkt_len)	
	{
		printf("ERROR:um->len:%d,pkt_len:%d\n",pkt->um.len,pkt_len);
		exit(0);
	}	
	pkt->um.srcmid = cur_mid;
	pkt->md.pkttype = 0;
#ifdef FAST_UA_FILE
	return write(uaf,(char *)pkt,pkt_len);
#else
	memcpy(NLMSG_DATA(nlh),pkt,pkt_len);
	nlh->nlmsg_len = pkt_len + 16;//除数据长度外再加上消息头长度
	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;
	dest_addr.nl_family = AF_NETLINK;
	netlink_msg.msg_name = (void *)&dest_addr;
	netlink_msg.msg_namelen = sizeof(dest_addr);
	netlink_msg.msg_iov = &iov;
	netlink_msg.msg_iovlen = 1;

	/*检验发送长度是否与报文长度一致,不同表示发送失败*/
	if(sendmsg(nlsk_recv, &netlink_msg, 0) != nlh->nlmsg_len)/*要与对齐后的长度相比*/
	{
		printf("Send len ERROR!\n");
		return -1;
	}
	return pkt_len;	
#endif	
}


int fast_cm_send(struct fast_packet *pkt,int pkt_len)
{		
#ifdef FAST_UA_FILE
	return write(uaf,(char *)pkt,pkt_len);
#else
	memcpy(NLMSG_DATA(nlh),pkt,pkt_len);
	nlh->nlmsg_len = pkt_len + 16;//除数据长度外再加上消息头长度
	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;
	dest_addr.nl_family = AF_NETLINK;
	netlink_msg.msg_name = (void *)&dest_addr;
	netlink_msg.msg_namelen = sizeof(dest_addr);
	netlink_msg.msg_iov = &iov;
	netlink_msg.msg_iovlen = 1;

	/*检验发送长度是否与报文长度一致,不同表示发送失败*/
	if(sendmsg(nlsk_recv, &netlink_msg, 0) != nlh->nlmsg_len)/*要与对齐后的长度相比*/
	{
		printf("Send len ERROR!\n");
		return -1;
	}
	return pkt_len;	
#endif	
}

u32 fast_ua_hw_rd(u8 dmid,u32 addr,u32 mask)
{
	u32 i = 0;
	u32 data = 0;
	struct fast_packet *pkt = (struct fast_packet *)malloc(sizeof(struct ctl_metadata));
	
	pkt->cm.pkttype = 1;/*控制报文*/
	pkt->cm.type = 1;
	pkt->cm.seq = seq++;
	pkt->cm.srcmid = cur_mid;
	pkt->cm.dstmid = dmid;
	pkt->cm.addr = addr;
	pkt->cm.data = 0x0923;
	pkt->cm.mask = mask;
	pkt->cm.reserve = 1;
	pkt->cm.sessionID = pkt;
	if(fast_cm_send(pkt,sizeof(struct ctl_metadata)) != sizeof(struct ctl_metadata))
	{
		printf("fast_um_rd send pkt Err!\n");
	}
	while(recv_poll && pkt->cm.reserve == 1)
	{
		if(i>10)
		{
			printf("ERR:fast_ua_hw_rd timeout\n");
			return -1;
		}
		usleep(20);
		i++;
	}		
	data = pkt->cm.data;
	free(pkt);	
	return data;
}

void fast_ua_hw_wr(u8 dmid,u32 addr,u32 value,u32 mask)
{
	struct fast_packet pkt;
	pkt.cm.pkttype = 1;/*控制报文*/
	pkt.cm.type = 2;
	pkt.cm.seq = seq++;
	pkt.cm.srcmid = cur_mid;
	pkt.cm.dstmid = dmid;
	pkt.cm.addr = addr;
	pkt.cm.data = value;
	pkt.cm.mask = mask;

	if(fast_cm_send(&pkt,sizeof(struct ctl_metadata)) != sizeof(struct ctl_metadata))
	{
		printf("fast_um_wr send pkt Err!\n");
	}
}

/**
* @brief 打开UA处理文件
*
 * 另一种发送UA报文的方法，通过proc文件进行写操作
* @return   0：表示成功\n
 *			否则退出程序
*/
int fast_ua_file(u8 mid)
{
	char ua_path[256] = {0};

	sprintf(ua_path,"/proc/openbox/ua_%d",mid);
	if((uaf=open(ua_path,O_RDWR,0)) == -1)
	{
		printf("open UA file Err!\n");
		exit (-1);
	}
	if(lseek(uaf,mid,SEEK_SET) != mid)
	{
		printf("Set Mid Err!\n");
		exit(-1);
	}
	return 0;
}

/**
* @brief UA初始化函数
*
* @param mid		：UA注册的模块ID号
* @param callback   ：处理此模块ID号报文的回调函数
*
* @return   0：表示注册成功\n
 *			其他：表示失败
*/
/*
 * FAST的UA程序初始化调用函数，需要UA程序提供两个参数
 * 参数一表示接收目的MID为此值的报文
 * 参数二表示接收到报文后通过此回调函数输出
 */
int fast_ua_init(int mid,fast_ua_recv_callback callback)
{
	int err = 0;

	fast_init_hw(0,0);
	
	/*向系统注册自己要接收报文的模块ID号*/
	if((err = fast_ua_register(mid)))
	{
		return err;
	}
	/*保存用户回调函数的指针信息*/
	ua_recv_callback = callback;
#ifdef FAST_UA_FILE
	fast_ua_file(mid);
#endif
	FAST_DBG("libua version:%s\n",LIBUA_VERSION);
	return 0;
}

/**
* @brief UA注销操作
*
* UA程序正常或非法退出时执行的操作，向内核告知自己将要结束，让内核清除此MID号上的报文转发功能
*/
void fast_ua_destroy(void)
{
	ua_improper_quit(0);
}


void ua_setaffinity(unsigned int core)
{
	cpu_set_t cpuset;
	pthread_t thread = pthread_self();
	//CPU_ZERO(&cpuset);
	memset(&cpuset,0,sizeof(cpu_set_t));
	//CPU_SET(core,&cpuset);
	memset(&cpuset,0x2,1);
	if(pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset) !=0)
	{
		printf("ua_setaffinity on core:%d ERR!!!\n",core);
	}
}


/**
* @brief 启动UA接收内核转发报文线程操作
*
* 循环接收内核分派报文，并调用用户回调函数进行输出\n
* UA接收到内核转发报文后，立即回调用户函数进行处理，用户必须快速处理完此报文，
* 将程序运行权限返回给该线程内部，这样才可以尽快再次接收报文回调用户处理
* @wraning 若用户在回调函数中处理流程过长，会导致该线程接收内核报文的队列满，
* 从而导致内核转发报文时发生丢弃。若用户可较长时间处理此报文，建议申请新的空间
* 复制此报文内容后再做处理。回调函数应尽可能减少操作开销
* @param argv   ：空参数，暂无使用
*
*/
void recv_thread(void *argv)
{
	struct fast_packet *pkt = (struct fast_packet *)malloc(FAST_UA_PKT_MAX_LEN);
	int rc = 0;	
	
	/*超时设置机制1
	int flag = fcntl(nlsk_recv,F_GETFL,0);
	fcntl(nlsk_recv,F_SETFL,flag|O_NONBLOCK);//非阻塞式，如果没有得到数据就返回，rc = -1
	*/
	/*超时设置机制2
	struct timeval timeout ={0,0};
	setsockopt(nlsk_recv,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(struct timeval));
	*/
	if(ua_recv_callback == NULL)
	{
		printf("callback is NULL!\n");
		exit(-1);
	}
	ua_setaffinity(1);
	FAST_DBG("fast_ua_recv......\n");
	while(recv_poll == 1)
	{
		//memset(pkt, 0, FAST_UA_PKT_MAX_LEN);
		/*阻塞式接收内核分派报文*/
#ifdef FAST_UA_FILE
		rc = read(uaf,pkt,FAST_UA_PKT_MAX_LEN);
#else
		rc = recv(nlsk_recv,pkt,FAST_UA_PKT_MAX_LEN, 0);
#endif
		if(rc >0)
		{			
			if(pkt->md.pkttype == 0)
			{
				ua_recv_callback(pkt,rc);
			}
			else
			{
				struct fast_packet *ctl_pkt = (struct fast_packet *)pkt->cm.sessionID;				
				ctl_pkt->cm.data = pkt->cm.data;
				ctl_pkt->cm.reserve = 0;
				
			}
		}
		//sleep(1);
	}
}

/**
* @brief 启动UA接收线程（用户代码继续执行）
*
* UA程序启动数据接收函数，此函数执行后，后继代码继续往下执行
*/
void fast_ua_recv()//可以提供类似SOCKET接收函数，直接带数据返回的
{
	int ret = 0;
	pthread_t p_t;
	void *tret;
	/*创建接收报文线程*/
	ret = pthread_create(&p_t,NULL,(void *)recv_thread,NULL);
	if(ret != 0)
	{
		FAST_ERR("fast_ua_recv error!\n");
	}
	//pthread_join(p_t,&tret);
}


/**
* @brief 启动UA接收线程（后续用户代码不执行）
*
* UA程序启动数据接收函数，此函数执行后，代码暂停在函数结束处
*/
void fast_ua_recvhold()//可以提供类似SOCKET接收函数，直接带数据返回的
{
	int ret = 0;
	pthread_t p_t;
	void *tret;
	ret = pthread_create(&p_t,NULL,(void *)recv_thread,NULL);
	if(ret != 0)
	{
		FAST_ERR("fast_ua_recv error!\n");
	}
	pthread_join(p_t,&tret);
}

/**
* @brief 打印FAST结构报文数据
*
* @param pkt		：FAST结构报文指针
* @param pkt_len	：打印报文长度
 * @warning 打印报文长度要小于等于FAST指针的有效数据长度
*/
/*打印FAST结构报文数据*/
void print_pkt(struct fast_packet *pkt,int pkt_len)
{
	int i=0;
	printf("-----------------------***FAST PACKET***-----------------------\n");
	printf("Packet Addr:%p\n",pkt);
	for(i=0;i<16;i++)
	{
		if(i % 16 == 0)
			printf("      ");
		printf(" %X ",i);
		if(i % 16 == 15)
			printf("\n");
	}
	
	for(i=0;i<pkt_len;i++)
	{
		if(i % 16 == 0)
			printf("%04X: ",i);
		printf("%02X ",*((u8 *)pkt+i));
		if(i % 16 == 15)
			printf("\n");
	}
	if(pkt_len % 16 !=0)
		printf("\n");
	printf("-----------------------***FAST PACKET***-----------------------\n\n");
}
