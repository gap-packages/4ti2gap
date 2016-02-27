#
# Generate the manual using AutoDoc
#
if fail = LoadPackage("AutoDoc", ">= 2016.01.21") then
    Error("AutoDoc 2016.01.21 or newer is required");
fi;

AutoDoc(rec(
    maketest := rec( commands := [ "LoadPackage(\"4ti2gap\");" ] ),
    scaffold := rec( bib := "4ti2gap-bib.xml" ),
    autodoc := rec( files := [ "doc/intro.autodoc", "src/4ti2gap.cc" ] ),
));
PrintTo("VERSION", GAPInfo.PackageInfoCurrent.Version);

QUIT;
