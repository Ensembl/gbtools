
SUBDIRS = .

noinst_LIBRARIES = libgbtoolsUtils.a

AM_CPPFLAGS = -g -Wall $(DEPS_CFLAGS) -I$(top_srcdir)/src/include

libgbtoolsUtils_a_SOURCES = gbtoolsUtils.cpp
libgbtoolsUtils_a_LIBADD  = 
libgbtoolsUtils_a_CFLAGS  = 

# Extra files to remove for the maintainer-clean target.
#
MAINTAINERCLEANFILES = $(top_builddir)/src/gbtoolsUtils/Makefile.in
