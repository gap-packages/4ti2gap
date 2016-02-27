#
# Generate the manual using AutoDoc
#
LoadPackage("AutoDoc", "2014.03.04");

SetPackagePath("4ti2gap", ".");
AutoDoc("4ti2gap" : scaffold := true,
        maketest := rec ( commands := [ "LoadPackage(\"4ti2gap\");" ] ),
        scaffold := rec( bib := "4ti2gap-bib.xml" ),
        autodoc := rec(
            files := [
                    "doc/intro.autodoc",
                    "src/4ti2gap.cc",
                    ]
            )
     );

PrintTo("VERSION", PackageInfo("4ti2gap")[1].Version);

QUIT;
