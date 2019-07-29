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

struct config_info
{
	u8	host_mac[6];
	u8	pad:7,
		direction:1;
	u8	pad1;
	u32	tb_rate;
	u32	time_slot;
}__attribute__((packed));

struct status_info
{
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
}__attribute__((packed));

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