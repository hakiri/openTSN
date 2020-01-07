## Process this file with automake to produce Makefile.in


lib_LIBRARIES =  \
	libnorthio.a

libnorthio_a_SOURCES =  \
	north_io.c \
	north_io.h \
	north_prtcl.h \
	controller_info

bin_PROGRAMS =  \
	test_northio

test_northio_SOURCES =  \
	main_northio.c

test_northio_LDADD = -L. \
	-lnorthio \
	-L../libtool \
	-ltool \
	-lpthread 
