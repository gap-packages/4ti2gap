# Check for GMP, taken from linbox

# Modified by Pascal Giorgi, 2003-12-03
# Modified by Alfredo Schez.-R., 2015-01-09

dnl LB_CHECK_GMP ([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl
dnl Test for the GNU Multiprecision library and define GMP_FLAGS, GMP_LIBS and GMP_HOME

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
    fi],
    [GMP_HOME_PATH="${DEFAULT_CHECKING_PATH}"]
    )

dnl Check for existence
BACKUP_CXXFLAGS=${CXXFLAGS}
BACKUP_LIBS=${LIBS}

AC_MSG_CHECKING(for GMP)
for GMP_HOME in ${GMP_HOME_PATH} 
    do	
    if test -r "$GMP_HOME/include/gmp.h"; then
        GMP_FLAGS="-I${GMP_HOME}/include"
        GMP_LIBS="-L${GMP_HOME}/lib -lgmp"	
        # We used to use -R here, but it's not portable
        ##-R${GMP_HOME}/lib 
        if test -r "$GMP_HOME/include/gmpxx.h"; then
            GMP_LIBS="${GMP_LIBS} -lgmpxx"
        fi
        CXXFLAGS="${CXXFLAGS} ${GMP_FLAGS}"
        LIBS="${LIBS} ${GMP_LIBS}"
        AC_TRY_LINK(
        [#include <gmp.h>],
        [mpz_t a; mpz_init (a);],
        [
            gmp_found="yes"
            AC_SUBST(GMP_HOME)
            AC_SUBST(GMP_FLAGS)
            AC_SUBST(GMP_LIBS)
            AC_MSG_CHECKING(for GMP support in 4ti2)
            CXXFLAGS="${CXXFLAGS} ${FTIT_CPPFLAGS} -D_4ti2_GMP_"
            LIBS="${LIBS} ${FTIT_LDFLAGS} -l4ti2gmp"
            AC_TRY_LINK(
            [#include "groebner/VectorArray.h"],
            [_4ti2_::VectorArray a(2, 2)],[
                gmp_4ti2="yes"
                AC_MSG_RESULT(found)
                AC_DEFINE(HAVE_GMP,1,[Define if GMP is installed])
            ],[			
                gmp_4ti2="no"
                break
            ])
            if test "x$gmp_4ti2" = "xyes"; then 
                AC_MSG_CHECKING(if GMP has c++ interface available)
                LIBS="${LIBS} -lgmpxx"
                AC_TRY_LINK(
                [#include <gmpxx.h>],
                [mpz_class v],[
                    gmp_4ti2="yes"
                    AC_MSG_RESULT(gmpxx found)
                    AC_DEFINE(HAVE_GMP,1,[Define if GMP is installed])
                    ifelse([$1], , :, [$1])
                ],[			
                    gmp_4ti2="no"
                ])
                break
             fi 
        ],[
            gmp_found="no"	
            unset GMP_FLAGS
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
