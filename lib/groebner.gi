InstallGlobalFunction(GroebnerBasis4ti2, function(arg)
    local a, c, narg, rows, cols, ones;

    narg:=Length(arg);
    if narg>0 then
        a:=arg[1];
        if not(IsRectangularTable(a)) then
            Error("The argument must be a matrix");
        fi;
        if not(IsInt(a[1][1])) then
            Error("The argument must be a matrix of integers");
        fi;

#!        if First(Flat(a),n->GMP_REDUCE(n)<>fail) <> fail then
#!            Error("The size of the input suggest to use the GMP version of this function");
#!        fi;

        if narg=1 then
            return _4ti2groebner_GroebnerBasis(a);
        fi;

        cols:=Length(a[1]);

        if narg=2 then
            c:=arg[2];
            if c="lex" then
                c:=IdentityMat(cols);
            fi;
            if c="grlex" then
                c:=IdentityMat(cols);
                ones:=List([1..cols],_->1);
                c:=Concatenation([ones],c){[1..cols]};
            fi;

            if c="grevlex" then
                c:=List(IdentityMat(cols),c->-Reversed(c));
                ones:=List([1..cols],_->1);
                c:=Concatenation([ones],c){[1..cols]};
            fi;
            if not(IsMatrix(c)) then
                Error("The second argument must be one of the following: a matrix of integers, or the strings lex, grlex or grevlex");
            fi;
            if not(IsInt(c[1][1])) then
                Error("The second argument must be one of the following: a matrix of integers, or the strings lex, grlex or grevlex");
            fi;
            rows:=Length(c);
            if RankMat(c)<>rows then
                Error("The matrix representing the ordering must have full rank.");
            fi;

            return _4ti2groebner_GroebnerBasisOrder(a, c);
        fi;
    fi;
    Error("Wrong number of arguments");
end);
