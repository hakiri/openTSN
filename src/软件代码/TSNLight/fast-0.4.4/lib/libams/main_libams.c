/** *************************************************************************
 *  @file       main_libams.c
 *  @brief		FAST架构下硬件测量模块(AMS)功能支持库
 *
 *  FAST平台支持硬件进行网络测量功能，可以由硬件实现精准时间的报文，接收报文可携带精准接收时间
 *  测量处理过程中，用户输入和计算得到的测量时间均为ns，硬件内部不同时钟频率换算已经在库内部完成。\n
 *  测量发送报文时间精度为：6.7ns(150M)\n
 *  测量接收报文时间精度为：8ns(125M)\n
 *  
 *  @author		XDL(Copyright  2017  XuDongLai)
 *  @email		<XuDongLai0923@163.com>
 *  @version	0.2.0
 *  @date		2017/03/23 15:23:59 星期四
 ****************************************************************************/
/*
 * main_libams.c
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

#include "../../include/fast.h"



#define AMS_MAX_PKTS 32
#define AMS_CLK 3		//150M时钟,1000/150 = 20/3
#define UM_CLK 8		//125M时钟

u64 send_pkt_ts[AMS_MAX_PKTS];
int ams_pkt_count = 0;

/**
 * @brief 硬件测量报文自定义数据部分
 */
struct fast_ams_head
{
	u64 ts;
	u64 ctl;
	u64 id:16,
		C:1,
		W:1,
		S:1,
		reserve:32,
		reserve2:13;
	u64 delay;
};

/**
* @brief 获取硬件测量权限
*  每个不同进程使用硬件测量模块时，都必须先调用此函数，以获得硬件的操作权限
* 如果获得权限，可以开始进行进行测量的后续操作；若获取权限失败，需要等待一会后再尝试
* @return   0：表示获得测量权限，可以开始进行测量\n
*			其他：表示当前正在进行测量的进程ID号
*/
int fast_ams_alloc(void)
{
	/*取当前进程号，写入硬件，并读返回。
	 *返回值与写入值相等，说明获得硬件测量权限*/
	return 0;/**/
}

/** 
 * @brief 释放测量权限
 * 执行完一次测量后，主动释放测量权限，以便其他应用可以获取测量权限
 */
void fast_ams_free(void)
{
	/*释放测量权限，另外的进程可以开始进行测量*/
}

/**
* @brief 发送测量报文到硬件AMS模块
*
* 先判断发送报文个数是否超限制，然后关闭硬件发送逻辑
 * 再判断首报文时间间隔是否为0，同时将其他所有时间计算好后填充
 * 到对应报文metadata字段中，最后把所有报文发送到硬件AMS模块 
* @param send_pkt[]		：需要发送的测量报文数组
* @param pkt_space_ts[] ：每前后两个测量报文的发送间隔数组，数组第0个元素的值为0
* @param count			：上述两个数组对应的有效元素个数，不能超过硬件最大存储的报文个数
*
* @return		0：表示所有测量报文成功发送到硬件AMS模块\n
				-1：表示发送报文个数超过硬件最大支持个数\n
				-2：表示发送的首报文时间间隔不为0\n
				-3：表示有报文发送到硬件时失败
*/
int fast_ams_send(struct fast_packet *send_pkt[],u64 pkt_space_ts[],int count)
{
	/**/
	/*填充报文间隔到metadata*/
	/*发送报文逻辑*/
	/*启动硬件发送*/
	int i = 0,ret = 0;
	struct fast_ams_head *ams_head = NULL;
	if(count > 32 || count < 2)return -1;/**< 一个报文或超过32个报文不支持测试*/
	fast_reg_wr(FAST_AMS_TX_START,0);/*先关闭硬件发送逻辑*/
	for(;i<count;i++)
	{
		ams_head = (struct fast_ams_head *)send_pkt[i];
		if(i==0)/*测量首报文，打上状态标志让硬件识别为触发发条*/
		{
			if(pkt_space_ts[0] != 0)
			{
				return -2;/*首报文要求时间间隔为0*/
			}
			ams_head->S = 1;
			ams_head->W = 1;
		}
		send_pkt[i]->um.dstmid = 1;/*将报文送到测量模块进行处理*/
		send_pkt_ts[i] = pkt_space_ts[i];
		ams_head->delay = pkt_space_ts[i]*AMS_CLK;/*将每个报文的发送间隔保存在metadata中*/
		ams_head->delay /= 20;/*将延时时间换算成测量模块的拍数计算*/

		if(send_pkt[i]->um.len != fast_ua_send (send_pkt[i],send_pkt[i]->um.len))
		{
			fast_ams_free();/*释放发送权限*/
			return -3;/*软件报文发送不成功*/
		}		
	}
	ams_pkt_count = count;/*记录发送测量报文个数*/
	return 0;/*测量报文软件发送成功*/
}

/**
* @brief 启动测量报文发送
 * 
 * 启动FAST的硬件测量模块发送报文，只需要给测量模块的状态寄存器写1即可
*/
void fast_ams_start(void)
{
	fast_reg_wr(FAST_AMS_TX_START,1);/*启动硬件开始发送报文*/
}

/**
* @brief 检测测量报文的发送状态
*
 * 判断硬件测量发送是不是每一个报文都按照指定时间完成了发送要求
* @return   0：表示此将测量报文发送完全按照指定时间要求完成了，说明测量报文发送成功
 *			-1：表示有报文发送没有严格按照指定时间完成，本次测量失败
*/
int fase_ams_check(void)
{
	/*判断硬件此次报文发送是否按照指定时间间隔进行了发送，读硬件寄存器标志，成功，说明此次测量有效*/
	/*读发送状态寄存器，失败则返回*/
	/*发送成功，则读到发送时刻值，并根据时间间隔计算所有报文时刻*/
	//return 0;
	u64 ams_state = fast_reg_rd(FAST_AMS_TX_STATUS);
	
	if(ams_state == 0)/*硬件返回0，表示本次测量有效，返回1，表示测量无效*/
	{
		u64 send_time = 0;

		send_time = fast_reg_rd(FAST_AMS_TX_TIME_H)<<32;		
		send_time |= fast_reg_rd(FAST_AMS_TX_TIME_L);
		send_pkt_ts[0] = send_time * 20/AMS_CLK;/*记录首报文时间,并转化为ns计算*/
		/*需要报文长度，再修正时间，放到下面计算时再处理*/
		return 0;/*本次测量有效,且记录了所有报文的绝对发送时刻*/
	}
	return -1;/*本次测量无效*/
}

/*
 * recv_pkt
 * pkt_space_ts
 * count
 */
/**
* @brief 
*
* @param recv_pkt[]			：接收到的测量报文数组
* @param pkt_ts_result[]	：用来计算存储报文传输间隔的数组
* @param count				：数组大小，前两数据应该相同大小
*
* @return   0：表示计算完成\n
			-1：表示计算前后的两次数据定义不相等（主要原因是发送报文与接收报文数不相等）
* @note 在调用此函数之前，一定要先调用fase_ams_check函数进行判断，判断此次报文发送是否成功 @see ::fase_ams_check
*/
int fast_ams_computer(struct fast_packet *recv_pkt[],u64 pkt_ts_result[],int count)
{
	/*计算本次测量结果，系统内先保存原来的测量时间，再通过用户输入接收报文，时行时间差计算，存储在pkt_ts_result中*/
	/*接收报文时刻提取，频率换算*/
	/*与原报文发送时刻做减法，写入pkt_ts_result*/
	int i = 0;

	if(ams_pkt_count != count)
	{
		return -1;/*发送测量报文与接收测量报文数据不相等，无法计算*/
	}
	
	send_pkt_ts[0] -= (recv_pkt[0]->um.len+16)*20/(16*3);/*记录时刻为报文发送完成时间，故开始发送时间要减去发送此长度报文的处理时间*/
	for(i=1;i<ams_pkt_count;i++)
	{
		/*当前报文发送时间＝前一个报文发送时刻+前一个报文发送处理时长+报文间隔（在等号左边）*/
		send_pkt_ts[i] += send_pkt_ts[i-1] + (recv_pkt[i-1]->um.len+16)*20/(16*3);/*计算每个报文的发送时刻（绝对时间）*/
	}
	
	printf("ID     Len      Send             Recv               Interval\n");
	for(i=0;i<ams_pkt_count;i++)
	{				
		pkt_ts_result[i] = recv_pkt[i]->um.ts*UM_CLK - send_pkt_ts[i];/*接收报文时间转换为ns*/	
		printf("%3d  %4d  %16ld %16lld %16lld\n",i,recv_pkt[i]->um.len,(long)recv_pkt[i]->um.ts*UM_CLK,send_pkt_ts[i],pkt_ts_result[i]);
	}
	return 0;/*测量报文延时计算成功,时间差存储在pkt_ts_result数组中*/
}
