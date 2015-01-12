# Check for GMP, taken from linbox

# Modified by Pascal Giorgi, 2003-12-03
# Modified by Alfredo Schez.-R., 2015-01-05

dnl LB_CHECK_GMP ([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl
dnl Test for the GNU Multiprecision library and define GMP_CFLAGS, GMP_LIBS and GMP_HOME

AC_DEFUN([LB_CHECK_GMP],
[
    DEFAULT_CHECKING_PATH="DEFAULT"
    GMP_HOME_PATH="${DEFAULT_CHECKING_PATH}"
    AC_ARG_WITH(gmp,
    AS_HELP_STRING([--with-gmp={DIR|no}],
    [Use the GMP library installed in DIR. 
    If the argument is no, do not use the GMP library; 
    some functionality will not be available then. 
    Otherwise, the library is searched in the standard locations 
    (like "/usr" or "/usr/local").]),
    [if test "$withval" = yes ; then
        GMP_HOME_PATH="${DEFAULT_CHECKING_PATH}"
    elif test "$withval" != no ; then
        GMP_HOME_PATH="$withval ${DEFAULT_CHECKING_PATH}"
    fi],
    [GMP_HOME_PATH="${DEFAULT_CHECKING_PATH}"]
    )

dnl Check for existence
BACKUP_CXXFLAGS=${CXXFLAGS}
BACKUP_LIBS=${LIBS}

AC_MSG_CHECKING(for GMP)
dnl ToDo: remember the values of GMP_* for a GMP found
for GMP_HOME in ${GMP_HOME_PATH} 
    do	
    if test "x$GMP_HOME" = "xDEFAULT" -o -r "$GMP_HOME/include/gmp.h"; then
        if test "x$GMP_HOME" != "xDEFAULT" ; then
            GMP_CFLAGS="-I${GMP_HOME}/include"
            GMP_LIBS="-L${GMP_HOME}/lib -lgmp"	
            # We used to use -R here, but it's not portable
            ##-R${GMP_HOME}/lib 
        else
            GMP_CFLAGS=
            GMP_LIBS="-lgmp"		
        fi
        CXXFLAGS="${CXXFLAGS} ${GMP_CFLAGS}"
        LIBS="${LIBS} ${GMP_LIBS}"
        AC_TRY_LINK(
        [#include <gmp.h>],
        [mpz_t a; mpz_init (a);],
        [
            gmp_found="yes"
            AC_SUBST(GMP_HOME)
            AC_SUBST(GMP_CFLAGS)
            AC_SUBST(GMP_LIBS)
            AC_MSG_CHECKING(for GMP support in 4ti2)
            CXXFLAGS="${CXXFLAGS} ${FTIT_CPPFLAGS} -D_4ti2_GMP_"
            LIBS="${LIBS} ${FTIT_LDFLAGS} -lgmpxx -l4ti2gmp"
            AC_TRY_LINK(
            [#include "groebner/VectorArray.h"],
            [_4ti2_::VectorArray a(2, 2)],[
                gmp_4ti2="yes"
                AC_MSG_RESULT(found)
                AC_DEFINE(HAVE_GMP,1,[Define if GMP is installed])
                ifelse([$1], , :, [$1])
                break
            ],[			
                gmp_4ti2="no"
                break
            ])
        ],[
            gmp_found="no"	
            unset GMP_CFLAGS
            unset GMP_LIBS	
        ])
    else
        gmp_found="no"
    fi
done
if test "x$gmp_found" != "xyes"; then
    AC_MSG_RESULT(not found)
    ifelse($2, , :, $2)
elif test "x$gmp_4ti2" != "xyes"; then
    AC_MSG_RESULT(4ti2 is not capable to manipulate arbitrary precision values. Disabling.)
fi


CXXFLAGS=${BACKUP_CXXFLAGS}
LIBS=${BACKUP_LIBS}
])
