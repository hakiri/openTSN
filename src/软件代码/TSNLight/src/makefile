#!/bin/bash
#
#make TOPOLOGY_TYPE=0 APP_TYPE=NA 表示对环形拓扑TSN_NA进行编译
#make TOPOLOGY_TYPE=1 APP_TYPE=NA 表示对星型拓扑TSN_NA进行编译

#make TOPOLOGY_TYPE=0 APP_TYPE=CNC 表示对环形拓扑TSN_CNC进行编译
#make TOPOLOGY_TYPE=1 APP_TYPE=NA  表示对星形拓扑TSN_NA进行编译


ifeq (CNC,$(APP_TYPE))
	CC:=gcc
	AR:=ar
else ifeq (NA,$(APP_TYPE))
	CC:=arm-xilinx-linux-gnueabi-gcc
	AR:=arm-xilinx-linux-gnueabi-ar
endif


#LDLIBS = -L . -ltopology -L . -ltool -L . -lresource  -lsouthio -lpthread
#LDLIBS += -L ./lib/libsouthio/lib -lrule -lreg -lua
LDLIBS = -L . -lsouthio -lstate -lsched -lresource -ltopology -lreserve -lnorthio -ltool -ljson 

ifeq (CNC,$(APP_TYPE))
	LDLIBS +=  -lpthread -lm -lnet -DTOPOLOGY_TYPE=$(TOPOLOGY_TYPE) -DTSN_APP_CNC=1 
else ifeq (NA,$(APP_TYPE))
	LDLIBS += -L ../lib/libfast -lrule -lreg -lua -lpthread -lm -DTOPOLOGY_TYPE=$(TOPOLOGY_TYPE) -DTSN_APP_NA=1 
endif

all: clean libjson libtool libnorthio libreserve libtopology libresource libsched libstate libsouthio tsn_controller

tsn_controller:
	$(CC) -o tsn_controller main.c $(LDLIBS) 

libsouthio:
ifeq (CNC,$(APP_TYPE))
	$(CC) -o libsouthio.o -c ../lib/libsouthio/south_io_CNC.c  $(LDLIBS)
	$(AR) -rc libsouthio.a libsouthio.o	
else ifeq (NA,$(APP_TYPE))
	$(CC) -o libsouthio.o -c ../lib/libsouthio/south_io.c  $(LDLIBS)
	$(AR) -rc libsouthio.a libsouthio.o	
endif
libstate:
	$(CC) -o libstate.o -c ../lib/libstate/state_mgr.c  $(LDLIBS)
	$(AR) -rc libstate.a libstate.o

libsched:
	$(CC) -o path_compute.o -c ../lib/libsched/path_compute.c $(LDLIBS)
	$(CC) -o flow_reorder.o -c ../lib/libsched/flow_reorder.c $(LDLIBS)
	$(CC) -o flow_sched.o -c ../lib/libsched/flow_sched.c $(LDLIBS)
	$(CC) -o flow_config.o -c ../lib/libsched/flow_config.c $(LDLIBS)
	$(CC) -o global_sched.o -c ../lib/libsched/global_sched.c $(LDLIBS)
	$(AR) -rc libsched.a path_compute.o flow_reorder.o flow_sched.o flow_config.o global_sched.o

libresource:
	$(CC) -o libresource.o -c ../lib/libresource/resource_mgr.c  $(LDLIBS)
	$(AR) -rc libresource.a libresource.o
	
libtopology:
	$(CC) -o libtopology.o -c ../lib/libtopology/topology_mgr.c $(LDLIBS)
	$(AR) -rc libtopology.a libtopology.o

libreserve:
	$(CC) -o libreserve.o -c ../lib/libreserve/reserve_mgr.c $(LDLIBS)
	$(AR) -rc libreserve.a libreserve.o

libnorthio:
	$(CC) -o libnorthio.o -c ../lib/libnorthio/north_io.c $(LDLIBS)
	$(AR) -rc libnorthio.a libnorthio.o

libtool:
	$(CC) -o libtool.o -c ../lib/libtool/tsn_tool.c $(LDLIBS)
	$(AR) -rc libtool.a libtool.o

libjson:
	$(CC) -o libjson.o -c ../lib/libjson/cJSON.c $(LDLIBS)
	$(AR) -rc libjson.a libjson.o

clean:
	rm -rf *.o *.a  tsn_controller
