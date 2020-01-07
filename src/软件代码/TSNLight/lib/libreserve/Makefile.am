## Process this file with automake to produce Makefile.in


lib_LIBRARIES =  \
	libreserve.a

libreserve_a_SOURCES =  \
	flow_feature.h \
	reserve_mgr.c \
	reserve_mgr.h \
	flow_info

bin_PROGRAMS =  \
	test_reserve

test_reserve_SOURCES =  \
	main_reserve.c

test_reserve_LDADD = -L. \
	-lreserve \
	-L../libtool \
	-ltool \
	-lpthread \
	-lm 
