InstallGlobalFunction(HilbertBasis4ti2gmp, function(arg)
    local a, narg, pos, result, recresult;

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
        result:=_4ti2zsolve_HilbertGMP(a);
        if Length(result)>1 then
            recresult:=rec(zhom:=result[1], zfree:=result[2]);
        else
            recresult:=rec(zhom:=result[1]);
        fi;
        return recresult;
    fi;
    Error("Wrong number of arguments");
end);
