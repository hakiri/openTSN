/***************************************************************************
 *            flow_sched.h
 *
 *  Wed May 08 22:15:07 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * flow_sched.h
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
#ifndef __FLOW_SCHED_H__
#define __FLOW_SCHED_H__

#include "sched_info.h"
#include "../libtopology/topology_mgr.h"
#include "../libresource/resource_mgr.h"

struct tsn_uni_cfg tsn_uni_sched(struct tsn_sched_info tsn_info);
struct bd_uni_cfg bd_uni_sched(struct bd_sched_info bd_info);
void print_tsn_cfg(struct tsn_uni_cfg tsn_cfg);
void print_bd_cfg(struct bd_uni_cfg bd_cfg);

#endif