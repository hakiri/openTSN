CC = arm-xilinx-linux-gnueabi-gcc
AR = arm-xilinx-linux-gnueabi-ar

#CC = gcc
#AR = ar

#LDHDPATH = -I ./include 
#LDHDPATH+= -I ./fast-0.4.4/include


LDLIBS =  -L ../libfast -lrule -lreg -lua -lpthread -lm
LDHDPATH = -I ./libfast/

test_rule:
	$(CC) -o test_rule test_hw_rule.c $(LDLIBS)  $(LDHDPATH)  


clean:
	rm -rf  test_rule


