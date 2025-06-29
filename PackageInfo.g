SetPackageInfo( rec(

PackageName := "4ti2gap",
Subtitle := "GAP wrapper for 4ti2",
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
    WWWHome       := "https://www.ugr.es/local/pedro",
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
 HTMLStart := "doc/chap0_mj.html",
 PDFFile   := "doc/manual.pdf",
 SixFile   := "doc/manual.six",
 LongTitle := ~.Subtitle,
),

Dependencies := rec(
  GAP                    := ">= 4.12",
  NeededOtherPackages    := [ ],
  SuggestedOtherPackages := [ ],
  ExternalConditions     := [ ],
),

AvailabilityTest := function()
  if not IsKernelExtensionAvailable("4ti2gap") then
    LogPackageLoadingMessage(PACKAGE_WARNING,
                             ["the kernel module is not compiled, ",
                              "the package cannot be loaded."]);
    return false;
  fi;
  return true;
end,


Keywords := [
  "4ti2",
  "groebner",
  "minimize",
  "hilbert",
  "graver",
  "zsolve",
],

TestFile := "tst/testall.g",

));
