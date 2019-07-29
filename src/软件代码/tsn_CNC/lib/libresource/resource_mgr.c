/***************************************************************************
 *            resource_mgr.c
 *
 *  Wed May 08 11:46:19 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * resource_mgr.c
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
#include "resource_mgr.h"

struct global_resource g_res = {0};

int build_and_send_global_resource()
{
	#if 0
	int i = 0;
	cJSON *res_json = NULL, *ele_json = NULL;
	char str_sw_id[8] = {0};
	char str_cqf_total_len[8] = {0};
	char str_cqf_free_len[8] = {0};
	char str_cqf_used_len[8] = {0};
	char str_total_bd[8] = {0};
	char str_free_bd[8] = {0};
	char str_used_bd[8] = {0};
	char str_tb_rate[8] = {0};
	char str_direction[8] = {0};
	char str_time_slot[8] = {0};
	char *data = NULL;
	u16 data_len = 0;

	res_json = cJSON_CreateArray();

	for(i = 0; i < g_res.cur_num; i++)
	{
		sprintf(str_sw_id, "%x", g_res.device[i].sw_id);
		sprintf(str_cqf_total_len, "%x", g_res.device[i].cqf_res[0].total_len);
		sprintf(str_cqf_free_len, "%x", g_res.device[i].cqf_res[0].free_len);
		sprintf(str_cqf_used_len, "%x", g_res.device[i].cqf_res[0].used_len);
		sprintf(str_total_bd, "%x", g_res.device[i].bd_res.total_bd);
		sprintf(str_free_bd, "%x", g_res.device[i].bd_res.free_bd);
		sprintf(str_used_bd, "%x", g_res.device[i].bd_res.used_bd);
		sprintf(str_tb_rate, "%x", g_res.device[i].bd_res.tb_bd);
		sprintf(str_direction, "%x", g_res.device[i].cfg.direction);
		sprintf(str_time_slot, "%x", ntohl(g_res.device[i].cfg.time_slot) / 125);

		ele_json = cJSON_CreateObject();
		cJSON_AddStringToObject(ele_json, "sw_id", str_sw_id);
		cJSON_AddStringToObject(ele_json, "cqf_total_len", str_cqf_total_len);
		cJSON_AddStringToObject(ele_json, "cqf_free_len", str_cqf_free_len);
		cJSON_AddStringToObject(ele_json, "cqf_used_len", str_cqf_used_len);
		cJSON_AddStringToObject(ele_json, "total_bd", str_total_bd);
		cJSON_AddStringToObject(ele_json, "free_bd", str_free_bd);
		cJSON_AddStringToObject(ele_json, "used_bd", str_used_bd);
		cJSON_AddStringToObject(ele_json, "tb_rate", str_tb_rate);
		cJSON_AddStringToObject(ele_json, "direction", str_direction);
		cJSON_AddStringToObject(ele_json, "time_slot", str_time_slot);

		cJSON_AddItemToArray(res_json, ele_json);
	}
	data = cJSON_Print(res_json);
	data_len = strlen(data);
	TSN_DBG("build resource json len: %d, info: %s\n", data_len, data);
	send_north_packet(RESOURCE_INFO, data, data_len);
	free(data);
	data = NULL;
	cJSON_Delete(res_json);
	return G_OK;
	#endif
}

int init_global_resource()
{
	u8 *p = NULL;
	int i = 0, j = 0;

	g_res.cur_num = get_sw_num();
	if(g_res.cur_num == 0)
	{
		TSN_ERR("The number of nodes is zero!\n");
		return G_ERR;
	}
	TSN_DBG("cur_num = %d\n", g_res.cur_num);
	
	for(i = 0; i < g_res.cur_num; i++)
	{
		g_res.device[i].sw_id = i;
		g_res.device[i].bd_res.total_bd = BANDWIDTH;
		g_res.device[i].bd_res.free_bd = BANDWIDTH;
		g_res.device[i].bd_res.used_bd = 0;
		g_res.device[i].bd_res.tb_bd = 0;
		
		for(j = 0; j < SLOT_NUM; j++)
		{
			g_res.device[i].cqf_res[j].total_len = CQF_LEN;
			g_res.device[i].cqf_res[j].free_len = CQF_LEN;
			g_res.device[i].cqf_res[j].used_len = 0;
		}
		
		g_res.device[i].cfg.direction = DEFAULT_DIREC;
		g_res.device[i].cfg.tb_rate = htonl(10);
		g_res.device[i].cfg.time_slot = htonl(DEFAULT_SLOT_SIZE*125);
		//g_res.device[i].cfg.time_slot = DEFAULT_SLOT_SIZE*125;
		memcpy(g_res.device[i].cfg.host_mac, get_host_mac_by_sw_id(i), MAC_LEN);
	}

	return G_OK;
}

u32 get_free_bd_resource(u8 sw_id)
{
	if(g_res.cur_num <= sw_id)
	{
		TSN_ERR("Could not find sw_id %d\n",sw_id);
		return G_ERR;
	}
	return g_res.device[sw_id].bd_res.free_bd;
}

u32 get_tb_bd_resource(u8 sw_id)
{
	if(g_res.cur_num <= sw_id)
	{
		TSN_ERR("Could not find sw_id %d\n",sw_id);
		return G_ERR;
	}
	return g_res.device[sw_id].bd_res.tb_bd;
}

u32 get_free_cqf_resource(u8 sw_id, u8 slot)
{
	if(g_res.cur_num <= sw_id)
	{
		TSN_ERR("Could not find sw_id %d\n",sw_id);
		return G_ERR;
	}
	return g_res.device[sw_id].cqf_res[slot].free_len;
}

int update_tsn_bd_resource(u8 sw_id, u32 bd_alloc)
{
	if(g_res.cur_num <= sw_id)
	{
		TSN_ERR("Could not find sw_id %d\n", sw_id);
		return G_ERR;
	}
	g_res.device[sw_id].bd_res.used_bd += bd_alloc;
	g_res.device[sw_id].bd_res.free_bd -= bd_alloc;
	return G_OK;
}

int update_tb_bd_resource(u8 sw_id, u32 bd_alloc)
{
	if(g_res.cur_num <= sw_id)
	{
		TSN_ERR("Could not find sw_id %d\n",sw_id);
		return G_ERR;
	}
	g_res.device[sw_id].bd_res.used_bd += bd_alloc;
	g_res.device[sw_id].bd_res.free_bd -= bd_alloc;
	g_res.device[sw_id].bd_res.tb_bd   += bd_alloc;
	return G_OK;
}

int update_cqf_resource(u8 sw_id, u32 queue_alloc, u8 slot)
{
	if(g_res.cur_num <= sw_id)
	{
		TSN_ERR("Could not find sw_id %d\n",sw_id);
		return G_ERR;
	}
	g_res.device[sw_id].cqf_res[slot].used_len += queue_alloc;
	g_res.device[sw_id].cqf_res[slot].free_len -= queue_alloc;

	return G_OK;
}

int update_cfg_info(u8 sw_id, struct config_info cfg)
{
	if(g_res.cur_num <= sw_id)
	{
		TSN_ERR("Could not find sw_id %d\n",sw_id);
		return G_ERR;
	}
	g_res.device[sw_id].cfg = cfg;
	return G_OK;
}

void get_cfg_info(u8 sw_id, struct config_info *cfg)
{
	if(g_res.cur_num <= sw_id)
	{
		TSN_ERR("Could not find sw_id %d\n",sw_id);
		return;
	}
	memcpy(cfg, &g_res.device[sw_id].cfg, sizeof(struct config_info));
}

void print_global_resource()
{
	int i = 0;
	TSN_DBG("*************************************************\n");
	TSN_DBG("cur_num %d\n",g_res.cur_num);
	for(i = 0; i < g_res.cur_num; i++)
	{
		TSN_DBG("sw id: %d\n", i);
		TSN_DBG("total_bd: %d, free_bd: %d, used_bd: %d, tb_bd: %d\n", g_res.device[i].bd_res.total_bd, \
		        g_res.device[i].bd_res.free_bd, g_res.device[i].bd_res.used_bd, g_res.device[i].bd_res.tb_bd);
		TSN_DBG("total_len: %d, free_len: %d, used_len: %d\n",g_res.device[i].cqf_res[0].total_len, \
		        g_res.device[i].cqf_res[0].free_len, g_res.device[i].cqf_res[0].used_len);
		TSN_DBG("slot size: %d\n", ntohl(g_res.device[i].cfg.time_slot) / 125);
	}
	TSN_DBG("*************************************************\n");
}