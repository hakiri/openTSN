/***************************************************************************
 *            topology_mgr.c
 *
 *  Wed May 08 11:28:29 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * topology_mgr.c
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
#include "topology_mgr.h"

#define BUF_LEN 1500

typedef enum
{
	NO_NODE = 0,
	NEW_NODE = 1,
}FLAG;

int node_flag = NO_NODE;

struct topology tp = {0};

static u8 master_id = 0;

static int insert_node_to_topology(struct switch_info *sw)
{
	if(tp.size == 0)//插入的为头结点
	{
		tp.head = sw;
		tp.tail = sw;
		sw->next = sw;
		sw->prev = sw;
	}
	else
	{
		sw->next = tp.head;
		sw->prev = tp.tail;
		tp.head->prev = sw;
		tp.tail->next = sw;
		tp.tail = sw;
	}
	tp.size++;
	return G_OK;
}

static int build_and_send_topology_info()
{
#if 0
	struct switch_info *sw = tp.head;
	int i = 0;
	cJSON *tp_json = NULL, *ele_json = NULL;
	char str_sw_id[8] = {0};
	char str_sync_type[8] = {0};
	char str_host_id[8] = {0};
	char str_sw_mac[20] = {0};
	char str_host_mac[20] = {0};
	char str_next_sw_id[8] = {0};
	char str_prev_sw_id[8] = {0};
	char str_next_port[8] = {0};
	char str_prev_port[8] = {0};
	char *data = NULL;
	u16 data_len = 0;

	tp_json = cJSON_CreateArray();

	for(i = 0; i < tp.size; i++)
	{
		sprintf(str_sw_id, "%x", sw->sw_id);
		if(sw->sync_type == MASTER)
			sprintf(str_sync_type, "%s", "master");
		else
			sprintf(str_sync_type, "%s", "slave");
		sprintf(str_host_id, "%x", sw->host_id);
		sprintf(str_sw_mac, "%x:%x:%x:%x:%x:%x", sw->sw_mac[0], sw->sw_mac[1], sw->sw_mac[2], \
		        sw->sw_mac[3], sw->sw_mac[4], sw->sw_mac[5]);
		sprintf(str_host_mac, "%x:%x:%x:%x:%x:%x", sw->host_mac[0], sw->host_mac[1], sw->host_mac[2], \
		        sw->host_mac[3], sw->host_mac[4], sw->host_mac[5]);
		sprintf(str_next_sw_id, "%x", sw->next->sw_id);
		sprintf(str_prev_sw_id, "%x", sw->prev->sw_id);
		sprintf(str_next_port, "%x", sw->next_port);
		sprintf(str_prev_port, "%x", sw->prev_port);

		ele_json = cJSON_CreateObject();
		cJSON_AddStringToObject(ele_json, "sw_id", str_sw_id);
		cJSON_AddStringToObject(ele_json, "sync_type", str_sync_type);
		cJSON_AddStringToObject(ele_json, "host_id", str_host_id);
		cJSON_AddStringToObject(ele_json, "sw_mac", str_sw_mac);
		cJSON_AddStringToObject(ele_json, "host_mac", str_host_mac);
		cJSON_AddStringToObject(ele_json, "next_sw_id", str_next_sw_id);
		cJSON_AddStringToObject(ele_json, "prev_sw_id", str_prev_sw_id);
		cJSON_AddStringToObject(ele_json, "next_port", str_next_port);
		cJSON_AddStringToObject(ele_json, "prev_port", str_prev_port);

		cJSON_AddItemToArray(tp_json, ele_json);
		sw = sw->next;
	}
	data = cJSON_Print(tp_json);
	data_len = strlen(data);
	TSN_DBG("build topology json len: %d, info: %s\n", data_len, data);
	send_north_packet(TOPOLOGY_INFO, data, data_len);
	free(data);
	data = NULL;
	cJSON_Delete(tp_json);
	return G_OK;	
	#endif
}

int init_topology()
{
	FILE *fp = NULL;
	int i = 0;
	char buf[BUF_LEN];
	char *save_pstr = NULL;
	char *fir_pstr = NULL;
	char *sec_pstr = NULL;
	u8 temp_mac[MAC_LEN] = {0};
	u32 cur_num = 0;
	struct switch_info *sw = NULL;

	fp = fopen(TOPOLOGY_FILE, "r");
	if(fp == NULL)
	{
		TSN_ERR("Could not read topology file!\n");
		return G_ERR;
	}
	TSN_DBG("Open topology file successfully!\n");
	while(fgets(buf, BUF_LEN, fp) != NULL)
	{
		TSN_DBG("line content: %s", buf);
		riprt(buf);
		if(node_flag == NO_NODE)
		{
			if(!strcmp(buf, "{"))
			{
				TSN_DBG("Find a new node!\n");
				sw = (struct switch_info *)malloc(sizeof(struct switch_info));
				memset(sw, 0, sizeof(struct switch_info));
				node_flag = NEW_NODE;
			}
			else
				continue;
		}

		else if(node_flag == NEW_NODE)
		{
			if(!strcmp(buf, "}"))
			{
				insert_node_to_topology(sw);
				node_flag = NO_NODE;
				TSN_DBG("sw_id: %d\n", tp.tail->sw_id);
				TSN_DBG("host_id: %d\n", tp.tail->host_id);
				TSN_DBG("sw_mac: %x\n", *(tp.tail->sw_mac));
				TSN_DBG("host_mac: %x\n", *(tp.tail->host_mac));
				TSN_DBG("next_port: %d\n", tp.tail->next_port);
				TSN_DBG("prev_port: %d\n", tp.tail->prev_port);
			}
			else
			{
				fir_pstr = strtok_r(buf, ":", &sec_pstr);
				if(!strcmp(fir_pstr + 1, "sw_id"))
					sw->sw_id = atoi(sec_pstr);
				else if(!strcmp(fir_pstr + 1, "sync_type"))
				{
					if(!strcmp(sec_pstr, "master"))
					{
						sw->sync_type = MASTER;
						master_id = sw->sw_id;
						TSN_DBG("sw[%d] is master!\n", sw->sw_id);
					}
					else
					{
						sw->sync_type = SLAVE;
						TSN_DBG("sw[%d] is slave!\n", sw->sw_id);
					}
				}
				else if(!strcmp(fir_pstr + 1, "host_id"))
					sw->host_id = atoi(sec_pstr);
				else if(!strcmp(fir_pstr + 1, "sw_mac"))
				{
					trans_str_mac_to_hex_mac(sec_pstr, temp_mac);
					memcpy(sw->sw_mac, temp_mac, MAC_LEN);
				}
				else if(!strcmp(fir_pstr + 1, "host_mac"))
				{
					trans_str_mac_to_hex_mac(sec_pstr, temp_mac);
					memcpy(sw->host_mac, temp_mac, MAC_LEN);
				}
				else if(!strcmp(fir_pstr + 1, "next_port"))
					sw->next_port = atoi(sec_pstr);
				else if(!strcmp(fir_pstr + 1, "prev_port"))
					sw->prev_port = atoi(sec_pstr);
				else
					continue;
			}
		}		
		else
		{
			continue;
		}	
	}

	build_and_send_topology_info();
	return G_OK;
}

void print_topology()
{
	struct switch_info *sw = tp.head;
	int i = 0;
	for(i = 0; i < tp.size; i++)
	{
		TSN_DBG("*************************************************\n");
		TSN_DBG("sw_id %d\n",sw->sw_id);
		TSN_DBG("sw_id %d\n",sw->sync_type);
		TSN_DBG("host_id %d\n",sw->host_id);
		TSN_DBG("sw_mac %x:%x:%x:%x:%x:%x\n",sw->sw_mac[0],sw->sw_mac[1],sw->sw_mac[2],sw->sw_mac[3],sw->sw_mac[4],sw->sw_mac[5]);
		TSN_DBG("host_mac %x:%x:%x:%x:%x:%x\n",sw->host_mac[0],sw->host_mac[1],sw->host_mac[2],sw->host_mac[3],sw->host_mac[4],sw->host_mac[5]);
		TSN_DBG("prev_port %d\n",sw->prev_port);
		TSN_DBG("next_port %d\n",sw->next_port);
		TSN_DBG("*************************************************\n");
		sw = sw->next;
	}
}

u8 *get_sw_mac_by_sw_id(u8 sw_id)
{
	struct switch_info *sw = tp.head;
	int i = 0;
	for(i = 0; i < tp.size; i++)
	{
		if(sw->sw_id == sw_id)
		{
			return sw->sw_mac;
		}
		else
			sw = sw->next;
	}
	TSN_ERR("Could not find sw_id\n");
	return NULL;
}

u8 get_sw_id_by_host_id(u8 host_id)
{
	struct switch_info *sw = tp.head;
	int i = 0;
	for(i = 0; i < tp.size; i++)
	{
		if(sw->host_id == host_id)
			return sw->sw_id;
		else
			sw = sw->next;
	}
	TSN_ERR("Could not find host_id\n");
	return G_ERR;
}

u8 get_sw_id_by_sw_mac(u8 *sw_mac)
{
	struct switch_info *sw = tp.head;
	int i = 0;
	for(i = 0; i < tp.size; i++)
	{
		if(memcmp(sw_mac, sw->sw_mac, 6) == 0)
			return sw->sw_id;
		else
			sw = sw->next;
	}
	TSN_ERR("Could not find sw_mac\n");
	return G_ERR;
}

u8 *get_host_mac_by_host_id(u8 host_id)
{
	struct switch_info *sw = tp.head;
	int i = 0;
	for(i = 0; i < tp.size; i++)
	{
		if(sw->host_id == host_id)
		{
			return sw->host_mac;
		}
		else
			sw = sw->next;
	}
	TSN_ERR("Could not find host_id\n");
	return NULL;
}

u8 *get_host_mac_by_sw_id(u8 sw_id)
{
	struct switch_info *sw = tp.head;
	int i = 0;
	TSN_DBG("@sw_id:%d\n",sw_id);
	for(i = 0; i < tp.size; i++)
	{
		TSN_DBG("!sw->sw_id:%d\n",sw->sw_id);
		if(sw->sw_id == sw_id)
		{
			return sw->host_mac;
		}
		else
			sw = sw->next;
	}
	TSN_ERR("Could not find sw_id\n");
	return NULL;
}

u8 get_sw_id_by_host_mac(u8 *host_mac)
{
	struct switch_info *sw = tp.head;
	int i = 0;

	TSN_DBG("%x:%x:%x:%x:%x:%x\n", host_mac[0], host_mac[1], host_mac[2], host_mac[3], host_mac[4], host_mac[5]);
	for(i = 0; i < tp.size; i++)
	{
		if(memcmp(host_mac, sw->host_mac, 6) == 0)
			return sw->sw_id;
		else
			sw = sw->next;
	}
	TSN_ERR("Could not find host_mac\n");
	return G_ERR;
}

u32 get_sw_num()
{
	return tp.size;
}

int get_device_type_id_by_mac(u8 *mac, u8 *type, u8 *id)
{
	struct switch_info *sw = tp.head;
	int i = 0;
	for(i = 0; i < tp.size; i++)
	{
		if(memcmp(mac, sw->sw_mac, 6) == 0)
		{
			*type = SWITCH;
			*id = sw->sw_id;
			return G_OK;
		}
		else if(memcmp(mac, sw->host_mac, 6) == 0)
		{
			*type = HOST;
			*id = sw->host_id;
			return G_OK;
		}
		else
			sw = sw->next;
	}
	TSN_ERR("Could not find mac\n");
	return G_ERR;
}

u8 get_master_id()
{
	return master_id;
}