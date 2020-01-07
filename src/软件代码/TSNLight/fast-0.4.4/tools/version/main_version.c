/***************************************************************************
 *            main_version.c
 *
 *  2018/09/18 16:36:00 星期二
 *  Copyright  2018  XuDongLai
 *  <XuDongLai0923@163.com>
 ****************************************************************************/
/*
 * main_version.c
 *
 * Copyright (C) 2018 - XuDongLai
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

char value[1024] = {0};
/**
* @brief 
*
* @param argv
*/
void Usage(char *argv)
{
	printf("Usage:\n\t%s fast\n",argv);	
}

char *get_value_by_shell(char *cmd)
{	
	int len = 0;
	FILE *stream = popen(cmd,"r");

	len = fread(value,sizeof(char),sizeof(value),stream);
	pclose (stream);	
	return value;
}

int main(int argc,char *argv[])
{
	fast_init_hw (0,0);
	printf("libs:-----------------------------\n");
	printf("%s:\t\t%s\n","libreg",LIBREG_VERSION);
	printf("%s:\t\t%s\n","libofp",LIBOFP_VERSION);
	printf("%s:\t%s\n","librule",LIBRULE_VERSION);
	printf("%s:\t\t%s\n","libua",LIBUA_VERSION);
	printf("%s:\t\t%s\n","libams",LIBAMS_VERSION);
	
	printf("apps:-----------------------------\n");	
	printf("%s:\t%s\n","l2switch",L2SWITCH_VERSION);
	printf("%s:\t\t%s\n","xofp",XOF_VERSION);
	printf("%s:\t\t%s\n","ams",AMS_VERSION);
	
	printf("tools:----------------------------\n");
	printf("%s:\t\t%s\n","reg_rw",REG_RW_VERSION);		
	printf("%s:\t\t%s\n","mem_rw",MEM_RW_VERSION);
	printf("%s:\t\t%s\n","ctl_rw",CTL_RW_VERSION);
	printf("%s:\t%s\n","rule_rw",RULE_RW_VERSION);
	printf("%s:\t\t%s\n","debug",DEBUG_VERSION);
	printf("%s:\t%s\n","version",VERSION_VERSION);
	printf("%s:\t%s\n","port_status",PORT_STATUS_VERSION);
	printf("%s:\t%s\n","port_counts",PORT_COUNTS_VERSION);
	printf("%s:\t%s\n","ua_fwd",UA_FWD_VERSION);

	printf("FAST FPGA OS:\t%llX\n",fast_reg_rd (FAST_HW_REG_VERSION));
	printf("FAST FPGA UM:\t%llX\n",fast_reg_rd (FAST_HW_REG_VERSION+8));
	printf("FAST FS:\t%s",get_value_by_shell("cat /etc/issue|grep HNXS|grep 20"));
	printf("----------------------------------\n");
	return 0;
}