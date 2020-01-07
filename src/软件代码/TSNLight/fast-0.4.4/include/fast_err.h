/** *************************************************************************
 *  @file          fast_err.h
 *  @brief		FAST平台的错误类型值定义
 * 
 *  @date	   2017/01/05 12:08:38 星期四
 *  @author		XDL(Copyright  2017  XuDongLai)
 *  @email		<XuDongLai0923@163.com>
 *  @version	0.2.0
 ****************************************************************************/
/*
 * fast_err.h
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
#ifndef __FAST_ERR_H__
#define __FAST_ERR_H__

/** 规则索引溢出错误 */
#define E_RULE_INDEX_OVERFLOW 1
/** UA创建NetLink SOCKET时出错 */
#define E_UA_NLSK_CREATE 2
/** UA绑定NetLink SOCKET时出错 */
#define E_UA_NLSK_BIND 3
/** UA注册时出错 */
#define E_UA_NLSK_REG_ERR 4
#endif//__FAST_ERR_H__