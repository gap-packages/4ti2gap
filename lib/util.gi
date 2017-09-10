InstallGlobalFunction(IsValidZSolveInput, function(input)
    local leninp, i;

    leninp := Length( input );
    i := 1;
    if (leninp mod 2) = 0 then
        while i < leninp do
            if not( IsString( input[i] ) and input[i] in ["mat", "lat", "rhs", "lb", "ub", "rel", "sign"] ) then
                return false;
            elif input[i] in ["mat", "lat"] then
                if not( IsRectangularTable( input[i+1] ) ) then
                    return false;
                fi;
            elif not( IsHomogeneousList( input[i+1] ) ) then
                return false;
            fi;
            i := i + 2;
        od;
        return true;
    fi;
    return false;
end);
