## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	xofp

xofp_SOURCES =  \
	main_xofp.c

xofp_LDADD = -L../../lib/libreg \
	-L../../lib/libua \
	-L../../lib/librule \
	-L../../lib/libofp \
	-lofp \
	-lrule \
	-lua \
	-lreg \
	-lpthread \
	-lpcap \
	-lnet 
