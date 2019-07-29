#CC = arm-xilinx-linux-gnueabi-gcc
#AR = arm-xilinx-linux-gnueabi-ar

CC = gcc
AR = ar

#LDHDPATH = -I ./include 
#LDHDPATH+= -I ./fast-0.4.4/include

#LDLIBS = -L . -ltopology -L . -ltool -L . -lresource  -lsouthio -lpthread
#LDLIBS += -L ./lib/libsouthio/lib -lrule -lreg -lua
#LDLIBS = -L . -lsouthio -lresource -ltopology -ltool -ljson -L ../libfast -lrule -lreg -lua  -lpthread -lm

LDLIBS = -L . -lsouthio -lresource -ltopology -ltool    -lpthread -lm -lnet

all: clean libtool libtopology libresource libsouthio tsn_cnc 

tsn_cnc:
	$(CC) -o tsn_cnc main_southio.c $(LDLIBS) 



libsouthio:
	$(CC) -o libsouthio.o -c south_io.c $(LDLIBS)
	$(AR) -rc libsouthio.a libsouthio.o	

libresource:
	$(CC) -o libresource.o -c ../libresource/resource_mgr.c $(LDLIBS)
	$(AR) -rc libresource.a libresource.o
	
libtopology:
	$(CC) -o libtopology.o -c ../libtopology/topology_mgr.c $(LDLIBS)
	$(AR) -rc libtopology.a libtopology.o

libtool:
	$(CC) -o libtool.o -c ../libtool/tsn_tool.c $(LDLIBS)
	$(AR) -rc libtool.a libtool.o



clean:
	rm -rf *.o   tsn_cnc