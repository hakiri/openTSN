/** *************************************************************************
 *  @file          main_libreg.c
 *  @brief		FAST平台的硬件寄存器读写控制库
 * 
 *  FAST平台支持带CPU功能的设备（如OpenBox），同时也支持不带CPU的设备（如NetMagic 08）
 *  但FAST软件架构的设计将对硬件的寄存器操作全封装为统一接口形式，在本库内根据不同设备
 *  的特征进行不同的硬件寄存器访问操作。\n
 *  OpenBox设备是通过PICe的寄存器形式访问，NetMagic 08则是通过构造NMAC报文进行寄存器读写
 *  @date		2017/02/15 16:36:51 星期三
 *  @author		XDL(Copyright  2017  XuDongLai)
 *  @email		<XuDongLai0923@163.com>
 *  @version	0.2.0
 ****************************************************************************/
/*
 * main_libreg.c
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

#define REG *(volatile u64 *)
#define REG32 *(volatile u32 *)

/** 
 * @brief FAST设备的硬件基地址
 * 
 *  目前针对不同平台手动设置
 * @note 通过lspci -x 命令，查看FAST设备的BAR0地址空间（0x10偏移位置的4字节地址信息）
 *  @todo 以后将会修改为自动从系统读取
 */

u64 OBX_BASE_ADDR = BASE_ADDR;

/** 
 * @brief FAST设备硬件地址的有效空间大小
 * 
 * 目前手动设置为512K
 * @todo 以后将会修改为自动从系统读取
 */
u64 OBX_REG_LEN = REG_LEN;

/** @brief 硬件设备的虚拟地址指针*/
void *obx_base = NULL;
int fm = 0;/*映射设备地址的文件句柄*/

/**
* @brief OpenBox设备的硬件资源注销函数
*/
void distroy_hw_OpenBox(void)
{
	if(obx_base != NULL)
	{
		munmap(obx_base,OBX_REG_LEN);
		close(fm);
		exit(0);
	}
}

/**
* @brief OpenBox设备的硬件资源注销，由退出中断信号调用
*
* @param argc   中断信号（捕获Ctrl + C操作）
*/
void __distroy_hw_OpenBox(int argc)
{
	printf("distroy_hw_OpenBox\n");
	distroy_hw_OpenBox();
}

#ifdef OBX_PCIE
int read_pcie_base(int vendor_id,int device_id)
{	
	char cmd[256]={0},value[1024]={0},value2[32]={0},value3[32]={0},value4='\0';
	int len = 0;
	FILE *stream = NULL;

	
	sprintf(cmd,"%s%X:%X%s","lspci -d ",vendor_id,device_id," -v|grep Memory");
	stream = popen(cmd,"r");
	len = fread(value,sizeof(char),sizeof(value),stream);
	if(len > 32)
	{		
		OBX_BASE_ADDR = 0;
		OBX_REG_LEN = 0;
		sscanf(value,"\tMemory at %lx %s %s [size=%ld%c]\n",&OBX_BASE_ADDR,value2,value3,&OBX_REG_LEN,&value4);//,value3,value4);// [size=%ldK],&OBX_REG_LEN);
		if(value4 == 'K')
			OBX_REG_LEN *=1024;
		else if(value4 == 'M')
			OBX_REG_LEN *=1024*1024;
	}
	pclose (stream);
}
#endif

/**
* @brief OpenBox设备硬件资源初始化函数
*
* @param addr   ：初始化硬件基地址
* @param len	：硬件地址的有效空间大小
*
* @return   0：表示初始化成功\n
* @warning	若不成功，则退出程序
*/
int init_hw_OpenBox(u64 addr,u64 len)
{	
	if((fm=open("/dev/mem",O_RDWR|O_SYNC)) == -1)
	{
		printf("Open MEM Err!\n");
		exit(0);
	}
	
#ifdef OBX_PCIE
	read_pcie_base(0x1172,0xE001);
#endif
	
	if(addr != 0 && len != 0)
	{
		OBX_BASE_ADDR = addr;
		OBX_REG_LEN = len;
	}
	obx_base = mmap(0,OBX_REG_LEN,PROT_READ|PROT_WRITE,MAP_SHARED,fm,OBX_BASE_ADDR);
	//printf("addr:%llX,len:%llX\n",OBX_BASE_ADDR,OBX_REG_LEN);
	FAST_DBG("REG Version:%s,OpenBox HW Version:%llX\n",LIBREG_VERSION,fast_reg_rd(FAST_HW_REG_VERSION));
	signal(SIGINT,__distroy_hw_OpenBox);//非法结束时，调用注销函数
	return 0;
}


/**
* @brief FAST设备的硬件初始化函数
*
* @param addr   ：初始化硬件基地址
* @param len	：硬件地址的有效空间大小
*
* @return   0：表示初始化成功\n
 *			其他或退出程序表示初始化失败
*/
int fast_init_hw(u64 addr,u64 len)
{	
#ifdef XDL_DEBUG
	return 0;
#else
	return init_hw_OpenBox(addr,len);
#endif
}

/**
* @brief FAST设备的硬件销毁函数
*/
void fast_distroy_hw(void)
{
#ifdef XDL_DEBUG
	return;
#else
	distroy_hw_OpenBox();
#endif
}




/**
* @brief FAST设备的硬件寄存器读操作
*
* @param regaddr	：准备读操作的寄存器地址
*
* @return   返回该寄存器地址对应的值
*/
u64 fast_reg_rd(u64 regaddr)
{
#ifdef XDL_DEBUG
	//printf("reg_rd:%lX = %X\n",regaddr,0x0923);
	return 0x0923;
#elif OPENBOX_S4
	u64 hv = REG32(obx_base + regaddr);
	u64 lv = REG32(obx_base + regaddr + 0x04);
	return (hv<<32)|lv;
#else	
	return REG(obx_base + regaddr);
	usleep(200);
#endif	
}

/**
* @brief FAST设备的硬件寄存器写操作
*
* @param regaddr	：准备写操作的寄存器地址
* @param regvalue   ：准备写操作的寄存值
*/
void fast_reg_wr(u64 regaddr,u64 regvalue)
{	
#ifdef XDL_DEBUG
	//printf("reg_wr:%lX = %lX\n",regaddr,regvalue);
	return;
#elif OPENBOX_S4
	u32 lv = regvalue & 0xFFFFFFFF;
	u32 hv = regvalue>>32;
	REG32(obx_base + regaddr) = hv;
	REG32(obx_base + regaddr + 0x04) = lv;
#else
	REG(obx_base + regaddr) = regvalue;	
	usleep(200);
#endif	
}

