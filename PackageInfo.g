SetPackageInfo( rec(

PackageName := "4ti2gap",
Subtitle := "GAP wraper for 4ti2",
Version := "0.2",
Date    := "25/01/2018", # dd/mm/yyyy format
License := "GPL-2.0-or-later",

Persons := [
  rec(
    LastName      := "García-Sánchez",
    FirstNames    := "Pedro A.",
    IsAuthor      := true,
    IsMaintainer  := true,
    Email         := "pedro@ugr.es",
    WWWHome       := "http://www.ugr.es/local/pedro",
    PostalAddress := Concatenation(
                       "Campus Fuentenueva s/n",
                       "" ),
    Place         := "Departamento de Álgebra",
    Institution   := "Universidad de Granada"
  ),
  rec(
    LastName      := "Sánchez-R.-Navarro",
    FirstNames    := "Alfredo",
    IsAuthor      := true,
    IsMaintainer  := true,
    Email         := "alfredo.sanchez@uca.es",
    WWWHome       := "",
    PostalAddress := Concatenation(
                       "Jerez de La Frontera",
                       "" ),
    Place         := "Departamento de Ingeniería Informática",
    Institution   := "Universidad de Cádiz"
  )
],

Status         := "dev",
#CommunicatedBy := "name (place)",
#AcceptDate     := "mm/yyyy",

PackageWWWHome := "https://github.com/gap-packages/4ti2gap",

#ArchiveFormats := ".tar.gz tar.bz2",
#ArchiveURL     := Concatenation("",
#                                "releases/download/v", ~.Version,
#                                "/4ti2gap-", ~.Version),
README_URL     := Concatenation( ~.PackageWWWHome, "README" ),
PackageInfoURL := Concatenation( ~.PackageWWWHome, "PackageInfo.g" ),

AbstractHTML   := Concatenation(
               "The 4ti2gap Package ... ",
               "TODO"),

PackageDoc := rec(
 BookName  := ~.PackageName,
 ArchiveURLSubset := [ "doc" ],
 HTMLStart := "doc/chap0.html",
 PDFFile   := "doc/manual.pdf",
 SixFile   := "doc/manual.six",
 LongTitle := ~.Subtitle,
),

Dependencies := rec(
  GAP                    := ">= 4.7",
  NeededOtherPackages    := [ ],
  SuggestedOtherPackages := [ ],
  ExternalConditions     := [ ],
),

AvailabilityTest := function()
    local path;
            # test for existence of the compiled binary
    path := DirectoriesPackagePrograms("4ti2gap");
    if not "4ti2gap" in SHOW_STAT() and
       Filename(path, "4ti2gap.so") = fail then
        return fail;
    fi;
    return true;
end,

Autoload := false,

Keywords := [
  "4ti2",
  "groebner",
  "minimize",
  "hilbert",
  "graver",
  "zsolve",
],

#TestFile := "tst/testall.g",

));
