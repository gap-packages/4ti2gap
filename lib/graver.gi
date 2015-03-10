#! @Chapter Graver bases
#! In this section we show how to interact with <C>graver</C> in <C>4ti2<C>


#! @Description
#! Computes a Graver basis of a matrix or a lattice
#! The input argument <A>a</a> is a list with structure: 
#! [ <type>, <list>, <type>, <list>, ... ] 
#! <type> is a text string, with possible values: 
#! "mat", "lat" to describe (see documentation at http://www.4ti2.de) 
#! the contents of each matrix, given as a GAP <list>.
#! @Returns a list with elements of the basis
#! @Arguments a
InstallGlobalFunction(GraverBasis4ti2, function(arg)
    local a, narg, pos;

    narg:=Length(arg);
    if narg>0 then
        a:=arg[1];
        if not(IsList(a)) then
            Error("The argument must be a list");
        fi;
        pos:=First([1..Length(a)], i->(a[i]="mat" or a[i]="lat"));
        if pos = fail or pos = Length(a) then
            Error("Input data bad constructed");
        fi;
        if First(Flat(a[pos+1]),n->GMP_REDUCE(n)<>fail) <> fail then
            Error("The size of the input suggest to use the GMP version of this function");
        fi;
        return _4ti2zsolve_Graver(a);
    fi;
    Error("Wrong number of arguments");
end);
