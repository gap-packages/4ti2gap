_4ti2_lib:=Filename(DirectoriesPackagePrograms("4ti2gap"), "4ti2gap.so");
if _4ti2_lib <> fail then
  Print("hola");
    ReadPackage("4ti2gap", "lib/groebner.gi");
    ReadPackage("4ti2gap", "lib/hilbert.gi");
fi;
Unbind(_4ti2_lib);

_4ti2_lib:=Filename(DirectoriesPackagePrograms("4ti2gap"), "4ti2gapgmp.so");
if _4ti2_lib <> fail then
  Print("adios");
    ReadPackage("4ti2gap", "lib/groebnergmp.gi");
    ReadPackage("4ti2gap", "lib/hilbertgmp.gi");
fi;
Unbind(_4ti2_lib);
