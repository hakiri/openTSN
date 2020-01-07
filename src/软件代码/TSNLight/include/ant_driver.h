/** *************************************************************************
 *  @file          ant_driver.h
 *  @brief		ANT相关函数头文件定义
 * 
 *   此头文件包含了ANT相关的所有定义文件，如虚拟地址空间定义、数据类型定义
 *   数据结构定义、枚举类型定义和错误信息定义
 * 
 *  @date		2018/10/03 12:14:24 星期三
 *  @author		Yang(Copyright  2018  Yang Xiangrui)
 *  @email		<nudtyxr@hotmail.com>
 *  @version	0.1.0
 ****************************************************************************/
/*
 * ant_driver.h
 *
 * Copyright (C) 2018 - Yang Xiangrui
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
 * */

#ifndef __ANT_DRIVER_H__
#define __ANT_DRIVER_H__

#include <unistd.h>
//#include "fast_type.h"

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

/*
//declaration of vaddr on ANT-PGM
 */
#define SENT_TIME_CNT 0x00000002         // 64b, test time counter                  
#define SENT_TIME_REG 0x00010002         // 64b, test time reg
#define WR_SOFT_RST   0x00000000         //  1b, software rst for pgm_wr
#define SENT_RATE_CNT 0x00000001         // 32b, sent rate counter
#define SENT_RATE_REG 0x00010001         // 32b, sent rate reg
#define LAT_PKT_CNT   0x00000002         // 32b, for latency test cnt
#define LAT_PKT_REG   0x00010002         // 32b, for latency test reg
#define SENT_BIT_CNT  0x00000004         // 64b, total sent bits
#define SENT_PKT_CNT  0x00000006         // 64b, total sent packets
#define LAT_FLAG      0x00010010         //  1b, latency test flag  
#define RD_SOFT_RST   0x00000000		 //  1b, software rst for pgm_rd
#define RD_STATE	  0x11111111		 //  6b, pgm_rd state machine

/*
//declaration of vaddr on ANT (SCM part)
 */
#define SCM_STATE 	  0x70000000		  //  3b, scm_state machine
#define SCM_SOFT_RST  0x70000001		  //  1b, software rst for SCM
#define N_RTT         0x70000002		  // 32b, total wait time after receiving finish signal
#define PROTO_TYPE    0x70000003		  //  8b, protype of packets
#define SCM_BIT_CNT   0x70000009		  // 64b, total received bits of SCM 
#define SCM_PKT_CNT   0x7000000b		  // 64b, total received packets of SCM
//#define SCM_TIME_CNT  0x7000000d		  // 64b, total monitoring time of SCM
#define SCM_RAM_ADDR  0x80000000 		  // 32b, the base addr of latency in SCM_RAM



#define ANT_HW_STATE  0x11111111

/*
//decalartion of state machine of PGM and SCM
 */
#define SCM_FETCH_S   4
#define PGM_RD_FIN_S  16
/**
 * 
 */
struct ant_cnt
{
	u64 test_time; /** 共计测试时间 */
	u64 sent_bits; /** total sent bytes of pcakets */
	u64 sent_pkts; /** total sent num of packets */
	u64 recv_bits; /** total received bytes of sending packets*/
	u64 recv_pkts; /** total received number of sending packets*/
};

struct ant_parameter
{
	u64 sent_time;  /**total testing time period*/
	u32 sent_rate;  /** sending rate, num of cycles between two packets*/
	u32 lat_pkt;    /** blocking num pof packet between two latency flag packets*/
	u8  lat_flag;   /** Flag for enabling latency test*/
	u32 n_rtt;      /** Controlling the waiting time after sending last packet*/
	u8  proto_type;  /** Protocol type used in SCM*/
};

/*-------------------ANT CORE FUNCTION ------------------*/

int  ant_collect_counters(struct ant_cnt *result); /**获取FPGA中相关计数器的当前值*/

int  ant_rst();  /**将ANT的硬件模块重置为idle状态*/

int  ant_check_finish(); /**检查ANT hw是否已经运行结束*/

int  ant_pkt_send(struct fast_packet *pkt, int pkt_len); /**发送ant报文，并触发ANT开始进行测试*/

u32  ant_dich_throughput_test(struct fast_packet *pkt, int pkt_len, int rnd, u32 sent_rate, u64 test_time); /**通过二分法测量吞吐率*/

int  ant_set_test_para(struct ant_parameter antp); /**设置ANT测试参数*/

//@TODO support latency test in the future
//int  ant_latency_test(struct fast_packet *pkt, int pkt_len); /**测量平均时延*/

void ant_print_counters(struct ant_cnt a_cnt, int len);

int import_latency_to_txt(); /**测量时延，并将时延信息写入latency.txt文件*/

/*-------------------ANT CORE FUNCTION ------------------*/



/*-------------------SET COUNTER & REG------------------*/

//caution that not all reg values could be set

int ant_set_sent_time_cnt(u64 regvalue);
int ant_set_sent_time_reg(u64 regvalue);
int ant_set_sent_rate_cnt(u64 regvalue);
int ant_set_sent_rate_reg(u64 regvalue);
int ant_set_lat_pkt_cnt(u64 regvalue);
int ant_set_lat_pkt_reg(u64 regvalue);
int ant_set_sent_bit_cnt(u64 regvalue);
int ant_set_sent_pkt_cnt(u64 regvalue);
int ant_set_lat_flag(u64 regvalue);
int ant_set_rd_soft_rst(u64 regvalue);


int ant_set_proto_type(u64 regvalue);
int ant_set_scm_soft_rst(u64 regvalue);
int ant_set_n_rtt(u64 regvalue);
int ant_set_scm_bit_cnt(u64 regvalue);
int ant_set_scm_pkt_cnt(u64 regvalue);

/*-------------------COUNTER & REG------------------*/


/*-------------------GET COUNTER & REG------------------*/

int ant_get_sent_time_cnt(u64 *regvalue);
int ant_get_sent_time_reg(u64 *regvalue);
int ant_get_sent_rate_cnt(u64 *regvalue);
int ant_get_sent_rate_reg(u64 *regvalue);
int ant_get_lat_pkt_cnt(u64 *regvalue);
int ant_get_lat_pkt_reg(u64 *regvalue);
int ant_get_sent_bit_cnt(u64 *regvalue);
int ant_get_sent_pkt_cnt(u64 *regvalue);
int ant_get_lat_flag(u64 *regvalue);
int ant_get_rd_soft_rst(u64 *regvalue);


int ant_get_proto_type(u64 *regvalue);
int ant_get_scm_soft_rst(u64 *regvalue);
int ant_get_n_rtt(u64 *regvalue);
int ant_get_scm_bit_cnt(u64 *regvalue);
int ant_get_scm_pkt_cnt(u64 *regvalue);
/*-------------------COUNTER & REG-------------------*/

#endif
