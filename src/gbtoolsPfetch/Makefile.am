
include $(top_srcdir)/src/Makefile.am.common

SUBDIRS = .

noinst_LTLIBRARIES = libgbtoolsPfetch.la

libgbtoolsPfetch_la_SOURCES = \
gbtoolsPfetch-utils.cpp \
gbtoolsPfetch.cpp \
gbtoolsPfetch.hpp \
gbtoolsPfetch_I.hpp \
gbtoolsPfetch_P.hpp \
$(NULL)  


# This stuff generates the glib marshalling code to support
# multi-language bindings....which are used for their version
# of objects, a pain....
#
gbtoolsPfetch_la_marshal = gbtoolsPfetch-cmarshal.list

gbtoolsPfetch_la_extra_sources = \
gbtoolsPfetch-cmarshal.hpp \
gbtoolsPfetch-cmarshal.cpp \
$(NULL)

nodist_libgbtoolsPfetch_la_SOURCES = $(gbtoolsPfetch_la_extra_sources)

libgbtoolsPfetch_la_DEPENDENCIES = $(nodist_gbtoolsPfetch_la_SOURCES)

BUILT_SOURCES = $(gbtoolsPfetch_la_extra_sources)
CLEANFILES = $(BUILT_SOURCES)
EXTRA_DIST = $(gbtoolsPfetch_la_marshal)   


GENMARSHAL_COMMAND = $(GLIB_GENMARSHAL) --prefix=gbtoolsPfetch_cmarshal

# gen-marshall generates C headers so we have to remove the the C++ guards from the header
# so that normal C++ name mangling can occur.
#
.list.hpp:
	$(GENMARSHAL_COMMAND) --header $< >$@
	perl -lne 's{G_BEGIN_DECLS}{}; print;' < $@ | perl -lne 's{G_END_DECLS}{}; print;' > $@.tmp_file
	rm $@
	mv $@.tmp_file $@

.list.cpp:
	$(GENMARSHAL_COMMAND) --body $< >$@


# Extra files to remove for the maintainer-clean target.
#
MAINTAINERCLEANFILES = $(top_srcdir)/src/gbtoolsPfetch/Makefile.in

