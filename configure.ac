dnl ##
dnl ## Process this file with autoconf to produce a configure script.
dnl ##

dnl ##
dnl ## Setup autoconf
dnl ##
AC_PREREQ([2.68])
AC_INIT([4ti2gap], m4_esyscmd([tr -d '\n' < VERSION]))
AC_CONFIG_SRCDIR([src/4ti2gap.cc])
AC_CONFIG_HEADER([src/pkgconfig.h:cnf/pkgconfig.h.in])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([cnf])

dnl ##
dnl ## Get canonical host info
dnl ##
AC_CANONICAL_HOST

dnl ##
dnl ## Setup automake
dnl ##
AM_INIT_AUTOMAKE([1.11 -Wall foreign subdir-objects])
AM_SILENT_RULES([yes])
AM_PROG_AR

dnl ##
dnl ## C++ is the language
dnl ##
AC_PROG_CXX
AC_LANG([C++])
case $host_os in
    darwin* )
        AX_CHECK_COMPILE_FLAG([-std=c99], [CXXFLAGS="$CXXFLAGS -std=c99"])
        ;;
    *)
        AX_CHECK_COMPILE_FLAG([-std=c++0x], [CXXFLAGS="$CXXFLAGS -std=c++0x"])
        ;;
esac

dnl ##
dnl ## Setup libtool (for building the GAP kernel extension)
dnl ##
LT_PREREQ([2.4.2])
LT_INIT([disable-static dlopen win32-dll])

dnl ##
dnl ## Locate the GAP root dir
dnl ##
AC_FIND_GAP

AC_ARG_WITH([hpcgap],
    [AC_HELP_STRING([--with-hpcgap], [to specify that this is hpc-gap version])],
    [
    for f in $GAPROOT/*hpc; do
        if test -e "$f"; then
            TOADDCFLAGS="../../build/cflags"
            TOADDLDFLAGS="../../build/ldflags"
        else
            echo "I don't see anything that tells this is hpcgap..."
        fi;
        break
    done
    ],[])

if test "x$TOADDCFLAGS" != "x"; then
    if test -e "$TOADDCFLAGS" && test -e "$TOADDLDFLAGS"; then
        CFLAGS="$CFLAGS `cat $TOADDCFLAGS`"
        CXXFLAGS="$CXXFLAGS `cat $TOADDCFLAGS`"
        LDFLAGS="$LDFLAGS `cat $TOADDLDFLAGS`"
    else
        AC_MSG_WARN(Could not find the some config files for compilation)
    fi
fi;

dnl ##
dnl ## Locate 4ti2
dnl ##

FTIT_PATH=

AC_ARG_WITH([4ti2],
    [AC_HELP_STRING([--with-4ti2=<path>], [specify root of 4ti2 installation])],
    [FTIT_PATH="$with_4ti2"],
    [
      AC_MSG_CHECKING([if 4ti2 is installed])
      BACKUP_LDFLAGS="$LDFLAGS"
      LDFLAGS="$LDFLAGS -lzsolve"
      AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [[#include <4ti2/4ti2.h>]], 
          [[_4ti2_state* zsolve_api = _4ti2_zsolve_create_state(_4ti2_PREC_INT_64);]])], 
        [AC_MSG_RESULT([yes])], 
        [AC_MSG_ERROR([it seems that it is not installed])])
      LDFLAGS="$BACKUP_LDFLAGS"
     ])

if test "x$FTIT_PATH" != "x"; then
  FTIT_LDFLAGS="-L$FTIT_PATH/lib"
  FTIT_CPPFLAGS="-I$FTIT_PATH/include"
  AC_SUBST(FTIT_LDFLAGS)
  AC_SUBST(FTIT_CPPFLAGS)
fi

dnl ##
dnl ## Locate GMP
dnl ##
LB_CHECK_GMP([],[])

if test "$gmp_found" = "no"; then 
  AC_MSG_ERROR([GMP is required in order to build 4ti2gap])
fi

dnl ##
dnl ## Check if 4ti2 is compiled with GMP
dnl ##
gmp4ti2_found="no"
if test ! -z "${GMP_LIBS}"; then
  AC_MSG_CHECKING(for GMP support in 4ti2)
  BACKUP_CPPFLAGS=${CPPFLAGS}
  BACKUP_LIBS=${LIBS}
  CPPFLAGS="${CPPFLAGS} ${FTIT_CPPFLAGS} -D_4ti2_GMP_"
  LIBS="${LIBS} ${FTIT_LDFLAGS} -l4ti2gmp"
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([[#include "groebner/VectorArray.h"]],
      [[_4ti2_::VectorArray a(2, 2)]])],
    [gmp4ti2_found="yes"])
  CPPFLAGS=${BACKUP_CPPFLAGS}
  LIBS=${BACKUP_LIBS}
fi

AC_EGREP_HEADER([USE_GMP 1],[${GAPROOT}/bin/${GAPARCH}/config.h],
               [gap_uses_gmp="yes"], [gap_uses_gmp="no"])

if test "$gmp4ti2_found" = "no" -a "$gap_uses_gmp" = "yes"; then
  AC_MSG_ERROR([GAP uses GMP and 4ti2 is compiled without GMP, this setting will not compile])
fi

AM_CONDITIONAL([WITH_GMPXX],[test "$gmpxx_found" != "no" -a "$gmp4ti2_found" != "no"])

dnl CXXFLAGS="${CXXFLAGS} ${FTIT_CPPFLAGS} -D_4TI2_GMP_"
dnl LIBS="${LIBS} ${FTIT_LDFLAGS} -l4ti2gmp"

dnl AC_TRY_LINK(
dnl     [#include "groebner/VectorArray.h"],
dnl     [std::VectorArray a(2, 2);],
dnl       ac_cv_ipv6=yes,
dnl       ac_cv_ipv6=no,
dnl       ac_cv_ipv6=no))
dnl AC_MSG_RESULT($ac_cv_ipv6)
dnl if test $ac_cv_ipv6 = yes ; then
dnl       AC_DEFINE(IPV6,,"check if system supports IPV6")
dnl fi

dnl ##
dnl ## Detect pointer size to distinguish 32 and 64 bit builds
dnl ##
AC_CHECK_SIZEOF([void **])
if test "${ac_cv_sizeof_void_pp}" = "8" ; then
INTBITS="-D_4ti2_INT64_"
FTITLIB="-l4ti2int64"
AC_SUBST([INTBITS])
AC_SUBST([FTITLIB])
elif test "${ac_cv_sizeof_void_pp}" = "4" ; then
INTBITS="-D_4ti2_INT32_"
FTITLIB="-l4ti2int32"
AC_SUBST([INTBITS])
AC_SUBST([FTITLIB])
else
AC_MSG_ERROR([Architecture detection fail])
fi

dnl ##
dnl ## Detect Windows resp. Cygwin
dnl ##
case $host_os in
  *cygwin* ) CYGWIN=yes;;
         * ) CYGWIN=no;;
esac
AM_CONDITIONAL([SYS_IS_CYGWIN], [test "$CYGWIN" = "yes"])
if test "$CYGWIN" = "yes"; then
  AC_DEFINE(SYS_IS_CYGWIN32, 1, are we on CYGWIN?)
else
  AC_DEFINE(SYS_IS_CYGWIN32, 0, are we on CYGWIN?)
fi

dnl ##
dnl ## Finally, generate the Makefiles and output everything
dnl ##
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
