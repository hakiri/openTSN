/** *************************************************************************
 *  @file          fast_type.h
 *  @brief		FAST平台的数据类型定义
 * 
 *  FAST平台主要为网络数据处理平台，为方便用户直观了解数据大小，用了一种非常直观的
 *  方式来定义数据，如u8,u16,u32,u64，分别表示无符号的8位（1字节），16位（2字节），
 *  32位（4字节），64位（8字节）；s8,s16,s32,s64分别表示对应有符号数。
 * 
 *  @date	   2017/01/05 12:04:20 星期四
 *  @author		XDL(Copyright  2017  XuDongLai) 
 *  @email		<XuDongLai0923@163.com>
 *  @version	0.2.0
 *  
 ****************************************************************************/
/*
 * fast_type.h
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
#ifndef __FAST_TYPE_H__
#define __FAST_TYPE_H__

typedef char s8;				/**< 有符号的8位（1字节）数据定义*/
typedef unsigned char u8;		/**< 无符号的8位（1字节）数据定义*/
typedef short s16;				/**< 有符号的16位（2字节）数据定义*/
typedef unsigned short u16;	/**< 无符号的16位（2字节）数据定义*/
typedef int s32;				/**< 有符号的32位（4字节）数据定义*/
typedef unsigned int u32;		/**< 无符号的32位（4字节）数据定义*/
typedef long long s64;				/**< 有符号的64位（8字节）数据定义*/
typedef unsigned long long u64;		/**< 无符号的64位（8字节）数据定义*/

#endif //__FAST_TYPE_H__
