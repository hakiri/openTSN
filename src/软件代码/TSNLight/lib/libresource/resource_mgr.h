/***************************************************************************
 *            resource_mgr.h
 *
 *  Wed May 08 11:46:19 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * resource_mgr.h
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
#ifndef __RESOURCE_MGR_H__
#define __RESOURCE_MGR_H__

#include "resource.h"
#include "../libtopology/topology_mgr.h"
#include "../libjson/cJSON.h"

int build_and_send_global_resource();
int init_global_resource();
u32 get_free_bd_resource(u8 sw_id);
u32 get_tb_bd_resource(u8 sw_id);
u32 get_free_cqf_resource(u8 sw_id, u8 slot);
int update_tsn_bd_resource(u8 sw_id, u32 bd_alloc);
int update_tb_bd_resource(u8 sw_id, u32 bd_alloc);
int update_cqf_resource(u8 sw_id, u32 queue_alloc, u8 slot);
int update_cfg_info(u8 sw_id, struct config_info cfg);
void get_cfg_info(u8 sw_id, struct config_info *cfg);
void print_global_resource();

#endif
