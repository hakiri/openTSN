/*
 * main.c
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
#include "../lib/libreserve/reserve_mgr.h"
#include "../lib/libsouthio/south_io.h"
#include "../lib/libnorthio/north_io.h"
#include "../lib/libsched/global_sched.h"
#include "../lib/libstate/state_mgr.h"



//定义了两种不同的APP类型，每次只能选择一种APP，默认为TSN_APP_NA，如果要编译成为TSN_APP_CNC，需要屏蔽#undef TSN_APP_CNC，和打开#undef TSN_APP_NA
//#define TSN_APP_CNC 1
//#undef TSN_APP_CNC

//#define TSN_APP_NA 1
//#undef TSN_APP_NA


#if TSN_APP_NA
static int port0_fun(struct fast_packet *pkt, int pkt_len)
{
	struct state_node *node;
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
	//ptp_debug_info_process(ptp);
	node = ptp_packet_state_process(ptp, ((u64)pkt->um.reserve << 32) + pkt->um.ts);

	//memcpy(&node,ptp_packet_state_process(ptp, ((u64)pkt->um.reserve << 32) + pkt->um.ts),sizeof(struct state_node));
	if(node->cur_sw_num == get_sw_num())
	{
		TSN_DBG("cur sw num is equal to node num: start to compute sync info\n");
		build_and_send_sync_info(node);
		build_and_send_cnt_info(node);
		delete_node_from_state_list(node);
	}
	return G_OK;

}

static int tsn_fast_packet_process(struct fast_packet *pkt, int pkt_len)
{
	//printf("port0 recv packet!\n");
	u64 time_used = 0;
	struct timeval start;
	struct timeval end;
	gettimeofday(&start,NULL);
	
	int tmp_port = 0;
	tmp_port = pkt->um.inport;
	switch(tmp_port)
	{
		case 0:port0_fun(pkt,pkt_len);break;
		case 1:printf("port0 recv packet!\n");break;
		case 2:printf("port2 recv packet!\n");break;
		default:printf("error\n");break;
	}
	gettimeofday(&end,NULL);

	return G_OK;

}

#endif

int main(int argc,char *argv[])
{
#if TSN_APP_NA
	u8 dmac[6]={0x06,0x05,0x04,0x03,0x02,0x01};
	u8 dmac_mask[6]={0xff,0xff,0xff,0xff,0xff,0xff};
	u16 type = PTP_TYPE; 
	u8 port  = 0;

	struct global_reserve_table *tbl = NULL;
//	pthread_t pt;

	init_north_io();
	init_topology();
	//print_topology();
	init_global_resource();
	//print_global_resource();
	init_state_list();
//	pt = tsn_create_timer(state_list_process, 1);
	tbl = init_global_reserve_table();

	//init_south_io();


	fast_init_hw(0,0);
	fast_ua_init(TSN_CTRL_MID, tsn_fast_packet_process);
	init_south_rule(dmac,dmac_mask,type,port);//增加初始化规则 2019.6.4 ljs
	fast_ua_recv();

	
	TSN_DBG("****************init south io success!******************\n");
	//global_flow_resource_sched(tbl);
	
//	pthread_join(pt, NULL);
	while(1){sleep(9999);}
	return G_OK;
#endif

#if TSN_APP_CNC
	struct ptp_packet ptp;
	u8  tmp_sw        = 0;
	u8  tmp_direction = 0;

	u16 tmp_tb_B   = 0;
	u16 tmp_tb_rate   = 0;
	
	u32 tmp_time_slot = 0;
	u8  ret = 0;
	

	if(argc == 2)
	{
		init_topology();
		//print_topology();
		init_global_resource();
		//print_global_resource();
		if(strcmp(argv[1],"all")==0)
			config_all_sw_default();
		else
		{
			tmp_sw = atoi(argv[1]);
			config_sw_default(tmp_sw);
		}
	}
	else if(argc == 6)
	{
		init_topology();
		init_global_resource();
	
		tmp_sw        = atoi(argv[1]);
		tmp_direction = atoi(argv[2]);
		tmp_tb_B      = atoi(argv[3]);
		tmp_tb_rate   = atoi(argv[4]);
		tmp_time_slot = atoi(argv[5]);
		get_cfg_info(tmp_sw, &(ptp.cfg));//获取全局的配置信息
		ptp_write_general_para(&ptp);
		ptp_write_host_mac(&ptp,get_host_mac_by_sw_id(tmp_sw));
		ptp_write_direction(&ptp,tmp_direction);
		ptp_write_tb_rate(&ptp,tmp_tb_rate);
		ptp_write_tb_B(&ptp,tmp_tb_B);
		ptp_write_time_slot(&ptp,tmp_time_slot);
		ptp_write_dst_mac(&ptp, tmp_sw);
		send_south_pkt(ptp);
	}
	else
	{
		printf("./tsn_cnc all\n");
		printf("./tsn_CNC sw_id\nor \n./tsn_CNC sw_id direction tb_B tb_rate time_slot\n");
		printf("defaul: direction = 0,tb_B = 2048 tb_rate = 100,time_slot = 14\n");

		return 0;
	}
	return 0;
#endif



}
