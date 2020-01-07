/***************************************************************************
 *            resource.h
 *
 *  Wed May 08 11:46:01 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * resource.h
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
#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "../../include/tsn_public.h"
#include "../libsouthio/south_prtcl.h"

#define SLOT_NUM 1
#define BANDWIDTH 1000
#define CQF_LEN 16
#define DEFAULT_DIREC RIGHT


struct bd_resource
{
 	u32 total_bd;
	u32 free_bd;
	u32 used_bd;
	u32 tb_bd;
};

struct cqf_resource
{
 	u32 total_len;
	u32 free_len;
	u32 used_len;
};

struct device_resource
{
	u8 sw_id;	
	struct bd_resource bd_res;
	struct cqf_resource cqf_res[SLOT_NUM];
	struct config_info cfg;
};

struct global_resource
{
	u32 cur_num;
	struct device_resource device[MAX_NODE_NUM];
};

#endif
