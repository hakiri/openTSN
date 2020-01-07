/*
 * main_state.c
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
#include "state_mgr.h"
#include "../libtool/tsn_tool.h"

int main()
{
	pthread_t pt;
	struct ptp_packet pkt[12] = {0};
	int i = 0;

	init_topology();
	print_topology();
	init_state_list();
//	pt = tsn_create_timer(state_list_process, 1);

	pkt[0].seq = 0;
	pkt[0].src_mac[5] = 0x01;
	pkt[1].seq = 1;
	pkt[1].src_mac[5] = 0x02;
	pkt[2].seq = 2;
	pkt[2].src_mac[5] = 0x01;
	pkt[3].seq = 2;
	pkt[3].src_mac[5] = 0x02;
	pkt[4].seq = 2;
	pkt[4].src_mac[5] = 0x03;
	pkt[5].seq = 2;
	pkt[5].src_mac[5] = 0x04;
	pkt[6].seq = 2;
	pkt[6].src_mac[5] = 0x05;
	pkt[7].seq = 2;
	pkt[7].src_mac[5] = 0x06;
	pkt[8].seq = 2;
	pkt[8].src_mac[5] = 0x07;
	pkt[9].seq = 2;
	pkt[9].src_mac[5] = 0x10;
	pkt[10].seq = 2;
	pkt[10].src_mac[5] = 0x20;
	pkt[11].seq = 2;
	pkt[11].src_mac[5] = 0x08;

	for(i = 0; i < 12; i++)
	{
		ptp_packet_state_process(pkt+i, 100);
	}

//	pthread_join(pt, NULL);
	return 1;
}