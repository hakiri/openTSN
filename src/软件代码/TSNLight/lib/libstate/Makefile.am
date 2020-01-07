## Process this file with automake to produce Makefile.in


lib_LIBRARIES =  \
	libstate.a

libstate_a_SOURCES =  \
	state_table.h \
	state_mgr.c \
	state_mgr.h

bin_PROGRAMS =  \
	test_state

test_state_SOURCES =  \
	main_state.c

test_state_LDADD = -L. \
	-lstate \
	-L../libtopology \
	-ltopology \
	-L../libtool \
	-ltool \
	-L../libjson \
	-ljson \
	-lpthread \
	-lm 
