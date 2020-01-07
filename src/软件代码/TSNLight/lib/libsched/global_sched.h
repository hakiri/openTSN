/***************************************************************************
 *            global_sched.h
 *
 *  Thu May 09 10:31:19 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * global_sched.h
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
#ifndef __GLOBAL_SCHED_H__
#define __GLOBAL_SCHED_H__

#include "path_compute.h"
#include "flow_reorder.h"
#include "flow_sched.h"
#include "flow_config.h"
#include "../libnorthio/north_io.h"

int global_flow_resource_sched(struct global_reserve_table *g_table);

#endif
