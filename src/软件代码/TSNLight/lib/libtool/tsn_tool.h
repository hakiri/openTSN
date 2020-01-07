/***************************************************************************
 *            tsn_tool.h
 *
 *  Thu May 09 11:13:05 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * tsn_tool.h
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
#ifndef __TSN_TOOL_H__
#define __TSN_TOOL_H__

#include "../../include/tsn_public.h"

//文本最多1024行
#define MAX_LINES 1024

//文本每行最大256字节
#define LINE_LEN  256

#define LINE(x)   file.lines[x].line
#define LINE_CNT  file.line_cnt

//读写文本相关结构体
struct one_line
{
	char line[LINE_LEN];
	int char_cnt;
};

//读写文本相关结构体
struct file_content
{
	struct one_line lines[MAX_LINES];
	int line_cnt;
};


typedef void(* timer_callback)();

struct timer_para
{
	u32 interval; 		/* 时间间隔，单位秒 */
	timer_callback func; /* 处理函数 */
};

void pkt_print(u8 *pkt,int len);
pthread_t tsn_create_timer(timer_callback func, u32 interval);
void trans_str_mac_to_hex_mac(char *str_mac, char *hex_mac);
void riprt(char *str);
int set_cfgx_file(const char *filpath, const char *nam, char *val);
int get_cfgx_file(const char *filpath, const char *nam, char *val);
u64 ntohll(u64 n);

#endif