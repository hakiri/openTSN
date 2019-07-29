## Process this file with automake to produce Makefile.in


lib_LIBRARIES =  \
	libsouthio.a

libsouthio_a_SOURCES =  \
	south_prtcl.h \
	south_io.c \
	south_io.h \
	debug_info

bin_PROGRAMS =  \
	test_southio

test_southio_SOURCES =  \
	main_southio.c

test_southio_LDFLAGS = -L. \
	-lsouthio \
	-L../libresource \
	-lresource \
	-L/home/michael/Documents/tsn_controller/lib/libfast \
	-lreg \
	-lrule \
	-lua \
	-lpthread 
