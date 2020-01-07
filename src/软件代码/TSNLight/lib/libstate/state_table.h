/***************************************************************************
 *            state_table.h
 *
 *  Thu May 09 10:36:07 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * state_table.h
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
#ifndef __STATE_TABLE_H__
#define __STATE_TABLE_H__

#include "../../include/tsn_public.h"
#include "../libsouthio/south_prtcl.h"

typedef enum
{
	OFFLINE = 0,
	ONLINE = 1
}STATE;

struct host_state_info
{
	u8 id;
	u8 state;
	u64 sync;
};

struct sw_state_info
{
	u8 id;
	u8 state;
	u16 pad;
	u64 sync;
	struct status_info status;
}__attribute__((packed));

struct state_node
{
	u32 seq;
	struct timeval create_time;
	u8 cur_host_num;
	u8 cur_sw_num;
	struct host_state_info host_state[MAX_NODE_NUM];
	struct sw_state_info sw_state[MAX_NODE_NUM];
	struct state_node *next;
};

struct state_list
{
	pthread_mutex_t mutex;
	struct state_node *head;
	struct state_node *tail;
	u32 size;
};

#endif