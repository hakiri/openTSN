/***************************************************************************
 *            fast_sys_dev.h
 *
 *  2018/09/18 21:17:17 星期二
 *  Copyright  2018  XuDongLai
 *  <XuDongLai0923@163.com>
 ****************************************************************************/
/*
 * fast_sys_dev.h
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
#ifndef __FAST_SYS_DEV_H__
#define __FAST_SYS_DEV_H__

/*---------------REG------------------*/
int fast_init_hw(u64 addr,u64 len);/** 硬件资源初始，NetMagic08则进行连接操作*/
void fast_distroy_hw(void);/** 销毁硬件资源信息，NetMagic08则进行释放连接操作*/



/*---------------RULE------------------*/
void print_hw_rule(void);/** 打印硬件规则(通过寄存器读返回,并显示)*/
void print_sw_rule(void);/** 打印软件缓存的规则*/
void init_rule(u32 default_action);	   /** 初始化规则模块,需要输入默认动作2017/06/01修改*/
int fast_add_rule(struct fast_rule *rule);/** 添加一条规则*/
int fast_modify_rule(struct fast_rule *rule,int idx);/** 修改指定位置的规则,函数内部会同步到硬件*/
int fast_del_rule(int idx);/** 删除一条规则,函数内部会同步到硬件*/
int read_hw_rule(struct fast_rule *rule,int index);/** 从硬件读取一条指定的规则内容*/

void set_rule_mac64(char *mac,u64 value);/** */
void set_rule_mac_oxm(char *mac,char *oxm);/** OXM是网络序*/
void set_rule_ipv6_oxm(char *ipv6,char *oxm);/** */
void oxm2rule(char *dst,char *oxm,int len);/** */
u32 fast_match_rule(struct flow *key);
void print_sw_rule_by_idx(int idx);
void print_user_rule(struct fast_rule *rule);


/**----UA库*/
void print_pkt(struct fast_packet *pkt,int pkt_len);/** 打印FAST结构报文*/

/*---------------Auto Measure System(AMS)------------------*/
int fast_ams_alloc(void);/** 获取硬件测量权限 */
void fast_ams_free(void);/** 释放测量权限  */
int fast_ams_send(struct fast_packet *send_pkt[],u64 pkt_space_ts[],int count);/** 发送测量报文，报文数组，对应时间间隔数组 */
void fast_ams_start(void);/** 启动硬件发送报文流程 */
int fase_ams_check(void);/** 读硬件状态，判断此次测量是否有效 */
int fast_ams_computer(struct fast_packet *recv_pkt[],u64 pkt_ts_result[],int count);/** 收到测量报文后，进行测量计算，结果存储在pkt_ts_result数组中 */


/*---------------OpenFlow Protocol(OFP)------------------*/
int ofp_init(int argc,char *argv[]);/** 初始化OFP通道*/
void ofp_exit(void);/** 退出OFP通道*/
#endif //__FAST_SYS_DEV_H__