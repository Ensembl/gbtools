
SUBDIRS = .

AM_CPPFLAGS = -g -Wall $(DEPS_CFLAGS) -I$(top_srcdir)/src/include

noinst_LTLIBRARIES = libgbtoolsCurl.la

libgbtoolsCurl_la_SOURCES = \
gbtoolsCurl.c \
gbtoolsCurl.h \
gbtoolsCurl_I.h \
$(NULL) 


# Extra files to remove for the maintainer-clean target.
#
MAINTAINERCLEANFILES = $(top_srcdir)/src/gbtoolsCurl/Makefile.in

