/***************************************************************************
 *            main_debug.c
 *
 *  2017/03/06 22:20:27 星期一
 *  Copyright  2017  XuDongLai
 *  <XuDongLai0923@163.com>
 ****************************************************************************/
/*
 * main_debug.c
 *
 * Copyright (C) 2017 - XuDongLai
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
#include "../../include/fast.h"


/**
* @brief 
*
* @param argv
*/
void Usage(char *argv)
{
	printf("Usage:\n\t%s um\n",argv);	
}

int main(int argc,char *argv[])
{
	fast_init_hw(0,0);
	if(argc != 2)
	{
		Usage(argv[0]);
		exit(0);
	}
	printf("---------------FAST UM DEBUG---------------\n");
	if(!strncmp("dac",argv[1],3)||!strncmp("um",argv[1],2))
	{
		printf("DAC:\n");
		printf("data_cache_status:\t(%08X)=%08llX\n",FAST_DEBUG_DC_DATA_CACHE_STATUS,fast_reg_rd(FAST_DEBUG_DC_DATA_CACHE_STATUS));
		printf("in_data_cache_count:\t(%08X)=%08llX\n",FAST_DEBUG_DC_IN_DATA_CACHE_COUNT,fast_reg_rd(FAST_DEBUG_DC_IN_DATA_CACHE_COUNT));
		printf("out_data_cache_count:\t(%08X)=%08llX\n",FAST_DEBUG_DC_OUT_DATA_CACHE_COUNT,fast_reg_rd(FAST_DEBUG_DC_OUT_DATA_CACHE_COUNT));
	}
	if(!strncmp("dac",argv[1],3)||!strncmp("um",argv[1],2))
	{
		printf("GPP:\n");
		printf("gpp_status:\t\t(%08X)=%08llX\n",FAST_DEBUG_GPP_GPP_STATUS,fast_reg_rd(FAST_DEBUG_GPP_GPP_STATUS));
		printf("in_gpp_data_count:\t(%08X)=%08llX\n",FAST_DEBUG_GPP_IN_GPP_DATA_COUNT,fast_reg_rd(FAST_DEBUG_GPP_IN_GPP_DATA_COUNT));
		printf("out_gpp_md_count:\t(%08X)=%08llX\n",FAST_DEBUG_GPP_OUT_GPP_MD_COUNT,fast_reg_rd(FAST_DEBUG_GPP_OUT_GPP_MD_COUNT));
		printf("out_gpp_phv_count:\t(%08X)=%08llX\n",FAST_DEBUG_GPP_OUT_GPP_PHY_COUNT,fast_reg_rd(FAST_DEBUG_GPP_OUT_GPP_PHY_COUNT));
		printf("out_gpp_cache_count:\t(%08X)=%08llX\n",FAST_DEBUG_GPP_OUT_GPP_CACHE_COUNT,fast_reg_rd(FAST_DEBUG_GPP_OUT_GPP_CACHE_COUNT));
	}
	if(!strncmp("dac",argv[1],3)||!strncmp("um",argv[1],2))
	{
		printf("GKE:\n");
		printf("gke_status:\t\t(%08X)=%08llX\n",FAST_DEBUG_GKE_GKE_STATUS,fast_reg_rd(FAST_DEBUG_GKE_GKE_STATUS));
		printf("in_gke_md_count:\t(%08X)=%08llX\n",FAST_DEBUG_GKE_IN_GKE_MD_COUNT,fast_reg_rd(FAST_DEBUG_GKE_IN_GKE_MD_COUNT));
		printf("in_gke_phv_count:\t(%08X)=%08llX\n",FAST_DEBUG_GKE_IN_GKE_PHY_COUNT,fast_reg_rd(FAST_DEBUG_GKE_IN_GKE_PHY_COUNT));
		printf("out_gke_md_count:\t(%08X)=%08llX\n",FAST_DEBUG_GKE_OUT_GKE_MD_COUNT,fast_reg_rd(FAST_DEBUG_GKE_OUT_GKE_MD_COUNT));
		printf("out_gke_phv_count:\t(%08X)=%08llX\n",FAST_DEBUG_GKE_OUT_GKE_PHY_COUNT,fast_reg_rd(FAST_DEBUG_GKE_OUT_GKE_PHY_COUNT));
		printf("out_gke_key_count:\t(%08X)=%08llX\n",FAST_DEBUG_GKE_OUT_GKE_KEY_COUNT,fast_reg_rd(FAST_DEBUG_GKE_OUT_GKE_KEY_COUNT));
	}
	if(!strncmp("dac",argv[1],3)||!strncmp("um",argv[1],2))
	{
		printf("GME:\n");
		printf("gme_status:\t\t(%08X)=%08llX\n",FAST_DEBUG_GME_GME_STATUS,fast_reg_rd(FAST_DEBUG_GME_GME_STATUS));
		printf("in_gme_md_count:\t(%08X)=%08llX\n",FAST_DEBUG_GME_IN_GME_MD_COUNT,fast_reg_rd(FAST_DEBUG_GME_IN_GME_MD_COUNT));
		printf("in_gme_phv_count:\t(%08X)=%08llX\n",FAST_DEBUG_GME_IN_GME_PHY_COUNT,fast_reg_rd(FAST_DEBUG_GME_IN_GME_PHY_COUNT));
		printf("in_gme_key_count:\t(%08X)=%08llX\n",FAST_DEBUG_GME_IN_GME_KEY_COUNT,fast_reg_rd(FAST_DEBUG_GME_IN_GME_KEY_COUNT));
		printf("in_gme_index_count:\t(%08X)=%08llX\n",FAST_DEBUG_GME_IN_GME_INDEX_COUNT,fast_reg_rd(FAST_DEBUG_GME_IN_GME_INDEX_COUNT));
		printf("out_gme_md_count:\t(%08X)=%08llX\n",FAST_DEBUG_GME_OUT_GME_MD_COUNT,fast_reg_rd(FAST_DEBUG_GME_OUT_GME_MD_COUNT));
		printf("out_gme_phv_count:\t(%08X)=%08llX\n",FAST_DEBUG_GME_OUT_GME_PHY_COUNT,fast_reg_rd(FAST_DEBUG_GME_OUT_GME_PHY_COUNT));
		printf("out_gme_key_count:\t(%08X)=%08llX\n",FAST_DEBUG_GME_OUT_GME_KEY_COUNT,fast_reg_rd(FAST_DEBUG_GME_OUT_GME_KEY_COUNT));
	}
	if(!strncmp("dac",argv[1],3)||!strncmp("um",argv[1],2))
	{
		printf("GAC:\n");
		printf("gac_status:\t\t(%08X)=%08llX\n",FAST_DEBUG_GAC_GAC_STATUS,fast_reg_rd(FAST_DEBUG_GAC_GAC_STATUS));
		printf("in_gac_data_count:\t(%08X)=%08llX\n",FAST_DEBUG_GAC_IN_GAC_DATA_COUNT,fast_reg_rd(FAST_DEBUG_GAC_IN_GAC_DATA_COUNT));
		printf("in_gac_md_count:\t(%08X)=%08llX\n",FAST_DEBUG_GAC_IN_GAC_MD_COUNT,fast_reg_rd(FAST_DEBUG_GAC_IN_GAC_MD_COUNT));
		printf("in_gac_phv_count:\t(%08X)=%08llX\n",FAST_DEBUG_GAC_IN_GAC_PHY_COUNT,fast_reg_rd(FAST_DEBUG_GAC_IN_GAC_PHY_COUNT));
		printf("out_gac_data_count:\t(%08X)=%08llX\n",FAST_DEBUG_GAC_OUT_GAC_DATA_COUNT,fast_reg_rd(FAST_DEBUG_GAC_OUT_GAC_DATA_COUNT));
		printf("out_gac_phv_count:\t(%08X)=%08llX\n",FAST_DEBUG_GAC_OUT_GAC_PHY_COUNT,fast_reg_rd(FAST_DEBUG_GAC_OUT_GAC_PHY_COUNT));
	}
	if(!strncmp("dac",argv[1],3)||!strncmp("um",argv[1],2))
	{
		printf("GOE:\n");
		printf("goe_status:\t\t(%08X)=%08llX\n",FAST_DEBUG_GOE_GOE_STATUS,fast_reg_rd(FAST_DEBUG_GOE_GOE_STATUS));
		printf("in_goe_data_count:\t(%08X)=%08llX\n",FAST_DEBUG_GOE_IN_GOE_DATA_COUNT,fast_reg_rd(FAST_DEBUG_GOE_IN_GOE_DATA_COUNT));
		printf("in_goe_phv_count:\t(%08X)=%08llX\n",FAST_DEBUG_GOE_IN_GOE_PHY_COUNT,fast_reg_rd(FAST_DEBUG_GOE_IN_GOE_PHY_COUNT));
		printf("out_goe_data_count:\t(%08X)=%08llX\n",FAST_DEBUG_GOE_OUT_GOE_DATA_COUNT,fast_reg_rd(FAST_DEBUG_GOE_OUT_GOE_DATA_COUNT));
	}
	printf("---------------FAST UM DEBUG---------------\n");
}
