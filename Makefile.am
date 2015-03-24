
SUBDIRS = src

lib_LIBRARIES = libgbtools.a

AM_CPPFLAGS = -g -Wall $(DEPS_CFLAGS)

libgbtools_a_SOURCES = 
libgbtools_a_LIBADD  = src/gbtoolsUtils/gbtoolsUtils.o src/gbtoolsGUI/gbtoolsGUI.o
libgbtools_a_CFLAGS  = 

# Extra files to remove for the maintainer-clean target.
#
MAINTAINERCLEANFILES = $(top_srcdir)/Makefile.in

