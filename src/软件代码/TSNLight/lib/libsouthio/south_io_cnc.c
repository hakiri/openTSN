/***************************************************************************
 *            south_io.c
 *
 *  Wed May 08 10:57:29 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * south_io.c
 *
 * Copyright (C) 2019 - michael
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
#include "south_io.h"
#include "libnet.h"

libnet_t *l = NULL;
libnet_ptag_t t;



int send_south_pkt(struct ptp_packet ptp)
{
	char err_buf[100] = "";
	if((l=libnet_init(LIBNET_LINK_ADV, NULL, err_buf))==NULL)
	{
		perror("LIBNET init failed!\n");
		exit(1);
	}

	t=libnet_build_ethernet
	(
		ptp.dst_mac,
		ptp.src_mac,
		ptp.eth_type,
		(u8 *)&ptp + 14,
		sizeof(struct ptp_packet)-sizeof(struct status_info)-14,
		l,
		0
	);

	if((libnet_write(l))==-1)
	{
		perror("Send packet failed!\n");
		libnet_destroy(l);
		
	}
	else
		printf("send success!\n");
	libnet_destroy(l);
	
	return G_OK;

}

void ptp_write_general_para(struct ptp_packet *ptp)
{
	ptp->eth_type = PTP_TYPE;		
	ptp->msg_type = MSG_UPDATE;
}

void ptp_write_dst_mac(struct ptp_packet *ptp, u8 sw_id)
{
	memcpy(ptp->dst_mac, get_sw_mac_by_sw_id(sw_id), 6);//对目的mac进行赋值，源mac无用，没有再赋特定值
}


void ptp_write_direction(struct ptp_packet *ptp, u8 direction)
{
	ptp->cfg.direction = direction;
}

void ptp_write_host_mac(struct ptp_packet *ptp, u8 *host_mac)
{
	memcpy(ptp->cfg.host_mac, host_mac, 6);
}

void ptp_write_tb_rate(struct ptp_packet *ptp, u16 rate)
{
	ptp->cfg.tb_rate   = htons(rate);
}

void ptp_write_tb_B(struct ptp_packet *ptp, u16 tb_B)
{
	ptp->cfg.tb_B   = htons(tb_B);
}

void ptp_write_time_slot(struct ptp_packet *ptp, u32 time_slot)
{
	ptp->cfg.time_slot = htonl(time_slot);
}



int config_sw_default(u8 sw_id)
{
	struct ptp_packet ptp;
	get_cfg_info(sw_id, &(ptp.cfg));//获取全局的配置信息
	memcpy(ptp.dst_mac, get_sw_mac_by_sw_id(sw_id), 6);//对目的mac进行赋值，源mac无用，没有再赋特定值
	memcpy(ptp.cfg.host_mac, get_host_mac_by_sw_id(sw_id), 6);//配置host的mac地址
	ptp.eth_type = PTP_TYPE;
	ptp.msg_type = MSG_UPDATE;
	send_south_pkt(ptp);
	return G_OK;
}

int config_all_sw_default()
{
	int sw_num = 0;
	int i = 0;
	int ret = G_OK;
	u8 *sw_mac = NULL, *host_mac = NULL;
	
	struct ptp_packet ptp;
	
	sw_num = get_sw_num();
	for(i = 0; i < sw_num; i++)
	{
		config_sw_default(i);

	}
	return G_OK;
}




