/***************************************************************************
 *            state_mgr.h
 *
 *  Thu May 09 10:36:21 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * state_mgr.h
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
#ifndef __STATE_MGR_H__
#define __STATE_MGR_H__

#include "state_table.h"
#include "../libtopology/topology_mgr.h"
#include "../libjson/cJSON.h"
#include "../libnorthio/north_io.h"

#define COMPENSATE_TIME DEFAULT_SLOT_SIZE * 1000
#define TIME_OUT 5
#define PHY_TIME 120
int init_state_list();
struct state_node *ptp_packet_state_process(struct ptp_packet *pkt, u64 recv_time);
void build_and_send_cnt_info(struct state_node *node);
void build_and_send_sync_info(struct state_node *node);
void delete_node_from_state_list(struct state_node *node);



//void state_list_process();

#endif
