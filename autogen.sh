#!/bin/bash
###############################################################################
# Simple script to bootstrap and create the configure script, should be run
# when any control files have been changed (e.g. new source files added which
# led to changes to Makefile.am files) including:
#
#    configure.ac
#    Makefile.am
#    Makefile.in
#
############################################################

# set up gbtools version number, this is arcane and horrible and all
# autoconf's fault. See http://sources.redhat.com/automake/automake.html#Rebuilding
# and the stuff about AC_INIT
# NOTE that gbtools_version.m4 is referenced from configure.ac
#
version_macro_file='gbtools_version.m4'
rm -f $version_macro_file

GBTOOLS_VERSION=`git describe --dirty --abbrev=1`

echo 'dnl gbtools_version.m4 generated by autogen.sh script. Do not hand edit.'  > $version_macro_file
echo "m4_define([VERSION_NUMBER], [$GBTOOLS_VERSION])" >> $version_macro_file
echo "gbtools version is: $GBTOOLS_VERSION"


# both config and m4 subdirectories must exist before we run autoreconf and configure.
#
if [ -e m4 ] ; then
  rm -rf m4
fi

if [ -e config ] ; then
  rm -rf config
fi

mkdir m4 config


# Now use autoreconf to bootstrap the autoconf/automake system.
#
autoreconf -fi
