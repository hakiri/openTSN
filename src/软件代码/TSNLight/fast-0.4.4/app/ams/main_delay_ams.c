/** *************************************************************************
 *  @file       main_delay_ams.c
 *  @brief		FAST架构下硬件实现精准时延测量示例演示程序
 * 
 *  该示例程序基于时延测量库实现了一个双端口环回时延测量的示例程序，程序内部构造
 *  32个报文，并设定各报文的发送时间间隔，通过硬件AMS模块从0号端口进行精准发送，
 *  报文输出后通过网络又环回到1号端口接收，在接口处对报文进行硬件级别的时间戳标记
 *  根据硬件发送状态判断报文是否按照精准时间发送，然后再利用接收到报文内携带的硬件
 *  时间戳信息进行对比计算，从而获取每个报文的传输时间进延。  
 *  Copyright  2017  XuDongLai
 * 
 *  @author		XDL
 *  @email		<XuDongLai0923@163.com>
 *  @version	0.2.0
 *  @date		2017/03/24 11:34:27 星期五
 *  @license	GNU General Public License
 *  @update		文件创建
 ****************************************************************************/
/*
 * main_delay_ams.c
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


#define MAX_PKT 32  /*单次最多提交硬件报文个数（支持背靠背发送报文功能）*/
int  pkt_count =  MAX_PKT,pkt_idx = 0;
struct fast_packet *recv_pkt[MAX_PKT] = {0};
u64 pkt_ts_result[MAX_PKT] = {0};

int pkt_len = 1000;

/**
* @brief 
*
* @param pkt
* @param pkt_len
*
* @return 
*/
int callback(struct fast_packet *pkt,int pkt_len)
{
	/*此处要增加用户逻辑，检验接收报文是否为测量返回报文*/
	/*TO DO*/
	
	recv_pkt[pkt_idx] = malloc(sizeof(struct fast_packet));
	memcpy(recv_pkt[pkt_idx],pkt,pkt_len);
	print_pkt(recv_pkt[pkt_idx],recv_pkt[pkt_idx]->um.len);/*打印接收的报文*/
	if(++pkt_idx == pkt_count)/*接收到了所有发出的测量报文，并检测本次测量成功*/
	{
		if(fase_ams_check() != 0)exit(0);
		printf("start computer!\n");
		if(fast_ams_computer(recv_pkt,&pkt_ts_result[0],pkt_count))
		{
			exit(0);/*计算结果失败*/
		}
		else
		{
			int i = 0;			
			printf("AMS ID PKT_SIZE               TS\n");
			for(;i<pkt_count;i++)
			{
				printf("%5d,    %5d,%16lld\n",i,recv_pkt[i]->um.len,pkt_ts_result[i]);
			}
			exit(0);/*测量和计算结束*/
		}
	}
	return 0;
}

/**
* @brief 
*/
void ua_init(void)
{
	int ret = 0;
	if((ret=fast_ua_init(FAST_UA_DFT_AMS,callback)))//UA模块实例化(输入参数1:接收模块ID号,输入参数2:接收报文的回调处理函数)
	{
		perror("fast_ua_init!\n");
		exit (ret);//如果初始化失败,则需要打印失败信息,并将程序结束退出!
	}
}

/**
* @brief 
*/
void rule_config_test(void)
{
	int i = 0;
	struct fast_rule rule[1] = {{0}};//初始化两条全空的规则

	/*测量报文从0号端口发送，经1号端口返回，故写规则接收1号端口报文*/
	rule[i].key.port = 0x4;
	
	rule[i].action = ACTION_SET_MID<<28|0x1;//动作字段的涵义请参考fast_type.h
	rule[i].md5[0] = 1;/*默认初始状态全为0，若不赋值无法增加*/
	
	rule[i].mask.port = 0xF;
	
	fast_add_rule(&rule[i]);	
}

/**
* @brief 
*/
void start_ams_func(void)
{
	int i = 0,count = pkt_count;
	struct fast_packet *send_pkt[MAX_PKT] = {0};
	u64 pkt_space_ts[MAX_PKT] = {0};

	for(;i<count;i++)
	{
		send_pkt[i] = malloc(sizeof(struct fast_packet));
		send_pkt[i]->um.outport = 0;
		send_pkt[i]->um.dstmid = 1;
		send_pkt[i]->um.len = pkt_len;//sizeof(struct fast_packet);
		if(i == 0)
			memset(send_pkt[i]->data,0xB,1500);
		else
			memset(send_pkt[i]->data,0xF,1500);
		pkt_space_ts[i] = 0;//时间单位为ns
	}
	
	pkt_space_ts[0] = 0;
	if(fast_ams_send(send_pkt,pkt_space_ts,count))
	{
		printf("start_ams_func failed!\n");
		exit(0);
	}
	fast_ams_start();//启动硬件发送流程
	printf("wait for ams pkts...\n");
	sleep(500);/*根据测量时间来定，等多长时间测量结束*/
	exit(0);
}

/**
* @brief 
*
* @param argc
* @param argv[]
*
* @return 
*/
int main(int argc,char* argv[])
{
	
	int ret = 0;

	if(argc > 1)
		pkt_len = atoi(argv[1]);
	
	if(fast_ams_alloc())
	{
		/*没有申请到测量权限*/
		return 0;
	}	
	
	ua_init();//UA模块初始化		
	init_rule(ACTION_SET_MID<<28|FAST_DMID_PROTO_STACK);//规则模块初始化，默认规则是送CPU的内核协议栈
	rule_config_test();//配置规则
	
	fast_ua_recv();/*先启动接收，再开始发送*/
	start_ams_func();//开始启动测量
	
	return (0);
}
