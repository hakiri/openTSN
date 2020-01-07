/***************************************************************************
 *            main_l3route.c
 *
 *  2018/11/12 17:03:05 星期一
 *  Copyright  2018  XuDongLai
 *  <XuDongLai0923@163.com>
 ****************************************************************************/
/*
 * main_l3route.c
 *
 * Copyright (C) 2018 - XuDongLai
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
#include "../../include/fast.h"
#include <netinet/if_ether.h>

#define FAST_ROUTE_PORT_CNT 4
#define FAST_NEIGH_MAX 8

struct neigh_table
{	
	u8 mac[6];
	u8 pad[2];
	u32 ip4;
	u32 time;
	struct in6_addr ip6;
};

struct port_stats
{
	u64 recv_pkts;
	u64 recv_bytes;
	u64 send_pkts;
	u64 send_bytes;
};/*32B*/

struct port_info
{
	u8 mac[6];
	u16 portidx;	/*端口物理端口号*/
	u8 updown;		/*端口UP或DOWN状态*/
	u8 flag;		/*标志位，XXXX*/
	u16 state;		/*接口状态*/
	u32 ip4;		/*接口IPv4地址，目前支持一个地址*/
	struct in6_addr ip6;/*IPv6地址，目前支持一个地址*/	
	struct neigh_table neigh[FAST_NEIGH_MAX];/*端口的邻接表*/
	struct port_stats stats;/*端口计数信息*/
};

struct fib4
{	
	u32 ip;			/*网络地址*/
	u32 mask;		/*掩码*/
	u32 gw;			/*网关*/
	u32 portidx;	/*输出端口号*/
};

struct fib6
{
	struct in6_addr ip;		/*网络地址*/
	struct in6_addr mask;   /*掩码*/
	struct in6_addr gw;		/*网关*/
	u32 portidx;			/*输出端口号*/
	u32 pad;
};

void create_arp_request(u32 dip)
{
}

int main(int argc,char* argv[])
{
	//FAST_UA_DFT_L3ROUTE
}