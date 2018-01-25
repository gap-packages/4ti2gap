InstallGlobalFunction(ZBasis4ti2, function(arg)
    local matrix, narg;

    narg:=Length(arg);
    if narg < 1 then
      Error("The number of arguments is not enough");
    fi;

    matrix := arg[1];
    if not(IsRectangularTable(matrix)) then
        Error("The first argument must be a matrix");
    fi;

    return _4ti2groebner_ZBasis( matrix );
end);
