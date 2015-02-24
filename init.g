_4ti2_lib:=Filename(DirectoriesPackagePrograms("4ti2gap"), "4ti2gap.so");
if _4ti2_lib <> fail then
    LoadDynamicModule(_4ti2_lib);
    ReadPackage("4ti2gap", "lib/groebner.gd");
    ReadPackage("4ti2gap", "lib/hilbert.gd");
    ReadPackage("4ti2gap", "lib/graver.gd");
fi;
Unbind(_4ti2_lib);

_4ti2gmp_lib:=Filename(DirectoriesPackagePrograms("4ti2gap"), "4ti2gapgmp.so");
if _4ti2gmp_lib <> fail then
    LoadDynamicModule(_4ti2gmp_lib);
    ReadPackage("4ti2gap", "lib/groebnergmp.gd");
    ReadPackage("4ti2gap", "lib/hilbertgmp.gd");
    ReadPackage("4ti2gap", "lib/gravergmp.gd");
fi;
Unbind(_4ti2gmp_lib);

