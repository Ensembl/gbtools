
SUBDIRS = src

ACLOCAL_AMFLAGS = -I m4


lib_LTLIBRARIES = libgbtools.la

libgbtools_la_SOURCES = 

libgbtools_la_LIBADD  = \
src/gbtoolsCurl/libgbtoolsCurl.la \
src/gbtoolsGUI/libgbtoolsGUI.la \
src/gbtoolsPfetch/libgbtoolsPfetch.la \
src/gbtoolsUtils/libgbtoolsUtils.la


# keep libtool up to date automatically.
LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status libtool


# I picked this up from the internet...doesn't seem to work for the very
# last invocation of libtool which produces the .la file, not sure if
# this actually matters or not...
#
# This forces automake to use the C++ linker
# (see the automake manual, section "Libtool Convenience Libraries")
#nodist_EXTRA_libexample_la_SOURCES = dummy.cpp
#sourcescheck_DUMMY = dummy.cpp




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

GENERATED_DIRS = $(top_srcdir)/config $(top_srcdir)/m4 $(top_srcdir)/.deps $(top_srcdir)/build/src

maintainer-clean-local:
	rm -rf $(GENERATED_DIRS)
	mkdir $(top_srcdir)/m4 $(top_srcdir)/config



