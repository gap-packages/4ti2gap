InstallGlobalFunction(Normalform4ti2, function(arg)
    local matrix, basis, gro, _cost, cost, feas, sign, narg;

    narg:=Length(arg);
    if narg < 6 then
      Error("The number of arguments is not enough");
    fi;

    matrix := arg[1]; basis := arg[2];
    if not(IsRectangularTable(matrix)) and not(IsRectangularTable(basis)) then
        Error("The first and or the second argument must be a matrix");
    fi;

    gro := arg[3];
    if not(IsRectangularTable(gro)) then
        Error("The third argument must be a list of vectors (the Groebner basis)");
    fi;

    _cost := arg[4];
    if IsRectangularTable(_cost) then
        cost := _cost;
    else
        if not(IsList(_cost)) then
            Error("The fourth argument should be a list or a matrix with one row");
        else
            cost := [_cost];
        fi;
    fi;

    feas := arg[5];
    if not(IsRectangularTable(feas)) then
        Error("The fifth argument must be a list of vectors");
    fi;

    sign := arg[6];

    return _4ti2groebner_Normalform( matrix, basis, gro, cost, feas, sign );
end);
