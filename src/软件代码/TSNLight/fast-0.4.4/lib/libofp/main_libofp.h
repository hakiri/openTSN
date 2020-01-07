/***************************************************************************
 *            main_libofp.h
 *
 *  2017/02/27 09:51:58 星期一
 *  Copyright  2017  XuDongLai
 *  <XuDongLai0923@163.com>
 ****************************************************************************/
/*
 * main_libofp.h
 *
 * Copyright (C) 2017 - XuDongLai
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
#ifndef __MAIN_LIBOFP_H__
#define __MAIN_LIBOFP_H__

#include <sys/prctl.h>
#include <libnet.h>
#include <pcap.h>
#include <linux/if_ether.h>
#include <ifaddrs.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>

#include "ofp_v4.h"

#undef NMS_METER
/***************************/
#if 0
#define NMS_METER
#endif

#define MAX_IF_CNT 16
#define MAX_RULE_CNT 256
#define MAX_NEIGH_CNT 256



/*打印控制使用*/
static int print_idx_nms[MAX_IF_CNT] = {0};
static int __oft = 3;
/*打印控制使用*/

/*颜色控制*/
#define NONE          "\033[m"   
#define RED           "\033[0;32;31m"   
#define LIGHT_RED     "\033[1;31m"   
#define GREEN         "\033[0;32;32m"   
#define LIGHT_GREEN   "\033[1;32m"   
#define BLUE          "\033[0;32;34m"   
#define LIGHT_BLUE    "\033[1;34m"   
#define DARY_GRAY     "\033[1;30m"   
#define CYAN          "\033[0;36m"   
#define LIGHT_CYAN    "\033[1;36m"   
#define PURPLE        "\033[0;35m"   
#define LIGHT_PURPLE  "\033[1;35m"   
#define BROWN         "\033[0;33m"   
#define YELLOW        "\033[1;33m"   
#define LIGHT_GRAY    "\033[0;37m"   
#define WHITE         "\033[1;37m" 
/*颜色控制*/

static int debug = 0xFFF;/*默认不输出任何打印信息*/
enum
{
	_FUN = 1<<0,
	_DBG = 1<<1,
	_ERR = 1<<2,
};

static char P_COLOR[MAX_IF_CNT][24]  = 
{
	NONE,WHITE,YELLOW,LIGHT_BLUE,DARY_GRAY,CYAN,PURPLE,BROWN,LIGHT_GRAY,GREEN
};

#define IDX(x) (x=='n'?0:x-48)

#define PRINT(fmt,argv...)do{ \
	char _thread_name[16] = {0}; \
	prctl(PR_GET_NAME,_thread_name);  \
	printf(YELLOW"%s)"fmt,_thread_name,##argv); \
	}while(0);

#define LOG_DBG(fmt,argv...)do{ \
	char _thread_name[16] = {0}; \
	prctl(PR_GET_NAME,_thread_name); \
	int tid = IDX(_thread_name[0]); \
	if(debug < 1 || (debug & (1<<tid)) != (1<<tid))break; \
	int __print_idx_nms = print_idx_nms[tid]; \
	int _cnt = __print_idx_nms + __oft; \
	char _out[1024] = "=="; \
	while(_cnt-->0)sprintf(_out,"%s%s",_out," "); \
	printf("%s%s)%s"fmt,P_COLOR[tid],_thread_name,_out,##argv); \
}while(0);

#define LOG_ERR(fmt,argv...)do{ \
	char _thread_name[16] = {0}; \
	prctl(PR_GET_NAME,_thread_name); \
	if(1){printf(RED "ERROR! %s)%s:%s[%d]\n" fmt NONE,_thread_name,__func__,__FILE__,__LINE__,##argv);} \
	exit(0); \
}while(0);

#define SHOW_FUN(a)do{ \
	char _thread_name[16] = {0}; \
	prctl(PR_GET_NAME,_thread_name); \
	int _cnt = 0; \
	int tid = IDX(_thread_name[0]); \
	if(debug < 1 || (debug & (1<<tid)) != (1<<tid))break; \
	char _out[1024] = {0}; \
	int __print_idx_nms = print_idx_nms[tid]; \
	if(!a){__print_idx_nms += __oft;sprintf(_out,"%s",">>");}else{sprintf(_out,"%s","<<");} \
	_cnt = __print_idx_nms; \
	while(_cnt-->0)sprintf(_out,"%s%s",_out," "); \
	printf("%s%s)%s%s :%s[%d]\n",P_COLOR[tid],_thread_name,_out,__func__,__FILE__,__LINE__); \
	if(a){__print_idx_nms -= __oft;} \
	print_idx_nms[tid] = __print_idx_nms; \
}while(0);

#define DIV_ROUND_UP(X,Y) (((X) + ((Y) - 1)) / (Y))
#define ROUND_UP(X,Y) (DIV_ROUND_UP(X,Y) * Y)



struct ofp_buffer
{
	struct ofp_header header;
	u8 data[0];

}__attribute__((packed));	

struct eth_header
{
	u8 dmac[6];
	u8 smac[6];
	u16 frame;
	u8 data[0];
}__attribute__((packed));

struct meter_buffer
{
	u8 data[60];
	u32 ts;
	u8 in_port;
	u8 pad[3];
}__attribute__((packed));

struct netdev_stats {
	unsigned long long rx_packets;    /* total packets received       */
	unsigned long long tx_packets;        /* total packets transmitted    */
	unsigned long long rx_bytes;  /* total bytes received         */
	unsigned long long tx_bytes;  /* total bytes transmitted      */
	unsigned long rx_errors;      /* bad packets received         */
	unsigned long tx_errors;      /* packet transmit problems     */
	unsigned long rx_dropped;     /* no space in linux buffers    */
	unsigned long tx_dropped;     /* no space available in linux  */
	unsigned long rx_multicast;   /* multicast packets received   */
	unsigned long rx_compressed;
	unsigned long tx_compressed;
	unsigned long collisions;
	 
	/* detailed rx_errors: */
	unsigned long rx_length_errors;
	unsigned long rx_over_errors; /* receiver ring buff overflow  */
	unsigned long rx_crc_errors;  /* recved pkt with crc error    */
	unsigned long rx_frame_errors;        /* recv'd frame alignment error */
	unsigned long rx_fifo_errors; /* recv'r fifo overrun          */
	unsigned long rx_missed_errors;       /* receiver missed packet     */
	/* detailed tx_errors */
	unsigned long tx_aborted_errors;
	unsigned long tx_carrier_errors;
	unsigned long tx_fifo_errors;
	unsigned long tx_heartbeat_errors;
	unsigned long tx_window_errors;
};



struct nms_port
{
	u8 name[16];
	struct ofp_port state;
	struct ofp_port_stats stats;
	union
	{
		u32 port_ip4;
		struct in6_addr port_ip6;
		u8 ip[16];
	};
	union
	{
		u32 gw_ip4;
		struct in6_addr gw_ip6;
		u8 gw[16];
		u64 gw_value[2];
	};
	u8 gwmac[6];
	u8 port_mac[6];
	pcap_t *pcap;
	libnet_t *net;
	u32 logport;
	u32 phyport;
	u32 old_state;
	u16 speed;
	u16 updown;
	pthread_mutex_t port_send_mutex;
};

struct nms_ports_info
{
	int cnt;	
	struct nms_port ports[MAX_IF_CNT];
};
#endif //__MAIN_LIBOFP_H__
