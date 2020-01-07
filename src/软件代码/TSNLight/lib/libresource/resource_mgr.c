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


#define RESOURCE_FILE "./resource_info"


struct global_resource g_res = {0};

int build_and_send_global_resource()
{
	#if 1
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

	u8 flag_num = 0;
	u8 *p = NULL;
	int i = 0, j = 0, k = 0, m = 0;
	u8 tmp_num = 0;
	u32 get_time_slot = 0;
	u32 tmp_time_slot = 0;
	
	char total_bd_char[50] = {0};
	char cqf_len_char[50]  = {0};
	char direction_char[50]  = {0};
	char tb_rate_char[50]  = {0};
	char tb_B_char[50]  = {0};
	char time_slot_char[50]  = {0};
	char TAP_char[50]  = {0};


	get_cfgx_file(RESOURCE_FILE, "total_bd", total_bd_char);
	get_cfgx_file(RESOURCE_FILE, "cqf_len", cqf_len_char);
	get_cfgx_file(RESOURCE_FILE, "direction", direction_char);
	get_cfgx_file(RESOURCE_FILE, "tb_rate", tb_rate_char);
	get_cfgx_file(RESOURCE_FILE, "tb_B", tb_B_char);
	get_cfgx_file(RESOURCE_FILE, "time_slot", time_slot_char);
	get_cfgx_file(RESOURCE_FILE, "TAP", TAP_char);

	get_time_slot = atoi(time_slot_char);
	if(get_time_slot == 4)
		tmp_time_slot = 9;
	else if(get_time_slot == 8)
		tmp_time_slot = 10;
	else if(get_time_slot == 16)
		tmp_time_slot = 11;
	else if(get_time_slot == 32)
		tmp_time_slot = 12;
	else if(get_time_slot == 65)
		tmp_time_slot = 13;
	else if(get_time_slot == 131)
		tmp_time_slot = 14;			
	else if(get_time_slot == 262)
		tmp_time_slot = 15;




	g_res.cur_num = get_sw_num();
	if(g_res.cur_num == 0)
	{
		TSN_ERR("The number of nodes is zero!\n");
		return G_ERR;
	}
	TSN_DBG("cur_num = %d\n", g_res.cur_num);
	
	for(i = 0; i < g_res.cur_num; i++)
	{
		flag_num = 0;//这个很重要，要从新清零
		g_res.device[i].sw_id = i;
		g_res.device[i].bd_res.total_bd = atoi(total_bd_char);
		g_res.device[i].bd_res.free_bd = atoi(total_bd_char);
		g_res.device[i].bd_res.used_bd = 0;
		g_res.device[i].bd_res.tb_bd = 0;
		
		for(j = 0; j < SLOT_NUM; j++)
		{
			g_res.device[i].cqf_res[j].total_len = atoi(cqf_len_char);
			g_res.device[i].cqf_res[j].free_len = atoi(cqf_len_char);
			g_res.device[i].cqf_res[j].used_len = 0;
		}
		
		g_res.device[i].cfg.direction = atoi(direction_char);
		g_res.device[i].cfg.tb_rate = htons(atoi(tb_rate_char));		
		g_res.device[i].cfg.tb_B = htons(atoi(tb_B_char));
		g_res.device[i].cfg.time_slot = htonl(tmp_time_slot);//时间槽的大小，默认为2的15次方，为262us 0x9
		memcpy(g_res.device[i].cfg.host_mac, get_host_mac_by_sw_id(i), MAC_LEN);


#if TOPOLOGY_TYPE==1
		if(i == 0)
			g_res.device[i].cfg.reg_TAP = 1;


		for(m=0;m<4;m++)//依次把0到3端口进行扫描
		{
			tmp_num = get_port_num_by_sw_id_port_id(i,m);//tmp_num代表当前端口有多少条流表
			for(k=0;k<tmp_num;k++)
			{
				if(flag_num == 0)//依次修改mac_ port_
				{
					memcpy(g_res.device[i].cfg.mac0, get_mac_by_sw_id_cur_num_port_id(i,k,m), MAC_LEN);
					g_res.device[i].cfg.port_0 = htons(1<<m);
				}
				else if(flag_num == 1)
				{
					memcpy(g_res.device[i].cfg.mac1, get_mac_by_sw_id_cur_num_port_id(i,k,m), MAC_LEN);
					g_res.device[i].cfg.port_1 = htons(1<<m);
				}
				else if(flag_num == 2)
				{
					memcpy(g_res.device[i].cfg.mac2, get_mac_by_sw_id_cur_num_port_id(i,k,m), MAC_LEN);
					g_res.device[i].cfg.port_2 = htons(1<<m);
				}
				else if(flag_num == 3)
				{
					memcpy(g_res.device[i].cfg.mac3, get_mac_by_sw_id_cur_num_port_id(i,k,m), MAC_LEN);
					g_res.device[i].cfg.port_3 = htons(1<<m);
				}
				else if(flag_num == 4)
				{
					memcpy(g_res.device[i].cfg.mac4, get_mac_by_sw_id_cur_num_port_id(i,k,m), MAC_LEN);
					g_res.device[i].cfg.port_4 = htons(1<<m);
				}
				else if(flag_num == 5)
				{
					memcpy(g_res.device[i].cfg.mac5, get_mac_by_sw_id_cur_num_port_id(i,k,m), MAC_LEN);
					g_res.device[i].cfg.port_5 = htons(1<<m);
				}
				else if(flag_num == 6)
				{
					memcpy(g_res.device[i].cfg.mac6, get_mac_by_sw_id_cur_num_port_id(i,k,m), MAC_LEN);
					g_res.device[i].cfg.port_6 = htons(1<<m);
				}				
				else if(flag_num == 7)
				{
					memcpy(g_res.device[i].cfg.mac7, get_mac_by_sw_id_cur_num_port_id(i,k,m), MAC_LEN);
					g_res.device[i].cfg.port_7 = htons(1<<m);
				}
				else if(flag_num == 8)
				{
					memcpy(g_res.device[i].cfg.mac8, get_mac_by_sw_id_cur_num_port_id(i,k,m), MAC_LEN);
					g_res.device[i].cfg.port_8 = htons(1<<m);
				}
				else if(flag_num == 9)
				{
					memcpy(g_res.device[i].cfg.mac9, get_mac_by_sw_id_cur_num_port_id(i,k,m), MAC_LEN);
					g_res.device[i].cfg.port_9 = htons(1<<m);
				}
				else if(flag_num == 10)
				{
					memcpy(g_res.device[i].cfg.mac10, get_mac_by_sw_id_cur_num_port_id(i,k,m), MAC_LEN);
					g_res.device[i].cfg.port_10 = htons(1<<m);
				}
				else if(flag_num == 11)
				{
					memcpy(g_res.device[i].cfg.mac11, get_mac_by_sw_id_cur_num_port_id(i,k,m), MAC_LEN);
					g_res.device[i].cfg.port_11 = htons(1<<m);
				}
				flag_num++;

			}
		}
#endif
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

#if TOPOLOGY_TYPE==0
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

#elif TOPOLOGY_TYPE==1
void print_global_resource()
{
	int i = 0;
	u8 tmp_num = 0;
	int m =0;
	
	int j =0;
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


		tmp_num = get_port_num_by_sw_id_port_id(i,0);//tmp_num代表当前端口有多少条流表,m代表端口号
		tmp_num += get_port_num_by_sw_id_port_id(i,1);
		tmp_num += get_port_num_by_sw_id_port_id(i,2);
		tmp_num += get_port_num_by_sw_id_port_id(i,3);
		printf("sw%d %x:%x:%x:%x:%x:%x\n",i,g_res.device[i].cfg.mac0[0],g_res.device[i].cfg.mac0[1],g_res.device[i].cfg.mac0[2],g_res.device[i].cfg.mac0[3],g_res.device[i].cfg.mac0[4],g_res.device[i].cfg.mac0[5]);
		printf("sw%d %x:%x:%x:%x:%x:%x\n",i,g_res.device[i].cfg.mac1[0],g_res.device[i].cfg.mac1[1],g_res.device[i].cfg.mac1[2],g_res.device[i].cfg.mac1[3],g_res.device[i].cfg.mac1[4],g_res.device[i].cfg.mac1[5]);
		printf("sw%d %x:%x:%x:%x:%x:%x\n",i,g_res.device[i].cfg.mac2[0],g_res.device[i].cfg.mac2[1],g_res.device[i].cfg.mac2[2],g_res.device[i].cfg.mac2[3],g_res.device[i].cfg.mac2[4],g_res.device[i].cfg.mac2[5]);
		printf("sw%d %x:%x:%x:%x:%x:%x\n",i,g_res.device[i].cfg.mac3[0],g_res.device[i].cfg.mac3[1],g_res.device[i].cfg.mac3[2],g_res.device[i].cfg.mac3[3],g_res.device[i].cfg.mac3[4],g_res.device[i].cfg.mac3[5]);
		
		printf("sw%d %x:%x:%x:%x:%x:%x\n",i,g_res.device[i].cfg.mac4[0],g_res.device[i].cfg.mac4[1],g_res.device[i].cfg.mac4[2],g_res.device[i].cfg.mac4[3],g_res.device[i].cfg.mac4[4],g_res.device[i].cfg.mac4[5]);
		printf("sw%d %x:%x:%x:%x:%x:%x\n",i,g_res.device[i].cfg.mac5[0],g_res.device[i].cfg.mac5[1],g_res.device[i].cfg.mac5[2],g_res.device[i].cfg.mac5[3],g_res.device[i].cfg.mac5[4],g_res.device[i].cfg.mac5[5]);
		printf("sw%d %x:%x:%x:%x:%x:%x\n",i,g_res.device[i].cfg.mac6[0],g_res.device[i].cfg.mac6[1],g_res.device[i].cfg.mac6[2],g_res.device[i].cfg.mac6[3],g_res.device[i].cfg.mac6[4],g_res.device[i].cfg.mac6[5]);
		printf("sw%d %x:%x:%x:%x:%x:%x\n",i,g_res.device[i].cfg.mac7[0],g_res.device[i].cfg.mac7[1],g_res.device[i].cfg.mac7[2],g_res.device[i].cfg.mac7[3],g_res.device[i].cfg.mac7[4],g_res.device[i].cfg.mac7[5]);
		printf("sw%d %x:%x:%x:%x:%x:%x\n",i,g_res.device[i].cfg.mac8[0],g_res.device[i].cfg.mac8[1],g_res.device[i].cfg.mac8[2],g_res.device[i].cfg.mac8[3],g_res.device[i].cfg.mac8[4],g_res.device[i].cfg.mac8[5]);
		printf("sw%d %x:%x:%x:%x:%x:%x\n",i,g_res.device[i].cfg.mac9[0],g_res.device[i].cfg.mac9[1],g_res.device[i].cfg.mac9[2],g_res.device[i].cfg.mac9[3],g_res.device[i].cfg.mac9[4],g_res.device[i].cfg.mac9[5]);
		printf("sw%d %x:%x:%x:%x:%x:%x\n",i,g_res.device[i].cfg.mac10[0],g_res.device[i].cfg.mac10[1],g_res.device[i].cfg.mac10[2],g_res.device[i].cfg.mac10[3],g_res.device[i].cfg.mac10[4],g_res.device[i].cfg.mac10[5]);
		printf("sw%d %x:%x:%x:%x:%x:%x\n",i,g_res.device[i].cfg.mac11[0],g_res.device[i].cfg.mac11[1],g_res.device[i].cfg.mac11[2],g_res.device[i].cfg.mac11[3],g_res.device[i].cfg.mac11[4],g_res.device[i].cfg.mac11[5]);
		printf("\n");
	}
	TSN_DBG("*************************************************\n");
}
#endif
