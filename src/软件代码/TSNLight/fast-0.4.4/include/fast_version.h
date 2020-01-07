/***************************************************************************
 *            fast_version.h
 *
 *  2018/09/18 17:47:43 星期二
 *  Copyright  2018  XuDongLai
 *  <XuDongLai0923@163.com>
 ****************************************************************************/
/*
 * fast_version.h
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

#ifndef __FAST_VERSION_H__
#define __FAST_VERSION_H__

/*文档编写规范：
 *以该版本定义说明开始
 * 最后一行为该版本定义的最新版本号，并带有详细描述
 * 中间注释以前使用过的版本，并对历史版本做出详细描述
 */

/** @brief 寄存器读写功能库的版本号定义 */
/*标准的寄存器读写控制方法，读写接口均使用64位地址（如果是32位系统，则拆分为2次操作）*/
#define LIBREG_VERSION "20190404"

/** @brief openflow协议通道库的版本号定义 */
/*该库包含了ofp协议通道相关功能，与控制器建立连接、交互状态与统计信息，支持流表配置等功能*/
#define LIBOFP_VERSION "20190404"


/** @brief 规则操作库的版本号定义 */
/*基于FAST架构，参考openflow流表规范定义的一种硬件流表管理示例开发库*/
#define LIBRULE_VERSION "20190404"

/** @brief UA开发库的版本号定义 */
/*基于FAST架构，提供用户应用（UA）的编程开发接口，支持收发硬件流水线报文，
 * 支持对硬件流水线模块进行管理配置（环形控制通路方式）
 */
#define LIBUA_VERSION "20190404"

/** @brief 自动测量系统库的版本号定义 */
/*自动测量系统是UA编程开发的一个具体示例，配合硬件AMS流水线模块一起完成网络测量功能
 *硬件最大支持一次提交32个报文，并按照每个报文携带的具体时间进行发送，回环测试时还
 * 可以对每个报文进行统计，计算每个报文的传输时间等信息
 */
#define LIBAMS_VERSION "20190404"


/** @brief 二层交换应用的版本号定义 */
#define L2SWITCH_VERSION "20190404"
/** @brief OpenFlow通道应用的版本号定义 */
#define XOF_VERSION "20190404"
/** @brief 自动测量系统应用的版本号定义 */
#define AMS_VERSION "20190404"


/** @brief 寄存器读写应用的版本号定义 */
#define REG_RW_VERSION "20190404"
/** @brief 端口计数应用的版本号定义 */
#define PORT_COUNTS_VERSION "20190404"
/** @brief 二层交换应用的版本号定义 */
#define CTL_RW_VERSION "20190404"
/** @brief MEM_RW应用的版本号定义 */
#define MEM_RW_VERSION "20190404"
/** @brief 硬件规则读写应用的版本号定义 */
#define RULE_RW_VERSION "20190404"
/** @brief 端口状态应用的版本号定义 */
#define PORT_STATUS_VERSION "20190404"
/** @brief UM模块DEBUG应用的版本号定义 */
#define DEBUG_VERSION "20190404"
/** @brief VERSION应用的版本号定义 */
#define VERSION_VERSION "20190404"
/** @brief UA_FWD应用的版本号定义 */
#define UA_FWD_VERSION "20190404"
#endif //__FAST_VERSION_H__