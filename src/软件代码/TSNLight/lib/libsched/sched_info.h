/*
 * sched_info.h
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
#ifndef __SCHED_INFO_H__
#define __SCHED_INFO_H__

#include "../../include/tsn_public.h"
#include "../libreserve/flow_feature.h"

struct uni_path
{
	u16 flow_id;
	u8 path_len;
	u8 src_sw_id;
	u8 dst_sw_id;
};

struct tsn_sched_info
{
	struct uni_path path;
	u16 interval;
	u16 pkt_num;
	u16 pkt_size;
	u16 latency;
};

struct tsn_sched_set
{
	u32 cur_num;
	struct tsn_sched_info tsn[MAX_FLOW_NUM];
};

struct bd_sched_info
{
	struct uni_path path;
	u16 bandwidth;
};

struct bd_sched_set
{
	u32 cur_num;
	struct bd_sched_info bd[MAX_FLOW_NUM];
};

struct tsn_uni_cfg
{
	u8 tx_enable;
	u8 host_mac[MAC_LEN];
};

struct bd_uni_cfg
{
	u8 tx_enable;
	u8 host_mac[MAC_LEN];
	u8 path_len;
	u8 sw_mac[MAX_NODE_NUM][MAC_LEN];
	u16 tb_rate[MAX_NODE_NUM];
};

#endif