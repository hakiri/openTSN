/** *************************************************************************
 *  @file          main_port_counts.c
 *  @brief  FAST软件示例程序－硬件端口计数器访问示例
 * 
 *  @date	   2017/03/05 20:55:12 星期日
 *  @author		XDL(Copyright  2017  XuDongLai)
 *  @email		<XuDongLai0923@163.com>
 *  @version	0.2.0
 ****************************************************************************/
/*
 * main_port_counts.c
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



/**
* @brief 
*
* @param port
* @param regaddr
*
* @return 
*/
u64 PORT_REG(u8 port,u64 regaddr)
{	
	u64 value = fast_reg_rd(FAST_PORT_BASE|(FAST_PORT_OFT*port)|regaddr);
	return (value>>32)|(value<<32);
}

/**
* @brief 
*/
void show_head_info(void)
{
#ifdef OPENBOX_S4
	printf("PORT   <SEND PKT> <RECV PKT> <SEND BYTE> <RECV BYTE> \n");
#elif OPENBOX_S28
	printf("PORT   <Send OK> <SNDSPKT> <SNDMPKT> <SNDBPKT> | <Recv OK> <RCVSPKT> <RCVMPKT> <RCVBPKT> | <CRC ERR> <ALIGNER> <SNDFERR> <RCVFERR>\n");
#endif
}

/**
* @brief 
*
* @param port
*/
void show_port_counts(int port)
{
#ifdef OPENBOX_S4
	printf("%d      %8llX  %8llX  %16llX  %16llX  \n",
	       port,
	       PORT_REG(port,FAST_COUNTS_SEND_PKT_L),
	       PORT_REG(port,FAST_COUNTS_RECV_PKT_L),
	       PORT_REG(port,FAST_COUNTS_SEND_BYTE_L),
	       PORT_REG(port,FAST_COUNTS_RECV_BYTE_L));
#elif OPENBOX_S28
	printf("%d      %8llX  %8llX  %8llX  %8llX  | %8llX  %8llX  %8llX  %8llX  | %8llX  %8llX  %8llX  %8llX\n",
	       port,
	       PORT_REG(port,FAST_COUNTS_SEND_PKT_L),
	       PORT_REG(port,FAST_COUNTS_SNDSPKT),
	       PORT_REG(port,FAST_COUNTS_SNDMPKT),
	       PORT_REG(port,FAST_COUNTS_SNDBPKT),
	       PORT_REG(port,FAST_COUNTS_RECV_PKT_L),
	       PORT_REG(port,FAST_COUNTS_RCVSPKT),
	       PORT_REG(port,FAST_COUNTS_RCVMPKT),
	       PORT_REG(port,FAST_COUNTS_RCVBPKT),
	       PORT_REG(port,FAST_COUNTS_CRC_ERR),
	       PORT_REG(port,FAST_COUNTS_ALIGNER),
	       PORT_REG(port,FAST_COUNTS_SNDFERR),
	       PORT_REG(port,FAST_COUNTS_RCVFERR));
#endif
}

/**
* @brief 
*
* @param argc
* @param argv[]
*
* @return 
*/
int main(int argc,char *argv[])
{
	int i = 0,port = -1;
#ifdef OPENBOX_S4
	int port_cnt = 4;
#else
	int port_cnt = 10;
#endif
	
	fast_init_hw(0,0);	
	printf("---------------------SHOW_PORT_COUNTS---------------------\n");	
	show_head_info();
	if(argc == 1)/*显示所有端口的计算信息*/
	{
		for(i = 0;i< port_cnt;i++)
		{
			show_port_counts(i);
		}
	}
	else if((port=atoi(argv[1])) > -1 && port < port_cnt)
	{
		show_port_counts(port);
	}	
	return 0;
}
