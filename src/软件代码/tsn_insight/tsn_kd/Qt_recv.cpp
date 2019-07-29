#include "Qt_recv.h"
#include "cJSON.h"
#include "stdlib.h"




struct topology_info tp_info[MAX_SW_NUM];
struct global_info  global;
struct flow_feature flow;


void trans_str_mac_to_hex_mac(char *str_mac,char *hex_mac)
{
    sscanf(str_mac,"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",hex_mac,hex_mac+1,hex_mac+2,hex_mac+3,hex_mac+4,hex_mac+5);

}


int switch_counter_json_parse(char *data)
{
    printf("enter switch_counter_json_parse\n");



    int array_num = 0;
    int array_idx = 0;

    int tmp_sw_id = 0;

    cJSON *info_json            = NULL;
    cJSON *array_json           = NULL;
    cJSON *seq_json             = NULL;
    cJSON *cnt_info_json        = NULL;

    cJSON *sw_id_json              = NULL;
    cJSON *esw_pkt_in_cnt_json     = NULL;
    cJSON *esw_pkt_out_cnt_json    = NULL;
    cJSON *buf_id_rest_cnt_json    = NULL;
    cJSON *eos_md_in_cnt_json      = NULL;
    cJSON *eos_md_out_cnt_json     = NULL;
    cJSON *eos_q0_used_cnt_json    = NULL;
    cJSON *eos_q1_used_cnt_json    = NULL;
    cJSON *eos_q2_used_cnt_json    = NULL;
    cJSON *eos_q3_used_cnt_json    = NULL;
    cJSON *goe_pkt_in_cnt_json     = NULL;
    cJSON *goe_port0_out_cnt_json  = NULL;
    cJSON *goe_port1_out_cnt_json  = NULL;
    cJSON *goe_discard_cnt_json    = NULL;

    info_json = cJSON_Parse(data);
    if(info_json == NULL)
    {
        printf("no have data\n");
        return -1;
    }

    //seq
    seq_json = cJSON_GetObjectItem(info_json,"seq");
    if(seq_json == NULL)
    {
        printf("no have seq\n");
        return -1;
    }
    else
        global.seq = strtol(seq_json->valuestring,NULL,16);


    cnt_info_json = cJSON_GetObjectItem(info_json,"cnt_info");
    if(cnt_info_json == NULL)
    {
        printf("no have tsn cnt_info\n");
        return -1;
    }
    array_num = cJSON_GetArraySize(cnt_info_json);

    printf("array size is %d\n",array_num);
    array_idx = 0;

    while(array_idx < array_num)
    {
        array_json = cJSON_GetArrayItem(cnt_info_json,array_idx);


        //sw_id
        if(NULL == (sw_id_json =cJSON_GetObjectItem(array_json,"sw_id")))
        {
            printf("have no sw_id\n");
            return -1;
        }
        else
            tmp_sw_id = strtol(sw_id_json->valuestring,NULL,16);

        //esw_pkt_in_cnt
        if(NULL == (esw_pkt_in_cnt_json =cJSON_GetObjectItem(array_json,"esw_pkt_in_cnt")))
        {
            printf("have no esw_pkt_in_cnt\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].cnt_info.esw_pkt_in_cnt = strtol(esw_pkt_in_cnt_json->valuestring,NULL,16);

        //esw_pkt_out_cnt
        if(NULL == (esw_pkt_out_cnt_json =cJSON_GetObjectItem(array_json,"esw_pkt_out_cnt")))
        {
            printf("have no esw_pkt_out_cnt\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].cnt_info.esw_pkt_out_cnt = strtol(esw_pkt_out_cnt_json->valuestring,NULL,16);

        //buf_id_rest_cnt
        if(NULL == (buf_id_rest_cnt_json =cJSON_GetObjectItem(array_json,"buf_id_rest_cnt")))
        {
            printf("have no buf_id_rest_cnt\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].cnt_info.buf_id_rest_cnt = strtol(buf_id_rest_cnt_json->valuestring,NULL,16);


        //eos_md_in_cnt
        if(NULL == (eos_md_in_cnt_json =cJSON_GetObjectItem(array_json,"eos_md_in_cnt")))
        {
            printf("have no eos_md_in_cnt\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].cnt_info.eos_md_in_cnt = strtol(eos_md_in_cnt_json->valuestring,NULL,16);

        //eos_md_out_cnt
        if(NULL == (eos_md_out_cnt_json =cJSON_GetObjectItem(array_json,"eos_md_out_cnt")))
        {
            printf("have no eos_md_out_cnt\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].cnt_info.eos_md_out_cnt = strtol(eos_md_out_cnt_json->valuestring,NULL,16);

        //eos_q0_used_cnt
        if(NULL == (eos_q0_used_cnt_json =cJSON_GetObjectItem(array_json,"eos_q0_used_cnt")))
        {
            printf("have no eos_q0_used_cnt\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].cnt_info.eos_q0rest_cnt = strtol(eos_q0_used_cnt_json->valuestring,NULL,16);

        //eos_q1_used_cnt
        if(NULL == (eos_q1_used_cnt_json =cJSON_GetObjectItem(array_json,"eos_q1_used_cnt")))
        {
            printf("have no eos_q1_used_cnt\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].cnt_info.eos_q1rest_cnt = strtol(eos_q1_used_cnt_json->valuestring,NULL,16);

        //eos_q2_used_cnt
        if(NULL == (eos_q2_used_cnt_json =cJSON_GetObjectItem(array_json,"eos_q2_used_cnt")))
        {
            printf("have no eos_q2_used_cnt\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].cnt_info.eos_q2rest_cnt = strtol(eos_q2_used_cnt_json->valuestring,NULL,16);

        //eos_q3_used_cnt
        if(NULL == (eos_q3_used_cnt_json =cJSON_GetObjectItem(array_json,"eos_q3_used_cnt")))
        {
            printf("have no eos_q3_used_cnt\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].cnt_info.eos_q3rest_cnt = strtol(eos_q3_used_cnt_json->valuestring,NULL,16);

        //goe_pkt_in_cnt
        if(NULL == (goe_pkt_in_cnt_json =cJSON_GetObjectItem(array_json,"goe_pkt_in_cnt")))
        {
            printf("have no goe_pkt_in_cnt\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].cnt_info.goe_pkt_in_cnt = strtol(goe_pkt_in_cnt_json->valuestring,NULL,16);

        //goe_port0_out_cnt
        if(NULL == (goe_port0_out_cnt_json =cJSON_GetObjectItem(array_json,"goe_port0_out_cnt")))
        {
            printf("have no goe_port0_out_cnt\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].cnt_info.goe_port0_out_cnt = strtol(goe_port0_out_cnt_json->valuestring,NULL,16);

        //goe_port1_out_cnt
        if(NULL == (goe_port1_out_cnt_json =cJSON_GetObjectItem(array_json,"goe_port1_out_cnt")))
        {
            printf("have no goe_port1_out_cnt\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].cnt_info.goe_port1_out_cnt = strtol(goe_port1_out_cnt_json->valuestring,NULL,16);

        //goe_discard_cnt
        if(NULL == (goe_discard_cnt_json =cJSON_GetObjectItem(array_json,"goe_discard_cnt")))
        {
            printf("have no goe_discard_cnt\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].cnt_info.goe_discard_cnt = strtol(goe_discard_cnt_json->valuestring,NULL,16);

        array_idx++;
    }

    printf("global.sw_info[tmp_sw_id].cnt_info.esw_pkt_in_cnt  %x\n",(unsigned int)global.sw_info[tmp_sw_id].cnt_info.esw_pkt_in_cnt);
    printf("global.sw_info[tmp_sw_id].cnt_info.esw_pkt_out_cnt %x\n",(unsigned int)global.sw_info[tmp_sw_id].cnt_info.esw_pkt_out_cnt);
    printf("global.sw_info[tmp_sw_id].cnt_info.buf_id_rest_cnt %x\n",(unsigned int)global.sw_info[tmp_sw_id].cnt_info.buf_id_rest_cnt);
    printf("global.sw_info[tmp_sw_id].cnt_info.eos_md_in_cnt %x\n",(unsigned int)global.sw_info[tmp_sw_id].cnt_info.eos_md_in_cnt);
    printf("global.sw_info[tmp_sw_id].cnt_info.eos_md_out_cnt %x\n",(unsigned int)global.sw_info[tmp_sw_id].cnt_info.eos_md_out_cnt);
    printf("global.sw_info[tmp_sw_id].cnt_info.eos_q0rest_cnt %x\n",(unsigned int)global.sw_info[tmp_sw_id].cnt_info.eos_q0rest_cnt);
    printf("global.sw_info[tmp_sw_id].cnt_info.eos_q1rest_cnt %x\n",(unsigned int)global.sw_info[tmp_sw_id].cnt_info.eos_q1rest_cnt);
    printf("global.sw_info[tmp_sw_id].cnt_info.eos_q2rest_cnt %x\n",(unsigned int)global.sw_info[tmp_sw_id].cnt_info.eos_q2rest_cnt);
    printf("global.sw_info[tmp_sw_id].cnt_info.eos_q3rest_cnt %x\n",(unsigned int)global.sw_info[tmp_sw_id].cnt_info.eos_q3rest_cnt);
    printf("global.sw_info[tmp_sw_id].cnt_info.goe_pkt_in_cnt %x\n",(unsigned int)global.sw_info[tmp_sw_id].cnt_info.goe_pkt_in_cnt);
    printf("global.sw_info[tmp_sw_id].cnt_info.goe_port0_out_cnt %x\n",(unsigned int)global.sw_info[tmp_sw_id].cnt_info.goe_port0_out_cnt);
    printf("global.sw_info[tmp_sw_id].cnt_info.goe_port1_out_cnt %x\n",(unsigned int)global.sw_info[tmp_sw_id].cnt_info.goe_port1_out_cnt);
    printf("global.sw_info[tmp_sw_id].cnt_info.goe_discard_cnt %x\n",(unsigned int)global.sw_info[tmp_sw_id].cnt_info.goe_discard_cnt);

    printf("exit switch_counter_json_parse\n");


    return 0;
}


int sync_state_json_parse(char *data)
{
    printf("enter flow_feature_json_parse\n");



    int array_num = 0;
    int array_idx = 0;


    int tmp_device_type = 0;
    int tmp_device_id = 0;

    cJSON *info_json            = NULL;
    cJSON *array_json           = NULL;
    cJSON *sync_state_json      = NULL;
    cJSON *seq_json             = NULL;
    cJSON *sync_precision_json  = NULL;

    cJSON *device_type_json  = NULL;
    cJSON *device_id_json    = NULL;
    cJSON *time_offset_json  = NULL;

    info_json = cJSON_Parse(data);
    if(info_json == NULL)
    {
        printf("no have data\n");
        return -1;
    }

    //seq
    seq_json = cJSON_GetObjectItem(info_json,"seq");
    if(seq_json == NULL)
    {
        printf("no have seq\n");
        return -1;
    }
    else
        global.seq = strtol(seq_json->valuestring,NULL,16);

    //sync_precision
    sync_precision_json = cJSON_GetObjectItem(info_json,"sync_precision");
    if(sync_precision_json == NULL)
    {
        printf("no have sync_precision\n");
        return -1;
    }
    else
        global.global_time_offset = strtol(sync_precision_json->valuestring,NULL,16)*8;


    sync_state_json = cJSON_GetObjectItem(info_json,"sync_state");
    if(sync_state_json == NULL)
    {
        printf("no have tsn sync_state\n");
        return -1;
    }

    array_num = cJSON_GetArraySize(sync_state_json);

    printf("array size is %d\n",array_num);
    array_idx = 0;

    while(array_idx < array_num)
    {
        array_json = cJSON_GetArrayItem(sync_state_json,array_idx);


        //device_type
        if(NULL == (device_type_json =cJSON_GetObjectItem(array_json,"device_type")))
        {
            printf("have no device_type\n");
            return -1;
        }
        else
        {
            if(0 == strncmp("switch",device_type_json->valuestring,6))
                tmp_device_type = SWITCH_TYPE;
            else if(0 == strncmp("host",device_type_json->valuestring,4))
                tmp_device_type = HOST_TYPE;
            else
            {
                printf("have no correct device_type\n");
                return -1;
            }
        }



        //device_id
        if(NULL == (device_id_json =cJSON_GetObjectItem(array_json,"device_id")))
        {
            printf("have no device_id\n");
            return -1;
        }
        else
            tmp_device_id = strtol(device_id_json->valuestring,NULL,10);

        //time_offset_json
        if(NULL == (time_offset_json =cJSON_GetObjectItem(array_json,"time_offset")))
        {
            printf("have no time_offset\n");
            return -1;
        }
        else
        {
            if(tmp_device_type == SWITCH_TYPE)
                global.sw_info[tmp_device_id].time_offset = strtol(time_offset_json->valuestring,NULL,16)*8;
            if(tmp_device_type == HOST_TYPE)
                global.hs_info[tmp_device_id].time_offset = strtol(time_offset_json->valuestring,NULL,16)*8;
        }

        array_idx++;
    }
    printf("exit flow_feature_json_parse\n");

     return 0;
}


int flow_feature_json_parse(char *data)
{

    printf("enter flow_feature_json_parse\n");



    int array_num = 0;
    int array_idx = 0;

    int tsn_flow_id = 0;
    int bd_flow_id  = 0;

    cJSON *array_json    = NULL;
    cJSON *flow_json     = NULL;
    cJSON *info_json     = NULL;

    //cJSON *sw_id_json      = NULL;
    cJSON *flow_type_json  = NULL;
    cJSON *flow_id_json    = NULL;
    cJSON *src_mac_json    = NULL;
    cJSON *dst_mac_json    = NULL;
    cJSON *priority_json   = NULL;
    cJSON *interval_json   = NULL;
    cJSON *pkt_num_json    = NULL;
    cJSON *pkt_size_json   = NULL;
    cJSON *latency_json    = NULL;
    cJSON *path_len_json   = NULL;
    cJSON *direction_json  = NULL;
    cJSON *src_sw_id_json  = NULL;
    cJSON *dst_sw_id_json  = NULL;
    cJSON *bandwidth_json  = NULL;


    info_json = cJSON_Parse(data);
    if(info_json == NULL)
    {
        printf("no have data\n");
        return -1;
    }
    flow_json = cJSON_GetObjectItem(info_json,"tsn");
    if(flow_json == NULL)
    {
        printf("no have tsn flow\n");
        return -1;
    }

    array_num = cJSON_GetArraySize(flow_json);

    printf("array size is %d\n",array_num);
    array_idx = 0;

    while(array_idx < array_num)
    {
        array_json = cJSON_GetArrayItem(flow_json,array_idx);


        //flow_id
        if(NULL == (flow_id_json =cJSON_GetObjectItem(array_json,"flow_id")))
        {
            printf("have no flow_id\n");
            return -1;
        }
        else
            tsn_flow_id = strtol(flow_id_json->valuestring,NULL,10);
        flow.tsn_flow[tsn_flow_id].flow_id = tsn_flow_id;


        //flow_type
        if(NULL == (flow_type_json =cJSON_GetObjectItem(array_json,"flow_type")))
        {
            printf("have no flow_type\n");
            return -1;
        }
        else
        {
            if(0 == strncmp("tsn",flow_type_json->valuestring,3))
                flow.tsn_flow[tsn_flow_id].flow_type = TSN_FLOW;
            else if(0 == strncmp("bd",flow_type_json->valuestring,2))
                flow.tsn_flow[tsn_flow_id].flow_type = BD_FLOW;
            else
            {
                printf("have no correct flow_type\n");
                return -1;
            }

        }

        //src_mac
        if(NULL == (src_mac_json =cJSON_GetObjectItem(array_json,"src_mac")))
        {
            printf("have no src_mac\n");
            return -1;
        }
        else
            trans_str_mac_to_hex_mac((char *)(src_mac_json->valuestring),(char *)flow.tsn_flow[tsn_flow_id].src_mac);

        //dst_mac
        if(NULL == (dst_mac_json =cJSON_GetObjectItem(array_json,"dst_mac")))
        {
            printf("have no dst_mac\n");
            return -1;
        }
        else
            trans_str_mac_to_hex_mac((char *)(dst_mac_json->valuestring),(char *)flow.tsn_flow[tsn_flow_id].dst_mac);

        //priority
        if(NULL == (priority_json =cJSON_GetObjectItem(array_json,"priority")))
        {
            printf("have no priority\n");
            return -1;
        }
        else
            flow.tsn_flow[tsn_flow_id].priority = strtol(priority_json->valuestring,NULL,10);

        //interval
        if(NULL == (interval_json =cJSON_GetObjectItem(array_json,"interval")))
        {
            printf("have no interval\n");
            return -1;
        }
        else
            flow.tsn_flow[tsn_flow_id].interval = strtol(interval_json->valuestring,NULL,10);


        //pkt_num
        if(NULL == (pkt_num_json =cJSON_GetObjectItem(array_json,"pkt_num")))
        {
            printf("have no pkt_num\n");
            return -1;
        }
        else
            flow.tsn_flow[tsn_flow_id].pkt_num = strtol(pkt_num_json->valuestring,NULL,10);

        //pkt_size
        if(NULL == (pkt_size_json =cJSON_GetObjectItem(array_json,"pkt_size")))
        {
            printf("have no pkt_size\n");
            return -1;
        }
        else
            flow.tsn_flow[tsn_flow_id].pkt_size = strtol(pkt_size_json->valuestring,NULL,10);


        //latency
        if(NULL == (latency_json =cJSON_GetObjectItem(array_json,"latency")))
        {
            printf("have no latency\n");
            return -1;
        }
        else
           flow.tsn_flow[tsn_flow_id].latency = strtol(latency_json->valuestring,NULL,10);

        //path_len
        if(NULL == (path_len_json =cJSON_GetObjectItem(array_json,"path_len")))
        {
            printf("have no path_len\n");
            return -1;
        }
        else
            flow.tsn_flow[tsn_flow_id].path_len = strtol(path_len_json->valuestring,NULL,10);

        //direction
        if(NULL == (direction_json =cJSON_GetObjectItem(array_json,"direction")))
        {
            printf("have no direction\n");
            return -1;
        }
        else
            flow.tsn_flow[tsn_flow_id].direction = strtol(direction_json->valuestring,NULL,10);

        //src_sw_id
        if(NULL == (src_sw_id_json =cJSON_GetObjectItem(array_json,"src_sw_id")))
        {
            printf("have no src_sw_id\n");
            return -1;
        }
        else
            flow.tsn_flow[tsn_flow_id].src_sw_id = strtol(src_sw_id_json->valuestring,NULL,10);

        //dst_sw_id
        if(NULL == (dst_sw_id_json =cJSON_GetObjectItem(array_json,"dst_sw_id")))
        {
            printf("have no dst_sw_id\n");
            return -1;
        }
        else
            flow.tsn_flow[tsn_flow_id].dst_sw_id = strtol(dst_sw_id_json->valuestring,NULL,10);

        array_idx++;
    }
    flow.tsn_num = array_idx;

    flow_json = cJSON_GetObjectItem(info_json,"bd");
    if(flow_json == NULL)
    {
        printf("no have bd flow\n");
        return -1;
    }

    array_num = cJSON_GetArraySize(flow_json);

    printf("array size is %d\n",array_num);
    array_idx = 0;

    while(array_idx < array_num)
    {
        array_json = cJSON_GetArrayItem(flow_json,array_idx);


        //flow_id
        if(NULL == (flow_id_json =cJSON_GetObjectItem(array_json,"flow_id")))
        {
            printf("have no flow_id\n");
            return -1;
        }
        else
            bd_flow_id = strtol(flow_id_json->valuestring,NULL,10);
        flow.bd_flow[bd_flow_id].flow_id = tsn_flow_id;


        //flow_type
        if(NULL == (flow_type_json =cJSON_GetObjectItem(array_json,"flow_type")))
        {
            printf("have no flow_type\n");
            return -1;
        }
        else
        {
            if(0 == strncmp("tsn",flow_type_json->valuestring,3))
                flow.bd_flow[bd_flow_id].flow_type = TSN_FLOW;
            else if(0 == strncmp("bd",flow_type_json->valuestring,2))
                flow.bd_flow[bd_flow_id].flow_type = BD_FLOW;
            else
            {
                printf("have no correct flow_type\n");
                return -1;
            }

        }

        //src_mac
        if(NULL == (src_mac_json =cJSON_GetObjectItem(array_json,"src_mac")))
        {
            printf("have no src_mac\n");
            return -1;
        }
        else
            trans_str_mac_to_hex_mac((char *)(src_mac_json->valuestring),(char *)flow.bd_flow[bd_flow_id].src_mac);

        //dst_mac
        if(NULL == (dst_mac_json =cJSON_GetObjectItem(array_json,"dst_mac")))
        {
            printf("have no dst_mac\n");
            return -1;
        }
        else
            trans_str_mac_to_hex_mac((char *)(dst_mac_json->valuestring),(char *)flow.bd_flow[bd_flow_id].dst_mac);

        //priority
        if(NULL == (priority_json =cJSON_GetObjectItem(array_json,"priority")))
        {
            printf("have no priority\n");
            return -1;
        }
        else
            flow.bd_flow[bd_flow_id].priority = strtol(priority_json->valuestring,NULL,10);

        //path_len
        if(NULL == (path_len_json =cJSON_GetObjectItem(array_json,"path_len")))
        {
            printf("have no path_len\n");
            return -1;
        }
        else
            flow.bd_flow[bd_flow_id].path_len = strtol(path_len_json->valuestring,NULL,10);

        //direction
        if(NULL == (direction_json =cJSON_GetObjectItem(array_json,"direction")))
        {
            printf("have no direction\n");
            return -1;
        }
        else
            flow.bd_flow[bd_flow_id].direction = strtol(direction_json->valuestring,NULL,10);

        //src_sw_id
        if(NULL == (src_sw_id_json =cJSON_GetObjectItem(array_json,"src_sw_id")))
        {
            printf("have no src_sw_id\n");
            return -1;
        }
        else
            flow.bd_flow[bd_flow_id].src_sw_id = strtol(src_sw_id_json->valuestring,NULL,10);

        //dst_sw_id
        if(NULL == (dst_sw_id_json =cJSON_GetObjectItem(array_json,"dst_sw_id")))
        {
            printf("have no dst_sw_id\n");
            return -1;
        }
        else
            flow.bd_flow[bd_flow_id].dst_sw_id = strtol(dst_sw_id_json->valuestring,NULL,10);

        //bandwidth
        if(NULL == (bandwidth_json =cJSON_GetObjectItem(array_json,"bandwidth")))
        {
            printf("have no bandwidth\n");
            return -1;
        }
        else
            flow.bd_flow[bd_flow_id].bandwidth = strtol(bandwidth_json->valuestring,NULL,10);

        array_idx++;
    }
    flow.bd_num = array_idx;

    printf("sw_mac %x:%x:%x:%x:%x:%x\n",flow.tsn_flow[tsn_flow_id].src_mac[0],flow.tsn_flow[tsn_flow_id].src_mac[1],flow.tsn_flow[tsn_flow_id].src_mac[2],flow.tsn_flow[tsn_flow_id].src_mac[3],flow.tsn_flow[tsn_flow_id].src_mac[4],flow.tsn_flow[tsn_flow_id].src_mac[5]);
    printf("dst_mac %x:%x:%x:%x:%x:%x\n",flow.tsn_flow[tsn_flow_id].dst_mac[0],flow.tsn_flow[tsn_flow_id].dst_mac[1],flow.tsn_flow[tsn_flow_id].dst_mac[2],flow.tsn_flow[tsn_flow_id].dst_mac[3],flow.tsn_flow[tsn_flow_id].dst_mac[4],flow.tsn_flow[tsn_flow_id].dst_mac[5]);
    printf("interval %d\n",flow.tsn_flow[tsn_flow_id].interval);
    printf("exit flow_feature_json_parse\n");
     return 0;
}


int resource_json_parse(char *data)
{
    printf("enter resource_json_parse\n");

    //struct resource_info tmp_res[6];

    int array_num = 0;
    int array_idx = 0;

    int tmp_sw_id = 0;

    cJSON *array_json    = NULL;
    cJSON *resource_json = NULL;

    cJSON *sw_id_json          = NULL;
    cJSON *cqf_total_len_json  = NULL;
    cJSON *cqf_free_len_json   = NULL;
    cJSON *cqf_used_len_json   = NULL;
    cJSON *total_bd_json       = NULL;
    cJSON *free_bd_json        = NULL;
    cJSON *used_bd_json        = NULL;
    cJSON *tb_rate_json        = NULL;
    cJSON *direction_json      = NULL;
    cJSON *time_slot_json      = NULL;

    resource_json = cJSON_Parse(data);
    array_num = cJSON_GetArraySize(resource_json);

    printf("array size is %d\n",array_num);
    array_idx = 0;

    while(array_idx < array_num)
    {
        array_json = cJSON_GetArrayItem(resource_json,array_idx);

        //sw_id
        if(NULL == (sw_id_json =cJSON_GetObjectItem(array_json,"sw_id")))
        {
            printf("have no sw_id\n");
            return -1;
        }
        else
           tmp_sw_id = strtol(sw_id_json->valuestring,NULL,10);
        //tmp_res[tmp_sw_id].sw_id = tmp_sw_id;
        global.sw_info[tmp_sw_id].res_info.sw_id = tmp_sw_id;
        //cqf_total_len
        if(NULL == (cqf_total_len_json =cJSON_GetObjectItem(array_json,"cqf_total_len")))
        {
            printf("have no cqf_total_len\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].res_info.cqf_total_len = strtol(cqf_total_len_json->valuestring,NULL,10);

        //cqf_free_len
        if(NULL == (cqf_free_len_json =cJSON_GetObjectItem(array_json,"cqf_free_len")))
        {
            printf("have no cqf_free_len\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].res_info.cqf_free_len = strtol(cqf_free_len_json->valuestring,NULL,10);

        //cqf_used_len
        if(NULL == (cqf_used_len_json =cJSON_GetObjectItem(array_json,"cqf_used_len")))
        {
            printf("have no cqf_used_len\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].res_info.cqf_used_len = strtol(cqf_used_len_json->valuestring,NULL,10);

        //total_bd
        if(NULL == (total_bd_json =cJSON_GetObjectItem(array_json,"total_bd")))
        {
            printf("have no total_bd\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].res_info.total_bd = strtol(total_bd_json->valuestring,NULL,10);

        //free_bd
        if(NULL == (free_bd_json =cJSON_GetObjectItem(array_json,"free_bd")))
        {
            printf("have no free_bd\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].res_info.free_bd = strtol(free_bd_json->valuestring,NULL,10);

        //used_bd
        if(NULL == (used_bd_json =cJSON_GetObjectItem(array_json,"used_bd")))
        {
            printf("have no used_bd\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].res_info.used_bd = strtol(used_bd_json->valuestring,NULL,10);

        //tb_rate_json
        if(NULL == (tb_rate_json =cJSON_GetObjectItem(array_json,"tb_rate")))
        {
            printf("have no tb_rate\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].res_info.tb_rate = strtol(tb_rate_json->valuestring,NULL,10);


        //direction
        if(NULL == (direction_json =cJSON_GetObjectItem(array_json,"direction")))
        {
            printf("have no direction\n");
            return -1;
        }
        else
           global.sw_info[tmp_sw_id].res_info.direction = strtol(direction_json->valuestring,NULL,10);

        //time_slot_json
        if(NULL == (time_slot_json =cJSON_GetObjectItem(array_json,"time_slot")))
        {
            printf("have no time_slot\n");
            return -1;
        }
        else
            global.sw_info[tmp_sw_id].res_info.time_slot = strtol(time_slot_json->valuestring,NULL,10);

        array_idx++;
    }


    printf("sw_id = %d\n",global.sw_info[tmp_sw_id].res_info.sw_id);
    printf("cqf_total_len = %d\n",global.sw_info[tmp_sw_id].res_info.cqf_total_len);
    printf("cqf_free_len = %d\n",global.sw_info[tmp_sw_id].res_info.cqf_free_len);
    printf("cqf_used_len = %d\n",global.sw_info[tmp_sw_id].res_info.cqf_used_len);
    printf("total_bd = %d\n",global.sw_info[tmp_sw_id].res_info.total_bd);
    printf("free_bd = %d\n",global.sw_info[tmp_sw_id].res_info.free_bd);
    printf("used_bd = %d\n",global.sw_info[tmp_sw_id].res_info.used_bd);
    printf("tb_rate = %d\n",global.sw_info[tmp_sw_id].res_info.tb_rate);
    printf("direction = %d\n",global.sw_info[tmp_sw_id].res_info.direction);
    printf("time_slot = %d\n",global.sw_info[tmp_sw_id].res_info.time_slot);



    printf("enter resource_json_parse\n");

    return 0;
}


int topology_json_parse(char *data)
{
    printf("enter topology_json_parse\n");

    int array_num = 0;
    int array_idx = 0;

    cJSON *array_json    = NULL;
    cJSON *topology_json = NULL;

    cJSON *sw_id_json      = NULL;
    cJSON *sync_type_json  = NULL;
    cJSON *host_id_json    = NULL;
    cJSON *sw_mac_json     = NULL;
    cJSON *host_mac_json   = NULL;
    cJSON *next_sw_id_json = NULL;
    cJSON *prev_sw_id_json = NULL;
    cJSON *next_port_json  = NULL;
    cJSON *prev_port_json  = NULL;


    int tmp_sw_id = 0;

    topology_json = cJSON_Parse(data);
    array_num = cJSON_GetArraySize(topology_json);
    //get array size

    printf("array size is %d\n",array_num);
    array_idx = 0;
    while(array_idx < array_num)
    {
        array_json = cJSON_GetArrayItem(topology_json,array_idx);

        //sw_id
        if(NULL == (sw_id_json =cJSON_GetObjectItem(array_json,"sw_id")))
        {
            printf("have no sw_id\n");
            return -1;
        }
        else
           tmp_sw_id = strtol(sw_id_json->valuestring,NULL,10);
        tp_info[tmp_sw_id].sw_id = tmp_sw_id;

        //sync_type
        if(NULL == (sync_type_json =cJSON_GetObjectItem(array_json,"sync_type")))
        {
            printf("have no sync_type\n");
            return -1;
        }
        else
        {
            if(0 == strncmp("master",sync_type_json->valuestring,6))
                tp_info[tmp_sw_id].sync_type = MASTER;
            else if(0 == strncmp("slave",sync_type_json->valuestring,6))
                tp_info[tmp_sw_id].sync_type = SLAVE;
            else
            {
                printf("have no correct sync_type\n");
                return -1;
            }

        }

        //host_id
        if(NULL == (host_id_json =cJSON_GetObjectItem(array_json,"host_id")))
        {
            printf("have no host_id\n");
            return -1;
        }
        else
           tp_info[tmp_sw_id].host_id = strtol(host_id_json->valuestring,NULL,10);

        //sw_mac
        if(NULL == (sw_mac_json =cJSON_GetObjectItem(array_json,"sw_mac")))
        {
            printf("have no sw_mac\n");
            return -1;
        }
        else
            trans_str_mac_to_hex_mac((char *)(sw_mac_json->valuestring),(char *)tp_info[tmp_sw_id].sw_mac);

        //host_mac
        if(NULL == (host_mac_json =cJSON_GetObjectItem(array_json,"host_mac")))
        {
            printf("have no host_mac\n");
            return -1;
        }
        else
            trans_str_mac_to_hex_mac((char *)(host_mac_json->valuestring),(char *)tp_info[tmp_sw_id].host_mac);

        //next_sw_id
        if(NULL == (next_sw_id_json =cJSON_GetObjectItem(array_json,"next_sw_id")))
        {
            printf("have no next_sw_id\n");
            return -1;
        }
        else
           tp_info[tmp_sw_id].next_sw_id = strtol(next_sw_id_json->valuestring,NULL,10);

        //prev_sw_id
        if(NULL == (prev_sw_id_json =cJSON_GetObjectItem(array_json,"prev_sw_id")))
        {
            printf("have no prev_sw_id\n");
            return -1;
        }
        else
           tp_info[tmp_sw_id].prev_sw_id = strtol(prev_sw_id_json->valuestring,NULL,10);

        //next_port
        if(NULL == (next_port_json =cJSON_GetObjectItem(array_json,"next_port")))
        {
            printf("have no next_port\n");
            return -1;
        }
        else
           tp_info[tmp_sw_id].next_port = strtol(next_port_json->valuestring,NULL,10);

        //next_port
        if(NULL == (next_port_json =cJSON_GetObjectItem(array_json,"next_port")))
        {
            printf("have no next_port\n");
            return -1;
        }
        else
           tp_info[tmp_sw_id].next_port = strtol(next_port_json->valuestring,NULL,10);

        //prev_port
        if(NULL == (prev_port_json =cJSON_GetObjectItem(array_json,"prev_port")))
        {
            printf("have no prev_port\n");
            return -1;
        }
        else
           tp_info[tmp_sw_id].prev_port = strtol(prev_port_json->valuestring,NULL,10);

        array_idx++;
    }

    printf("sw_id = %d\n",tp_info[0].sw_id);
    printf("sync_type = %d\n",tp_info[0].sync_type);
    printf("host_id = %d\n",tp_info[0].host_id);
    //printf("sw_id = %d\n",tp_info[0].sw_id);
    //printf("sw_id = %d\n",tp_info[0].sw_id);
    printf("next_sw_id = %d\n",tp_info[0].next_sw_id);
    printf("prev_sw_id = %d\n",tp_info[0].prev_sw_id);
    printf("next_port = %d\n",tp_info[0].next_port);
    printf("prev_port = %d\n",tp_info[0].prev_port);
    printf("sw_mac %x:%x:%x:%x:%x:%x\n",tp_info[0].sw_mac[0],tp_info[0].sw_mac[1],tp_info[0].sw_mac[2],tp_info[0].sw_mac[3],tp_info[0].sw_mac[4],tp_info[0].sw_mac[5]);


    printf("exit topology_json_parse\n");
    return 0;
}



int pkt_parse(char *data)
{
    char type = 0;
    //char *tmp_pkt = NULL;
    //pkt_header tmp;
    int head_len = sizeof(pkt_header);
    //memcpy(&tmp,data,sizeof(pkt_header));
    type = ((recv_pkt *)data)->header.type;
    //tmp_pkt = (char *)data;
    // type = tmp.type;
     printf("type = %d\n",type);
     printf("%s\n",((recv_pkt *)data)->data);
	switch(type)
	{
        case 0:topology_json_parse(((recv_pkt *)data)->data);break;
        case 1:resource_json_parse(data + head_len);break;
        case 2:flow_feature_json_parse(data + head_len);break;
        case 3:sync_state_json_parse(data + head_len);break;
        case 4:switch_counter_json_parse(data + head_len);break;
		default:return -1;
	}
	return 0;
}








void *init_socket_tcp_server(void* arg)
{
	int len = 0;
	int opt = 1;
    int sfd,cfd = 0;
	struct sockaddr_in my_addr,client_addr;
	socklen_t client_addr_len;
    char *buf = (char*)malloc(PKT_MAX_LEN);
	sfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//creater socket
	if(sfd<0)
		perror("socket");
	setsockopt(cfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	memset(&my_addr,0,sizeof(struct sockaddr_in));
	my_addr.sin_family=AF_INET;
	my_addr.sin_port = htons(8080);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr_in))== -1)
		perror("bind");
	if(listen(sfd,10)==-1)
		perror("listen");
	client_addr_len = sizeof(struct sockaddr_in);
	cfd=accept(sfd,(struct sockaddr *)&client_addr,&client_addr_len);
	if(cfd==-1)
		perror("accept");
	printf("get %d\n",cfd);
	//write(cfd,str,sizeof(str));
	while(1)
	{
        //printf("2222222222222222\n");
		memset(buf, 0, PKT_MAX_LEN);
		len = read(cfd, buf, sizeof(pkt_header));
        if(len == 0)
           continue;
        printf("len = %d\n",len);
        if(ntohs(((recv_pkt *)buf)->header.length) > len)
		{
            printf("44444444444444444444 %d\n",((recv_pkt *)buf)->header.length);
            int l = ntohs(((recv_pkt *)buf)->header.length) -len;
            printf("l = %d\n",l);
			int tmp_len = 0;
            char *start_addr = (char *)buf + sizeof(pkt_header);
			while(l > 0)
			{
                printf("666666666\n");
                tmp_len = read(cfd, start_addr, l>PKT_MAX_LEN?PKT_MAX_LEN:l);
                printf("tmp_len %d\n",tmp_len);
				l = l - tmp_len;
				len += tmp_len;
				start_addr +=tmp_len;
			}
		}	
        //printf("%s\n",((recv_pkt *)buf)->data);
        printf("8888888888888888\n");
        pkt_parse(buf);


	}
	
	close(cfd);
	close(sfd);
    //return 0;
}




