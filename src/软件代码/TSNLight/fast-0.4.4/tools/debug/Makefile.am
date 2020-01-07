## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	debug

debug_SOURCES =  \
	main_debug.c

debug_LDADD = -L../../lib/libreg \
	-L../../lib/libua \
	-L../../lib/librule \
	-lreg \
	-lua \
	-lrule 
