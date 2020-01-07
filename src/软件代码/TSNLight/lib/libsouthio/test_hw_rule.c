

#if 0
#include "south_prtcl.h"
#include "../libfast/fast.h"


static int test_process(struct fast_packet *pkt, int pkt_len)
{
	printf("ua recv packet!!!\n");
	
	return 0;
}




static void south_ua_init(void)
{
	int ret = 0;
	if((ret=fast_ua_init(151, test_process)))//UA模块实例化(输入参数1:接收模块ID号,输入参数2:接收报文的回调处理函数)
	{
		perror("fast_ua_init!\n");
		exit (ret);//如果初始化失败,则需要打印失败信息,并将程序结束退出!
	}
}



int main()
{
	fast_init_hw(0,0);

	south_ua_init();//初始化ua

	fast_ua_recv();
	while(1)
	{
		sleep(9999);
	}
	return 0;


}

#endif
