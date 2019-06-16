InstallGlobalFunction(GraverBasis4ti2, function(arg)
    local a, narg, pos;

    narg:=Length(arg);
    if narg>0 then
        a:=arg[1];
        if not(IsList(a)) then
            Error("The argument must be a list");
        fi;
        pos:=First([1..Length(a)], i->(a[i]="mat" or a[i]="lat"));
        if pos = fail or pos = Length(a) or not( IsValidZSolveInput( a ) ) then
            Error("Input data bad constructed");
        fi;
#!        if First(Flat(a[pos+1]),n->GMP_REDUCE(n)<>fail) <> fail then
#!            Error("The size of the input suggest to use the GMP version of this function");
#!        fi;
        return _4ti2zsolve_Graver(a);
    fi;
    Error("Wrong number of arguments");
end);
