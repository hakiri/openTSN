/***************************************************************************
 *            topology_mgr.h
 *
 *  Wed May 08 11:28:29 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * topology_mgr.h
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
#ifndef __TOPOLOGY_MGR_H__
#define __TOPOLOGY_MGR_H__

#include "topology.h"
#include "../libtool/tsn_tool.h"
#include "../libjson/cJSON.h"
#include "../libnorthio/north_prtcl.h"
#include "../libnorthio/north_io.h"

#define TOPOLOGY_FILE "./topology_info"


int init_topology();
void print_topology();
u8 *get_sw_mac_by_sw_id(u8 sw_id);
u8 get_sw_id_by_host_id(u8 host_id);
u8 get_sw_id_by_sw_mac(u8 *sw_mac);
u8 *get_host_mac_by_host_id(u8 host_id);
u8 *get_host_mac_by_sw_id(u8 sw_id);
u8 get_sw_id_by_host_mac(u8 *host_mac);
u32 get_sw_num();
int get_device_type_id_by_mac(u8 *mac, u8 *type, u8 *id);
u8 get_master_id();

u8 get_port_num_by_sw_id_port_id(u8 sw_id,u8 port_id);
u8 *get_mac_by_sw_id_cur_num_port_id(u8 sw_id,u8 cur_num,u8 port_id);

#endif