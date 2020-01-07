/** *************************************************************************
 *  @file          fast.h
 *  @brief		FAST架构平台所有相关头文件定义
 * 
 *   此头文件包含了其他FAST相关的所有定义文件，如虚拟地址空间定义、数据类型定义
 *   数据结构定义、枚举类型定义和错误信息定义
 * 
 *  @date		2017/01/05 12:14:24 星期四
 *  @author		XDL(Copyright  2017  XuDongLai)
 *  @email		<XuDongLai0923@163.com>
 *  @version	0.2.0
 ****************************************************************************/
/*
 * fast.h
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
#ifndef __FAST_H__
#define __FAST_H__
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


/** 是否启动软件调试模式，不启用对硬件操作，对硬件寄存器读写进行屏蔽，仅做打印输出 */
#define XDL_DEBUG 1   /**< 启用软件调试模式 */
#undef XDL_DEBUG	/**< 不启用软件调试模式 */

/** 硬件平台使用ZYNQ系列的定义，20180601开始使用*/
#define OPENBOX_S4  1  /**< 硬件平台使用ZYNQ平台 */
//#undef OPENBOX_S4 /**< 硬件平台不使用ZYNQ平台 */

/** 硬件平台使用OpenBox-S28系列的定义，20180601开始使用*/
#define OPENBOX_S28  1  /**< 硬件平台使用ZYNQ平台 */
#undef OPENBOX_S28 /**< 硬件平台不使用ZYNQ平台 */


/*FAST架构数据结构版本号1.0*/
#define FAST_10 1
#undef FAST_10
/*FAST架构数据结构版本号2.0*/
#define FAST_20 1
//#undef FAST_20

/** 硬件查表模式是否使用BV查表，默认为顺序匹配查表 */
#define LOOKUP_BV 1   /**< 硬件查表使用BV */
#undef LOOKUP_BV	/**< 硬件查表不使用BV */


#include "fast_type.h"
#include "fast_struct.h"
#include "fast_err.h"
#include "fast_vaddr.h"
#include "fast_version.h"
#include "fast_sys_dev.h"
#include "ant_driver.h"

/*---------------REG------------------*/
u64 fast_reg_rd(u64 regaddr);/** 读硬件64位寄存器*/
void fast_reg_wr(u64 regaddr,u64 regvalue);/** 写硬件64位寄存器*/

/*---------------UA------------------*/
int fast_ua_init(int mid,fast_ua_recv_callback callback);/** UA模块初始化*/
void fast_ua_destroy(void);/** UA模块注销(销毁)*/
int fast_ua_send(struct fast_packet *pkt,int pkt_len);/** UA发送报文功能函数*/
void fast_ua_recv();/** UA启动报文接收线程(接收到报文后,回调用户注册函数)*/
void fast_ua_hw_wr(u8 dmid,u32 addr,u32 value,u32 mask);
u32 fast_ua_hw_rd(u8 dmid,u32 addr,u32 mask);

/*--------------DEBUG----------------*/
#ifdef FAST_KERNEL
#define PFX "fastK->"
#define EPFX "KERR-fast->"
#define FAST_DBG(args...) printk(PFX args)
#define FAST_ERR(args...) printk(EPFX args)
#else
#define PFX "fastU->"
#define EPFX "UERR-fast->"
#define FAST_DBG(args...) do{printf(PFX);printf(args);}while(0)
#define FAST_ERR(args...) do{printf(EPFX);printf(args);exit(0);}while(0)
#endif
#endif//__FAST_H__
