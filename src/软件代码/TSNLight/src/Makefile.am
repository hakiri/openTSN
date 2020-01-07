## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = tsn_controller

tsn_controller_SOURCES =  \
	main.c

tsn_controller_LDFLAGS = 

tsn_controller_LDADD =  \
	-L../lib/libreserve \
	-L../lib/libtool \
	-lreserve \
	-ltool \
	-lpthread

