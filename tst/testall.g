LoadPackage("4ti2gap");
TestDirectory(DirectoriesPackageLibrary( "4ti2gap", "tst" ), rec(
    exitGAP := true,
    testOptions := rec(compareFunction := "uptowhitespace"),
));

FORCE_QUIT_GAP(1); # if we ever get here, there was an error
