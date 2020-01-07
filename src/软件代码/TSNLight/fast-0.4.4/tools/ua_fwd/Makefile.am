## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	ua_fwd

ua_fwd_SOURCES =  \
	main_ua_fwd.c

ua_fwd_LDADD = -L../../lib/libreg \
	-L../../lib/libua \
	-L../../lib/librule \
	-lreg \
	-lua \
	-lrule \
	-lpthread 
