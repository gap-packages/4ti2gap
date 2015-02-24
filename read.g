_4ti2_lib:=Filename(DirectoriesPackagePrograms("4ti2gap"), "4ti2gap.so");
if _4ti2_lib <> fail then
    ReadPackage("4ti2gap", "lib/groebner.gi");
    ReadPackage("4ti2gap", "lib/hilbert.gi");
    ReadPackage("4ti2gap", "lib/graver.gi");
fi;
Unbind(_4ti2_lib);

_4ti2gmp_lib:=Filename(DirectoriesPackagePrograms("4ti2gap"), "4ti2gapgmp.so");
if _4ti2gmp_lib <> fail then
    ReadPackage("4ti2gap", "lib/groebnergmp.gi");
    ReadPackage("4ti2gap", "lib/hilbertgmp.gi");
    ReadPackage("4ti2gap", "lib/gravergmp.gi");
fi;
Unbind(_4ti2gmp_lib);
