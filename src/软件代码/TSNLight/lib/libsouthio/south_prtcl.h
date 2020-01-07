/***************************************************************************
 *            south_prtcl.h
 *
 *  Wed May 08 10:30:41 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * south_prtcl.h
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
#ifndef __SOUTH_PRTCL_H__
#define __SOUTH_PRTCL_H__

#include "../../include/tsn_public.h"

#define PTP_TYPE 0x88F7

#define MSG_REPORT 0xE //响应的报文
#define MSG_UPDATE 0xF //主动上报的报文和软件主动下发的PTP报文




typedef enum
{
	TB_RATE_TYPE = 0,
	HOST_MAC_TYPE = 1,
	DIRECTION_TYPE = 2
}CFG_TYPE;

typedef enum
{
	LEFT = 0,
	RIGHT = 1
}DIRECTION_ENUM;


#if TOPOLOGY_TYPE==0

struct config_info
{
	u8	host_mac[6];//主机的mac地址
	u8	pad:7,
		direction:1;//方向，0代表1口进，0口出，1则相反
	u8	pad1;
	u16 tb_B;//把令牌桶的速率分为桶深和速率，高16位代表桶深，低十六位代表速率
	u16	tb_rate;
	u32	time_slot;//时间槽的大小，2的次方拍，没拍为8ns
}__attribute__((packed));


struct status_info
{
	u64 esw_pkt_in_cnt;//进入ESW模块的分组计数器
	u64 esw_pkt_out_cnt;//ESW输出模块的分组计数器
	u8	local_mac_addr;//TSN本地MAC地址，只有8位软件可读
	u8	buf_id_rest_cnt;//Bufm中所使用的ID计数器
	u8  pad0[14];
	u64 eos_md_in_cnt;//进入EOS模块的元数据计数器
	u64 eos_md_out_cnt;//EOS模块输出的元数据计数器
	u8	eos_q0_used_cnt;//EOS模块Q0队列的剩余长度计数器,tsn队列
	u8	eos_q1_used_cnt;//EOS模块Q1队列的剩余长度计数器,tsn队列
	u8	eos_q2_used_cnt;//EOS模块Q2队列的剩余长度计数器,带宽预约队列
	u8	eos_q3_used_cnt;//EOS模块Q3队列的剩余长度计数器,尽力转发队列
	u8  pad2[12];
	u64 goe_pkt_in_cnt;//进入GOE模块的分组计数器
	u64 goe_port0_out_cnt;//GOE模块往0口输出的分组计数器
	u64 goe_port1_out_cnt;//GOE模块往1口输出的分组计数器
	u64 goe_discard_cnt;//GOE模块丢弃的分组计数器
}__attribute__((packed));

#elif TOPOLOGY_TYPE==1

struct config_info
{
	u8	host_mac[6];
	u8	pad0:7,
		direction:1;
	u8	reg_TAP:1,
		pad1:7;//修改2019.10.09--ljs TAP口只有一位
	u16 tb_B;//把令牌桶的速率分为桶深和速率，高16位代表桶深，低十六位代表速率
	u16	tb_rate;
	u32	time_slot;
	//修改修改2019.9.17--ljs
	u8	mac0[6];
	u16	port_0;
	u8	mac1[6];
	u16	port_1;
	u8	mac2[6];
	u16	port_2;
	u8	mac3[6];
	u16	port_3;
	u8	mac4[6];
	u16	port_4;
	u8	mac5[6];
	u16	port_5;
	u8	mac6[6];
	u16	port_6;
	u8	mac7[6];
	u16	port_7;

	u8	mac8[6];
	u16	port_8;
	u8	mac9[6];
	u16	port_9;
	u8	mac10[6];
	u16	port_10;
	u8	mac11[6];
	u16	port_11;
	
}__attribute__((packed));

struct status_info
{

	u8 eos_q0_used_cnt_0;
	u8 eos_q1_used_cnt_0;
	u8 eos_q2_used_cnt_0;
	u8 eos_q3_used_cnt_0;
	
	u8 eos_q0_used_cnt_1;
	u8 eos_q1_used_cnt_1;
	u8 eos_q2_used_cnt_1;
	u8 eos_q3_used_cnt_1;

	u8 eos_q0_used_cnt_2;
	u8 eos_q1_used_cnt_2;
	u8 eos_q2_used_cnt_2;
	u8 eos_q3_used_cnt_2;

	u8 eos_q0_used_cnt_3;
	u8 eos_q1_used_cnt_3;
	u8 eos_q2_used_cnt_3;
	u8 eos_q3_used_cnt_3;

	u32 pad0;
	u8	buf_id_rest_cnt_0;//每个端口有一个缓存区
	u8	buf_id_rest_cnt_1;
	u8	buf_id_rest_cnt_2;
	u8	buf_id_rest_cnt_3;

	u64 esw_pkt_in_cnt;
	u64 esw_pkt_out_cnt_0;
	u64 esw_pkt_out_cnt_1;
	u64 esw_pkt_out_cnt_2;
	u64 esw_pkt_out_cnt_3;

	u64 eos_md_in_cnt_0;
	u64 eos_md_out_cnt_0;
	u64 eos_md_in_cnt_1;
	u64 eos_md_out_cnt_1;

	u64 eos_md_in_cnt_2;
	u64 eos_md_out_cnt_2;

	u64 eos_md_in_cnt_3;
	u64 eos_md_out_cnt_3;

	/*
	u64 esw_pkt_in_cnt;
	u64 esw_pkt_out_cnt;
	u8	local_mac_addr;
	u8	buf_id_rest_cnt;
	u8  pad0[14];
	u64 eos_md_in_cnt;
	u64 eos_md_out_cnt;
	u8	eos_q0_used_cnt;
	u8	eos_q1_used_cnt;
	u8	eos_q2_used_cnt;
	u8	eos_q3_used_cnt;
	u8  pad2[12];
	u64 goe_pkt_in_cnt;
	u64 goe_port0_out_cnt;
	u64 goe_port1_out_cnt;
	u64 goe_discard_cnt;
	*/
}__attribute__((packed));

#endif

typedef enum
{
	NO_RESPONSE = 0,
	WR_RESPONSE = 1
}WRITE_FLAG;

struct ptp_packet
{
	u8 dst_mac[MAC_LEN];
	u8 src_mac[MAC_LEN];
	u16 eth_type;
	u16 msg_type:4,//msg_type和transpec换位置进行主机序转网络序
		transpec:4,		
		reserve0:4,//reserve0和ver_ptp换位置进行主机序转网络序		
		ver_ptp:4;
	u16 pkt_len;
	u8	domain_no;
	u8 	reserve1;
	u16	flag;
	u64	correct_field;
	u16	seq;
	u8 pad[16];
	u16 timestamp[5];	
	u8 reserve2[6];
	struct config_info cfg;	
	struct status_info status;
}__attribute__((packed));

#endif
