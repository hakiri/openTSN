/***************************************************************************
 *            south_io.c
 *
 *  Wed May 08 10:57:29 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * south_io.c
 *
 * Copyright (C) 2019 - michael
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
#include "south_io.h"
#include "libnet.h"

libnet_t *l = NULL;
libnet_ptag_t t;


//int poll_flag = FALSE;
//struct addr_map map = {0};


/*
static struct fast_packet *build_fast_ptp_packet(struct ptp_packet ptp)
{
	u32 pkt_len = 0;
	
	struct fast_packet *pkt = (struct fast_packet *)malloc(sizeof(struct fast_packet));
	pkt_len = sizeof(struct um_metadata) + sizeof(struct ptp_packet);
	pkt->um.pktsrc = 1;
	pkt->um.pktdst = 0;//输出带网络端口
	pkt->um.len = pkt_len;

	memcpy(pkt->data, &ptp, sizeof(struct ptp_packet));
	return pkt;
}
*/

int ptp_write_direction(u8 *sw_mac, u8 direction)
{
	#if 0
	u32 pkt_len = 0;
	u8 sw_id = 0;
	struct ptp_packet ptp;
	struct ptp_packet *recv_ptp = NULL;
	memset(&ptp, 0, sizeof(struct ptp_packet));
	struct fast_packet *pkt = NULL;

	poll_flag = FALSE;
	sw_id = get_sw_id_by_sw_mac(sw_mac);
	get_cfg_info(sw_id, &(ptp.cfg));
	memcpy(ptp.dst_mac, sw_mac, 6);
	memcpy(ptp.src_mac, get_host_mac_by_sw_id(0), 6);
	ptp.cfg.direction = direction;
	ptp.eth_type = htons(PTP_TYPE);

	ptp.msg_type = MSG_UPDATE;
	pkt = build_fast_ptp_packet(ptp);

	//地址映射
	memcpy(map.sw_mac, sw_mac, 6);
	map.pkt_buf_addr = (u32)(pkt->data);

	pkt->um.dstmid = 5;
	pkt->um.pktdst = 0;//0代表输出到网络端口
	pkt->um.pktsrc = 1;//1代表从CPU输入
	pkt->um.outport = 0;//从0端口出去

	pkt_len = pkt->um.len;

	
	pkt_print((u8 *)pkt,pkt_len);//增加打印发送报文的函数2019.6.4 ljs
	
	if(pkt_len == fast_ua_send(pkt, pkt_len))
	{
		TSN_DBG("send success\n");
	}
	else
	{
		TSN_ERR("send fail\n");
		free(pkt);
		pkt = NULL;
		return G_ERR;
	}
	
	while(poll_flag != TRUE)
	{
		usleep(20);
	}		

	recv_ptp = (struct ptp_packet *)(pkt->data);
	if(direction == recv_ptp->cfg.direction) //.........
	{
		TSN_DBG("direction success!\n");
		update_cfg_info(sw_id, recv_ptp->cfg);
		free(pkt);
		pkt = NULL;
		return G_OK;
	}
	else
	{
		TSN_ERR("direction fail!\n");
		free(pkt);
		pkt = NULL;		
		return G_ERR;
	}
	#endif
}

int ptp_write_host_mac(u8 *sw_mac, u8 *host_mac)
{
//	u32 rate = 10;
//	u32 time_slot = 0x7a12;

	char err_buf[100] = "";


	u32 pkt_len = 0;
	u8 sw_id = 0;
	
	struct ptp_packet ptp;
	struct ptp_packet *recv_ptp = NULL;
	memset(&ptp, 0, sizeof(struct ptp_packet));
	struct fast_packet *pkt = NULL;
	//poll_flag = FALSE;

	sw_id = get_sw_id_by_sw_mac(sw_mac);
	get_cfg_info(sw_id, &(ptp.cfg));//获取全局的配置信息               注释掉用于测试 2019-6.4
	memcpy(ptp.dst_mac, sw_mac, 6);
	memcpy(ptp.src_mac, get_host_mac_by_sw_id(0), 6);
	TSN_DBG("ptp.dst_macfffffffff: %x:%x:%x:%x:%x:%x\n", ptp.dst_mac[0], ptp.dst_mac[1], \
	       ptp.dst_mac[2], ptp.dst_mac[3], ptp.dst_mac[4], ptp.dst_mac[5]);
	memcpy(ptp.cfg.host_mac, host_mac, 6);
	//ptp.eth_type = htons(PTP_TYPE);
	ptp.eth_type = PTP_TYPE;
	

	
	ptp.msg_type = MSG_UPDATE;

	if((l=libnet_init(LIBNET_LINK_ADV, "enp0s3", err_buf))==NULL)
	{
		printf("2222222222\n");
		perror("LIBNET init failed!\n");
		exit(1);
	}

	t=libnet_build_ethernet
	(
		ptp.dst_mac,
		ptp.src_mac,
		ptp.eth_type,
		(u8 *)&ptp + 14,
		sizeof(struct ptp_packet),
		l,
		0
	);

	if((libnet_write(l))==-1)
	{
		perror("Send packet failed!\n");
		libnet_destroy(l);
		
	}
	else
		printf("send success!\n");
	libnet_destroy(l);
	
	return G_OK;
	//pkt = build_fast_ptp_packet(ptp);

	//地址映射
	//memcpy(map.sw_mac, sw_mac, 6);
	//map.pkt_buf_addr = (u32)(pkt->data);
	/*
	pkt_len = pkt->um.len;
	pkt->um.dstmid = 5;
	pkt->um.pktdst = 0;//0代表输出到网络端口
	pkt->um.pktsrc = 1;//1代表从CPU输入
	pkt->um.outport = 0;//从0端口出去
	*/
	//pkt_print((u8 *)pkt,pkt_len);//增加打印发送报文的函数2019.6.4 ljs
	/*
	if(pkt_len == fast_ua_send(pkt, pkt_len))
	{
		TSN_DBG("send success!!!\n");
	}
	else
	{
		TSN_ERR("send fail\n");
		free(pkt);
		pkt = NULL;
		return G_ERR;
	}
	free(pkt);
	pkt = NULL;
	return G_OK;
	*/
/*
	while(poll_flag != TRUE)
	{
		usleep(20);
	}

	recv_ptp = (struct ptp_packet *)(pkt->data);
	if(0 == memcmp(host_mac, recv_ptp->cfg.host_mac, 6))//比较相同返回0，与0相比较，2019.6.4 ljs
	{
		TSN_DBG("****************host_mac success!******************\n");

		update_cfg_info(sw_id, recv_ptp->cfg);  //测试南向接口，注释该函数
		free(pkt);
		pkt = NULL;
		return G_OK;
	}
	else
	{
		TSN_ERR("*****************host_mac fail!*********************\n");
	
		free(pkt);
		pkt = NULL;		
		return G_ERR;
	}
*/
}

int ptp_write_tb_rate(u8 *sw_mac, u32 rate)
{
	#if 0
	u32 pkt_len = 0;
	u8 sw_id = 0;
	struct ptp_packet ptp;
	struct ptp_packet *recv_ptp = NULL;
	memset(&ptp, 0, sizeof(struct ptp_packet));
	struct fast_packet *pkt = NULL;
	poll_flag = FALSE;

	sw_id = get_sw_id_by_sw_mac(sw_mac);
	get_cfg_info(sw_id, &(ptp.cfg));//获取全局的配置信息
	memcpy(ptp.dst_mac, sw_mac, 6);
	memcpy(ptp.src_mac, get_host_mac_by_sw_id(0), 6);
	ptp.cfg.tb_rate = htonl(rate);
	ptp.eth_type = htons(PTP_TYPE);
	ptp.msg_type = MSG_UPDATE;

	pkt = build_fast_ptp_packet(ptp);
	//地址映射
	memcpy(map.sw_mac, sw_mac, 6);
	map.pkt_buf_addr = (u32)(pkt->data);
	
	pkt_len = pkt->um.len;
	pkt->um.dstmid = 5;
	pkt->um.pktdst = 0;//0代表输出到网络端口
	pkt->um.pktsrc = 1;//1代表从CPU输入
	pkt->um.outport = 0;//从0端口出去
	
	pkt_print((u8 *)pkt,pkt_len);//增加打印发送报文的函数2019.6.4 ljs
	
	if(pkt_len == fast_ua_send(pkt, pkt_len))
	{
		TSN_DBG("send success\n");
	}
	else
	{
		TSN_ERR("send fail\n");
		free(pkt);
		pkt = NULL;
		return G_ERR;
	}
	//轮询是否接收到报文
	while(poll_flag != TRUE)
	{
		usleep(20);
	}
	
	recv_ptp = (struct ptp_packet *)(pkt->data);
	//如果接收到的速率与写的速率一致，说明写成功，否则写失败
	if(htonl(rate) == recv_ptp->cfg.tb_rate)
	{
		TSN_DBG("tb_rate success!\n");
		update_cfg_info(sw_id, recv_ptp->cfg);
		free(pkt);
		pkt = NULL;
		return G_OK;
	}
	else
	{
		TSN_ERR("tb_rate fail!\n");
		free(pkt);
		pkt = NULL;		
		return G_ERR;
	}
	#endif
}

int ptp_write_time_slot(u8 *sw_mac, u32 time_slot)
{
	#if 0
	u32 pkt_len = 0;
	u8 sw_id = 0;
	struct ptp_packet ptp;
	struct ptp_packet *recv_ptp = NULL;
	struct fast_packet *pkt = NULL;

	poll_flag = FALSE;
	sw_id = get_sw_id_by_sw_mac(sw_mac);
	get_cfg_info(sw_id, &(ptp.cfg));//获取全局的配置信息
	memcpy(ptp.dst_mac, sw_mac, 6);
	memcpy(ptp.src_mac, get_host_mac_by_sw_id(0), 6);
	ptp.cfg.time_slot = htonl(time_slot);
	ptp.eth_type = htons(PTP_TYPE);
	ptp.msg_type = MSG_UPDATE;
	pkt = build_fast_ptp_packet(ptp);
	//地址映射
	memcpy(map.sw_mac, sw_mac, 6);
	map.pkt_buf_addr = (u32)(pkt->data);

	pkt_len = pkt->um.len;
	pkt->um.dstmid = 5;
	pkt->um.pktdst = 0;//0代表输出到网络端口
	pkt->um.pktsrc = 1;//1代表从CPU输入
	pkt->um.outport = 0;//从0端口出去

	pkt_print((u8 *)pkt,pkt_len);//增加打印发送报文的函数2019.6.4 ljs

	
	if(pkt_len == fast_ua_send(pkt, pkt_len))
	{
		TSN_DBG("send success\n");
	}
	else
	{
		TSN_ERR("send fail\n");
		free(pkt);
		pkt = NULL;
		return G_ERR;
	}
	//轮询是否接收到报文
	while(poll_flag != TRUE)
	{
		usleep(20);
	}		

	recv_ptp = (struct ptp_packet *)(pkt->data);
	if(htonl(time_slot) == recv_ptp->cfg.time_slot)
	{
		TSN_DBG("time_slot success!\n");
		update_cfg_info(sw_id, recv_ptp->cfg);
		free(pkt);
		pkt = NULL;
		return G_OK;
	}
	else
	{
		TSN_ERR("time_slot fail!\n");
		free(pkt);
		pkt = NULL;		
		return G_ERR;
	}
	#endif
}

static void ptp_debug_info_process(struct ptp_packet *ptp)
{
	#if 0
	FILE *fp;
	u8 buf[50] = {0};
	u8 time_buf[50] = {0};
	u8 device_type = 0;
	u8 device_id = 0;
	struct tm *t;
	time_t tt;
	time(&tt);
	t = localtime(&tt);
	strftime(time_buf,50,"%Y-%m-%d %H:%M:%S",t);//把时间以字符串的形式存放在time_buf中
	
	if((fp=fopen(DEBUG_FILE,"a+")) == NULL)
	{
		TSN_ERR("cannot open debug file\n");
		return ;
	}

	sprintf(buf, "%c\n", '{');
	fwrite(buf,sizeof(char), strlen(buf),fp);
	
	sprintf(buf, "time:%s\n", time_buf);
	fwrite(buf, sizeof(u8), strlen(buf), fp);

	sprintf(buf, "seq:%d\n", ptp->seq);
	fwrite(buf, sizeof(u8), strlen(buf), fp);

	get_device_type_id_by_mac(ptp->src_mac, &device_type, &device_id);

	if(device_type == SWITCH)
	{
		sprintf(buf, "sw_id:%d\n", device_id);
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "correct_field:%lld\n", ntohll(ptp->correct_field));
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "direction:%d\n", ptp->cfg.direction);
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "flag:%d\n", ptp->flag);
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "host_mac:%x:%x:%x:%x:%x:%x\n", ptp->cfg.host_mac[0], ptp->cfg.host_mac[1], \
			    ptp->cfg.host_mac[2], ptp->cfg.host_mac[3], ptp->cfg.host_mac[4], ptp->cfg.host_mac[5]);
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "tb_rate:%d\n", ntohl(ptp->cfg.tb_rate));
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "time_slot:%d\n", ntohl(ptp->cfg.time_slot));
		fwrite(buf, sizeof(u8), strlen(buf), fp);
		
		sprintf(buf, "esw_pkt_in_cnt:%llu\n", ntohll(ptp->status.esw_pkt_in_cnt));
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "esw_pkt_out_cnt:%llu\n", ntohll(ptp->status.esw_pkt_out_cnt));
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "local_mac_addr:%d\n", ptp->status.local_mac_addr);
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "buf_id_used_cnt:%d\n", ptp->status.buf_id_rest_cnt);
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "eos_md_in_cnt:%llu\n", ntohll(ptp->status.eos_md_in_cnt));
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "eos_md_out_cnt:%llu\n", ntohll(ptp->status.eos_md_out_cnt));
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "eos_q0_used_cnt:%d\n", ptp->status.eos_q0_used_cnt);
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "eos_q1_used_cnt:%d\n", ptp->status.eos_q1_used_cnt);
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "eos_q2_used_cnt:%d\n", ptp->status.eos_q2_used_cnt);
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "eos_q3_used_cnt:%d\n", ptp->status.eos_q3_used_cnt);
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "goe_pkt_in_cnt:%llu\n", ntohll(ptp->status.goe_pkt_in_cnt));
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "goe_port0_out_cnt:%llu\n", ntohll(ptp->status.goe_port0_out_cnt));
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "goe_port1_out_cnt:%llu\n", ntohll(ptp->status.goe_port1_out_cnt));
		fwrite(buf, sizeof(u8), strlen(buf), fp);

		sprintf(buf, "goe_discard_cnt:%llu\n", ntohll(ptp->status.goe_discard_cnt));
		fwrite(buf, sizeof(u8), strlen(buf), fp);
	}
	else
	{
		sprintf(buf, "host_id:%d\n", device_id);
		fwrite(buf, sizeof(u8), strlen(buf), fp);
	}

	sprintf(buf, "%c\n", '}');
	fwrite(buf, sizeof(char), strlen(buf), fp);

/*
	TSN_DBG("esw_pkt_in_cnt:%llu\n", ntohll(ptp->status.esw_pkt_in_cnt));
	TSN_DBG("esw_pkt_out_cnt:%llu\n", ntohll(ptp->status.esw_pkt_out_cnt));
	TSN_DBG("local_mac_addr:%d\n", ptp->status.local_mac_addr);
	TSN_DBG("buf_id_used_cnt:%d\n", ptp->status.buf_id_rest_cnt);
	TSN_DBG("eos_md_in_cnt:%llu\n", ntohll(ptp->status.eos_md_in_cnt));
	TSN_DBG("eos_md_out_cnt:%llu\n", ntohll(ptp->status.eos_md_out_cnt));
	TSN_DBG("eos_q0_used_cnt:%d\n", ptp->status.eos_q0_used_cnt);
	TSN_DBG("eos_q1_used_cnt:%d\n", ptp->status.eos_q1_used_cnt);
	TSN_DBG("eos_q2_used_cnt:%d\n", ptp->status.eos_q2_used_cnt);
	TSN_DBG("eos_q3_used_cnt:%d\n", ptp->status.eos_q3_used_cnt);
	TSN_DBG( "goe_pkt_in_cnt:%llu\n", ntohll(ptp->status.goe_pkt_in_cnt));
	TSN_DBG("goe_port0_out_cnt:%llu\n", ntohll(ptp->status.goe_port0_out_cnt));
	TSN_DBG("goe_port1_out_cnt:%llu\n", ntohll(ptp->status.goe_port1_out_cnt));
	TSN_DBG("goe_discard_cnt:%llu\n", ntohll(ptp->status.goe_discard_cnt));
*/
	


	fclose(fp);
#endif
}

static void ptp_write_reply_process(struct ptp_packet *ptp)
{
	#if 0
	struct ptp_packet *tmp_ptp;
	//tmp_ptp指向发送线程中的报文地址
	tmp_ptp = (struct ptp_packet *)(map.pkt_buf_addr);
	//把发送线程中的报文配置信息的内容该为接收到配置信息的内容
	memcpy(&(tmp_ptp->cfg), &(ptp->cfg), sizeof(struct config_info));
	TSN_DBG("direction: %d\n",tmp_ptp->cfg.direction);
	TSN_DBG("host_mac: %x:%x:%x:%x:%x:%x\n",tmp_ptp->cfg.host_mac[0],tmp_ptp->cfg.host_mac[1],tmp_ptp->cfg.host_mac[2],
											 tmp_ptp->cfg.host_mac[3],tmp_ptp->cfg.host_mac[4],tmp_ptp->cfg.host_mac[5]);
	TSN_DBG("tb_rate: %x\n",ntohl(tmp_ptp->cfg.tb_rate));
	TSN_DBG("time_slot: %x\n",ntohl(tmp_ptp->cfg.time_slot));
	#endif
	
}
#if 0
static int tsn_fast_packet_process(struct fast_packet *pkt, int pkt_len)
{

	pkt_print((u8 *)pkt,pkt_len);//增加打印发送报文的函数2019.6.4 ljs
	//回调函数 判断是否为PTP报文
	struct ptp_packet *ptp = NULL;
	ptp = (struct ptp_packet *)pkt->data;
	if(ptp->eth_type == htons(PTP_TYPE))
		TSN_DBG("this is PTP packet\n");
	else
	{
		TSN_ERR("this is not PTP packet\n");
		return G_ERR;
	}
	//把PTP报文写入文本
	ptp_debug_info_process(ptp);

	ptp_packet_state_process(ptp, ((u64)pkt->um.reserve << 32) + pkt->um.ts);
	//TSN_DBG("pkt->um.reserve %x\n",pkt->um.reserve);
	//TSN_DBG("pkt->um.ts %x\n",pkt->um.ts);
	TSN_DBG("(pkt->um.reserve << 32) + pkt->um.ts %llx\n",((u64)pkt->um.reserve << 32) + pkt->um.ts);

	
	if(ptp->msg_type == MSG_REPORT)
	{
		TSN_DBG("this is report\n");
		ptp_write_reply_process(ptp);
		poll_flag = TRUE;
	}
	
	return G_OK;

}
#endif
//设置UA回调函数
static void south_ua_init(void)
{
	#if 0
	int ret = 0;
	if((ret=fast_ua_init(TSN_CTRL_MID, tsn_fast_packet_process)))//UA模块实例化(输入参数1:接收模块ID号,输入参数2:接收报文的回调处理函数)
	{
		perror("fast_ua_init!\n");
		exit (ret);//如果初始化失败,则需要打印失败信息,并将程序结束退出!
	}
	#endif
}

static int config_all_sw_host_mac()
{
	#if 0
	int sw_num = 0;
	int i = 0;
	int ret = G_OK;
	u8 *sw_mac = NULL, *host_mac = NULL;
	
	sw_num = get_sw_num();
	for(i = 0; i < sw_num; i++)
	{
		sw_mac = get_sw_mac_by_sw_id(i);
		host_mac = get_host_mac_by_sw_id(i);
		ret = ptp_write_host_mac(sw_mac, host_mac);
		if(ret == G_ERR)
		{
			TSN_ERR("Config sw[%d] host mac error!\n", i);
			return G_ERR;
		}
	}
	return G_OK;
	#endif
}

void init_south_rule()
{
	#if 0
	struct fast_rule rule;
	memset(&rule,0,sizeof(struct fast_rule));
	
	rule.key.dmac[0] = 0x06; 
	rule.key.dmac[1] = 0x05; 
	rule.key.dmac[2] = 0x04; 
	rule.key.dmac[3] = 0x03; 
	rule.key.dmac[4] = 0x02; 
	rule.key.dmac[5] = 0x01; 

	rule.mask.dmac[0] = 0xff; 
	rule.mask.dmac[1] = 0xff; 
	rule.mask.dmac[2] = 0xff; 
	rule.mask.dmac[3] = 0xff; 
	rule.mask.dmac[4] = 0xff; 
	rule.mask.dmac[5] = 0xff; 

	rule.key.type  = PTP_TYPE;
	rule.mask.type = 0xffff;
	rule.action = ACTION_SET_MID<<28|150;
	
//	init_rule(fast_reg_rd(FAST_ACTION_REG_ADDR|FAST_DEFAULT_RULE_ADDR));//初始化规则
	fast_add_rule(&rule);//增加规则
	#endif
}


int init_south_io()
{
	int ret = G_OK;
	//fast_init_hw(0,0);
	//south_ua_init();
	/*配置硬件规则，将硬件所有报文送到模块ID为1的进程处理*/
	//fast_reg_wr(FAST_ACTION_REG_ADDR|FAST_DEFAULT_RULE_ADDR, ACTION_SET_MID<<28|TSN_CTRL_MID);

	//配置两条规则，第一条为PTP组播报文送UA，组播地址0x010203040506；第二条为默认规则，丢弃
	//init_south_rule();//增加初始化规则 2019.6.4 ljs
	
	//fast_ua_recv();
	//主进程进入循环休眠中,数据处理主要在回调函数

	//ret = config_all_sw_host_mac();
	return ret;
}