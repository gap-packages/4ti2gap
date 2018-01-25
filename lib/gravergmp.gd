#! @Chapter Functions
#! @Section Graver bases
#! In this section we show how to interact with <C>4ti2<C>'s <C>graver<C>


#! @Description
#! Computes a Graver basis of a matrix or a lattice
#! The input argument <A>a</A> is a list with structure: 
#! [ <type>, <list>, <type>, <list>, ... ] 
#! <type> is a text string, with possible values: 
#! "mat", "lat" to describe (see documentation at http://www.4ti2.de) 
#! the contents of each matrix, given as a GAP <list>.
#! @Returns a list with elements of the basis
#! @Arguments a
DeclareGlobalFunction("GraverBasis4ti2gmp");
