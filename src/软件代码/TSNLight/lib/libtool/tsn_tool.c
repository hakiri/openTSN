/***************************************************************************
 *            tsn_tool.c
 *
 *  Thu May 09 11:13:05 2019
 *  Copyright  2019  michael
 *  <user@host>
 ****************************************************************************/
/*
 * tsn_tool.c
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
#include "tsn_tool.h"

struct timer_para timer_val = {0};

void pkt_print(u8 *pkt,int len)
{
	#if 0
	int i=0;

	printf("-----------------------***TSN PACKET***-----------------------\n");
	printf("Packet Addr:%p\n",pkt);
	for(i=0;i<16;i++)
	{
		if(i % 16 == 0)
			printf("      ");
		printf(" %X ",i);
		if(i % 16 == 15)
			printf("\n");
	}
	
	for(i=0;i<len;i++)
	{
		if(i % 16 == 0)
			printf("%04X: ",i);
		printf("%02X ",*((u8 *)pkt+i));
		if(i % 16 == 15)
			printf("\n");
	}
	if(len % 16 !=0)
		printf("\n");
	printf("-----------------------***TSN PACKET***-----------------------\n\n");
	#endif
}

static void tsn_start_timer(void *argv)
{
	struct itimerval tick;
	struct timer_para *timer_val = (struct timer_para *)argv;

//	pthread_detach(pthread_self());	
	TSN_DBG("Timer start!\n");
	signal(SIGALRM, timer_val->func);
	memset(&tick, 0, sizeof(tick));
	
	//Timeout to run first time
	tick.it_value.tv_sec = timer_val->interval;
	tick.it_value.tv_usec = 0;
	
	//After first, the Interval time for clock
	tick.it_interval.tv_sec = timer_val->interval;
	tick.it_interval.tv_usec = 0;
	
	if(setitimer(ITIMER_REAL, &tick, NULL) < 0)
	{
		TSN_ERR("Set timer failed!\n");
	}
	
	while(1)
	{
		pause();
	}

	TSN_DBG("Timer exit!\n");
}

pthread_t tsn_create_timer(timer_callback func, u32 interval)
{
	int ret;
	pthread_t timerid;
	timer_val.interval = interval;
	timer_val.func = func;
	
	ret = pthread_create(&timerid, NULL, (void *)tsn_start_timer, &timer_val); 
	if(0 != ret)
	{
		TSN_ERR("Create timer failed!ret=%d err=%s\n",ret, strerror(ret));
	}

	return timerid;
}


void trans_str_mac_to_hex_mac(char *str_mac, char *hex_mac)
{
	sscanf(str_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", hex_mac, hex_mac + 1, \
	       hex_mac + 2, hex_mac + 3, hex_mac + 4, hex_mac + 5);
}

void riprt(char *str)
{
	int len, i;

	if (str == NULL)
		return;
	len = strlen(str);
	if (len == 0) 
		return;

	for (i = 0; i < len; i++)
	{
		if (str[i] == '\r' || str[i] == '\n')
			str[i] = '\0';
    }
}

int set_cfgx_file(const char *filpath, const char *nam, char *val)
{
	FILE *fd = NULL;
	struct file_content file;
	char tmp[LINE_LEN];
	char *saveptr1;
	char *saveptr2;
	char *pstr = NULL;
	int i = 0;
	int ret = -1;

	fd = fopen(filpath, "r");
	if (fd == NULL)
		return ret;
    memset(&file, 0, sizeof(file));

        //read file stream to memory
    while (fgets(LINE(LINE_CNT), LINE_LEN, fd) != NULL)
		LINE_CNT++;
        
        //modify memory
    for (i = 0; i < LINE_CNT; i++)
	{
		strcpy(tmp, LINE(i));
		if (tmp[0] == '#')
            continue;
                
        if ((pstr = strtok_r(tmp, "=", &saveptr1)) != NULL)
		{
			if (!strcmp(pstr, nam))
			{
				sprintf(LINE(i), "%s=%s\n", nam, val);
				ret = 0;
                break;
            }
        }
                
        if ((pstr = strtok_r(tmp, " ", &saveptr2)) != NULL)
		{
			if (!strcmp(pstr, nam))
			{
				sprintf(LINE(i), "%s %s\n", nam, val);
				ret = 0;
                break;
            }
        }
    }
		
    fclose(fd);
    if(i == LINE_CNT)
	{
		sprintf(LINE(i), "%s=%s\n", nam, val);
		LINE_CNT++;
    }
        
    //write back to file
    fd = fopen(filpath, "w");
    if (fd == NULL)
		return 0;
                
    for (i = 0; i < LINE_CNT; i++)
		if (fputs(LINE(i), fd) == EOF)
			break;
    fflush(fd);
    fclose(fd);
        
	return ret;
}

int get_cfgx_file(const char *filpath, const char *nam, char *val)
{
	FILE *fd = NULL;
	struct file_content file;
    char *saveptr1;
    char *saveptr2;
    char *pstr = NULL;
    int i = 0;
    int ret = -1;

    if ( (NULL == filpath) || (NULL == nam) || (NULL == val))
		return ret;
        
    fd = fopen(filpath, "r");
    if (fd == NULL)
		return ret;
    memset(&file, 0, sizeof(file));

        //read file stream to memory
    while (fgets(LINE(LINE_CNT), LINE_LEN, fd) != NULL)
		LINE_CNT++;

        //compare and put out the value
    for (i = 0; i < LINE_CNT; i++)
	{
        if (LINE(i)[0] == '#')
            continue;
                
        if ((pstr = strtok_r(LINE(i), "=", &saveptr1)) != NULL)
		{
			if (!strcmp(pstr, nam))
			{
				pstr = strtok_r(NULL, "=", &saveptr1);
                strcpy(val, pstr);
				riprt(val);
                ret = 0;
                break;
            }
        }
                
		if ((pstr = strtok_r(LINE(i), " ", &saveptr2)) != NULL)
		{
            if (!strcmp(pstr, nam))
			{
				pstr = strtok_r(NULL, " ", &saveptr2);
                strcpy(val, pstr);
                riprt(val);
                ret = 0;
                break;
            }
        }
    }
    
	fclose(fd);
    return ret;
}

u64 ntohll(u64 n)
{
	return htonl(1) == 1 ? n : (((u64)ntohl(n)) << 32) | ntohl(n >> 32);
}