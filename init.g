_4ti2_lib:=Filename(DirectoriesPackagePrograms("4ti2gap"), "gap4ti2.so");
if _4ti2_lib <> fail then
    LoadDynamicModule(_4ti2_lib);
fi;
Unbind(_4ti2_lib);

ReadPackage("4ti2gap", "lib/groebner.gd");
