/***************************************************************************
 *            reserve_mgr.c
 *
 *  Wed May 08 20:34:24 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * reserve_mgr.c
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
#include "reserve_mgr.h"

struct global_reserve_table g_table = {0};

typedef enum
{
	NO_FLOW = 0,
	NEW_FLOW = 1,
	FLOW_TSN = 2,
	FLOW_BD = 3
}FLAG;

int flow_flag = NO_FLOW;

struct global_reserve_table *init_global_reserve_table()
{
	FILE *fp = NULL;
	int i = 0;
	char buf[BUF_LEN];
	char *save_pstr = NULL;
	char *fir_pstr = NULL;
	char *sec_pstr = NULL;
	u8 temp_mac[MAC_LEN] = {0};
	u32 cur_num;

	fp = fopen(FLOW_FILE, "r");
	if(fp == NULL)
	{
		TSN_ERR("Could not read flow file!\n");
		return NULL;
	}
	TSN_DBG("Open flow reserve file successfully!\n");
	while(fgets(buf, BUF_LEN, fp) != NULL)
	{
		TSN_DBG("line content: %s", buf);
		riprt(buf);
		if(flow_flag == NO_FLOW)
		{
			if(!strcmp(buf, "{"))
			{
				TSN_DBG("Find a new flow!\n");
				flow_flag = NEW_FLOW;
			}
			else
				continue;
		}

		else if(flow_flag == NEW_FLOW)
		{
			fir_pstr = strtok_r(buf, ":", &sec_pstr);
			if(!strcmp(fir_pstr + 1, "type"))
			{
				if(!strcmp(sec_pstr, "tsn"))
				{
					flow_flag = FLOW_TSN;
					TSN_DBG("Find TSN flow!\n");
				}
				else if(!strcmp(sec_pstr, "bd"))
					flow_flag = FLOW_BD;
				else
					flow_flag = NO_FLOW;
			}
			else
				flow_flag = NO_FLOW;
		}

		else if(flow_flag == FLOW_TSN)
		{
			if(!strcmp(buf, "}"))
			{
				flow_flag = NO_FLOW;
				cur_num = g_table.tsn_uni_tbl.cur_num;
				TSN_DBG("cur index: %d\n", cur_num);
				TSN_DBG("flow id: %d\n", g_table.tsn_uni_tbl.tsn_uni[cur_num].flow_id);
				TSN_DBG("src mac: %x:%x\n", *(g_table.tsn_uni_tbl.tsn_uni[cur_num].src_host_mac), *(g_table.tsn_uni_tbl.tsn_uni[cur_num].src_host_mac + 1));
				TSN_DBG("dst mac: %x:%x\n", *(g_table.tsn_uni_tbl.tsn_uni[cur_num].dst_host_mac), *(g_table.tsn_uni_tbl.tsn_uni[cur_num].src_host_mac + 1));
				TSN_DBG("pkt num: %d\n", g_table.tsn_uni_tbl.tsn_uni[cur_num].pkt_num);
				TSN_DBG("interval: %d\n", g_table.tsn_uni_tbl.tsn_uni[cur_num].interval);
				g_table.tsn_uni_tbl.cur_num++;
			}
			else
			{
				cur_num = g_table.tsn_uni_tbl.cur_num;
				fir_pstr = strtok_r(buf, ":", &sec_pstr);
				if(!strcmp(fir_pstr + 1, "flow_id"))
					g_table.tsn_uni_tbl.tsn_uni[cur_num].flow_id = atoi(sec_pstr);
				else if(!strcmp(fir_pstr + 1, "src_mac"))
				{
					trans_str_mac_to_hex_mac(sec_pstr, temp_mac);
					memcpy(g_table.tsn_uni_tbl.tsn_uni[cur_num].src_host_mac, temp_mac, MAC_LEN);
				}
				else if(!strcmp(fir_pstr + 1, "dst_mac"))
				{
					trans_str_mac_to_hex_mac(sec_pstr, temp_mac);
					memcpy(g_table.tsn_uni_tbl.tsn_uni[cur_num].dst_host_mac, temp_mac, MAC_LEN);
				}
				else if(!strcmp(fir_pstr + 1, "priority"))
					g_table.tsn_uni_tbl.tsn_uni[cur_num].priority = atoi(sec_pstr);
				else if(!strcmp(fir_pstr + 1, "interval"))
					g_table.tsn_uni_tbl.tsn_uni[cur_num].interval = atoi(sec_pstr);
				else if(!strcmp(fir_pstr + 1, "pkt_num"))
					g_table.tsn_uni_tbl.tsn_uni[cur_num].pkt_num = atoi(sec_pstr);
				else if(!strcmp(fir_pstr + 1, "pkt_size"))
					g_table.tsn_uni_tbl.tsn_uni[cur_num].pkt_size = atoi(sec_pstr);
				else if(!strcmp(fir_pstr + 1, "latency"))
					g_table.tsn_uni_tbl.tsn_uni[cur_num].latency = atoi(sec_pstr);
				else
					continue;
			}

		}

		else if(flow_flag == FLOW_BD)
		{
			if(!strcmp(buf, "}"))
			{
				flow_flag = NO_FLOW;
				cur_num = g_table.bd_uni_tbl.cur_num;
				TSN_DBG("cur index: %d\n", cur_num);
				TSN_DBG("src mac: %x\n", *(g_table.tsn_uni_tbl.tsn_uni[cur_num].src_host_mac + 5));
				TSN_DBG("dst mac: %x\n", *(g_table.tsn_uni_tbl.tsn_uni[cur_num].dst_host_mac + 5));
				TSN_DBG("bandwidth: %d\n", g_table.bd_uni_tbl.bd_uni[cur_num].bandwidth);
				g_table.bd_uni_tbl.cur_num++;
			}
			else
			{
				cur_num = g_table.bd_uni_tbl.cur_num;
				fir_pstr = strtok_r(buf, ":", &sec_pstr);
				if(!strcmp(fir_pstr + 1, "flow_id"))
					g_table.bd_uni_tbl.bd_uni[cur_num].flow_id = atoi(sec_pstr);
				else if(!strcmp(fir_pstr + 1, "src_mac"))
				{
					trans_str_mac_to_hex_mac(sec_pstr, temp_mac);
					memcpy(g_table.bd_uni_tbl.bd_uni[cur_num].src_host_mac, temp_mac, MAC_LEN);
				}
				else if(!strcmp(fir_pstr + 1, "dst_mac"))
				{
					trans_str_mac_to_hex_mac(sec_pstr, temp_mac);
					memcpy(g_table.bd_uni_tbl.bd_uni[cur_num].dst_host_mac, temp_mac, MAC_LEN);
				}
				else if(!strcmp(fir_pstr + 1, "priority"))
					g_table.bd_uni_tbl.bd_uni[cur_num].priority = atoi(sec_pstr);
				else if(!strcmp(fir_pstr + 1, "bandwidth"))
					g_table.bd_uni_tbl.bd_uni[cur_num].bandwidth = atoi(sec_pstr);
				else
					continue;
			}
		}
		
		else
		{
			continue;
		}	
	}
	return &g_table;
}