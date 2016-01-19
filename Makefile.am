
SUBDIRS = src

AM_CPPFLAGS = -g -Wall $(DEPS_CFLAGS)

ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libgbtools.la

libgbtools_la_SOURCES = 


# Temp....until I convert curl and pfetch to C++
#libgbtools_la_LIBADD  = \
#src/gbtoolsCurl/libgbtoolsCurl.la \
#src/gbtoolsGUI/libgbtoolsGUI.la \
#src/gbtoolsPfetch/libgbtoolsPfetch.la \
#src/gbtoolsUtils/libgbtoolsUtils.la

libgbtools_la_LIBADD  = \
src/gbtoolsGUI/libgbtoolsGUI.la \
src/gbtoolsUtils/libgbtoolsUtils.la




# Extra files to remove for the maintainer-clean target.
# Note you cannot use this target to remove directories,
# hence the extra "local" target.
#
MAINTAINERCLEANFILES = \
$(top_srcdir)/Makefile.in \
$(top_srcdir)/aclocal.m4 \
$(top_srcdir)/config.h.in \
$(top_srcdir)/config.h.in~ \
$(top_srcdir)/configure \
$(top_srcdir)/gbtools_version.m4

GENERATED_FILES = $(top_srcdir)/config $(top_srcdir)/m4

maintainer-clean-local:
	rm -rf $(GENERATED_FILES) $(top_srcdir)/.deps
	mkdir $(top_srcdir)/m4 $(top_srcdir)/config



