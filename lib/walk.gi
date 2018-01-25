InstallGlobalFunction(Walk4ti2, function(arg)
    local matrix, basis, gro_start, _cost, cost_start, cost, _zsol, zsol, sign, narg;

    narg:=Length(arg);
    if narg < 6 then
      Error("The number of arguments is not enough");
    fi;

    matrix := arg[1]; basis := arg[2];
    if not(IsRectangularTable(matrix)) and not(IsRectangularTable(basis)) then
        Error("The first and or the second argument must be a matrix");
    fi;

    gro_start := arg[3];
    if gro_start = [] then
        Error("The third agument is needed");
    fi; 

    _cost := arg[4];
    if IsRectangularTable(_cost) then
        cost_start := _cost;
    else
        if not(IsList(_cost)) then
            Error("The forth argument should be a list, or a matrix with one row");
        else
            cost_start := [_cost];
        fi;
    fi;

    _cost := arg[5];
    if not(IsList(_cost)) or _cost = [] then 
        Error("The fifth agument is needed");
    else  
        if IsRectangularTable(_cost) then
            cost := _cost;
        else
#            if not(IsList(_cost)) then
#                Error("The fifth argument should be a list, or a matrix with one row");
#            else
                cost := [_cost];
#            fi;
        fi;
    fi;

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

    sign := arg[7];

#    Print("Arguments:", matrix, "\n", basis, "\n", gro_start, "\n", cost_start, "\n", cost, "\n", zsol, "\n");

#    return _4ti2groebner_Walk( matrix, basis, gro_start, cost_start, cost, zsol);
    return _4ti2groebner_Walk( arg );
#    return [];
end);
