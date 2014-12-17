#! @Chapter Hilbert bases
#! In this section we show how to interact with <C>hilbert</C> in <C>4ti2<C>


#! @Description
#! Computes a Hilbert basis of the cone defined by equations especified by a matrix ....
#! generators are the columns of <A>a</A>
#! If a second argument <A>m</A> is given, then it must be a matrix specifying the
#! order to compute the Groebner basis
#! @Returns a list containing the differences of the exponents of the binomials
#! @Arguments a[,m]
InstallGlobalFunction(HilbertBasis4ti2, function(arg)
    local a, narg;

    narg:=Length(arg);
    if narg>0 then
        a:=arg[1];
        if not(IsList(a)) then
            Error("The argument must be a list");
        fi;
        return _4ti2_Hilbert(a);
    fi;
    Error("Wrong number of arguments");
end);
