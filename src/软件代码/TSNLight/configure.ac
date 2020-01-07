dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(tsn_controller, 0.1)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CC




dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************
IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=tsn_controller
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])

AM_GLIB_GNU_GETTEXT





LT_INIT





AC_OUTPUT([
Makefile
src/Makefile
po/Makefile.in
include/Makefile
lib/Makefile
lib/libsouthio/Makefile
lib/libnorthio/Makefile
lib/libtopology/Makefile
lib/libresource/Makefile
lib/libreserve/Makefile
lib/libsched/Makefile
lib/libstate/Makefile
lib/libtool/Makefile
lib/libjson/Makefile
])
