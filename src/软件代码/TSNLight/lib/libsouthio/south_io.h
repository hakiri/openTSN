/***************************************************************************
 *            south_io.h
 *
 *  Wed May 08 10:57:29 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * south_io.h
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
#ifndef __SOUTH_IO_H__
#define __SOUTH_IO_H__

#include "south_prtcl.h"
#include "../libtopology/topology_mgr.h"
#include "../libresource/resource_mgr.h"
#include "../libfast/fast.h"

#define DEBUG_FILE "./debug_info"

/*
struct addr_map
{
	u8 sw_mac[MAC_LEN];
	u32 pkt_buf_addr;
};
*/
/*
int ptp_write_direction(u8 *sw_mac, u8 direction);
int ptp_write_host_mac(u8 *sw_mac, u8 *host_mac);
int ptp_write_tb_rate(u8 *sw_mac, u32 rate);
int ptp_write_time_slot(u8 *sw_mac, u32 time_slot);
*/


void init_south_rule(u8 *dmac,u8 *dmac_mask,u16 type,u8 port);

int init_south_io();

int config_all_sw_default();
int config_sw_default(u8 sw_id);
void ptp_write_time_slot(struct ptp_packet *ptp, u32 time_slot);
void ptp_write_tb_B(struct ptp_packet *ptp, u16 tb_B);
void ptp_write_tb_rate(struct ptp_packet *ptp, u16 rate);
void ptp_write_host_mac(struct ptp_packet *ptp, u8 *host_mac);
void ptp_write_direction(struct ptp_packet *ptp, u8 direction);
void ptp_write_general_para(struct ptp_packet *ptp);
int send_south_pkt(struct ptp_packet ptp);

void ptp_write_dst_mac(struct ptp_packet *ptp, u8 sw_id);







#endif
