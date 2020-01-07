/** *************************************************************************
 *  @file          main_rule.c
 *  @brief  FAST软件示例程序－硬件流表规则打印示例
 * 
 *  @date	   2017/03/05 22:13:29 星期日
 *  @author		Copyright  2017  XuDongLai
 *  @email		<XuDongLai0923@163.com>
 *  @version	0.2.0
 ****************************************************************************/
/*
 * main_rule.c
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
void Usage(int argc,char *argv[])
{
	printf("Usage:\n\t%s hw|test|clear|{p1 p2}\n",argv[0]);
	exit(0);
}

void main_rule_test(int argc,char *argv[])
{
	int i = 0,p1 = 1,p2 = 2,idx1 = 0,idx2 = 1;
	struct fast_rule rule[2] = {{0},{0}};//初始化两条全空的规则

	if(argc == 3)
	{
		p1 = atoi(argv[1]);
		p2 = atoi(argv[2]); 
	}
	else if(argc == 4)
	{
		p1 = atoi(argv[1]);
		p2 = atoi(argv[2]); 
		idx1 = atoi(argv[3]);
	}
	else if(argc == 5)
	{
		p1 = atoi(argv[1]);
		p2 = atoi(argv[2]); 
		idx1 = atoi(argv[3]);
		idx2 = atoi(argv[4]);
	}
	
	//给规则的各字段赋值
	*(u64 *)rule[i].key.dmac = htole64(0x14187754ec9c);//0x000123456789,MAC=00:01:23:45:67:89
	*(u64 *)rule[i].key.smac = htole64(0x0021ccbf9dff);//0x00ABCDEF1234,MAC=00:AB:CD:EF:12:34
	rule[i].key.tci = htole16(0xBB00);
	rule[i].key.type = htole16(0x0800);
	rule[i].key.tos = 0x0C;
	rule[i].key.proto = 0x11;//0x1:ICMP,0x6:TCP,0x11:UDP
	rule[i].key.ipv4.src = htole32(0xC0A80104);//IP=192.168.1.4
	rule[i].key.ipv4.dst = htole32(0xC0A80105);//IP=192.168.1.5
	rule[i].key.ipv4.tp.sport = htole16(0x1234);
	rule[i].key.ipv4.tp.dport = htole16(0x5678);
	rule[i].key.port = p1;
	rule[i].priority =0xA;
	rule[i].action = ACTION_PORT<<28|p2;//动作字段的涵义请参考fast_type.h
	rule[i].md5[0] = i+1;
	
	//给规则对应字段设置掩码，掩码为1表示使用，为0表示忽略
#if 0
	*(u64 *)rule[i].mask.dmac = 0xFFFFFFFFFFFFL;
	*(u64 *)rule[i].mask.smac = 0xFFFFFFFFFFFFL;

	rule[i].mask.tci = 0xFFFF;
	rule[i].mask.type = 0xFFFF;
	rule[i].mask.tos = 0xFF;
	rule[i].mask.proto = 0xFF;//0x1:ICMP,0x6:TCP,0x11:UDP
	rule[i].mask.ipv4.src = 0xFFFFFFFF;
	rule[i].mask.ipv4.dst = 0xFFFFFFFF;
	rule[i].mask.ipv4.tp.sport = 0xFFFF;
	rule[i].mask.ipv4.tp.dport = 0xFFFF;
#endif
	rule[i].mask.port = 0x3F;/*6bit*/

	//fast_add_rule(&rule[i]);
	//fast_modify_rule(&rule[i],0);

	i++;
	*(u64 *)rule[i].key.dmac = htole64(0x0021ccbf9dff);//MAC=00:AB:CD:EF:12:34
	*(u64 *)rule[i].key.smac =htole64(0x14187754ec9c);//MAC=00:01:23:45:67:89
	rule[i].key.tci = htole16(0xCC00);
	rule[i].key.type = htole16(0x0800);
	rule[i].key.tos = 0x0B;
	rule[i].key.proto = 0x11;//0x1:ICMP,0x6:TCP,0x11:UDP
	rule[i].key.ipv4.src = htole32(0xC0A80105);//IP=192.168.1.5
	rule[i].key.ipv4.dst = htole32(0xC0A80104);//IP=192.168.1.4
	rule[i].key.ipv4.tp.sport = htole16(0x5678);
	rule[i].key.ipv4.tp.dport = htole16(0x1234);
	rule[i].key.port = p2;
	rule[i].priority =0xB;
	rule[i].action = ACTION_PORT<<28|p1;//动作字段的涵义请参考fast_type.h
	rule[i].md5[0] = i+1;
	
	//给规则对应字段设置掩码，掩码为1表示使用，为0表示忽略
#if 0
	*(u64 *)rule[i].mask.dmac = 0xFFFFFFFFFFFFL;
	*(u64 *)rule[i].mask.smac = 0xFFFFFFFFFFFFL;

	rule[i].mask.tci = 0xFFFF;
	rule[i].mask.type = 0xFFFF;
	rule[i].mask.tos = 0xFF;
	rule[i].mask.proto = 0xFF;//0x1:ICMP,0x6:TCP,0x11:UDP
	rule[i].mask.ipv4.src = 0xFFFFFFFF;
	rule[i].mask.ipv4.dst = 0xFFFFFFFF;
	rule[i].mask.ipv4.tp.sport = 0xFFFF;
	rule[i].mask.ipv4.tp.dport = 0xFFFF;
#endif
	rule[i].mask.port = 0x3F;
	
	//fast_add_rule(&rule[i]);
	//fast_modify_rule(&rule[i],1);


	if(argc == 3 || argc == 1)
	{
		fast_modify_rule(&rule[i-1],idx1);
		fast_modify_rule(&rule[i],idx2);
		printf("Row[%d],Port[%d]----->Port[%d]\n",idx1,p1,p2);
		printf("Row[%d],Port[%d]----->Port[%d]\n",idx2,p2,p1);
	}
	else if(argc == 4)
	{
		p1 = atoi(argv[1]);
		p2 = atoi(argv[2]); 
		idx1 = atoi(argv[3]);
		fast_modify_rule(&rule[i-1],idx1);
		printf("Row[%d],Port[%d]----->Port[%d]\n",idx1,p1,p2);
	}
	else if(argc == 5)
	{
		p1 = atoi(argv[1]);
		p2 = atoi(argv[2]); 
		idx1 = atoi(argv[3]);
		idx2 = atoi(argv[4]);
		fast_modify_rule(&rule[i-1],idx1);
		fast_modify_rule(&rule[i],idx2);
		printf("Row[%d],Port[%d]----->Port[%d]\n",idx1,p1,p2);
		printf("Row[%d],Port[%d]----->Port[%d]\n",idx2,p2,p1);
	}	
	//print_sw_rule();	
	//print_hw_rule();	
	//read_hw_rule (&rule[i],0);//将第0条规则读出
}

void hw_fwd_testbench(void)
{
	/*S4端口硬件转发测试*/
	int i = 0;
	struct fast_rule rule[4] = {{0},{0}};//初始化两条全空的规则

	rule[i].key.port = 0;
	rule[i].mask.port = 0x3F;
	rule[i].md5[0] = i+1;
	rule[i].action = ACTION_PORT<<28|1;
	fast_add_rule(&rule[i]);

	i++;
	rule[i].key.port = 2;
	rule[i].mask.port = 0x3F;
	rule[i].md5[0] = i+1;
	rule[i].action = ACTION_PORT<<28|3;
	fast_add_rule(&rule[i]);

	i++;
	rule[i].key.port = 3;
	rule[i].mask.port = 0x3F;
	rule[i].md5[0] = i+1;
	rule[i].action = ACTION_PORT<<28|2;
	fast_add_rule(&rule[i]);

	i++;
	rule[i].key.port = 1;
	rule[i].mask.port = 0x3F;
	rule[i].md5[0] = i+1;
	rule[i].action = ACTION_PORT<<28|0;
	fast_add_rule(&rule[i]);

	printf("Row[%d],Port[%d]----->Port[%d]\n",0,0,1);
	printf("Row[%d],Port[%d]----->Port[%d]\n",1,2,3);
	printf("Row[%d],Port[%d]----->Port[%d]\n",2,3,2);
	printf("Row[%d],Port[%d]----->Port[%d]\n",3,1,0);
}

void rule_hw_clear(void)
{
	init_rule(fast_reg_rd(FAST_ACTION_REG_ADDR|FAST_DEFAULT_RULE_ADDR));
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
	
	fast_init_hw(0,0);

	if(argc == 1)
	{
		Usage(argc,argv);		
	}
	
	if(argc == 2 && !strncmp("hw",argv[1],2))/*打印输出硬件规则内容*/
	{
		printf("---------------------SHOW_HW_RULE---------------------\n");
		print_hw_rule();
	}
	else if(argc == 2 && !strncmp("test",argv[1],4))/*使用硬件流表进行测试*/
	{
		printf("---------------------HW RULE FWD TEST---------------------\n");
		hw_fwd_testbench();
	}
	else if(argc == 2 && !strncmp("clear",argv[1],5))/*清除硬件流表*/
	{
		printf("---------------------HW RULE FWD CLEAR---------------------\n");
		rule_hw_clear();
	}
	else/*默认配置软件规则*/
	{
		main_rule_test(argc,argv);//此处无法实现,因为软件规则只存储在用户的进程中,我们无法读到(除非缓存进内核)
	}
	return 0;
}
