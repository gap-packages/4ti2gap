#! @Chapter Functions
#! @Section ZSolve
#! In this section we show how to interact with <C>zsolve</C> in <C>4ti2<C>


#! @Description
#! Computes the solutions of a linear system over the set of the integers
#! The input argument <A>a</A> is a list with structure: 
#! [ <type>, <list>, <type>, <list>, ... ] 
#! <type> is a text string, with possible values: 
#! "mat", "lat", "sign", "rel", "ub", to
#! describe (see documentation at http://www.4ti2.de and/or the file 
#! 4ti2_manual.pdf in 4ti2 installation tree) the contents of each 
#! matrix, given as a GAP <list>
#! @Returns a record with description of all solutions, with the selectors:
#! zinhom, zhom, zfree 
#! @Arguments a
DeclareGlobalFunction("ZSolve4ti2gmp");
