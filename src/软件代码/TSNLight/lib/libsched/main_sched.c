/*
 * main_sched.c
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
#include "global_sched.h"
#include "../libreserve/reserve_mgr.h"

int main()
{
	struct global_reserve_table *tbl = NULL;
	tbl = init_global_reserve_table();
	init_topology();
	init_global_resource();
	build_and_send_global_resource();
	global_flow_resource_sched(tbl);
	build_and_send_global_resource();
	return 1;
}
