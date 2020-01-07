## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	l2switch

l2switch_SOURCES =  \
	main_l2switch.c

l2switch_LDADD = -L../../lib/libreg \
	-L../../lib/libua \
	-L../../lib/librule \
	-lreg \
	-lua \
	-lrule \
	-lpthread 
