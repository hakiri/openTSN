/***************************************************************************
 *            flow_config.c
 *
 *  Thu May 09 10:26:37 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * flow_config.c
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
#include "flow_config.h"

int config_tsn_uni_flow(struct tsn_uni_cfg tsn_cfg)
{
	return G_OK;
}

int config_bd_uni_flow(struct bd_uni_cfg bd_cfg)
{
	int i = 0;
	int ret = G_OK;
	unsigned char sw_id;
	struct config_info cfg;
	
	for(i = 0; i < bd_cfg.path_len; i++)
	{
		#if 1
		sw_id = get_sw_id_by_sw_mac((unsigned char*)bd_cfg.sw_mac);
		get_cfg_info(sw_id, &cfg);
		cfg.tb_rate = htonl(ceil((float)(bd_cfg.tb_rate[i]) / 10));
		ret = update_cfg_info(sw_id, cfg);
		#endif
		//ret = ptp_write_tb_rate(bd_cfg.sw_mac[i], ceil((float)(bd_cfg.tb_rate[i]) / 10));
		if(ret == G_ERR)
			return G_ERR;
	}
	return G_OK;
}

int update_tsn_uni_resource(struct tsn_sched_info tsn_info)
{
	int i = 0;
	u8 src_sw_id = tsn_info.path.src_sw_id;
	u8 cur_sw_id;
	u32 bd_used = 0;
	
	bd_used = ceil((float)(tsn_info.pkt_num * tsn_info.pkt_size * 8) / tsn_info.interval);
	TSN_DBG("tsn bd used: %dMb/s\n", bd_used);
	for(i = 0; i < tsn_info.path.path_len; i++)
	{
		cur_sw_id = (src_sw_id + i) % get_sw_num();
		update_cqf_resource(cur_sw_id, tsn_info.pkt_num, 0);
		update_tsn_bd_resource(cur_sw_id, bd_used);
	}

	return G_OK;
}

int update_bd_uni_resource(struct bd_sched_info bd_info)
{
	int i = 0;
	u8 src_sw_id = bd_info.path.src_sw_id;
	u8 cur_sw_id;
	
	for(i = 0; i < bd_info.path.path_len; i++)
	{
		cur_sw_id = (src_sw_id + i) % get_sw_num();
		update_tb_bd_resource(cur_sw_id, bd_info.bandwidth);
	}

	return G_OK;
}