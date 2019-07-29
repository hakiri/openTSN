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




#define DEBUG_FILE "./debug_info"

struct addr_map
{
	u8 sw_mac[MAC_LEN];
	u32 pkt_buf_addr;
};

int ptp_write_direction(u8 *sw_mac, u8 direction);
int ptp_write_host_mac(u8 *sw_mac, u8 *host_mac);
int ptp_write_tb_rate(u8 *sw_mac, u32 rate);
int ptp_write_time_slot(u8 *sw_mac, u32 time_slot);

int init_south_io();

#endif