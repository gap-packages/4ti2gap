_4ti2_lib:=Filename(DirectoriesPackagePrograms("4ti2gap"), "4ti2gap.so");
if _4ti2_lib <> fail then
    Print("buenas");
    LoadDynamicModule(_4ti2_lib);
    ReadPackage("4ti2gap", "lib/groebner.gd");
    ReadPackage("4ti2gap", "lib/hilbert.gd");
fi;
Unbind(_4ti2_lib);

_4ti2_lib:=Filename(DirectoriesPackagePrograms("4ti2gap"), "4ti2gapgmp.so");
if _4ti2_lib <> fail then
    Print("noches");
    LoadDynamicModule(_4ti2_lib);
    ReadPackage("4ti2gap", "lib/groebnergmp.gd");
    ReadPackage("4ti2gap", "lib/hilbertgmp.gd");
fi;
Unbind(_4ti2_lib);

