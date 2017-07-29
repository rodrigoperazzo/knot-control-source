AM_MAKEFLAGS = --no-print-directory

include_HEADERS =

AM_CFLAGS = $(WARNING_CFLAGS) $(BUILD_CFLAGS) @GLIB_CFLAGS@
AM_LDFLAGS = $(BUILD_LDFLAGS)

bin_PROGRAMS = src/knotctld tools/knotctl

src_knotctld_SOURCES = src/main.c
src_knotctld_LDADD = @GLIB_LIBS@
src_knotctld_LDFLAGS = $(AM_LDFLAGS)
src_knotctld_CFLAGS = $(AM_CFLAGS) @GLIB_CFLAGS@

tools_knotctl_SOURCES = tools/knotctl.c
tools_knotctl_LDADD = @GLIB_LIBS@
tools_knotctl_LDFLAGS = $(AM_LDFLAGS)
tools_knotctl_CFLAGS = $(AM_CFLAGS) @GLIB_CFLAGS@

DISTCLEANFILES =

MAINTAINERCLEANFILES = Makefile.in \
	aclocal.m4 configure config.h.in config.sub config.guess \
	ltmain.sh depcomp compile missing install-sh

clean-local:
	$(RM) -r src/knotctld tools/knotctl
