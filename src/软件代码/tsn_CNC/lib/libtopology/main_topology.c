/*
 * main_topology.c
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

#include "topology_mgr.h"

int main()
{
	u8 *mac = NULL;
	u8 id = 0;
	u8 sw_mac[MAC_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x04};
	u8 host_mac[MAC_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x40};
	u8 num = 0;
	u8 type = 0;
	
	init_topology();
	print_topology();
	
	mac = get_sw_mac_by_sw_id(4);
	printf("%x:%x:%x:%x:%x:%x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	
	id = get_sw_id_by_host_id(4);
	printf("%d\n", id);
	
	id = get_sw_id_by_sw_mac(sw_mac);
	printf("%d\n", id);
	
	mac = get_host_mac_by_host_id(4);
	printf("%x:%x:%x:%x:%x:%x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	
	mac = get_host_mac_by_sw_id(4);
	printf("%x:%x:%x:%x:%x:%x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	
	id = get_sw_id_by_host_mac(host_mac);
	printf("%d\n", id);
	
	num = get_sw_num();
	printf("num: %d\n", num);
	
	get_device_type_id_by_mac(host_mac, &type, &id);
	printf("type: %d, id:%d\n", type, id);
	
	return 1;
}