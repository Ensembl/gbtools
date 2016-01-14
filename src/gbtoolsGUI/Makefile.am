
SUBDIRS = .

AM_CPPFLAGS = -g -Wall $(DEPS_CFLAGS) -I$(top_srcdir)/src/include

noinst_LTLIBRARIES = libgbtoolsGUI.la

libgbtoolsGUI_la_SOURCES = gbtoolsGUI.cpp

# Extra files to remove for the maintainer-clean target.
#
MAINTAINERCLEANFILES = $(top_builddir)/src/gbtoolsGUI/Makefile.in
