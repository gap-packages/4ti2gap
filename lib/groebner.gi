#! @Chapter Gröbner bases
#! In this section we show how to interact with <C>groebner</C> in <C>4ti2<C>


#! @Description
#! Computes a Groebner basis of the Ideal associated to the monoid whose
#! generators are the columns of <A>a</A>
#! If a second argument <A>m</A> is given, then it must be a matrix specifying the
#! order to compute the Groebner basis
#! @Returns a list containing the differences of the exponents of the binomials
#! @Arguments a[,m]
InstallGlobalFunction(GroebnerBasis4ti2, function(arg)
    local a, c, narg, rows,cols,ones;

    narg:=Length(arg);
    if narg>0 then
        a:=arg[1];
        if not(IsRectangularTable(a)) then
            Error("The argument must be a matrix");
        fi;
        if not(IsInt(a[1][1])) then
            Error("The argument must be a matrix of integers");
        fi;

        if narg=1 then
            return _4ti2_GroebnerBasis(a);
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

            return _4ti2_GroebnerBasisOrder(a, c);
        fi;
    fi;
    Error("Wrong number of arguments");
end);
