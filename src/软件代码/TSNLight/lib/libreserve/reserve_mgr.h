/***************************************************************************
 *            reserve_mgr.h
 *
 *  Wed May 08 20:34:24 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * reserve_mgr.h
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

#ifndef __RESERVE_MGR_H__
#define __RESERVE_MGR_H__

#include "flow_feature.h"
#include "../libtool/tsn_tool.h"

#define FLOW_FILE "./flow_info"

#define BUF_LEN 1500

struct global_reserve_table *init_global_reserve_table();

#endif