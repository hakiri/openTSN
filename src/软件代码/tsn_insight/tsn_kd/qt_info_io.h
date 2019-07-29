#ifndef QT_INFO_IO_H
#define QT_INFO_IO_H


#include "stdio.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

typedef char s8;				/**< 有符号的8位（1字节）数据定义*/
typedef unsigned char u8;		/**< 无符号的8位（1字节）数据定义*/
typedef short s16;				/**< 有符号的16位（2字节）数据定义*/
typedef unsigned short u16;	/**< 无符号的16位（2字节）数据定义*/
typedef int s32;				/**< 有符号的32位（4字节）数据定义*/
typedef unsigned int u32;		/**< 无符号的32位（4字节）数据定义*/
typedef long long s64;				/**< 有符号的64位（8字节）数据定义*/
typedef unsigned long long u64;		/**< 无符号的64位（8字节）数据定义*/


#define MAX_SW_NUM 6
#define MAX_HOST_NUM 6

#define MAC_TSN_FLOW_NUM 3
#define MAC_BD_FLOW_NUM 3



#define PKT_MAX_LEN 65535
#define MASTER 0
#define SLAVE 1
#define SW_CNT 6

#define TSN_FLOW 0
#define BD_FLOW 1


#define SWITCH_TYPE 0
#define HOST_TYPE 1


struct topology_info
{
    u8 sw_id;
    u8 sync_type;
    u8 host_id;
    u8 sw_mac[6];
    u8 host_mac[6];
    u8 next_sw_id;
    u8 prev_sw_id;
    u8 next_port;
    u8 prev_port;

};

struct resource_info
{
    u8 sw_id;
    u32 cqf_total_len;
    u32 cqf_free_len;
    u32 cqf_used_len;
    u32 total_bd;
    u32 free_bd;
    u32 used_bd;
    u32 tb_rate;
    u8  direction;
    u32 time_slot;

};

struct tsn_feature
{
    //u8 dst_sw_id;
    u8 flow_type;    //流类型，tsn流
    u16 flow_id;    //流ID
    u8 src_mac[6];  //源端系统MAC地址
    u8 dst_mac[6];  //目的端系统MAC地址
    u8 priority;    //目的端系统MAC地址
    u8 interval;    //时间周期
    u16 pkt_num;    //周期内发送数量
    u16 pkt_size;   //报文大小
    u16 latency;     //最大传输延迟
    u8 path_len;    //路径长度
    u8 direction;   //传输方向:1为正向（1口出，0口进）,0为反向
    u8 src_sw_id;   //路径起始的交换机ID
    u8 dst_sw_id;   //路径终点的交换机ID

};

struct bd_feature
{
    //u8 dst_sw_id;
    u8 flow_type;    //流类型，tsn流
    u16 flow_id;    //流ID
    u8 src_mac[6];  //源端系统MAC地址
    u8 dst_mac[6];  //目的端系统MAC地址
    u8 priority;    //目的端系统MAC地址
    u32 bandwidth;  //带宽
    u8 path_len;    //路径长度
    u8 direction;   //传输方向:1为正向（1口出，0口进）,0为反向
    u8 src_sw_id;   //路径起始的交换机ID
    u8 dst_sw_id;   //路径终点的交换机ID
};


struct flow_feature
{
    s32 tsn_num;
    s32 bd_num;
    struct tsn_feature tsn_flow[MAC_TSN_FLOW_NUM];
    struct bd_feature  bd_flow[MAC_BD_FLOW_NUM];
};

struct counter_info
{
    u64 esw_pkt_in_cnt;
    u64 esw_pkt_out_cnt;
    u8 buf_id_rest_cnt;
    u64 eos_md_in_cnt;
    u64 eos_md_out_cnt;
    u8 eos_q0rest_cnt;      //CQF0
    u8 eos_q1rest_cnt;      //CQF1
    u8 eos_q2rest_cnt;      //bd
    u8 eos_q3rest_cnt;      //best effort
    u64 goe_pkt_in_cnt;
    u64 goe_port0_out_cnt;
    u64 goe_port1_out_cnt;
    u64 goe_discard_cnt;

};

struct switch_info
{
    u64 time_offset;    //时间同步偏移量
    u8 sync_type;       //主、从
    struct resource_info  res_info;//资源信息
    struct counter_info   cnt_info;//计数器信息
};

struct host_info
{

    u64 time_offset;    //时间同步偏移量
    u8 sync_type;       //主、从
};

struct global_info
{
    u64    seq;
    u64    global_time_offset;
    struct switch_info  sw_info[MAX_SW_NUM];    //交换机数组,数组的下标即为sw_id
    struct host_info    hs_info[MAX_HOST_NUM];  //主机数组,数组的下标即为host_id
};




extern struct global_info  global;  //
extern struct flow_feature flow;    //
extern struct topology_info tp_info[MAX_SW_NUM];//数组的下标即为sw_id







#endif // QT_INFO_IO_H
