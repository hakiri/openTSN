/*
 * main_southio.c
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
#include "south_io.h"

int main(int argc,char *argv[])
{
	u8 tmp = 0;
	tmp = atoi(argv[1]);
	init_topology();
	init_global_resource();
	init_south_io();

	//ptp_write_tb_rate(get_sw_mac_by_sw_id(0),0x20);

	ptp_write_host_mac(get_sw_mac_by_sw_id(tmp),get_host_mac_by_sw_id(tmp));


	
	//ptp_write_direction(get_sw_mac_by_sw_id(0),0);
	//ptp_write_tb_rate(get_sw_mac_by_sw_id(tmp),0x7a12);
	//ptp_write_time_slot(get_sw_mac_by_sw_id(tmp),0);
	
	while(1)
	{
		sleep(9999);
	}
	//ptp_write_host_mac(get_sw_mac_by_sw_id(1),get_host_mac_by_sw_id(1));
	//ptp_write_host_mac(get_sw_mac_by_sw_id(2),get_host_mac_by_sw_id(2));
	
	return 1;
}

