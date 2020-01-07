/** *************************************************************************
 *  @file          main_reg_rw.c
 *  @brief  FAST软件示例程序－硬件寄存器读写访问示例
 *  @date		2017/03/06 21:25:42 星期一
 *  @author		Copyright  2017  XuDongLai
 *  @email		<XuDongLai0923@163.com>
 *  @version	0.2.0
 ****************************************************************************/
/*
 * main_reg_rw.c
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
	printf("Usage:\n\t%s {rd|wr} regaddr [regvalue]\n",argv);
	exit(0);
}

/**
* @brief 
*
* @param argc
* @param argv[]
*
* @return 
*/
int main(int argc,char *argv[])
{
	u64 regaddr = 0,regvalue = 0;
	if(argc != 3 && argc != 4)Usage(argv[0]);
	
	if(fast_init_hw(0,0) != 0)
	{
		printf("Init FAST HW ERR!\n");
		exit(0);
	}

	if(argc == 3 && !strncmp("rd",argv[1],2))/*读寄存器*/
	{
		if(sscanf(argv[2],"%llX",&regaddr)==1)
		{
#ifdef OPENBOX_S4
			printf("REG READ ->Addr:0x%llX = 0x%llX\n",regaddr,fast_reg_rd (regaddr));
#else
			printf("REG READ ->Addr:0x%lX = 0x%lX\n",regaddr,fast_reg_rd (regaddr));
#endif
		}		
	}
	else if(argc == 4 && !strncmp("wr",argv[1],2))/*写寄存器*/
	{
		if(sscanf(argv[2],"%llX",&regaddr)==1 && sscanf(argv[3],"%llX",&regvalue)==1)
		{
			u64 regvalue_back = 0;
			fast_reg_wr (regaddr,regvalue);
			regvalue_back = fast_reg_rd (regaddr);
#ifdef OPENBOX_S4
			printf("REG WRITE->Addr:0x%llX = 0x%llX%s\n",regaddr,regvalue_back,regvalue==regvalue_back?"(OK)":"(ER)");
#else
			printf("REG WRITE->Addr:0x%lX = 0x%lX%s\n",regaddr,regvalue_back,regvalue==regvalue_back?"(OK)":"(ER)");
#endif
		}
	}	
	else
	{
		Usage(argv[0]);
	}
	return 0;
}
