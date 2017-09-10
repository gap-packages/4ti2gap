#! @Chapter Functions
#! @Section Util
#! Here are the utility functions


#! @Description
#! Checks the the input is well formed according to 4ti2's ZSolve.
#! The input argument <A>input</A> is a list with structure: 
#! [ <type>, <list>, <type>, <list>, ... ] 
#! <type> is a text string, with possible values (but these are nor checked): 
#! "mat", "lat", "sign", "rel", "ub", to
#! describe (see documentation at http://www.4ti2.de and/or the file 
#! 4ti2_manual.pdf in 4ti2 installation tree) the contents of each 
#! matrix, given as a GAP <list>
#! @Returns true in the case that the input is well formed, false otherwise
#! @Arguments input
DeclareGlobalFunction("IsValidZSolveInput");
