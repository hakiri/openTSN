/***************************************************************************
 *            path_compute.h
 *
 *  Wed May 08 21:21:45 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * path_compute.h
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
#ifndef __PATH_COMPUTE_H__
#define __PATH_COMPUTE_H__

#include "sched_info.h"
#include "../libreserve/flow_feature.h"
#include "../libtopology/topology_mgr.h"


struct tsn_sched_set compute_tsn_uni_path(struct tsn_uni_table *tsn_tbl);
struct bd_sched_set compute_bd_uni_path(struct bd_uni_table *bd_tbl);
void print_tsn_set(struct tsn_sched_set tsn_set);
void print_bd_set(struct bd_sched_set bd_set);

#endif