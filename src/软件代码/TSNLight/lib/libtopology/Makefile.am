## Process this file with automake to produce Makefile.in


lib_LIBRARIES =  \
	libtopology.a

libtopology_a_SOURCES =  \
	topology.h \
	topology_mgr.c \
	topology_mgr.h \
	topology_info

bin_PROGRAMS =  \
	test_topology

test_topology_SOURCES =  \
	main_topology.c

test_topology_LDADD = -L. \
	-ltopology \
	-L../libtool \
	-ltool \
	-L../libjson \
	-ljson \
	-lpthread \
	-lm 
