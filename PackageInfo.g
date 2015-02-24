SetPackageInfo( rec(

PackageName := "4ti2gap",
Subtitle := "GAP wraper for 4ti2",
Version := "0.0.2",
Date    := "9/1/2015", # dd/mm/yyyy format

Persons := [
  rec(
    LastName      := "",
    FirstNames    := "",
    IsAuthor      := true,
    IsMaintainer  := true,
    Email         := "@",
    WWWHome       := "",
    PostalAddress := Concatenation(
                       "\n",
                       "\n",
                       "\n",
                       "" ),
    Place         := "",
    Institution   := ""
  ),
],

Status         := "dev",
#CommunicatedBy := "name (place)",
#AcceptDate     := "mm/yyyy",

PackageWWWHome := "",

#ArchiveFormats := ".tar.gz tar.bz2",
#ArchiveURL     := Concatenation("",
#                                "releases/download/v", ~.Version,
#                                "/4ti2gap-", ~.Version),
#README_URL     := Concatenation( ~.PackageWWWHome, "README" ),
#PackageInfoURL := Concatenation( ~.PackageWWWHome, "PackageInfo.g" ),

#AbstractHTML   := Concatenation(
#               "The 4ti2gap Package ... ",
#               "TODO"),

#PackageDoc := rec(
#  BookName  := "4ti2gap",
#  ArchiveURLSubset := [ "doc" ],
#  HTMLStart := "doc/chap0_mj.html",
#  PDFFile   := "doc/manual.pdf",
#  SixFile   := "doc/manual.six",
#  LongTitle := "GAP wrapper for 4ti2",
#  Autoload  := true
#),

Dependencies := rec(
  GAP                    := ">= 4.7",
  NeededOtherPackages    := [ ],
  SuggestedOtherPackages := [ ],
  ExternalConditions     := [ ]
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
  "hilbert",
  "graver",
],

#TestFile := "tst/testall.g",

));
