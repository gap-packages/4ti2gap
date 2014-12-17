_4ti2_lib:=Filename(DirectoriesPackagePrograms("4ti2gap"), "4ti2gap.so");
if _4ti2_lib <> fail then
    LoadDynamicModule(_4ti2_lib);
fi;
Unbind(_4ti2_lib);

ReadPackage("4ti2gap", "lib/groebner.gd");
ReadPackage("4ti2gap", "lib/hilbert.gd");
