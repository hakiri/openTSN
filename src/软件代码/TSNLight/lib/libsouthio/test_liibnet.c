
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "libnet.h"


libnet_t *l = NULL;
libnet_ptag_t t;



int main()
{
	char senddata[]={"hello"};
	char err_buf[100] = "";
	char src_mac[6] = {0x01,0x02,0x03,0x04,0x05,0x06};
	char dst_mac[6] = {0x07,0x08,0x09,0x0a,0x0b,0x0c};
	
	if((l=libnet_init(LIBNET_LINK_ADV, "enp0s3", err_buf))==NULL)
	{
		perror("LIBNET init failed!\n");
		exit(1);
	}
	printf("11111111\n");
	t=libnet_build_ethernet
		(
			src_mac,
			dst_mac,
			0x88f7,
			senddata,
			sizeof(senddata),
			l,
			0
		);

	if((libnet_write(l))==-1)
	{
		perror("Send packet failed!\n");
		libnet_destroy(l);
		
	}
	libnet_destroy(l);
	return 0;
	
}


