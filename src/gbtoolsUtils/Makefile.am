
SUBDIRS = .

AM_CPPFLAGS = -g -Wall $(DEPS_CFLAGS) -I$(top_srcdir)/src/include

noinst_LTLIBRARIES = libgbtoolsUtils.la

libgbtoolsUtils_la_SOURCES = gbtoolsUtils.cpp

# Extra files to remove for the maintainer-clean target.
#
MAINTAINERCLEANFILES = $(top_builddir)/src/gbtoolsUtils/Makefile.in
