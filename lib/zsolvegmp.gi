#! @Chapter ZSolve
#! In this section we show how to interact with <C>zsolve</C> in <C>4ti2<C>


#! @Description
#! Computes the solutions of a linear system over the set of the integers
#! The input argument <A>a</a> is a list with structure: 
#! [ <type>, <list>, <type>, <list>, ... ] 
#! <type> is a text string, with possible values: 
#! "mat", "lat", "sign", "rel", "ub", to
#! describe (see documentation at http://www.4ti2.de and/or the file 
#! 4ti2_manual.pdf in 4ti2 installation tree) the contents of each 
#! matrix, given as a GAP <list>
#! @Returns a record with description of all solutions, with the selectors:
#! zinhom, zhom, zfree 
#! @Arguments a
InstallGlobalFunction(ZSolve4ti2gmp, function(arg)
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
        result:=_4ti2zsolve_ZSolveGMP(a);
        if Length(result)>2 then
            recresult:=rec(zinhom:=result[1], zhom:=result[2], zfree:=result[3]);
        else
            recresult:=rec(zinhom:=result[1], zhom:=result[2]);
        fi;
        return recresult;
    fi;
    Error("Wrong number of arguments");
end);
