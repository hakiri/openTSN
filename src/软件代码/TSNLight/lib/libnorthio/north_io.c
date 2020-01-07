/***************************************************************************
 *            north_io.c
 *
 *  Wed May 08 11:14:29 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * north_io.c
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
#include "north_io.h"

int client_sockfd = 0;
int client_state = FALSE;
char controller_ip[256] = {0};
char controller_port[256] = {0};

int init_north_io()
{
	struct sockaddr_in controller_addr;
	int ret = 0, len = 0;
//	u8 *pkt = (u8 *)malloc(CTRL_PKT_MAX_LEN);
	const char opt = 1;

RESTART:
	client_state = FALSE;
	client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_sockfd < 0)
	{
		TSN_ERR("Controller client start failed: create TCP socket failed!\n");
		return G_OK;
	}
	setsockopt(client_sockfd, IPPROTO_TCP, 1, &opt, sizeof(char));

	get_cfgx_file(CONTROLLER_FILE, "controller_ip", controller_ip);
	get_cfgx_file(CONTROLLER_FILE, "controller_port", controller_port);
		
	bzero(&controller_addr, sizeof(struct sockaddr_in));
	controller_addr.sin_family = AF_INET;
	inet_pton(AF_INET, controller_ip, &controller_addr.sin_addr);
	controller_addr.sin_port = htons(atoi(controller_port));
	TSN_DBG("Conncet controller[tcp: %s: %s]!\n", controller_ip, controller_port);

	ret = connect(client_sockfd, (struct sockaddr *)&controller_addr, sizeof(controller_addr));
	if(ret < 0)
	{
		TSN_ERR("Controller client start failed: connect TCP socket failed!\n");
		close(client_sockfd);
		sleep(10);
		goto RESTART;
	}
	client_state = TRUE;
	TSN_DBG("Conncet controller[tcp: %s: %s] successfully!\n", controller_ip, controller_port);
	return G_OK;
}

struct north_packet *build_north_packet(u8 type, u8 *data, u16 len)
{
	struct north_packet *pkt = NULL;
	u16 pkt_len;
	pkt_len = sizeof(struct north_packet) + len;

	pkt = (struct north_packet *)malloc(pkt_len + 1);
	memset(pkt, 0, pkt_len + 1);
	pkt->version = 0x1;
	pkt->type = type;
	pkt->len = htons(pkt_len);
	memcpy(pkt->data, data, len);
	return pkt;
}

int send_north_packet(u8 type, u8 *data, u16 len)
{
	int ret = 0;
	struct north_packet *pkt = NULL;
	if(client_state == FALSE)
	{
		TSN_ERR("Send packet to controller failed: controller is offline!\n");
		return G_ERR;
	}

	pkt = build_north_packet(type, data, len);
	ret = write(client_sockfd, (u8 *)pkt, sizeof(struct north_packet) + len);
	TSN_DBG("send packet to controller, len is: %d!\n", ret);
	if(ret < 0)
	{
		TSN_ERR("Send controller packet failed!\n");
		return G_ERR;
	}

	TSN_DBG("Send controller packet successfully!\n");
	pkt_print((u8 *)pkt, sizeof(struct north_packet) + len);
	return G_OK;
}