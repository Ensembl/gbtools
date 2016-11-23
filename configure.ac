#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_include([./gbtools_version.m4])

AC_PREREQ([2.50])
AC_INIT([gbtools], VERSION_NUMBER, [annosoft@sanger.ac.uk])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/gbtoolsGUI/gbtoolsGUI.cpp])
AM_INIT_AUTOMAKE([1.9 foreign])
AC_CONFIG_HEADERS([config.h])


AC_CONFIG_FILES([Makefile
src/Makefile
src/include/Makefile
src/gbtoolsUtils/Makefile
src/gbtoolsGUI/Makefile
src/gbtoolsCurl/Makefile
src/gbtoolsTrackhub/Makefile
src/gbtoolsPfetch/Makefile
src/jsoncpp/Makefile
src/blatSrc/Makefile
])



# Check if this is a Git repository and, if so, include info about the current
# status in the version info. Otherwise use PACKAGE_VERSION (which will get
# set from VERSION_NUMBER in version.m4).
if test -d "$srcdir/../.git" 
then AC_DEFINE_UNQUOTED([GBTOOLS_VERSION], ["`git describe --abbrev=1`"], ["gbtools package version"])
else AC_DEFINE([GBTOOLS_VERSION], [PACKAGE_VERSION], ["gbtools package version"])
fi


AC_CONFIG_MACRO_DIR([m4])

LT_INIT([disable-shared])

LT_LANG([C++])

# keep libtool up to date automatically.
AC_SUBST([LIBTOOL_DEPS])

# Checks for programs.
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET


# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h gtk.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([dup2 floor gethostbyname memset socket sqrt strcasecmp strchr strcspn strerror strncasecmp strrchr strstr strtol uname])

# Check for dependencies required by all executables
PKG_CHECK_MODULES([DEPS], [glib-2.0 gtk+-2.0 >= 2.10])

# Check for optional dependencies
AX_CHECK_OPENSSL()

AC_CHECK_HEADERS([openssl.h], [HAVE_OPENSSL_HEADER=1])
AM_CONDITIONAL([USE_SSL], [ test -n "$HAVE_OPENSSL_HEADER" || test -n "$OPENSSL_INCLUDES" ])

AM_COND_IF([USE_SSL], [ echo 'openssl is available!' ])
AM_COND_IF([USE_SSL], [AC_DEFINE([USE_SSL], [1], [Define to 1 if openssl is available])])


AC_ARG_VAR(GLIB_GENMARSHAL, [The glib-genmarshal executable.])
AC_CHECK_PROG(GLIB_GENMARSHAL, glib-genmarshal, glib-genmarshal)


# Check for the curl library. This is optional, so don't fail
# if we can't find it, but set the USE_LIBCURL to false. Note: I think we should 
# probably be using pkg-config to find the curl dependencies, but that doesn't work
# on my mac (perhaps because it's in /usr instead of /opt/local), so I just check for
# the header and set the dependencies manually in Makefile.am.
#PKG_CHECK_MODULES([LIBCURL_DEPS], [curl], [HAVE_LIBCURL=1], [HAVE_LIBCURL=0])
AC_CHECK_HEADERS([curl/curl.h], [HAVE_LIBCURL=1], [HAVE_LIBCURL=0])
AM_CONDITIONAL([USE_LIBCURL], [test "$HAVE_LIBCURL" -eq 1])


# Check for the blatSrc library deps. Again, optional.
AC_CHECK_HEADERS([stdio.h stdlib.h stdarg.h ctype.h sys/types.h sys/stat.h strings.h fcntl.h assert.h setjmp.h time.h math.h errno.h unistd.h libgen.h sys/wait.h mingw/math.h sys/socket.h netinet/in.h arpa/inet.h netdb.h pthread.h regex.h signal.h execinfo.h limits.h sys/mman.h syslog.h sys/time.h dirent.h sys/utsname.h sys/statvfs.h pwd.h termios.h utime.h io.h direct.h ieeefp.h sys/file.h zlib.h], [HAVE_BLATSRC_DEPS=1], [HAVE_BLATSRC_DEPS=0])
AM_CONDITIONAL([USE_BLATSRC], [test "$HAVE_BLATSRC_DEPS" -eq 1])
AM_COND_IF([USE_BLATSRC], [ echo 'blatSrc is available!' ])
AM_COND_IF([USE_BLATSRC], [AC_DEFINE([USE_BLATSRC], [1], [Define to 1 if blatSrc deps are available])])
AM_COND_IF([USE_BLATSRC], [AC_SUBST([USE_BLATSRC], [use_blatsrc])])



# Check for execinfo.h. This is used to provide backtraces but is not available
# on all systems.
AC_CHECK_HEADERS([execinfo.h])


# what are we building on?
AC_CANONICAL_BUILD
case $build in
     *-*-linux*)
     os_type="Linux";;
     *-*-darwin*)
     os_type="darwin" ;;
     *-*-cygwin*)
     os_type="cygwin" ;;
     *-*-mingw*)
     os_type="windows" ;;
     *-*-msys*)
     os_type="windows" ;;
     *)
     os_type="Unknown";;
esac

AM_CONDITIONAL([LINUX], [ test "$os_type" = "Linux" ])
AM_CONDITIONAL([DARWIN], [ test "$os_type" = "darwin" ])
AM_CONDITIONAL([CYGWIN], [ test "$os_type" = "cygwin" ])
AM_CONDITIONAL([WINDOWS], [ test "$os_type" = "windows" ])

# replaces -DLINUX
if test "$os_type" = "Linux" ; then
   AC_MSG_RESULT([Got OS type of LINUX])
   AC_DEFINE(LINUX, [], [Operating System is LINUX.])

elif test "$os_type" = "darwin" ; then

   AC_MSG_RESULT([Got OS type of Darwin])
   AC_DEFINE(DARWIN, [], [Operating System is DARWIN.])

elif test "$os_type" = "cygwin" ; then

   AC_MSG_RESULT([Got OS type of Cygwin])
   AC_DEFINE(CYGWIN, [], [Operating System is CYGWIN.])

elif test "$os_type" = "windows" ; then

   AC_MSG_RESULT([Got OS type of Windows])
   AC_DEFINE(WINDOWS, [], [Operating System is WINDOWS.])

fi



AC_OUTPUT


echo \
"-------------------------------------------------

 ${PACKAGE_NAME} Version ${PACKAGE_VERSION}

 Prefix: '${prefix}'
 Compiler: '${CXX} ${CXXFLAGS} ${CPPFLAGS}'

 Now type 'make @<:@<target>@:>@'
   where the optional @<:@<target>@:>@ is:
     all       - build all libraries (default)
     install   - build and install everything
"

if test "$HAVE_LIBCURL" -eq 0; then
echo " Warning: libcurl not found; track hub and pfetch functionality will not be available"
fi

if test "$HAVE_BLATSRC_DEPS" -eq 0; then
echo " Warning: Dependencies not found for Bed/bigBed/bigWig support"
fi

echo "-------------------------------------------------
"

