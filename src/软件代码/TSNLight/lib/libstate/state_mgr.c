/***************************************************************************
 *            state_mgr.c
 *
 *  Thu May 09 10:36:20 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * state_mgr.c
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
#include "state_mgr.h"

u32 flag_seq;


struct state_list s_list = {0};
static u8 master_id = 0;

int init_state_list()
{
	pthread_mutex_init(&(s_list.mutex), NULL);
	master_id = get_master_id();
	return G_OK;
}

static int insert_node_to_state_list(struct state_node *node)
{
	if(s_list.head == NULL)
	{
		s_list.head = node;
		s_list.tail = node;
		s_list.size++;
	}
	else
	{
		s_list.tail->next = node;
		s_list.tail = node;
		s_list.size++;
	}

	return G_OK;
}

void delete_node_from_state_list(struct state_node *node)
{
	struct state_node *pre_node = NULL;
	struct state_node *cur_node = s_list.head;

	if(s_list.size == 1)
	{
		s_list.head = NULL;
		s_list.tail = NULL;
		TSN_DBG("there is only one node!\n");
	}
	else
	{
		while(cur_node->seq != node->seq)
		{
			pre_node = cur_node;
			cur_node = cur_node->next;
		}
		if(node == s_list.head)
		{
			TSN_DBG("node is head\n");
			s_list.head = node->next;
		}
		else if(node == s_list.tail)
		{
			TSN_DBG("node is tail\n");
			s_list.tail = pre_node;
			s_list.tail->next = NULL;
		}
		else
		{
			TSN_DBG("node is middle!\n");
			pre_node->next = node->next;
		}
	}
	free(node);
	s_list.size--;
}

static struct state_node *lookup_state_node_by_seq(u32 seq)
{
	struct state_node *node = s_list.head;
	int i = 0;
	while(node != NULL)
	{
		if(node->seq == seq)
		{
			TSN_DBG("the position of node is %d\n", i);
			return node;
		}
		node = node->next;
		i++;
	}
	return NULL;
}

static void build_send_link_fault_info(u8 src_sw_id, u8 dst_sw_id)
{
	int i = 0;
	cJSON *link_json = NULL;
	char *data = NULL;
	u16 data_len = 0;
	char str_src_sw_id[8] = {0};
	char str_dst_sw_id[8] = {0};

	link_json = cJSON_CreateObject();
	sprintf(str_src_sw_id, "%x", src_sw_id);
	sprintf(str_dst_sw_id, "%x", dst_sw_id);
	cJSON_AddStringToObject(link_json, "src_sw_id", str_src_sw_id);
	cJSON_AddStringToObject(link_json, "dst_sw_id", str_dst_sw_id);

	data = cJSON_Print(link_json);
	data_len = strlen(data);
	TSN_DBG("build link fault json len: %d, info: %s\n", data_len, data);
	send_north_packet(LINK_ERR, data, data_len);
	free(data);
	data = NULL;
	cJSON_Delete(link_json);
}


#if TOPOLOGY_TYPE==0
void build_and_send_cnt_info(struct state_node *node)
{
	int i = 0;
	cJSON *cnt_json = NULL, *nodes_json = NULL, *ele_json = NULL;
	char *data = NULL;
	u16 data_len = 0;
	char str_seq[8] = {0};
	char str_sw_id[8] = {0};
	char str_esw_pkt_in_cnt[20] = {0};
	char str_esw_pkt_out_cnt[20] = {0};
	char str_buf_id_rest_cnt[20] = {0};
	char str_eos_md_in_cnt[20] = {0};
	char str_eos_md_out_cnt[20] = {0};
	char str_eos_q0_used_cnt[8] = {0};
	char str_eos_q1_used_cnt[8] = {0};
	char str_eos_q2_used_cnt[8] = {0};
	char str_eos_q3_used_cnt[8] = {0};
	char str_goe_pkt_in_cnt[20] = {0};
	char str_goe_port0_out_cnt[20] = {0};
	char str_goe_port1_out_cnt[20] = {0};
	char str_goe_discard_cnt[20] = {0};

	cnt_json = cJSON_CreateObject();
	nodes_json = cJSON_CreateArray();

	for(i = 0; i < get_sw_num(); i++)
	{
		if(node->sw_state[i].state == OFFLINE)
			continue;
		
		sprintf(str_sw_id, "%x", node->sw_state[i].id);
		sprintf(str_esw_pkt_in_cnt, "%llx", ntohll(node->sw_state[i].status.esw_pkt_in_cnt));
		sprintf(str_esw_pkt_out_cnt, "%llx", ntohll(node->sw_state[i].status.esw_pkt_out_cnt));
		sprintf(str_buf_id_rest_cnt, "%x", node->sw_state[i].status.buf_id_rest_cnt);
		sprintf(str_eos_md_in_cnt, "%llx", ntohll(node->sw_state[i].status.eos_md_in_cnt));
		sprintf(str_eos_md_out_cnt, "%llx", ntohll(node->sw_state[i].status.eos_md_out_cnt));
		sprintf(str_eos_q0_used_cnt, "%x", node->sw_state[i].status.eos_q0_used_cnt);
		sprintf(str_eos_q1_used_cnt, "%x", node->sw_state[i].status.eos_q1_used_cnt);
		sprintf(str_eos_q2_used_cnt, "%x", node->sw_state[i].status.eos_q2_used_cnt);
		sprintf(str_eos_q3_used_cnt, "%x", node->sw_state[i].status.eos_q3_used_cnt);
		sprintf(str_goe_pkt_in_cnt, "%llx", ntohll(node->sw_state[i].status.goe_pkt_in_cnt));
		sprintf(str_goe_port0_out_cnt, "%llx", ntohll(node->sw_state[i].status.goe_port0_out_cnt));
		sprintf(str_goe_port1_out_cnt, "%llx", ntohll(node->sw_state[i].status.goe_port1_out_cnt));
		sprintf(str_goe_discard_cnt, "%llx", ntohll(node->sw_state[i].status.goe_discard_cnt));

		ele_json = cJSON_CreateObject();
		cJSON_AddStringToObject(ele_json, "sw_id", str_sw_id);
		cJSON_AddStringToObject(ele_json, "esw_pkt_in_cnt", str_esw_pkt_in_cnt);
		cJSON_AddStringToObject(ele_json, "esw_pkt_out_cnt", str_esw_pkt_out_cnt);
		cJSON_AddStringToObject(ele_json, "buf_id_rest_cnt", str_buf_id_rest_cnt);
		cJSON_AddStringToObject(ele_json, "eos_md_in_cnt", str_eos_md_in_cnt);
		cJSON_AddStringToObject(ele_json, "eos_md_out_cnt", str_eos_md_out_cnt);
		cJSON_AddStringToObject(ele_json, "eos_q0_used_cnt", str_eos_q0_used_cnt);
		cJSON_AddStringToObject(ele_json, "eos_q1_used_cnt", str_eos_q1_used_cnt);
		cJSON_AddStringToObject(ele_json, "eos_q2_used_cnt", str_eos_q2_used_cnt);
		cJSON_AddStringToObject(ele_json, "eos_q3_used_cnt", str_eos_q3_used_cnt);
		cJSON_AddStringToObject(ele_json, "goe_pkt_in_cnt", str_goe_pkt_in_cnt);
		cJSON_AddStringToObject(ele_json, "goe_port0_out_cnt", str_goe_port0_out_cnt);
		cJSON_AddStringToObject(ele_json, "goe_port1_out_cnt", str_goe_port1_out_cnt);
		cJSON_AddStringToObject(ele_json, "goe_discard_cnt", str_goe_discard_cnt);

		cJSON_AddItemToArray(nodes_json, ele_json);
	}

	sprintf(str_seq, "%x", node->seq);
	cJSON_AddStringToObject(cnt_json, "seq", str_seq);
	cJSON_AddItemToObject(cnt_json, "cnt_info", nodes_json);
	
	data = cJSON_Print(cnt_json);
	data_len = strlen(data);
	TSN_DBG("build cnt json len: %d, info: %s\n", data_len, data);
	send_north_packet(SW_CNT_INFO, data, data_len);
	free(data);
	data = NULL;
	cJSON_Delete(cnt_json);
}
//u64 precise = 0; //统计历史记录中同步精度最大值
void build_and_send_sync_info(struct state_node *node)
{

	u8 buf[50] = {0};

	int i = 0;
	u64 ref_time = 0;
	u64 precise = 0;
	cJSON *sync_json = NULL, *nodes_json = NULL, *ele_json = NULL;
	char *data = NULL;
	u16 data_len = 0;
	char str_seq[8] = {0};
	char str_device_type[10] = {0};
	char str_device_id[8] = {0};
	char str_time_offset[32] = {0};  //by sxt  2019-06-19
	char str_sync_precision[32] = {0}; //by sxt  2019-06-19


	if(node->sw_state[master_id].state == OFFLINE)
	{
		TSN_ERR("TSN controller did not receive the master[%d] sync time!\n", master_id);
		return;
	}

	sync_json = cJSON_CreateObject();
	nodes_json = cJSON_CreateArray();

	ref_time = node->sw_state[master_id].sync;
	TSN_DBG("build_and_send_sync_info: :ref_time:%llx!\n",  node->sw_state[master_id].sync);
	for(i = 0; i < get_sw_num(); i++)
	{
		/*
		if(node->sw_state[i].state == OFFLINE)
			continue;
		*/
		TSN_DBG("build_and_send_sync_info  before: device type:%s, time_offset:%llx!\n", "switch", node->sw_state[i].sync);
		node->sw_state[i].sync = node->sw_state[i].sync > ref_time ? node->sw_state[i].sync - ref_time: \
			ref_time - node->sw_state[i].sync;


		if(node->sw_state[i].sync > precise)
			precise = node->sw_state[i].sync;

		TSN_DBG("build_and_send_sync_info: : device type:%s, device id:%d!\n", "switch",node->sw_state[i].id);
		TSN_DBG("build_and_send_sync_info: : device type:%s, device id:%x!\n", "switch", node->sw_state[i].id);
		TSN_DBG("build_and_send_sync_info: : device type:%s, time_offset:%llx!\n", "switch", node->sw_state[i].sync);
		sprintf(str_device_id, "%x", node->sw_state[i].id);
		sprintf(str_time_offset, "%llx", node->sw_state[i].sync);

		if(node->sw_state[i].id == 0)
		{
			printf("sw[0]=%lld\n", node->sw_state[i].sync * 8);
			//sprintf(buf, "%lld\n", node->sw_state[i].sync * 8);//修改记录文本的格式
			//fwrite(buf, sizeof(u8), strlen(buf), fp_0);
		}
		else if(node->sw_state[i].id == 1)
		{
			printf("sw[1]=%lld\n", node->sw_state[i].sync * 8);
			//sprintf(buf, "%lld\n", node->sw_state[i].sync * 8);//修改记录文本的格式
			//fwrite(buf, sizeof(u8), strlen(buf), fp_1);
		}
		else if(node->sw_state[i].id == 2)
		{
			printf("sw[2]=%lld\n", node->sw_state[i].sync * 8);
			//sprintf(buf, "%lld\n", node->sw_state[i].sync * 8);//修改记录文本的格式
			//fwrite(buf, sizeof(u8), strlen(buf), fp_2);
		}
		else if(node->sw_state[i].id == 3)
		{
			printf("sw[3]=%lld\n", node->sw_state[i].sync * 8);
			//sprintf(buf, "%lld\n", node->sw_state[i].sync * 8);//修改记录文本的格式
			//fwrite(buf, sizeof(u8), strlen(buf), fp_3);
		}
		else if(node->sw_state[i].id == 4)
		{
			printf("sw[4]=%lld\n", node->sw_state[i].sync * 8);
			//sprintf(buf, "%lld\n", node->sw_state[i].sync * 8);//修改记录文本的格式
			//fwrite(buf, sizeof(u8), strlen(buf), fp_4);
		}
		else if(node->sw_state[i].id == 5)
		{
			printf("sw[5]=%lld\n", node->sw_state[i].sync * 8);
			//sprintf(buf, "%lld\n", node->sw_state[i].sync * 8);//修改记录文本的格式
			//fwrite(buf, sizeof(u8), strlen(buf), fp_5);
		}




		ele_json = cJSON_CreateObject();
		cJSON_AddStringToObject(ele_json, "device_type", "switch");
		cJSON_AddStringToObject(ele_json, "device_id", str_device_id);
		cJSON_AddStringToObject(ele_json, "time_offset", str_time_offset);

		cJSON_AddItemToArray(nodes_json, ele_json);
	}

	for(i = 0; i < get_sw_num(); i++)
	{
		/*
		if(node->host_state[i].state == OFFLINE)
			continue;
		*/
		node->host_state[i].sync = node->host_state[i].sync > ref_time ? node->host_state[i].sync - ref_time: \
			ref_time - node->host_state[i].sync;

		
		sprintf(str_device_id, "%x", node->host_state[i].id);
		sprintf(str_time_offset, "%llx", node->host_state[i].sync);

		ele_json = cJSON_CreateObject();
		cJSON_AddStringToObject(ele_json, "device_type", "host");
		cJSON_AddStringToObject(ele_json, "device_id", str_device_id);
		cJSON_AddStringToObject(ele_json, "time_offset", str_time_offset);

		cJSON_AddItemToArray(nodes_json, ele_json);
	}

	sprintf(str_seq, "%x", node->seq);
	cJSON_AddStringToObject(sync_json, "seq", str_seq);
	cJSON_AddItemToObject(sync_json, "sync_state", nodes_json);
	sprintf(str_sync_precision, "%llx", precise);
	cJSON_AddStringToObject(sync_json, "sync_precision", str_sync_precision);
	
	data = cJSON_Print(sync_json);
	data_len = strlen(data);
	TSN_DBG("build sync json len: %d, info: %s\n", data_len, data);
	send_north_packet(CLOCK_SYNC, data, data_len);//send sync json
	free(data);
	data = NULL;
	cJSON_Delete(sync_json);

}


struct state_node *ptp_packet_state_process(struct ptp_packet *pkt, u64 _recv_time)
{

	u32 sw_num = 0;
	u32 seq = pkt->seq;
	struct state_node *node = NULL;
	struct timeval cur_time;
	u8 dev_type = 0;
	u8 dev_id = 0;
	u8 hop_num = 0;
	TSN_DBG("_recv_time:%llx,timestamp_l_17:%llx, timestamp_h_32: %llx\n", _recv_time,_recv_time&0x1ffff, _recv_time>>17);
	u64 recv_time = (_recv_time>>17)*125000+(_recv_time&0x1ffff);
	u64 trans_clock = ntohll(pkt->correct_field);
	int i = 0;

	gettimeofday(&cur_time, NULL);
	if(G_ERR ==get_device_type_id_by_mac(pkt->src_mac, &dev_type, &dev_id))
		TSN_DBG("could not find mac\n");
	
	TSN_DBG("Insert ptp to state list: seq: %d, device type:%d, device id:%d!\n", seq, dev_type, dev_id);
	TSN_DBG("recv_time:%lld, trans_clock: %lld\n", recv_time, trans_clock);


	//printf("trans_clock=%lld\n",trans_clock);
	printf("recv_time=%llu\n",recv_time);
	u64 tmp_timestamp = 0;
	u64 tmp = 0;
	tmp = ntohs(pkt->timestamp[2]);
	tmp_timestamp = tmp << 32;
	tmp = ntohs(pkt->timestamp[3]);
	tmp_timestamp = tmp_timestamp + tmp<<16;
	tmp = ntohs(pkt->timestamp[4]);
	tmp_timestamp = tmp_timestamp + tmp;

	printf("tmp_timestamp=%lld\n",tmp_timestamp);


	if(flag_seq == 0)
	{
		node = NULL;
		flag_seq++;
	}
	else
		node = s_list.head;
	//node = lookup_state_node_by_seq(seq);
	//node = NULL;
	if(node == NULL)
	{
		TSN_DBG("Could not find node, Create one!\n");
		node = (struct state_node *)malloc(sizeof(struct state_node));
		memset((u8 *)node, 0, sizeof(struct state_node));
		node->seq = seq;
		node->create_time = cur_time;
		insert_node_to_state_list(node);
		TSN_DBG("s_list size: %d\n", s_list.size);
	}
	sw_num = get_sw_num();
	if(dev_type == SWITCH)
	{
		node->cur_sw_num++;
		node->sw_state[dev_id].id = dev_id;
		node->sw_state[dev_id].state = ONLINE;
		hop_num = dev_id+1;

		if(dev_id == 0)
			node->sw_state[dev_id].sync = (recv_time - trans_clock);
		/*
		else
			node->sw_state[dev_id].sync = (recv_time - trans_clock - dev_id*90);
		*/

		else
			if(pkt->cfg.direction == 0)
				node->sw_state[dev_id].sync = (recv_time - trans_clock - dev_id*89 -dev_id );
			else
				node->sw_state[dev_id].sync = (recv_time - trans_clock - (sw_num-dev_id)*89 - (sw_num - dev_id) );

		TSN_DBG("dev_id:%d,recv_time:%lld, COMPENSATE_TIME:%lld,trans_clock: %lld\n", dev_id,recv_time, COMPENSATE_TIME,trans_clock);
		memcpy(&(node->sw_state[dev_id].status), &(pkt->status), sizeof(struct status_info));
		TSN_DBG("ptp from switch, sw num: %d\n", node->cur_sw_num);
	}
	else
	{
		node->cur_host_num++;
		node->host_state[dev_id].id = dev_id;
		node->host_state[dev_id].state = ONLINE;
		node->host_state[dev_id].sync = recv_time + COMPENSATE_TIME - trans_clock;
		TSN_DBG("ptp from host, host num: %d\n", node->cur_host_num);
	}

	return node;
}


#elif TOPOLOGY_TYPE==1

void build_and_send_cnt_info(struct state_node *node)
{
	int i = 0;
	cJSON *cnt_json = NULL, *nodes_json = NULL, *ele_json = NULL;
	char *data = NULL;
	u16 data_len = 0;
	char str_seq[8] = {0};
	char str_sw_id[8] = {0};
	char str_esw_pkt_in_cnt[20] = {0};
	char str_esw_pkt_out_cnt[20] = {0};
	char str_buf_id_rest_cnt[20] = {0};
	char str_eos_md_in_cnt[20] = {0};
	char str_eos_md_out_cnt[20] = {0};
	
	char str_eos_q0_used_cnt_0[8] = {0};
	char str_eos_q1_used_cnt_0[8] = {0};
	char str_eos_q2_used_cnt_0[8] = {0};
	char str_eos_q3_used_cnt_0[8] = {0};

	char str_eos_q0_used_cnt_1[8] = {0};
	char str_eos_q1_used_cnt_1[8] = {0};
	char str_eos_q2_used_cnt_1[8] = {0};
	char str_eos_q3_used_cnt_1[8] = {0};

	char str_eos_q0_used_cnt_2[8] = {0};
	char str_eos_q1_used_cnt_2[8] = {0};
	char str_eos_q2_used_cnt_2[8] = {0};
	char str_eos_q3_used_cnt_2[8] = {0};

	char str_eos_q0_used_cnt_3[8] = {0};
	char str_eos_q1_used_cnt_3[8] = {0};
	char str_eos_q2_used_cnt_3[8] = {0};
	char str_eos_q3_used_cnt_3[8] = {0};

	
	char str_goe_pkt_in_cnt[20] = {0};
	char str_goe_port0_out_cnt[20] = {0};
	char str_goe_port1_out_cnt[20] = {0};
	char str_goe_discard_cnt[20] = {0};

	cnt_json = cJSON_CreateObject();
	nodes_json = cJSON_CreateArray();

	for(i = 0; i < get_sw_num(); i++)
	{
		if(node->sw_state[i].state == OFFLINE)
			continue;
		/*ljs修改测试2019.9.20*/
		sprintf(str_sw_id, "%x", node->sw_state[i].id);
		sprintf(str_esw_pkt_in_cnt, "%llx", ntohll(node->sw_state[i].status.esw_pkt_in_cnt));
		/*
		sprintf(str_esw_pkt_out_cnt, "%llx", ntohll(node->sw_state[i].status.esw_pkt_out_cnt));
		sprintf(str_buf_id_rest_cnt, "%x", node->sw_state[i].status.buf_id_rest_cnt);
		sprintf(str_eos_md_in_cnt, "%llx", ntohll(node->sw_state[i].status.eos_md_in_cnt));
		sprintf(str_eos_md_out_cnt, "%llx", ntohll(node->sw_state[i].status.eos_md_out_cnt));
		*/
		
		sprintf(str_eos_q0_used_cnt_0, "%x", node->sw_state[i].status.eos_q0_used_cnt_0);
		sprintf(str_eos_q1_used_cnt_0, "%x", node->sw_state[i].status.eos_q1_used_cnt_0);
		sprintf(str_eos_q2_used_cnt_0, "%x", node->sw_state[i].status.eos_q2_used_cnt_0);
		sprintf(str_eos_q3_used_cnt_0, "%x", node->sw_state[i].status.eos_q3_used_cnt_0);

		sprintf(str_eos_q0_used_cnt_1, "%x", node->sw_state[i].status.eos_q0_used_cnt_1);
		sprintf(str_eos_q1_used_cnt_1, "%x", node->sw_state[i].status.eos_q1_used_cnt_1);
		sprintf(str_eos_q2_used_cnt_1, "%x", node->sw_state[i].status.eos_q2_used_cnt_1);
		sprintf(str_eos_q3_used_cnt_1, "%x", node->sw_state[i].status.eos_q3_used_cnt_1);

		sprintf(str_eos_q0_used_cnt_2, "%x", node->sw_state[i].status.eos_q0_used_cnt_2);
		sprintf(str_eos_q1_used_cnt_2, "%x", node->sw_state[i].status.eos_q1_used_cnt_2);
		sprintf(str_eos_q2_used_cnt_2, "%x", node->sw_state[i].status.eos_q2_used_cnt_2);
		sprintf(str_eos_q3_used_cnt_2, "%x", node->sw_state[i].status.eos_q3_used_cnt_2);

		sprintf(str_eos_q0_used_cnt_3, "%x", node->sw_state[i].status.eos_q0_used_cnt_3);
		sprintf(str_eos_q1_used_cnt_3, "%x", node->sw_state[i].status.eos_q1_used_cnt_3);
		sprintf(str_eos_q2_used_cnt_3, "%x", node->sw_state[i].status.eos_q2_used_cnt_3);
		sprintf(str_eos_q3_used_cnt_3, "%x", node->sw_state[i].status.eos_q3_used_cnt_3);

		/*
		sprintf(str_goe_pkt_in_cnt, "%llx", ntohll(node->sw_state[i].status.goe_pkt_in_cnt));
		sprintf(str_goe_port0_out_cnt, "%llx", ntohll(node->sw_state[i].status.goe_port0_out_cnt));
		sprintf(str_goe_port1_out_cnt, "%llx", ntohll(node->sw_state[i].status.goe_port1_out_cnt));
		sprintf(str_goe_discard_cnt, "%llx", ntohll(node->sw_state[i].status.goe_discard_cnt));
		*/
		ele_json = cJSON_CreateObject();
		cJSON_AddStringToObject(ele_json, "sw_id", str_sw_id);
		cJSON_AddStringToObject(ele_json, "esw_pkt_in_cnt", str_esw_pkt_in_cnt);
		cJSON_AddStringToObject(ele_json, "esw_pkt_out_cnt", str_esw_pkt_out_cnt);
		cJSON_AddStringToObject(ele_json, "buf_id_rest_cnt", str_buf_id_rest_cnt);
		cJSON_AddStringToObject(ele_json, "eos_md_in_cnt", str_eos_md_in_cnt);
		cJSON_AddStringToObject(ele_json, "eos_md_out_cnt", str_eos_md_out_cnt);
		
		cJSON_AddStringToObject(ele_json, "eos_q0_used_cnt_0", str_eos_q0_used_cnt_0);
		cJSON_AddStringToObject(ele_json, "eos_q1_used_cnt_0", str_eos_q1_used_cnt_0);
		cJSON_AddStringToObject(ele_json, "eos_q2_used_cnt_0", str_eos_q2_used_cnt_0);
		cJSON_AddStringToObject(ele_json, "eos_q3_used_cnt_0", str_eos_q3_used_cnt_0);

		cJSON_AddStringToObject(ele_json, "eos_q0_used_cnt_1", str_eos_q0_used_cnt_1);
		cJSON_AddStringToObject(ele_json, "eos_q1_used_cnt_1", str_eos_q1_used_cnt_1);
		cJSON_AddStringToObject(ele_json, "eos_q2_used_cnt_1", str_eos_q2_used_cnt_1);
		cJSON_AddStringToObject(ele_json, "eos_q3_used_cnt_1", str_eos_q3_used_cnt_1);

		cJSON_AddStringToObject(ele_json, "eos_q0_used_cnt_2", str_eos_q0_used_cnt_2);
		cJSON_AddStringToObject(ele_json, "eos_q1_used_cnt_2", str_eos_q1_used_cnt_2);
		cJSON_AddStringToObject(ele_json, "eos_q2_used_cnt_2", str_eos_q2_used_cnt_2);
		cJSON_AddStringToObject(ele_json, "eos_q3_used_cnt_2", str_eos_q3_used_cnt_2);

		cJSON_AddStringToObject(ele_json, "eos_q0_used_cnt_3", str_eos_q0_used_cnt_3);
		cJSON_AddStringToObject(ele_json, "eos_q1_used_cnt_3", str_eos_q1_used_cnt_3);
		cJSON_AddStringToObject(ele_json, "eos_q2_used_cnt_3", str_eos_q2_used_cnt_3);
		cJSON_AddStringToObject(ele_json, "eos_q3_used_cnt_3", str_eos_q3_used_cnt_3);
		
		cJSON_AddStringToObject(ele_json, "goe_pkt_in_cnt", str_goe_pkt_in_cnt);
		cJSON_AddStringToObject(ele_json, "goe_port0_out_cnt", str_goe_port0_out_cnt);
		cJSON_AddStringToObject(ele_json, "goe_port1_out_cnt", str_goe_port1_out_cnt);
		cJSON_AddStringToObject(ele_json, "goe_discard_cnt", str_goe_discard_cnt);

		cJSON_AddItemToArray(nodes_json, ele_json);
	}

	sprintf(str_seq, "%x", node->seq);
	cJSON_AddStringToObject(cnt_json, "seq", str_seq);
	cJSON_AddItemToObject(cnt_json, "cnt_info", nodes_json);
	
	data = cJSON_Print(cnt_json);
	data_len = strlen(data);
	TSN_DBG("build cnt json len: %d, info: %s\n", data_len, data);
	//printf("build cnt json len: %d, info: %s\n", data_len, data);
	send_north_packet(SW_CNT_INFO, data, data_len);
	free(data);
	data = NULL;
	cJSON_Delete(cnt_json);
}

void build_and_send_sync_info(struct state_node *node)
{

#if 0
	u8 buf[50] = {0};

	int i = 0;
	u64 ref_time = 0;
	u64 precise = 0;
	cJSON *sync_json = NULL, *nodes_json = NULL, *ele_json = NULL;
	char *data = NULL;
	u16 data_len = 0;
	char str_seq[8] = {0};
	char str_device_type[10] = {0};
	char str_device_id[8] = {0};
	char str_time_offset[32] = {0};  //by sxt  2019-06-19
	char str_sync_precision[32] = {0}; //by sxt  2019-06-19


	if(node->sw_state[master_id].state == OFFLINE)
	{
		TSN_ERR("TSN controller did not receive the master[%d] sync time!\n", master_id);
		return;
	}

	sync_json = cJSON_CreateObject();
	nodes_json = cJSON_CreateArray();

	ref_time = node->sw_state[master_id].sync;
	TSN_DBG("build_and_send_sync_info: :ref_time:%llx!\n",  node->sw_state[master_id].sync);
	for(i = 0; i < get_sw_num(); i++)
	{
		/*
		if(node->sw_state[i].state == OFFLINE)
			continue;
		*/
		TSN_DBG("build_and_send_sync_info  before: device type:%s, time_offset:%llx!\n", "switch", node->sw_state[i].sync);
		node->sw_state[i].sync = node->sw_state[i].sync > ref_time ? node->sw_state[i].sync - ref_time: \
			ref_time - node->sw_state[i].sync;


		if(node->sw_state[i].sync > precise)
			precise = node->sw_state[i].sync;

		TSN_DBG("build_and_send_sync_info: : device type:%s, device id:%d!\n", "switch",node->sw_state[i].id);
		TSN_DBG("build_and_send_sync_info: : device type:%s, device id:%x!\n", "switch", node->sw_state[i].id);
		TSN_DBG("build_and_send_sync_info: : device type:%s, time_offset:%llx!\n", "switch", node->sw_state[i].sync);
		sprintf(str_device_id, "%x", node->sw_state[i].id);
		sprintf(str_time_offset, "%llx", node->sw_state[i].sync);

		ele_json = cJSON_CreateObject();
		cJSON_AddStringToObject(ele_json, "device_type", "switch");
		cJSON_AddStringToObject(ele_json, "device_id", str_device_id);
		cJSON_AddStringToObject(ele_json, "time_offset", str_time_offset);

		cJSON_AddItemToArray(nodes_json, ele_json);
	}

	for(i = 0; i < get_sw_num(); i++)
	{

		node->host_state[i].sync = node->host_state[i].sync > ref_time ? node->host_state[i].sync - ref_time: \
			ref_time - node->host_state[i].sync;

		
		sprintf(str_device_id, "%x", node->host_state[i].id);
		sprintf(str_time_offset, "%llx", node->host_state[i].sync);

		ele_json = cJSON_CreateObject();
		cJSON_AddStringToObject(ele_json, "device_type", "host");
		cJSON_AddStringToObject(ele_json, "device_id", str_device_id);
		cJSON_AddStringToObject(ele_json, "time_offset", str_time_offset);

		cJSON_AddItemToArray(nodes_json, ele_json);
	}

	sprintf(str_seq, "%x", node->seq);
	cJSON_AddStringToObject(sync_json, "seq", str_seq);
	cJSON_AddItemToObject(sync_json, "sync_state", nodes_json);
	sprintf(str_sync_precision, "%llx", precise);
	cJSON_AddStringToObject(sync_json, "sync_precision", str_sync_precision);
	
	data = cJSON_Print(sync_json);
	data_len = strlen(data);
	TSN_DBG("build sync json len: %d, info: %s\n", data_len, data);
	send_north_packet(CLOCK_SYNC, data, data_len);//send sync json
	free(data);
	data = NULL;
	cJSON_Delete(sync_json);
#endif

	u8 buf[50] = {0};

	int i = 0;
	u64 ref_time = 0;
	u64 precise = 0;
	cJSON *sync_json = NULL, *nodes_json = NULL, *ele_json = NULL;
	char *data = NULL;
	u16 data_len = 0;
	char str_seq[8] = {0};
	char str_device_type[10] = {0};
	char str_device_id[8] = {0};
	char str_time_offset[32] = {0};  //by sxt  2019-06-19
	char str_sync_precision[32] = {0}; //by sxt  2019-06-19


	if(node->sw_state[master_id].state == OFFLINE)
	{
		TSN_ERR("TSN controller did not receive the master[%d] sync time!\n", master_id);
		return;
	}

	sync_json = cJSON_CreateObject();
	nodes_json = cJSON_CreateArray();

	ref_time = node->sw_state[master_id].sync;
	TSN_DBG("build_and_send_sync_info: :ref_time:%llx!\n",  node->sw_state[master_id].sync);
	for(i = 0; i < get_sw_num(); i++)
	{
		/*
		if(node->sw_state[i].state == OFFLINE)
			continue;
		*/
		TSN_DBG("build_and_send_sync_info  before: device type:%s, time_offset:%llx!\n", "switch", node->sw_state[i].sync);
		node->sw_state[i].sync = node->sw_state[i].sync > ref_time ? node->sw_state[i].sync - ref_time: \
			ref_time - node->sw_state[i].sync;


		if(node->sw_state[i].sync > precise)
			precise = node->sw_state[i].sync;

		TSN_DBG("build_and_send_sync_info: : device type:%s, device id:%d!\n", "switch",node->sw_state[i].id);
		TSN_DBG("build_and_send_sync_info: : device type:%s, device id:%x!\n", "switch", node->sw_state[i].id);
		TSN_DBG("build_and_send_sync_info: : device type:%s, time_offset:%llx!\n", "switch", node->sw_state[i].sync);
		sprintf(str_device_id, "%x", node->sw_state[i].id);
		sprintf(str_time_offset, "%llx", node->sw_state[i].sync);

		if(node->sw_state[i].id == 0)
		{
			printf("sw[0]=%lld\n", node->sw_state[i].sync * 8);
		}
		else if(node->sw_state[i].id == 1)
		{
			printf("sw[1]=%lld\n", node->sw_state[i].sync * 8);
		}
		else if(node->sw_state[i].id == 2)
		{
			printf("sw[2]=%lld\n", node->sw_state[i].sync * 8);
		}
		else if(node->sw_state[i].id == 3)
		{
			printf("sw[3]=%lld\n", node->sw_state[i].sync * 8);
		}
		else if(node->sw_state[i].id == 4)
		{
			printf("sw[4]=%lld\n", node->sw_state[i].sync * 8);
		}
		else if(node->sw_state[i].id == 5)
		{
			printf("sw[5]=%lld\n", node->sw_state[i].sync * 8);
		}




		ele_json = cJSON_CreateObject();
		cJSON_AddStringToObject(ele_json, "device_type", "switch");
		cJSON_AddStringToObject(ele_json, "device_id", str_device_id);
		cJSON_AddStringToObject(ele_json, "time_offset", str_time_offset);

		cJSON_AddItemToArray(nodes_json, ele_json);
	}

	for(i = 0; i < get_sw_num(); i++)
	{

		node->host_state[i].sync = node->host_state[i].sync > ref_time ? node->host_state[i].sync - ref_time: \
			ref_time - node->host_state[i].sync;

		sprintf(str_device_id, "%x", node->host_state[i].id);
		sprintf(str_time_offset, "%llx", node->host_state[i].sync);

		ele_json = cJSON_CreateObject();
		cJSON_AddStringToObject(ele_json, "device_type", "host");
		cJSON_AddStringToObject(ele_json, "device_id", str_device_id);
		cJSON_AddStringToObject(ele_json, "time_offset", str_time_offset);

		cJSON_AddItemToArray(nodes_json, ele_json);
	}

	sprintf(str_seq, "%x", node->seq);
	cJSON_AddStringToObject(sync_json, "seq", str_seq);
	cJSON_AddItemToObject(sync_json, "sync_state", nodes_json);
	sprintf(str_sync_precision, "%llx", precise);
	cJSON_AddStringToObject(sync_json, "sync_precision", str_sync_precision);
	
	data = cJSON_Print(sync_json);
	data_len = strlen(data);
	TSN_DBG("build sync json len: %d, info: %s\n", data_len, data);
	send_north_packet(CLOCK_SYNC, data, data_len);//send sync json
	free(data);
	data = NULL;
	cJSON_Delete(sync_json);



}



struct state_node *ptp_packet_state_process(struct ptp_packet *pkt, u64 _recv_time)
{

	u32 seq = pkt->seq;
	struct state_node *node = NULL;
	struct timeval cur_time;
	u8 dev_type = 0;
	u8 dev_id = 0;
	u8 hop_num = 0;
	TSN_DBG("_recv_time:%llx,timestamp_l_17:%llx, timestamp_h_32: %llx\n", _recv_time,_recv_time&0x1ffff, _recv_time>>17);
	u64 recv_time = (_recv_time>>17)*125000+(_recv_time&0x1ffff);
	u64 trans_clock = ntohll(pkt->correct_field);
	int i = 0;

	gettimeofday(&cur_time, NULL);
	if(G_ERR ==get_device_type_id_by_mac(pkt->src_mac, &dev_type, &dev_id))
		TSN_DBG("could not find mac\n");
	
	TSN_DBG("Insert ptp to state list: seq: %d, device type:%d, device id:%d!\n", seq, dev_type, dev_id);
	TSN_DBG("recv_time:%lld, trans_clock: %lld\n", recv_time, trans_clock);


	printf("trans_clock=%lld\n",trans_clock);
	printf("recv_time=%lld\n",recv_time);

/*
	if(flag_seq == 0)
	{
		node = NULL;
		flag_seq++;
	}
	else
		node = s_list.head;
*/
	node = s_list.head;

	if(node == NULL)
	{
		TSN_DBG("Could not find node, Create one!\n");
		node = (struct state_node *)malloc(sizeof(struct state_node));
		memset((u8 *)node, 0, sizeof(struct state_node));
		node->seq = seq;
		node->create_time = cur_time;
		insert_node_to_state_list(node);

		s_list.head = node;
		TSN_DBG("s_list size: %d\n", s_list.size);
	}

	if(dev_type == SWITCH)
	{
		node->cur_sw_num++;
		node->sw_state[dev_id].id = dev_id;
		node->sw_state[dev_id].state = ONLINE;
		hop_num = dev_id+1;


		if(dev_id == 0)
			node->sw_state[dev_id].sync = (recv_time - trans_clock);
		else if(dev_id == 1)
			node->sw_state[dev_id].sync = (recv_time - trans_clock - dev_id*89 - 2);
		else if(dev_id == 2)
			node->sw_state[dev_id].sync = (recv_time - trans_clock - dev_id*89 -2);
		else if(dev_id == 3)
			node->sw_state[dev_id].sync = (recv_time - trans_clock - 2*89 -2);
		else if(dev_id == 4)
			node->sw_state[dev_id].sync = (recv_time - trans_clock - 2*89 -2);
		else if(dev_id == 5)
			node->sw_state[dev_id].sync = (recv_time - trans_clock - 3*89 -3);
		
		TSN_DBG("dev_id:%d,recv_time:%lld, COMPENSATE_TIME:%lld,trans_clock: %lld\n", dev_id,recv_time, COMPENSATE_TIME,trans_clock);
		memcpy(&(node->sw_state[dev_id].status), &(pkt->status), sizeof(struct status_info));
		TSN_DBG("ptp from switch, sw num: %d\n", node->cur_sw_num);
#if 0
		if(node->cur_sw_num == get_sw_num())
		{
			TSN_DBG("cur sw num is equal to node num: start to compute sync info\n");
			build_and_send_sync_info(node);
			build_and_send_cnt_info(node);
			//build_send_link_fault_info(2, 3);
			delete_node_from_state_list(node);
			//flag_seq = 0;
			return G_OK;
		}
#endif
	}
	else
	{
		node->cur_host_num++;
		node->host_state[dev_id].id = dev_id;
		node->host_state[dev_id].state = ONLINE;
		node->host_state[dev_id].sync = recv_time + COMPENSATE_TIME - trans_clock;
		TSN_DBG("ptp from host, host num: %d\n", node->cur_host_num);
	}

	return node;
}


#endif

#if 0
int ptp_packet_state_process(struct ptp_packet *pkt, u64 recv_time)
{
	u32 seq = pkt->seq;
	struct state_node *node = NULL;
	struct timeval create_time;
	u8 dev_type = 0;
	u8 dev_id = 0;
	u64 trans_clock = pkt->correct_field;

	pthread_mutex_lock(&(s_list.mutex));
	get_device_type_id_by_mac(pkt->src_mac, &dev_type, &dev_id);
	printf("seq: %d, device type:%d, device id:%d!\n", seq, dev_type, dev_id);

	node = lookup_state_node_by_seq(seq);
	if(node == NULL)
	{
		TSN_DBG("Could not find node, Create one!\n");
		node = (struct state_node *)malloc(sizeof(struct state_node));
		memset((u8 *)node, 0, sizeof(struct state_node));
		node->seq = seq;
		gettimeofday(&create_time, NULL);
		node->create_time = create_time;
		insert_node_to_state_list(node);
		printf("s_list size: %d\n", s_list.size);
	}

	if(dev_type == SWITCH)
	{
		node->cur_sw_num++;
		node->sw_state[dev_id].id = dev_id;
		node->sw_state[dev_id].state = ONLINE;
		node->sw_state[dev_id].sync = recv_time + COMPENSATE_TIME - trans_clock;
		printf("sw num: %d\n", node->cur_sw_num);
	}
	else
	{
		node->cur_host_num++;
		node->host_state[dev_id].id = dev_id;
		node->host_state[dev_id].state = ONLINE;
		node->host_state[dev_id].sync = recv_time + COMPENSATE_TIME - trans_clock;
		printf("host num: %d\n", node->cur_host_num);
	}
	
	pthread_mutex_unlock(&(s_list.mutex));
	return G_OK;
}



void state_list_process()
{
	struct timeval cur_time;
	struct state_node *node = s_list.head;
	struct state_node *pre_node = NULL;
	int i = 0;

	pthread_mutex_lock(&(s_list.mutex));
	printf("start to process state list!\n");
	gettimeofday(&cur_time, NULL);
	if(s_list.size == 0)
	{
		printf("s_list is empty!\n");
		pthread_mutex_unlock(&(s_list.mutex));
		return;
	}
	while(node != NULL)
	{
		printf("slist size: %d\n", s_list.size);
		if(cur_time.tv_sec - node->create_time.tv_sec > 1)
		{
			TSN_DBG("node[%d] timeout!\n", node->seq);
			if(node->cur_sw_num < get_sw_num())
			{
				printf("there is link fault!\n");
				//link fault
				for(i = get_sw_num() - 1; i >= 0; i--)
				{
					if(node->sw_state[i].state == OFFLINE)
						break;
				}
				//repair link fault and send link packet	
			}
			
			compute_and_send_sync_info(node);

			if(s_list.size == 1)
			{
				s_list.head = NULL;
				s_list.tail = NULL;
				s_list.size--;
				free(node);
				printf("there is only one node\n");
				pthread_mutex_unlock(&(s_list.mutex));
				return;
			}
			else
			{
				if(node == s_list.head)
				{
					printf("node is head\n");
					s_list.head = node->next;
					free(node);
					node = s_list.head;
					s_list.size--;
				}
				else if(node == s_list.tail)
				{
					printf("node is tail\n");
					s_list.tail = pre_node;
					s_list.tail->next = NULL;
					s_list.size--;
					free(node);
					pthread_mutex_unlock(&(s_list.mutex));
					return;
				}
				else
				{
					printf("node is middle\n");
					pre_node->next = node->next;
					free(node);
					node = pre_node->next;
					s_list.size--;
				}
				continue;
			}
			
		}
		pre_node = node;
		node = node->next;
	}
	pthread_mutex_unlock(&(s_list.mutex));
}
#endif
