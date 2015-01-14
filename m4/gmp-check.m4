# Check for GMP, taken from linbox

# Modified by Pascal Giorgi, 2003-12-03
# Modified by Alfredo Schez.-R., 2015-01-13

dnl LB_CHECK_GMP ([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl
dnl Test for the GNU Multiprecision library and define GMP_CPPFLAGS, GMP_LIBS and GMP_HOME
dnl also assings gmp_found, gmpxx_found and gmp4ti2_found

AC_DEFUN([LB_CHECK_GMP],
[
    DEFAULT_CHECKING_PATH="/usr /usr/local"
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
    else
        GMP_HOME_PATH=""
    fi],
    [GMP_HOME_PATH="${DEFAULT_CHECKING_PATH}"]
    )

dnl Check for existence
BACKUP_CPPFLAGS=${CPPFLAGS}
BACKUP_LIBS=${LIBS}

AC_MSG_CHECKING(for GMP)
for GMP_HOME in ${GMP_HOME_PATH} 
    do
    if test -r "$GMP_HOME/include/gmp.h"; then
        gmp_found="yes"
        GMP_CPPFLAGS="-I${GMP_HOME}/include"
        GMP_LIBS="-L${GMP_HOME}/lib -lgmp"
        CPPFLAGS="${CPPFLAGS} ${GMP_CPPFLAGS}"
        LIBS="${LIBS} ${GMP_LIBS}"
        AC_TRY_LINK(
        [#include <gmp.h>],
        [mpz_t a; mpz_init (a);],
        [
            AC_MSG_RESULT(found)
            AC_SUBST(GMP_HOME)
            AC_SUBST(GMP_CPPFLAGS)
            AC_SUBST(GMP_LIBS)
            if test -r "$GMP_HOME/include/gmpxx.h"; then
                gmpxx_found="yes"
                GMP_LIBS="${GMP_LIBS} -lgmpxx"
                CXXFLAGS="${CXXFLAGS} ${CPPFLAGS}"
                LIBS="${LIBS} -lgmpxx"
                AC_MSG_CHECKING(if GMP has c++ interface available)
                AC_TRY_LINK(
                [#include <gmpxx.h>],
                [mpz_class v],[
                    AC_MSG_RESULT(gmpxx found)
                    ifelse([$1], , :, [$1])
                    break
                ],[			
                    gmpxx_found="no"
                ])
            fi
        ],[
            gmp_found="no"
            unset GMP_CPPFLAGS
            unset GMP_LIBS	
        ])
    else
        gmp_found="no"
    fi
done
AC_SUBST(gmp_found)
AC_SUBST(gmpxx_found)
if test "x$gmp_found" != "xyes"; then
    AC_MSG_RESULT(not found)
    ifelse([$2], , :, [$2])
elif test "x$gmpxx_found" != "xyes"; then
    AC_MSG_RESULT(could not find gmpxx)
fi


CPPFLAGS=${BACKUP_CPPFLAGS}
LIBS=${BACKUP_LIBS}
])
