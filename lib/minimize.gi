InstallGlobalFunction(Minimize4ti2, function(arg)
    local matrix, basis, _cost, cost, _zsol, zsol, sign, narg;

    narg:=Length(arg);
    if narg < 5 then
      Error("The number of arguments is not enough");
    fi;

    matrix := arg[1]; basis := arg[2];
    if not(IsRectangularTable(matrix)) and not(IsRectangularTable(basis)) then
        Error("The first and or the second argument must be a matrix");
    fi;

    _cost := arg[3];
    if IsRectangularTable(_cost) then
        cost := _cost;
    else
        if not(IsList(_cost)) then
            Error("The third argument should be a list or a matrix with one row");
        else
            cost := [_cost];
        fi;
    fi;

    _zsol := arg[4];
    if IsRectangularTable(_zsol) then
        zsol := _zsol;
    else
        if not(IsList(_zsol)) then
            Error("The fourth argument should be a list or a matrix with one row");
        else
            zsol := [_zsol];
        fi;
    fi;

    sign := arg[5];

    return _4ti2groebner_Minimize( matrix, basis, cost, zsol, sign );
end);
