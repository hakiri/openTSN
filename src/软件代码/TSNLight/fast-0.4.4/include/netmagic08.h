/***************************************************************************
 *            netmagic08.h
 *
 *  2018/08/19 17:54:30 星期日
 *  Copyright  2018  XuDongLai
 *  <XuDongLai0923@163.com>
 ****************************************************************************/
/*
 * netmagic08.h
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
#ifndef __NETMAGIC08_H__
#define __NETMAGIC08_H__

//硬件版本号
#define FAST_HW_REG_VERSION 0x0	/**< 硬件版本寄存器地址*/

//动作寄存器
#define FAST_ACTION_REG_ADDR 0x20000	/**< 写动作寄存器*/
#define FAST_DEFAULT_RULE_ADDR 0x1F8	/**< 默认动作(匹配不到规则时执行默认动作)地址*/

//规则寄存器
#define FAST_RULE_REG_WADDR 0x8000	   /**< 规则写寄存器*/
#define FAST_RULE_REG_RADDR 0x8008	   /**< 规则读寄存器*/
#define FAST_RULE_REG_VADDR 0x8010	   /**< 读规则返回值寄存器*/

//端口寄存器
/** 
 * 端口计数寄存器的访问由3部分组成：\n
 * 基地址|端口偏移地址*端口序号|计数寄存器地址\n
 * @par 示例说明
 * REG=FAST_PORT_BASE|FAST_PORT_OFT*port_idx|XX_REG;
 * 
 * XX_REG可以是计数寄存器、状态寄存器和端口的其他配置寄存器的地址
 */
#define FAST_PORT_BASE			0x40000 /**< 此为端口寄存器起始地址*/
#define FAST_PORT_OFT			0x200   /**< 此为端口寄存器起始地址*/

#define BASE_ADDR 0
#define REG_LEN 0
#define FAST_RULE_CNT   32  /**< 硬件流表条数*/

#define  FAST_COUNTS_RECV_BYTE_L 0
#define  FAST_COUNTS_SEND_BYTE_L 0
#define  FAST_COUNTS_RECV_PKT_L 0
#define  FAST_COUNTS_SEND_PKT_L 0

#endif//__NETMAGIC08_H__