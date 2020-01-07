/*
 * main_resource.c
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
#include "resource_mgr.h"
#include "../libtopology/topology_mgr.h"

int main()
{
	u32 val = 0;
	struct config_info cfg = {0};
	cfg.direction = RIGHT;
	cfg.tb_rate = htonl(100);
	cfg.host_mac[5] = 0x90;
	
	init_topology();
	init_global_resource();
	print_global_resource();
	build_and_send_global_resource();
	
	val = get_free_bd_resource(2);
	printf("free bd: %d\n", val);

	val = get_tb_bd_resource(2);
	printf("tb bd: %d\n", val);
	
	val = get_free_cqf_resource(2, 0);
	printf("free cqf: %d\n", val);
	
	update_tsn_bd_resource(2, 100);
	val = get_free_bd_resource(2);
	printf("free bd: %d\n", val);
	
	val = get_tb_bd_resource(2);
	printf("tb bd: %d\n", val);

	
	update_tb_bd_resource(2, 100);
	val = get_free_bd_resource(2);
	printf("free bd: %d\n", val);
	
	val = get_tb_bd_resource(2);
	printf("tb bd: %d\n", val);
	
	update_cqf_resource(2, 10, 0);
	val = get_free_cqf_resource(2, 0);
	printf("free cqf: %d\n", val);
	
	update_cfg_info(2, cfg);
	memset(&cfg, 0, sizeof(struct config_info));
	get_cfg_info(2, &cfg);
	printf("%x:%x:%x:%x:%x:%x\n", cfg.host_mac[0], cfg.host_mac[1], cfg.host_mac[2], cfg.host_mac[3], cfg.host_mac[4], cfg.host_mac[5]);

	print_global_resource();
	build_and_send_global_resource();
}