/** *************************************************************************
 *  @file          main_port_status.c
 *  @brief  FAST软件示例程序－硬件端口状态访问示例
 * 
 *  @date	   2017/03/05 20:55:12 星期日
 *  @author		XDL(Copyright  2017  XuDongLai)
 *  @email		<XuDongLai0923@163.com>
 *  @version	0.2.0
 ****************************************************************************/
/*
 * main_port_status.c
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
#include <net/if.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>



/**
* @brief 
*
* @param port
* @param regaddr
*
* @return 
*/
u64 PORT_REG(int port,u64 regaddr)
{		
	return fast_reg_rd(FAST_PORT_BASE|(FAST_PORT_OFT*port)|(regaddr<<3));
}

/**
* @brief 
*/
void show_head_info(void)
{
#ifdef OPENBOX_S4
	printf("PORT\t<LINK>\t<SPEED>\t<DUPLEX>\n");
#else
	printf("PORT      <MAC ADDR> <FRAME LEN> <FRAME SPACE> <BUF LEVEL>  <PCS MODE> <PCS STUTUS> <NEG STATUS> <LINK> <AUTONEG EN> <AUTONEG> <SPEED> <DUPLEX>\n");
#endif
}

void read_port_status(char ifname[6],u8 port,u8 *updown,u8 *duplex,u16 *speed)
{	
    int fd = 0,err = -1;
	struct ifreq ifr;
    struct ethtool_cmd ecmd = {0};
    struct ethtool_value edata = {0};
	
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, ifname);

	fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
	{
        perror("read_port_status->Get control socket ERR");
        return ;
    }

	edata.cmd = ETHTOOL_GLINK;
    ifr.ifr_data = (caddr_t)&edata;
    err = ioctl(fd, SIOCETHTOOL, &ifr);
	if(err == 0)
	{
		*updown = edata.data ? 1: 0;
	}	

	if(*updown)
	{
		ecmd.cmd = ETHTOOL_GSET;
		ifr.ifr_data = (caddr_t)&ecmd;
		err = ioctl(fd, SIOCETHTOOL, &ifr);
		if (err != 0)
		{
			printf("read_port_status->ioctl ERR\n");
			close(fd);
			return ;
		}
		//printf("port %s %d,%d,%d\n",ifname,port,ecmd.duplex,ecmd.speed);
		*duplex = ecmd.duplex;
		switch(ecmd.speed)
		{
			case SPEED_10:
				*speed = 10;
				break;
			case SPEED_100:
				*speed = 100;
				break;
			case SPEED_1000:
				*speed = 1000;
				break;
			case SPEED_10000:
				*speed = 10000;
				break;
			default:
				*speed = 10;
				break;
		}	
	}
	close(fd);
}

/**
* @brief 
*
* @param port
*/
void show_port_status(int port)
{
#ifdef OPENBOX_S4
	u8 updown = 0,duplex=0;
	u16 speed = 0;
	char ifname[16] = {0};
	sprintf(ifname,"obx%d",port);
	read_port_status(ifname,port,&updown,&duplex,&speed);
	printf("%d\t%s\t%d\t%s\n",
	       port,updown?"UP":"-",speed,duplex?"FULL":"HALF");
#elif OPENBOX_S28
	u64 pcs_status = PORT_REG(port,FAST_PORT_PCS_STATUS),neg_status = PORT_REG(port,FAST_PORT_NEG_STATUS);
	printf("%d    0x%08lX%04lX        %4ld            %2ld          %2ld         0x%lX         0x%02lX   0x%08lX %6s          %3s       %3s    %4d     %4s\n",
	       port,
	       PORT_REG(port,FAST_PORT_MAC_0),
	       PORT_REG(port,FAST_PORT_MAC_1),
	       PORT_REG(port,FAST_PORT_FRAME_MAX_LEN),
	       PORT_REG(port,FAST_PORT_FRAME_SPACE),
	       PORT_REG(port,FAST_PORT_BUF_LEVEL),
	       PORT_REG(port,FAST_PORT_PCS_MODE),
	       pcs_status,
	       neg_status,
	       (neg_status&FAST_PORT_NEG_STATUS_UP)==FAST_PORT_NEG_STATUS_UP?"UP":"DOWN",
	       (pcs_status&FAST_PORT_PCS_STATUS_AUTONEG_EN)>0?"YES":"NO",
	       (pcs_status&FAST_PORT_PCS_STATUS_AUTONEG_OK)>0?"YES":"NO",
	       (neg_status&FAST_PORT_NEG_STATUS_100M)==FAST_PORT_NEG_STATUS_100M?100:((neg_status&FAST_PORT_NEG_STATUS_1G)==FAST_PORT_NEG_STATUS_1G?1000:10),
	       (neg_status&FAST_PORT_NEG_STATUS_FULL)==0?"half":"full");
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
	printf("---------------------SHOW_PORT_STATUS---------------------\n");	
	show_head_info();
	if(argc == 1)/*显示所有端口的计算信息*/
	{
		for(i = 0;i< port_cnt;i++)
		{
			show_port_status(i);
		}
	}
	else if((port = atoi(argv[1])) > -1 && port < port_cnt)
	{
		show_port_status(port);
	}	
	return 0;
}
