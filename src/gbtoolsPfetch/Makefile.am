
include $(top_srcdir)/src/Makefile.am.common

SUBDIRS = .

noinst_LTLIBRARIES = libgbtoolsPfetch.la

libgbtoolsPfetch_la_SOURCES = \
gbtoolsPfetch.cpp \
gbtoolsPfetchPipe.cpp \
gbtoolsPfetchHttp.cpp \
$(NULL)  


# Extra files to remove for the maintainer-clean target.
#
MAINTAINERCLEANFILES = $(top_srcdir)/src/gbtoolsPfetch/Makefile.in

