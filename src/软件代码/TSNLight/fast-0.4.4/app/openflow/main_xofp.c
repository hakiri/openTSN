/***************************************************************************
 *            main_xofp.c
 *
 *  2017/02/28 15:52:34 星期二
 *  Copyright  2017  XuDongLai
 *  <XuDongLai0923@163.com>
 ****************************************************************************/
/*
 * main_xofp.c
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
* @param argc
* @param argv[]
*
* @return 
*/
int main(int argc,char* argv[])
{
	ofp_init(argc,argv);
	while(1){sleep(9999);}
	return (0);
}
