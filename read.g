_4ti2_lib:=Filename(DirectoriesPackagePrograms("4ti2gap"), "4ti2gap.so");
if _4ti2_lib <> fail then
    ReadPackage("4ti2gap", "lib/util.gi");
    ReadPackage("4ti2gap", "lib/groebner.gi");
    ReadPackage("4ti2gap", "lib/minimize.gi");
    ReadPackage("4ti2gap", "lib/normalform.gi");
    ReadPackage("4ti2gap", "lib/markov.gi");
    ReadPackage("4ti2gap", "lib/walk.gi");
    ReadPackage("4ti2gap", "lib/zbasis.gi");
    ReadPackage("4ti2gap", "lib/hilbert.gi");
    ReadPackage("4ti2gap", "lib/graver.gi");
    ReadPackage("4ti2gap", "lib/zsolve.gi");
    if IsBound(_4ti2zsolve_HilbertGMP) then
        ReadPackage("4ti2gap", "lib/hilbertgmp.gi");
        ReadPackage("4ti2gap", "lib/gravergmp.gi");
        ReadPackage("4ti2gap", "lib/zsolvegmp.gi");
    fi;
fi;
Unbind(_4ti2_lib);
