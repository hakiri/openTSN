## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	ctl_rw

ctl_rw_SOURCES =  \
	main_ctl_rw.c

ctl_rw_LDADD = -L../../lib/libua \
	-L../../lib/libreg \
	-lua \
	-lpthread \
	-lreg 
