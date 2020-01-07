
ifeq( ${CNC} , "CNC" )
	CC:=gcc
	AR:=ar
else
	CC:=arm-xilinx-linux-gnueabi-gcc
	AR:=arm-xilinx-linux-gnueabi-ar
endif

all:
	echo ${CC}
