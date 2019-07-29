## Process this file with automake to produce Makefile.in


lib_LIBRARIES =  \
	libresource.a

libresource_a_SOURCES =  \
	resource.h \
	resource_mgr.c \
	resource_mgr.h

bin_PROGRAMS =  \
	test_resource

test_resource_SOURCES =  \
	main_resource.c

test_resource_LDADD = -L. \
	-lresource \
	-L../libtopology \
	-ltopology \
	-L../libtool \
	-ltool \
	-L../libjson \
	-ljson \
	 \
	-lpthread \
	-lm 
