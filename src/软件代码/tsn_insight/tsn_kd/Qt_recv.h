
#ifndef __QT_RECV_H__
#define __QT_RECV_H__


#include "qt_info_io.h"


/*通用报文头部*/
typedef struct
{
    char version; 		/* 版本号 */
    char type; 			/* 报文类型usg_type */
	u16 length; 		/* 报文总长度 */
}pkt_header;


/*报文结构*/
typedef struct
{
	pkt_header header;      /* 通用报文头部 */
    char data[0];        /* 报文子头部和数据 */
}recv_pkt;


void *init_socket_tcp_server(void *arg);



#endif

