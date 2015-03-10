_4ti2_lib:=Filename(DirectoriesPackagePrograms("4ti2gap"), "4ti2gap.so");
if _4ti2_lib <> fail then
    ReadPackage("4ti2gap", "lib/groebner.gi");
    ReadPackage("4ti2gap", "lib/hilbert.gi");
    ReadPackage("4ti2gap", "lib/graver.gi");
    ReadPackage("4ti2gap", "lib/zsolve.gi");
    if IsBound(_4ti2_HilbertGMP) then
        ReadPackage("4ti2gap", "lib/hilbertgmp.gi");
        ReadPackage("4ti2gap", "lib/gravergmp.gi");
        ReadPackage("4ti2gap", "lib/zsolvegmp.gi");
    fi;
fi;
Unbind(_4ti2_lib);
