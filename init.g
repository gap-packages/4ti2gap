_4ti2_lib:=Filename(DirectoriesPackagePrograms("gap4ti2"), "gap4ti2.so");
if _4ti2_lib <> fail then
    LoadDynamicModule(_4ti2_lib);
fi;
Unbind(_4ti2_lib);

ReadPackage("gap4ti2", "lib/groebner.gd");
