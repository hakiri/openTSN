/***************************************************************************
 *            path_compute.c
 *
 *  Wed May 08 21:21:45 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * path_compute.c
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
#include "path_compute.h"

static struct uni_path compute_uni_path(u8 *src_host_mac, u8 *dst_host_mac)
{
	struct uni_path path = {0};
	path.src_sw_id = get_sw_id_by_host_mac(src_host_mac);
	TSN_DBG("src sw id: %d\n", path.src_sw_id);
	path.dst_sw_id = get_sw_id_by_host_mac(dst_host_mac);
	TSN_DBG("dst sw id: %d\n", path.dst_sw_id);
	if(path.src_sw_id < path.dst_sw_id)
	{
		path.path_len = path.dst_sw_id - path.src_sw_id + 1;
	}
	else
	{
		path.path_len = get_sw_num() - path.src_sw_id + path.dst_sw_id + 1;
	}
	return path;
}

struct tsn_sched_set compute_tsn_uni_path(struct tsn_uni_table *tsn_tbl)
{
	int i = 0;
	struct tsn_sched_set tsn_set= {0};
	for(i = 0; i < tsn_tbl->cur_num; i++)
	{
		tsn_set.tsn[i].path = compute_uni_path(tsn_tbl->tsn_uni[i].src_host_mac, tsn_tbl->tsn_uni[i].dst_host_mac);
		tsn_set.tsn[i].path.flow_id = tsn_tbl->tsn_uni[i].flow_id;
		tsn_set.tsn[i].interval = tsn_tbl->tsn_uni[i].interval;
		tsn_set.tsn[i].pkt_num = tsn_tbl->tsn_uni[i].pkt_num;
		tsn_set.tsn[i].pkt_size = tsn_tbl->tsn_uni[i].pkt_size;
		tsn_set.tsn[i].latency = tsn_tbl->tsn_uni[i].latency;
		tsn_set.cur_num++;
	}
	return tsn_set;
}

struct bd_sched_set compute_bd_uni_path(struct bd_uni_table *bd_tbl)
{
	int i = 0;
	struct bd_sched_set bd_set= {0};
	for(i = 0; i < bd_tbl->cur_num; i++)
	{
		bd_set.bd[i].path = compute_uni_path(bd_tbl->bd_uni[i].src_host_mac, bd_tbl->bd_uni[i].dst_host_mac);
		bd_set.bd[i].path.flow_id = bd_tbl->bd_uni[i].flow_id;
		bd_set.bd[i].bandwidth = bd_tbl->bd_uni[i].bandwidth;
		bd_set.cur_num++;
	}
	return bd_set;
}

void print_tsn_set(struct tsn_sched_set tsn_set)
{
	int i = 0;
	for(i = 0; i < tsn_set.cur_num; i++)
	{
		TSN_DBG("tsn flow id: %d\n", tsn_set.tsn[i].path.flow_id);
		TSN_DBG("path len: %d\n", tsn_set.tsn[i].path.path_len);
		TSN_DBG("src sw id: %d\n", tsn_set.tsn[i].path.src_sw_id);
		TSN_DBG("dst sw id: %d\n", tsn_set.tsn[i].path.dst_sw_id);
		TSN_DBG("interval: %d\n", tsn_set.tsn[i].interval);
		TSN_DBG("pkt num: %d\n", tsn_set.tsn[i].pkt_num);
		TSN_DBG("pkt size: %d\n", tsn_set.tsn[i].pkt_size);
		TSN_DBG("latency: %d\n", tsn_set.tsn[i].latency);
	}
}
void print_bd_set(struct bd_sched_set bd_set)
{
	int i = 0;
	for(i = 0; i < bd_set.cur_num; i++)
	{
		TSN_DBG("bd flow id: %d\n", bd_set.bd[i].path.flow_id);
		TSN_DBG("path len: %d\n", bd_set.bd[i].path.path_len);
		TSN_DBG("src sw id: %d\n", bd_set.bd[i].path.src_sw_id);
		TSN_DBG("dst sw id: %d\n", bd_set.bd[i].path.dst_sw_id);
		TSN_DBG("bandwidth: %d\n", bd_set.bd[i].bandwidth);
	}
}