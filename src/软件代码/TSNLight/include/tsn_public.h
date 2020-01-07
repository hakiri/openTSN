/***************************************************************************
 *            tsn_public.h
 *
 *  Wed May 08 10:10:41 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * tsn_public.h
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
#ifndef __TSN_PUBLIC_H__
#define __TSN_PUBLIC_H__
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>
#include <signal.h>
#include <memory.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <linux/netlink.h>
#include <netinet/in.h> /*IPv6 addr*/
#include <sys/file.h>
#include <sys/mman.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

typedef char s8;				/**< 有符号的8位（1字节）数据定义*/
typedef unsigned char u8;		/**< 无符号的8位（1字节）数据定义*/
typedef short s16;				/**< 有符号的16位（2字节）数据定义*/
typedef unsigned short u16;	/**< 无符号的16位（2字节）数据定义*/
typedef int s32;				/**< 有符号的32位（4字节）数据定义*/
typedef unsigned int u32;		/**< 无符号的32位（4字节）数据定义*/
typedef long long s64;				/**< 有符号的64位（8字节）数据定义*/
typedef unsigned long long u64;		/**< 无符号的64位（8字节）数据定义*/

#define MAC_LEN 6 //MAC地址长度

#define MAX_NODE_NUM 8 //最大节点数目

#define DEFAULT_SLOT_SIZE 250 //时间槽大小

#define TSN_CTRL_MID 150 //控制器MID

enum ERROR_CODE
{
	 G_ERR = -1,
	 G_OK = 1
};


#define TRUE 1
#define FALSE 0

#ifndef NULL
#define NULL 0
#endif

#define TSN_DEBUG 1
#undef TSN_DEBUG

#if TSN_DEBUG
	#define TSN_DBG(args...) do{printf("TSN-INFO:");printf(args);}while(0)
	#define TSN_ERR(args...) do{printf("TSN-ERROR:");printf(args);}while(0)
#else
	#define TSN_DBG(args...)
	#define TSN_ERR(args...)
#endif

#endif