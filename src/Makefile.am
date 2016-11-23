
SUBDIRS = include gbtoolsUtils gbtoolsGUI

SUBDIRS += jsoncpp

if USE_BLATSRC
SUBDIRS += blatSrc
endif

if USE_LIBCURL
SUBDIRS += gbtoolsCurl gbtoolsPfetch
SUBDIRS += gbtoolsTrackhub
endif

# Extra files to remove for the maintainer-clean target.
#
MAINTAINERCLEANFILES = $(top_srcdir)/src/Makefile.in

