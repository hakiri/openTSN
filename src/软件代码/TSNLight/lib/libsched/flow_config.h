/***************************************************************************
 *            flow_config.h
 *
 *  Thu May 09 10:26:37 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * flow_config.h
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
#ifndef __FLOW_CONFIG_H__
#define __FLOW_CONFIG_H__

#include "sched_info.h"
#include "../libsouthio/south_io.h"
#include "../libresource/resource_mgr.h"

int config_tsn_uni_flow(struct tsn_uni_cfg tsn_cfg);
int config_bd_uni_flow(struct bd_uni_cfg bd_cfg);

int update_tsn_uni_resource(struct tsn_sched_info tsn_info);
int update_bd_uni_resource(struct bd_sched_info bd_info);

#endif
