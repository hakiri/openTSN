/***************************************************************************
 *            main_ctl_rw.c
 *
 *  2018/08/30 19:22:18 星期四
 *  Copyright  2018  XuDongLai
 *  <XuDongLai0923@163.com>
 ****************************************************************************/
/*
 * main_ctl_rw.c
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
	int outport = -1;

	printf("inport:%d,dstmid:%d,len:%d,dmac:%02X:%02X:%02X:%02X:%02X:%02X,smac:%02X:%02X:%02X:%02X:%02X:%02X\n",pkt->um.inport,pkt->um.dstmid,pkt_len,
	       pkt->data[0],pkt->data[1],pkt->data[2],pkt->data[3],pkt->data[4],pkt->data[5],
	       pkt->data[6],pkt->data[7],pkt->data[8],pkt->data[9],pkt->data[10],pkt->data[11]);

	//print_pkt(pkt,pkt_len);
	//pkt->um.outport = 1;
	fast_ua_send(pkt,pkt_len);/*正常发送报文*/
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

void Usage(char *argv)
{
	printf("Usage:\n\t%s srcmid dstmid addr [value]\n",argv);	
}

void main_ctl_test(int argc,char* argv[])
{
	u32 srcmid = 0,dstmid = 0;
	u32 addr = 0,value = 0;
	if(argc == 4 &&
	   sscanf(argv[1],"%X",&srcmid)==1 &&
	   sscanf(argv[2],"%X",&dstmid)==1 &&
	   sscanf(argv[3],"%X",&addr)==1
	   )
	{	
		/*UA模块初始化	*/
		if(srcmid<129) srcmid = FAST_UA_DFT_CTL_RW;
		ua_init(srcmid);		
		/*启动线程接收分派给UA进程的报文*/
		fast_ua_recv();		
read:
		value = fast_ua_hw_rd(dstmid,addr,0);
		printf("READ ->srcmid[%d] dstmid[%d] addr[%X] = %X\n",srcmid,dstmid,addr,value);
	}else if(argc == 5 &&
	         sscanf(argv[1],"%X",&srcmid)==1 &&
	         sscanf(argv[2],"%X",&dstmid)==1 &&
	         sscanf(argv[3],"%X",&addr)==1 &&
	         sscanf(argv[4],"%X",&value)==1
	         )
	{			
		/*UA模块初始化*/
		if(srcmid<129) srcmid = FAST_UA_DFT_CTL_RW;
		ua_init(srcmid);		
		/*启动线程接收分派给UA进程的报文*/
		fast_ua_recv();
		fast_ua_hw_wr(dstmid,addr,value,0);
		printf("WRITE->srcmid[%d] dstmid[%d] addr[%X] = %X\n",srcmid,dstmid,addr,value);
		goto read;
	}
	else
	{
		Usage(argv[0]);
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
	main_ctl_test(argc,argv);		
	return 0;
}