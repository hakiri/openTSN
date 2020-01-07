/***************************************************************************
 *            topology.h
 *
 *  Wed May 08 11:28:09 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * topology.h
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
#ifndef __TOPOLOGY_H__
#define __TOPOLOGY_H__

#include "../../include/tsn_public.h"

#define NEXT_PORT 1
#define PREV_PORT 0

typedef enum
{
	HOST = 0,
	SWITCH = 1
}DEVICE_TYPE;

typedef enum
{
	MASTER = 0,
	SLAVE = 1
}SYNC_TYPE;

struct switch_info
{
	u8 sw_id;
	u8 sync_type;
	u8 host_id;
	u8 sw_mac[MAC_LEN];
	u8 host_mac[MAC_LEN];
	u8 next_port;
	u8 prev_port;

#if TOPOLOGY_TYPE==1
	u8 port0_num;
	u8 port1_num;
	u8 port2_num;
	u8 port3_num;
	u8 port0[20][MAC_LEN];
	u8 port1[20][MAC_LEN];
	u8 port2[20][MAC_LEN];
	u8 port3[20][MAC_LEN];
#endif

	struct switch_info *next;
	struct switch_info *prev;
};

struct topology
{
	struct switch_info *head;
	struct switch_info *tail;
	u32 size;
};

#endif
