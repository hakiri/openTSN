## Process this file with automake to produce Makefile.in


lib_LIBRARIES =  \
	libsched.a

libsched_a_SOURCES =  \
	path_compute.c \
	path_compute.h \
	flow_reorder.c \
	flow_reorder.h \
	flow_sched.c \
	flow_sched.h \
	flow_config.c \
	flow_config.h \
	global_sched.c \
	global_sched.h \
	sched_info.h

bin_PROGRAMS =  \
	test_sched

test_sched_SOURCES =  \
	main_sched.c

test_sched_LDADD = -L. \
	-lsched \
	-L../libreserve \
	-lreserve \
	-L../libresource \
	-lresource \
	-L../libtopology \
	-ltopology \
	-L../libtool \
	-ltool \
	-L../libjson \
	-ljson \
	-lpthread \
	-lm
