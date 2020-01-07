/***************************************************************************
 *            flow_sched.c
 *
 *  Wed May 08 22:15:07 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * flow_sched.c
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
#include "flow_sched.h"

struct tsn_uni_cfg tsn_uni_sched(struct tsn_sched_info tsn_info)
{
	int i = 0;
	u8 src_sw_id = tsn_info.path.src_sw_id;
	u8 cur_sw_id;
	u32 cqf_free_len = 0;
	int result = TRUE;
	u16 worst_latency = (tsn_info.path.path_len + 1) * DEFAULT_SLOT_SIZE;
	struct tsn_uni_cfg tsn_cfg = {0};
	u8 *host_mac = NULL;

	if(worst_latency > tsn_info.latency)
		return tsn_cfg;

	for(i = 0; i < tsn_info.path.path_len; i++)
	{
		cur_sw_id = (src_sw_id + i) % get_sw_num();
		TSN_DBG("###cur_sw_id %d\n",cur_sw_id);
		cqf_free_len = get_free_cqf_resource(cur_sw_id, 0);
		if(cqf_free_len < tsn_info.pkt_num)
		{
			result = FALSE;
			break;
		}
	}

	if(result == TRUE)
	{
		tsn_cfg.tx_enable = 1;
		host_mac = get_host_mac_by_sw_id(tsn_info.path.src_sw_id);
		TSN_DBG("###host_mac1 %p\n",host_mac);
		memcpy(tsn_cfg.host_mac, host_mac, MAC_LEN);
		TSN_DBG("###host_mac2 %p\n",host_mac);
	}

	return tsn_cfg;
}

struct bd_uni_cfg bd_uni_sched(struct bd_sched_info bd_info)
{
	int i = 0;
	u8 src_sw_id = bd_info.path.src_sw_id;
	u8 cur_sw_id;
	u32 free_bd = 0;
	u32 tb_bd = 0;
	int result = TRUE;
	struct bd_uni_cfg bd_cfg = {0};
	u8 *host_mac = NULL;
	u8 *sw_mac = NULL;

	for(i = 0; i < bd_info.path.path_len; i++)
	{
		cur_sw_id = (src_sw_id + i) % get_sw_num();
		free_bd = get_free_bd_resource(cur_sw_id);
		if(free_bd < bd_info.bandwidth)
		{
			result = FALSE;
			break;
		}
	}

	if(result == TRUE)
	{
		bd_cfg.tx_enable = 1;
		host_mac = get_host_mac_by_sw_id(bd_info.path.src_sw_id);
		memcpy(bd_cfg.host_mac, host_mac, MAC_LEN);
		bd_cfg.path_len = bd_info.path.path_len;
		for(i = 0; i < bd_cfg.path_len; i++)
		{
			cur_sw_id = (src_sw_id + i) % get_sw_num();
			sw_mac = get_sw_mac_by_sw_id(cur_sw_id);
			TSN_DBG("sw id: %d\n", cur_sw_id);
			TSN_DBG("%x:%x:%x:%x:%x:%x\n", sw_mac[0], sw_mac[1], sw_mac[2], sw_mac[3], sw_mac[4], sw_mac[5]);
			memcpy(bd_cfg.sw_mac[i], sw_mac, MAC_LEN);
			tb_bd = get_tb_bd_resource(cur_sw_id);
			bd_cfg.tb_rate[i] = tb_bd + bd_info.bandwidth;
			TSN_DBG("tb rate: %d\n", bd_cfg.tb_rate[i]);
		}
	}

	return bd_cfg;	
}

void print_tsn_cfg(struct tsn_uni_cfg tsn_cfg)
{
	TSN_DBG("tx_enable: %d\n", tsn_cfg.tx_enable);
	TSN_DBG("host_mac: %x:%x:%x:%x:%x:%x\n", tsn_cfg.host_mac[0], tsn_cfg.host_mac[1], tsn_cfg.host_mac[2], \
	        tsn_cfg.host_mac[3], tsn_cfg.host_mac[4], tsn_cfg.host_mac[5]);
}

void print_bd_cfg(struct bd_uni_cfg bd_cfg)
{
	int i = 0;
	TSN_DBG("tx_enable: %d\n", bd_cfg.tx_enable);
	TSN_DBG("host_mac: %x:%x:%x:%x:%x:%x\n", bd_cfg.host_mac[0], bd_cfg.host_mac[1], bd_cfg.host_mac[2], \
	        bd_cfg.host_mac[3], bd_cfg.host_mac[4], bd_cfg.host_mac[5]);
	TSN_DBG("path_len: %d\n", bd_cfg.path_len);
	for(i = 0; i < bd_cfg.path_len; i++)
	{
		TSN_DBG("sw_mac: %x:%x:%x:%x:%x:%x\n", bd_cfg.sw_mac[i][0], bd_cfg.sw_mac[i][1], bd_cfg.sw_mac[i][2], \
	        bd_cfg.sw_mac[i][3], bd_cfg.sw_mac[i][4], bd_cfg.sw_mac[i][5]);
		TSN_DBG("tb_rate: %d\n", bd_cfg.tb_rate[i]);
	}
}