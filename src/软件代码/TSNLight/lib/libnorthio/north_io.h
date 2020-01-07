/***************************************************************************
 *            north_io.h
 *
 *  Wed May 08 11:14:29 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * north_io.h
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
#ifndef __NORTH_IO_H__
#define __NORTH_IO_H__
#include "north_prtcl.h"
#include "../libtool/tsn_tool.h"

#define CTRL_PKT_MAX_LEN 65535
#define CONTROLLER_FILE "./controller_info"

int init_north_io();
int send_north_packet(u8 type, u8 *data, u16 len);

#endif