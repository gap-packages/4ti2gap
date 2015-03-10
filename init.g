_4ti2_lib:=Filename(DirectoriesPackagePrograms("4ti2gap"), "4ti2gap.so");
if _4ti2_lib <> fail then
    LoadDynamicModule(_4ti2_lib);
    ReadPackage("4ti2gap", "lib/groebner.gd");
    ReadPackage("4ti2gap", "lib/hilbert.gd");
    ReadPackage("4ti2gap", "lib/graver.gd");
    ReadPackage("4ti2gap", "lib/zsolve.gd");
    if IsBound(_4ti2zsolve_HilbertGMP) then
        ReadPackage("4ti2gap", "lib/hilbertgmp.gd");
        ReadPackage("4ti2gap", "lib/gravergmp.gd");
        ReadPackage("4ti2gap", "lib/zsolvegmp.gd");
    fi;
fi;
Unbind(_4ti2_lib);

