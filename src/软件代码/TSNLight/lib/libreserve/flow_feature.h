/***************************************************************************
 *            flow_feature.h
 *
 *  Wed May 08 20:33:52 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * flow_feature.h
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
#ifndef __FLOW_FEATURE_H__
#define __FLOW_FEATURE_H__

#include "../../include/tsn_public.h"

#define MAX_FLOW_NUM 5

struct tsn_uni_feature
{
	u16 flow_id;
	u8 src_host_mac[MAC_LEN];
	u8 dst_host_mac[MAC_LEN];
	u8 priority;
	u16 interval;
	u16 pkt_num;
	u16 pkt_size;
	u16 latency;
};

struct bd_uni_feature
{
	u16 flow_id;
	u8 src_host_mac[MAC_LEN];
	u8 dst_host_mac[MAC_LEN];
	u8 priority;
	u16 bandwidth;
};

struct tsn_uni_table
{
	u32 cur_num;
	struct tsn_uni_feature tsn_uni[MAX_FLOW_NUM];
};

struct bd_uni_table
{
	u32 cur_num;
	struct bd_uni_feature bd_uni[MAX_FLOW_NUM];
};

struct global_reserve_table
{
	struct tsn_uni_table tsn_uni_tbl;
	struct bd_uni_table bd_uni_tbl;
};

#endif