#! @Chapter Normalform4ti2
#! In this section we show how to interact with <C>normalform</C> in <C>4ti2<C>


#! @Description
#! Computes the normal form of a list of feasible points.
#! Input arguments (using 4ti2 specification for minimize):
#! A matrix (optional as an empty list [], only if lattice basis is given),
#! a lattice basis (optional as an empty list [], only if matrix is given),
#! the Groebner basis of the lattice,
#! a cost matrix (optional, default is degrevlex), ties are broken with degrevlex,
#! a list of integer feasible solutions,
#! the sign constraints of variables (1 o 0 for non-negative and free
#! variable, resp., optional as an empty list [] all variables non-negative).
#! Signature: Normalform4ti2(matrix, lattice, groebner, cost, feas, sign)
InstallGlobalFunction(Normalform4ti2, function(arg)
    local matrix, basis, cost, gro, _feas, feas, sign, narg;

    narg:=Length(arg);
    if narg < 6 then
      Error("The number of arguments is not enough");
    fi;

    matrix := arg[1]; basis := arg[2];
    if not(IsRectangularTable(matrix)) and not(IsRectangularTable(basis)) then
        Error("The first and or the second argument must be a matrix");
    fi;

    gro := arg[3];
    if not(IsRectangularTable(matrix)) then
        Error("The third argument must be a list of vectors (the Groebner basis)");
    fi;

    cost := arg[4];
    if not(IsRectangularTable(cost)) then
        Error("The fourth argument should be a matrix");
    fi;

    _feas := arg[5];
    if IsRectangularTable(_feas) then
        feas := _feas;
    else
        if not(IsList(_feas)) then
            Error("The fifth argument should be a list or a matrix with one row");
        else
            feas := [_feas];
        fi;
    fi;

    sign := arg[6];

    return _4ti2groebner_Normalform( matrix, basis, gro, cost, feas, sign );
end);
