/***************************************************************************
 *            global_sched.c
 *
 *  Thu May 09 10:31:19 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * global_sched.c
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
#include "global_sched.h"

int global_flow_resource_sched(struct global_reserve_table *g_table)
{
	struct tsn_sched_set tsn_set = {0};
	struct tsn_uni_cfg tsn_cfg = {0};
	struct bd_sched_set bd_set = {0};
	struct bd_uni_cfg bd_cfg = {0};
	int i = 0;

	cJSON *flow_json = NULL, *tsn_json = NULL, *bd_json = NULL, *ele_json = NULL;
	char *data = NULL;
	u16 data_len = 0;
	char str_flow_type[8] = {0};
	char str_flow_id[8] = {0};
	char str_src_mac[20] = {0};
	char str_dst_mac[20] = {0};
	char str_priority[8] = {0};
	char str_interval[8] = {0};
	char str_pkt_num[8] = {0};
	char str_pkt_size[8] = {0};
	char str_latency[8] = {0};
	char str_bandwidth[8] = {0};
	char str_path_len[8] = {0};
	char str_direction[8] = {0};
	char str_src_sw_id[8] = {0};
	char str_dst_sw_id[8] = {0};
	
	tsn_set = compute_tsn_uni_path(&(g_table->tsn_uni_tbl));
	TSN_DBG("before sort:\n");
	print_tsn_set(tsn_set);

	flow_json = cJSON_CreateObject();
	tsn_json = cJSON_CreateArray();
	bd_json = cJSON_CreateArray();
	for(i = 0; i < tsn_set.cur_num; i++)
	{
		sprintf(str_flow_id, "%x", tsn_set.tsn[i].path.flow_id);
		sprintf(str_src_mac, "%x:%x:%x:%x:%x:%x", g_table->tsn_uni_tbl.tsn_uni[i].src_host_mac[0], 
		        g_table->tsn_uni_tbl.tsn_uni[i].src_host_mac[1], \
		        g_table->tsn_uni_tbl.tsn_uni[i].src_host_mac[2], \
		        g_table->tsn_uni_tbl.tsn_uni[i].src_host_mac[3], \
		        g_table->tsn_uni_tbl.tsn_uni[i].src_host_mac[4], \
		        g_table->tsn_uni_tbl.tsn_uni[i].src_host_mac[5]);
		sprintf(str_dst_mac, "%x:%x:%x:%x:%x:%x", g_table->tsn_uni_tbl.tsn_uni[i].dst_host_mac[0], 
		        g_table->tsn_uni_tbl.tsn_uni[i].dst_host_mac[1], \
		        g_table->tsn_uni_tbl.tsn_uni[i].dst_host_mac[2], \
		        g_table->tsn_uni_tbl.tsn_uni[i].dst_host_mac[3], \
		        g_table->tsn_uni_tbl.tsn_uni[i].dst_host_mac[4], \
		        g_table->tsn_uni_tbl.tsn_uni[i].dst_host_mac[5]);
		sprintf(str_priority, "%x", g_table->tsn_uni_tbl.tsn_uni[i].priority);
		sprintf(str_interval, "%x",  tsn_set.tsn[i].interval);
		sprintf(str_pkt_num, "%x",  tsn_set.tsn[i].pkt_num);
		sprintf(str_pkt_size, "%x",  tsn_set.tsn[i].pkt_size);
		sprintf(str_latency, "%x",  tsn_set.tsn[i].latency);
		sprintf(str_path_len, "%x", tsn_set.tsn[i].path.path_len);
		sprintf(str_direction, "%x", DEFAULT_DIREC);
		sprintf(str_src_sw_id, "%x", tsn_set.tsn[i].path.src_sw_id);
		sprintf(str_dst_sw_id, "%x", tsn_set.tsn[i].path.dst_sw_id);

		ele_json = cJSON_CreateObject();
		cJSON_AddStringToObject(ele_json, "flow_type", "tsn");
		cJSON_AddStringToObject(ele_json, "flow_id", str_flow_id);
		cJSON_AddStringToObject(ele_json, "src_mac", str_src_mac);
		cJSON_AddStringToObject(ele_json, "dst_mac", str_dst_mac);
		cJSON_AddStringToObject(ele_json, "priority", str_priority);
		cJSON_AddStringToObject(ele_json, "interval", str_interval);
		cJSON_AddStringToObject(ele_json, "pkt_num", str_pkt_num);
		cJSON_AddStringToObject(ele_json, "pkt_size", str_pkt_size);
		cJSON_AddStringToObject(ele_json, "latency", str_latency);
		cJSON_AddStringToObject(ele_json, "path_len", str_path_len);
		cJSON_AddStringToObject(ele_json, "direction", str_direction);
		cJSON_AddStringToObject(ele_json, "src_sw_id", str_src_sw_id);
		cJSON_AddStringToObject(ele_json, "dst_sw_id", str_dst_sw_id);

		cJSON_AddItemToArray(tsn_json, ele_json);
	}
	
	tsn_set = tsn_flow_reorder_by_pkt_num(tsn_set);
	TSN_DBG("after sort:\n");
	print_tsn_set(tsn_set);

	bd_set = compute_bd_uni_path(&(g_table->bd_uni_tbl));
	TSN_DBG("before sort:\n");
	print_bd_set(bd_set);

	for(i = 0; i < bd_set.cur_num; i++)
	{
		sprintf(str_flow_id, "%x", bd_set.bd[i].path.flow_id);
		sprintf(str_src_mac, "%x:%x:%x:%x:%x:%x", g_table->bd_uni_tbl.bd_uni[i].src_host_mac[0], 
		        g_table->bd_uni_tbl.bd_uni[i].src_host_mac[1], \
		        g_table->bd_uni_tbl.bd_uni[i].src_host_mac[2], \
		        g_table->bd_uni_tbl.bd_uni[i].src_host_mac[3], \
		        g_table->bd_uni_tbl.bd_uni[i].src_host_mac[4], \
		        g_table->bd_uni_tbl.bd_uni[i].src_host_mac[5]);
		sprintf(str_dst_mac, "%x:%x:%x:%x:%x:%x", g_table->bd_uni_tbl.bd_uni[i].dst_host_mac[0], 
		        g_table->bd_uni_tbl.bd_uni[i].dst_host_mac[1], \
		        g_table->bd_uni_tbl.bd_uni[i].dst_host_mac[2], \
		        g_table->bd_uni_tbl.bd_uni[i].dst_host_mac[3], \
		        g_table->bd_uni_tbl.bd_uni[i].dst_host_mac[4], \
		        g_table->bd_uni_tbl.bd_uni[i].dst_host_mac[5]);
		sprintf(str_priority, "%x", g_table->bd_uni_tbl.bd_uni[i].priority);
		sprintf(str_bandwidth, "%x",  bd_set.bd[i].bandwidth);
		sprintf(str_path_len, "%x", bd_set.bd[i].path.path_len);
		sprintf(str_direction, "%x", DEFAULT_DIREC);
		sprintf(str_src_sw_id, "%x", bd_set.bd[i].path.src_sw_id);
		sprintf(str_dst_sw_id, "%x", bd_set.bd[i].path.dst_sw_id);

		ele_json = cJSON_CreateObject();
		cJSON_AddStringToObject(ele_json, "flow_type", "bd");
		cJSON_AddStringToObject(ele_json, "flow_id", str_flow_id);
		cJSON_AddStringToObject(ele_json, "src_mac", str_src_mac);
		cJSON_AddStringToObject(ele_json, "dst_mac", str_dst_mac);
		cJSON_AddStringToObject(ele_json, "priority", str_priority);
		cJSON_AddStringToObject(ele_json, "bandwidth", str_bandwidth);
		cJSON_AddStringToObject(ele_json, "path_len", str_path_len);
		cJSON_AddStringToObject(ele_json, "direction", str_direction);
		cJSON_AddStringToObject(ele_json, "src_sw_id", str_src_sw_id);
		cJSON_AddStringToObject(ele_json, "dst_sw_id", str_dst_sw_id);

		cJSON_AddItemToArray(bd_json, ele_json);
	}
	
	bd_set = bd_flow_reorder_by_bd(bd_set);
	TSN_DBG("after sort:\n");
	print_bd_set(bd_set);

	cJSON_AddItemToObject(flow_json, "tsn", tsn_json);
	cJSON_AddItemToObject(flow_json, "bd", bd_json);
	data = cJSON_Print(flow_json);
	data_len = strlen(data);
	TSN_DBG("build flow json len: %d, info: %s\n", data_len, data);
	//send flow info
	send_north_packet(FLOW_FEATURE, data, data_len);
	free(data);
	data = NULL;
	cJSON_Delete(flow_json);
	
	for(i = 0; i < tsn_set.cur_num; i++)
	{
		tsn_cfg = tsn_uni_sched(tsn_set.tsn[i]);
		if(tsn_cfg.tx_enable == 1)
		{
			TSN_DBG("TSN flow[%d] can be sched!\n", tsn_set.tsn[i].path.flow_id);
			print_tsn_cfg(tsn_cfg);
			config_tsn_uni_flow(tsn_cfg);
			update_tsn_uni_resource(tsn_set.tsn[i]);
			print_global_resource();
		}
		else
		{
			TSN_ERR("TSN flow[%d] cannot be sched!\n", tsn_set.tsn[i].path.flow_id);
		}
	}
	
	for(i = 0; i < bd_set.cur_num; i++)
	{
		bd_cfg = bd_uni_sched(bd_set.bd[i]);
		if(bd_cfg.tx_enable == 1)
		{
			TSN_DBG("BD flow[%d] can be sched!\n", bd_set.bd[i].path.flow_id);
			print_bd_cfg(bd_cfg);
			config_bd_uni_flow(bd_cfg);
			update_bd_uni_resource(bd_set.bd[i]);
			print_global_resource();
		}
		else
		{
			TSN_ERR("BD flow[%d] cannot be sched!\n", bd_set.bd[i].path.flow_id);
		}
	}

	return G_OK;
}