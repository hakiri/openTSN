/***************************************************************************
 *            main_mem_rw.c
 *
 *  2018/09/13 11:32:22 星期四
 *  Copyright  2018  XuDongLai
 *  <XuDongLai0923@163.com>
 ****************************************************************************/
/*
 * main_mem_rw.c
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



void Usage(char *argv)
{
	printf("Usage:\n\t%s memaddr len offset value\n",argv);	
}

int main_rw_test(int argc,char *argv[])
{
	void *base = NULL;
	int fm = 0;
	u32 REG_BASE_ADDR = 0,REG_REG_LEN = 0,REG = 0,VALUE = 0;
	
	if((fm=open("/dev/mem",O_RDWR|O_SYNC)) == -1)
	{
		printf("Open MEM Err!\n");
		Usage(argv[0]);
		exit(0);
	}
read:
	if(argc == 4 &&
		sscanf(argv[1],"%X",&REG_BASE_ADDR)==1 &&
		sscanf(argv[2],"%X",&REG_REG_LEN)==1 &&
		sscanf(argv[3],"%X",&REG)==1
		)
	{
		base = mmap(0,REG_REG_LEN,PROT_READ|PROT_WRITE,MAP_SHARED,fm,REG_BASE_ADDR);
		//base = (void *)REG_BASE_ADDR;
		printf("READ ->(%X + %X) = %X\n",REG_BASE_ADDR,REG,*(volatile u32 *)(base+REG));
	}else if(argc == 5 &&
		sscanf(argv[1],"%X",&REG_BASE_ADDR)==1 &&
		sscanf(argv[2],"%X",&REG_REG_LEN)==1 &&
		sscanf(argv[3],"%X",&REG)==1 &&
		sscanf(argv[4],"%X",&VALUE)==1
		)
	{
		base = mmap(0,REG_REG_LEN,PROT_READ|PROT_WRITE,MAP_SHARED,fm,REG_BASE_ADDR);
		//base = (void *)REG_BASE_ADDR;
		printf("WRITE->(%X + %X) = %X\n",REG_BASE_ADDR,REG,VALUE);
		*(volatile u32 *)(base+REG) = VALUE;
		argc = 4;
		goto read;
	}
	else
		Usage(argv[0]);
	munmap(base,REG_REG_LEN);
	close(fm);
	return 0;
}


int main(int argc,char *argv[])
{
	main_rw_test(argc,argv);
	return 0;
}