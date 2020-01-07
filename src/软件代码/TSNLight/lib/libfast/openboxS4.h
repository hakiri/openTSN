/*
 * openboxS4.h
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
#ifndef __OPENBOXS4_H__
#define __OPENBOXS4_H__

//硬件版本号
#define FAST_HW_REG_VERSION 0x0	/**< 硬件版本寄存器地址*/

//动作寄存器
#define FAST_ACTION_REG_ADDR 0x88000	/**< 写动作寄存器*/
#define FAST_DEFAULT_RULE_ADDR 0x1F8	/**< 默认动作(匹配不到规则时执行默认动作)地址*/

//规则寄存器
#define FAST_RULE_REG_WADDR 0x100000	   /**< 规则写寄存器*/
#define FAST_RULE_REG_RADDR 0x100000	   /**< 规则读寄存器*/
//#define FAST_RULE_REG_VADDR 0x210	   /**< 读规则返回值寄存器*/

//端口寄存器
/** 
 * 端口计数寄存器的访问由3部分组成：\n
 * 基地址|端口偏移地址*端口序号|计数寄存器地址\n
 * @par 示例说明
 * REG=FAST_PORT_BASE|FAST_PORT_OFT*port_idx|XX_REG;
 * 
 * XX_REG可以是计数寄存器、状态寄存器和端口的其他配置寄存器的地址
 */
#define FAST_PORT_BASE			0x180000 /**< 此为端口寄存器起始地址*/
#define FAST_PORT_OFT			0x010000   /**< 此为端口寄存器起始地址*/


//OpenFlow交换机流表计数器定义
#define FAST_OFP_FLOW_STATS_PKTS	0x8B000
#define FAST_OFP_FLOW_STATS_BYTES   0x8B800

#define BASE_ADDR		0x43C00000  /**< 硬件资源起始地址*/
#define REG_LEN			0x01000000	/**< 硬件资源可用空间*/

#define FAST_HW_SYS		0x0
#define FAST_HW_UM		0x80000
#define FAST_HW_LOOKUP  0x100000
/*
 * 0x43C00000,0x80000 SYS
 * 0x43C80000,0x80000 UM
 * 0x43D00000,0x80000 GME
 * 0x43D80000,0x10000 Port0
 * 0x43D90000,0x10000 Port0
 * 0x43DA0000,0x10000 Port0
 * 0x43DB0000,0x10000 Port0
 */

#define FAST_RULE_CNT   8  /**< S4目前只支持8条硬件流表数*/

//PORT COUNTS
#define FAST_COUNTS_RECV_BYTE_L		0x200	/**< 计数寄存器：成功接收字节数低位计数*/
#define FAST_COUNTS_RECV_BYTE_H		0x204	/**< 计数寄存器：成功接收字节数高位计数*/
#define FAST_COUNTS_SEND_BYTE_L		0x208	/**< 计数寄存器：成功发送字节数低位计数*/
#define FAST_COUNTS_SEND_BYTE_H		0x20C	/**< 计数寄存器：成功发送字节数高位计数*/

#define FAST_COUNTS_RECV_PKT_L		0x290	/**< 计数寄存器：接收成功的计数*/
#define FAST_COUNTS_RECV_PKT_H		0x294	/**< 计数寄存器：接收成功的计数*/
#define FAST_COUNTS_SEND_PKT_L		0x2D8	/**< 计数寄存器：发送成功的计数*/
#define FAST_COUNTS_SEND_PKT_H		0x2DC	/**< 计数寄存器：发送成功的计数*/


/*UM DEBUG*/
#define FAST_DEBUG_UM_DC	FAST_HW_UM+0x2000
#define FAST_DEBUG_UM_GPP   FAST_HW_UM+0x0000
#define FAST_DEBUG_UM_GKE   FAST_HW_UM+0x4000
#define FAST_DEBUG_UM_GME   FAST_HW_UM+0x6000
#define FAST_DEBUG_UM_GAC   FAST_HW_UM+0x8400
#define FAST_DEBUG_UM_GOE   FAST_HW_UM+0xA000

#define FAST_DEBUG_GPP_GPP_STATUS			FAST_DEBUG_UM_GPP+0x00
#define FAST_DEBUG_GPP_IN_GPP_DATA_COUNT	FAST_DEBUG_UM_GPP+0x08
#define FAST_DEBUG_GPP_OUT_GPP_MD_COUNT		FAST_DEBUG_UM_GPP+0x10
#define FAST_DEBUG_GPP_OUT_GPP_PHY_COUNT	FAST_DEBUG_UM_GPP+0x18
#define FAST_DEBUG_GPP_OUT_GPP_CACHE_COUNT  FAST_DEBUG_UM_GPP+0x20


#define FAST_DEBUG_GKE_GKE_STATUS			FAST_DEBUG_UM_GKE+0x00
#define FAST_DEBUG_GKE_IN_GKE_MD_COUNT		FAST_DEBUG_UM_GKE+0x08
#define FAST_DEBUG_GKE_IN_GKE_PHY_COUNT		FAST_DEBUG_UM_GKE+0x10
#define FAST_DEBUG_GKE_OUT_GKE_MD_COUNT		FAST_DEBUG_UM_GKE+0x18
#define FAST_DEBUG_GKE_OUT_GKE_PHY_COUNT	FAST_DEBUG_UM_GKE+0x20
#define FAST_DEBUG_GKE_OUT_GKE_KEY_COUNT	FAST_DEBUG_UM_GKE+0x28


#define FAST_DEBUG_GME_GME_STATUS			FAST_DEBUG_UM_GME+0x00
#define FAST_DEBUG_GME_IN_GME_MD_COUNT		FAST_DEBUG_UM_GME+0x08
#define FAST_DEBUG_GME_IN_GME_PHY_COUNT		FAST_DEBUG_UM_GME+0x10
#define FAST_DEBUG_GME_IN_GME_KEY_COUNT		FAST_DEBUG_UM_GME+0x18
#define FAST_DEBUG_GME_IN_GME_INDEX_COUNT   FAST_DEBUG_UM_GME+0x20
#define FAST_DEBUG_GME_OUT_GME_MD_COUNT		FAST_DEBUG_UM_GME+0x28
#define FAST_DEBUG_GME_OUT_GME_PHY_COUNT	FAST_DEBUG_UM_GME+0x30
#define FAST_DEBUG_GME_OUT_GME_KEY_COUNT	FAST_DEBUG_UM_GME+0x38


#define FAST_DEBUG_GAC_GAC_STATUS			FAST_DEBUG_UM_GAC+0x00
#define FAST_DEBUG_GAC_IN_GAC_DATA_COUNT	FAST_DEBUG_UM_GAC+0x08
#define FAST_DEBUG_GAC_IN_GAC_MD_COUNT		FAST_DEBUG_UM_GAC+0x10
#define FAST_DEBUG_GAC_IN_GAC_PHY_COUNT		FAST_DEBUG_UM_GAC+0x18
#define FAST_DEBUG_GAC_OUT_GAC_DATA_COUNT   FAST_DEBUG_UM_GAC+0x20
#define FAST_DEBUG_GAC_OUT_GAC_PHY_COUNT	FAST_DEBUG_UM_GAC+0x28


#define FAST_DEBUG_GOE_GOE_STATUS			FAST_DEBUG_UM_GOE+0x00
#define FAST_DEBUG_GOE_IN_GOE_DATA_COUNT	FAST_DEBUG_UM_GOE+0x08
#define FAST_DEBUG_GOE_IN_GOE_PHY_COUNT		FAST_DEBUG_UM_GOE+0x10
#define FAST_DEBUG_GOE_OUT_GOE_DATA_COUNT   FAST_DEBUG_UM_GOE+0x18


#define FAST_DEBUG_DC_DATA_CACHE_STATUS		FAST_DEBUG_UM_DC+0x00
#define FAST_DEBUG_DC_IN_DATA_CACHE_COUNT   FAST_DEBUG_UM_DC+0x08
#define FAST_DEBUG_DC_OUT_DATA_CACHE_COUNT  FAST_DEBUG_UM_DC+0x10
#endif//__OPENBOXS4_H__
