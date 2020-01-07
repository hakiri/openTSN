/***************************************************************************
 *            north_prtcl.h
 *
 *  Wed May 08 11:18:51 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * north_prtcl.h
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
#ifndef __NORTH_PRTCL_H__
#define __NORTH_PRTCL_H__
#include "../../include/tsn_public.h"

typedef enum
{
	TOPOLOGY_INFO = 0,
	RESOURCE_INFO = 1,
	FLOW_FEATURE = 2,
	CLOCK_SYNC = 3,
	SW_CNT_INFO = 4,
	LINK_ERR = 5
}NORTH_PKT_TYPE;

struct north_packet
{
	 u8 version;
	 u8 type;
	 u16 len;
	 u8 data[0];
}__attribute__((packed));

#endif