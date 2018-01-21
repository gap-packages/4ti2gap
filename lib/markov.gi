InstallGlobalFunction(MarkovBasis4ti2, function(arg)
    local matrix, basis, cost, sign, weights, weights_max, _zsol, zsol, narg;

    narg:=Length(arg);
    if narg < 6 then
      Error("The number of arguments is not enough");
    fi;

    matrix := arg[1]; basis := arg[2];
    if not(IsRectangularTable(matrix)) and not(IsRectangularTable(basis)) then
        Error("The first and or the second argument must be a matrix");
    fi;

    sign := arg[3];

    weights := arg[4];

    weights_max := arg[5];

    _zsol := arg[6];
    if IsRectangularTable(_zsol) then
        zsol := _zsol;
    else
        if not(IsList(_zsol)) then
            Error("The sixth argument should be a list or a matrix with one row");
        else
            zsol := [_zsol];
        fi;
    fi;

    return _4ti2groebner_MarkovBasis( matrix, basis, sign, weights, weights_max, zsol );
end);
