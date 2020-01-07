/***************************************************************************
 *            main_ua_fwd.c
 *
 *  2018/10/13 11:22:01 星期六
 *  Copyright  2018  XuDongLai
 *  <XuDongLai0923@163.com>
 ****************************************************************************/
/*
 * main_ua_fwd.c
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

int outport = -1;
int debug = 0;
#define xprintf(argc...) if(debug)printf(argc);

/**
* @brief 
*
* @param pkt
* @param pkt_len
*/
void pkt_send_normal(struct fast_packet *pkt,int pkt_len)
{
	xprintf("pkt_send_normal->%p,outport:%d,len:%d\n",pkt,pkt->um.outport,pkt_len);
	pkt->um.pktsrc = 1;/*报文来源为CPU输入*/
	pkt->um.pktdst = 0;/*报文目的为硬件输出*/
	pkt->um.dstmid = 5;/*直接从硬件GOE模块输出，不走解析、查表等模块*/
	fast_ua_send(pkt,pkt_len);
}

/**
* @brief 
*
* @param pkt
* @param pkt_len
*/
void pkt_send_flood(struct fast_packet *pkt,int pkt_len)
{
	int i = 0,inport = pkt->um.inport;/*保存输入端口*/
	xprintf("------pkt_send_flood------\n");
	for(;i< 4;i++)/*除输入端口外，其他都发送一份*/
	{
		if(i != inport)
		{
			pkt->um.outport = i;
			pkt_send_normal(pkt,pkt_len);
		}
	}
}

/**
* @brief 
*
* @param pkt
* @param pkt_len
*
* @return 
*/
int callback(struct fast_packet *pkt,int pkt_len)
{
	xprintf("inport:%d,dstmid:%d,len:%d,dmac:%02X:%02X:%02X:%02X:%02X:%02X,smac:%02X:%02X:%02X:%02X:%02X:%02X\n",pkt->um.inport,pkt->um.dstmid,pkt_len,
	       pkt->data[0],pkt->data[1],pkt->data[2],pkt->data[3],pkt->data[4],pkt->data[5],
	       pkt->data[6],pkt->data[7],pkt->data[8],pkt->data[9],pkt->data[10],pkt->data[11]);

	/*发送报文*/
	switch(outport)
	{
		case -2:						/*软件端口转发测试（0<-->1,2<-->3)*/
			if(pkt->um.inport == 0)
				pkt->um.outport = 1;
			else if(pkt->um.inport == 2)
				pkt->um.outport = 3;
			else if(pkt->um.inport == 3)
				pkt->um.outport = 2;
			else if(pkt->um.inport == 1)
				pkt->um.outport = 0;
			pkt_send_normal(pkt,pkt_len);/*正常发送报文*/
			break;
		case -1:						/*端口泛洪测试*/
			pkt_send_flood(pkt,pkt_len);/*泛洪发送，保留输入端口不变调用*/
			break;
		default:
			pkt->um.outport = outport;/*修改报文输出端口号*/
			pkt_send_normal(pkt,pkt_len);/*正常发送报文*/
			break;
	}
	
	return 0;
}

/**
* @brief 
*/
void ua_init(u8 mid)
{
	int ret = 0;
	/*向系统注册，自己进程处理报文模块ID为1的所有报文*/
	if((ret=fast_ua_init(mid,callback)))//UA模块实例化(输入参数1:接收模块ID号,输入参数2:接收报文的回调处理函数)
	{
		perror("fast_ua_init!\n");
		exit (ret);//如果初始化失败,则需要打印失败信息,并将程序结束退出!
	}
}

/**
* @brief 
*
* @param argc
* @param argv[]
*
* @return 
*/
int main(int argc,char* argv[])
{
	u8 mid = FAST_UA_DFT_UA_FWD;
	debug = 1;
	if(argc == 2)
	{
		debug = atoi(argv[1]);		
	}
	else if(argc == 3)
	{
		debug = atoi(argv[1]);
		mid = atoi(argv[2]);
	}
	else if(argc == 4)
	{
		debug = atoi(argv[1]);
		mid = atoi(argv[2]);
		outport = atoi(argv[3]);
	}
	else
	{
		printf("Usage:\n\t%s debug mid port(all param decimal)\n",argv[0]);
		printf("\tdeubg[0,1],UA mid rang [129,255],port[-1:flood,-2:test(0<->1,2<->3)]!\n");
		exit(0);
	}

	if(mid <129)
	{
		mid = FAST_UA_DFT_UA_FWD;
	}	

	/*UA模块初始化*/
	ua_init(mid);
	
	/*配置硬件规则，将硬件所有报文送到模块ID为1的进程处理*/
	fast_reg_wr(FAST_ACTION_REG_ADDR|FAST_DEFAULT_RULE_ADDR,ACTION_SET_MID<<28|mid);	
	
	/*启动线程接收分派给UA进程的报文*/
	fast_ua_recv();
	
	/*主进程进入循环休眠中,数据处理主要在回调函数*/
	while(1){sleep(9999);}
	return 0;
}