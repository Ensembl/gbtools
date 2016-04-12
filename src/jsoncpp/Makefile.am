
include $(top_srcdir)/src/Makefile.am.common

SUBDIRS = .

noinst_LTLIBRARIES = libjsoncpp.la

libjsoncpp_la_SOURCES = \
jsoncpp.cpp \
json/json.h \
json/json-forwards.h \
$(NULL) 

# Extra files to remove for the maintainer-clean target.
#
MAINTAINERCLEANFILES = $(top_srcdir)/src/jsoncpp/Makefile.in

