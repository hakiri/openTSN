/** *************************************************************************
 *  @file          fast_vaddr.h
 *  @brief		FAST平台硬件虚拟地址空间定义
 * 
 *  FAST平台硬件的所有地址有一套规范的地址空间定义，根据功能需求进行地址分区管理
 *  其地址分区如下：\n
 *  
 *  @date		2017/01/05 13:31:40 星期四
 *  @author		XDL(Copyright  2017  XuDongLai)
 *  @email		<XuDongLai0923@163.com>
 *  @version	0.2.0
 ****************************************************************************/
/*
 * fast_vaddr.h
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
#ifndef __FAST_VADDR_H__
#define __FAST_VADDR_H__

#ifdef OPENBOX_S4
#include "openboxS4.h"
#elif OPENBOX_S28
#include "openboxS28.h"
#elif NETMAGIC08
#include "netmagic08.h"
#elif XDL_DEBUG
#include "xdl_debug.h"
#else
#error	"Please Select Platform(OPENBOX_S4/OPENBOX_S28/XDL_DEBUG)!"
#include "xdl_debug.h"
#endif

/** 系统默认UA使用MID值定义*/
#define FAST_DMID_PROTO_STACK   0x80	/**< 协议栈的MID值的定义,目的MID为0x80表示送到内核协议栈,大于0x80为软件模块，否则为硬件模块 */
#define FAST_UA_DFT_L2SWITCH	0x81	/**< 系统发布版本中的2层交换工程使用*/
#define FAST_UA_DFT_XOFP		0x82	/**< 系统发布版本中的OpenFlow交换机工程使用*/
#define FAST_UA_DFT_AMS			0x83	/**< 系统发布版本中的AMS测量模块工程使用*/
#define FAST_UA_DFT_L3ROUTE		0x84	/**< 系统发布版本中的3层路由工程使用*/
#define FAST_UA_DFT_CTL_RW		0x85	/**< 系统发布版本中的控制通路测试工程使用*/
#define FAST_UA_DFT_UA_FWD		0x86	/**< 系统发布版本中的UA示例转发与测试工程使用*/


#define SPEED_10        10
#define SPEED_100       100
#define SPEED_1000      1000
#define SPEED_2500      2500
#define SPEED_10000     10000

/*仅做申明*/
#define FAST_AMS_TX_TIME_H 0
#define FAST_AMS_TX_TIME_L 0
#define FAST_AMS_TX_STATUS 0
#define FAST_AMS_TX_START  0

#endif//__FAST_VADDR_H__
