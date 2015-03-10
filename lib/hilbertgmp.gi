#! @Chapter Hilbert bases
#! In this section we show how to interact with <C>hilbert</C> in <C>4ti2<C>


#! @Description
#! Computes a Hilbert basis of the cone or by a monoid
#! The input argument <A>a</a> is a list with structure: 
#! [ <type>, <list>, <type>, <list>, ... ] 
#! <type> is a text string, with possible values: 
#! "mat", "lat", "sign", "rel", "ub", to
#! describe (see documentation at http://www.4ti2.de) the contents of each 
#! matrix, given as a GAP <list>
#! @Returns a list with elements of the basis.
#! @Arguments a
InstallGlobalFunction(HilbertBasis4ti2gmp, function(arg)
    local a, narg;

    narg:=Length(arg);
    if narg>0 then
        a:=arg[1];
        if not(IsList(a)) then
            Error("The argument must be a list");
        fi;
        return _4ti2zsolve_HilbertGMP(a);
    fi;
    Error("Wrong number of arguments");
end);
