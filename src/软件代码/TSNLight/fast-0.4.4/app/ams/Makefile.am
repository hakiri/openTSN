## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	ams

ams_SOURCES =  \
	main_delay_ams.c

ams_LDADD = -L../../lib/libreg \
	 \
	-L../../lib/libua \
	 \
	-L../../lib/librule \
	 \
	-L../../lib/libams \
	 \
	-lams \
	 \
	-lreg \
	 \
	-lua \
	 \
	-lrule \
	 \
	-lpthread 
