/***************************************************************************
 *            flow_reorder.c
 *
 *  Wed May 08 22:10:35 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * flow_reorder.c
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
#include "flow_reorder.h"

struct tsn_sched_set tsn_flow_reorder_by_pkt_num(struct tsn_sched_set tsn_set)
{
	int i = 0, j = 0;
	struct tsn_sched_info temp = {0};
	for(i = 0; i < tsn_set.cur_num - 1; i++)
	{
		for(j = 0; j < tsn_set.cur_num - i - 1; j++)
		{
			if(tsn_set.tsn[j].pkt_num > tsn_set.tsn[j + 1].pkt_num)
			{
				temp = tsn_set.tsn[j];
				tsn_set.tsn[j] = tsn_set.tsn[j + 1];
				tsn_set.tsn[j + 1] = temp;
			}
		}
	}
	return tsn_set;
}

struct bd_sched_set bd_flow_reorder_by_bd(struct bd_sched_set bd_set)
{
	int i = 0, j = 0;
	struct bd_sched_info temp = {0};
	for(i = 0; i < bd_set.cur_num - 1; i++)
	{
		for(j = 0; j < bd_set.cur_num - i - 1; j++)
		{
			if(bd_set.bd[j].bandwidth > bd_set.bd[j + 1].bandwidth)
			{
				temp = bd_set.bd[j];
				bd_set.bd[j] = bd_set.bd[j + 1];
				bd_set.bd[j + 1] = temp;
			}
		}
	}
	return bd_set;
}