/***************************************************************************
 *            flow_reorder.h
 *
 *  Wed May 08 22:10:35 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * flow_reorder.h
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
#ifndef __FLOW_REORDER_H__
#define __FLOW_REORDER_H__

#include "sched_info.h"

struct tsn_sched_set tsn_flow_reorder_by_pkt_num(struct tsn_sched_set tsn_set);
struct bd_sched_set bd_flow_reorder_by_bd(struct bd_sched_set bd_set);

#endif