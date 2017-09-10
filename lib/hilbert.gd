#! @Chapter Functions
#! @Section Hilbert bases
#! In this section we show how to interact with <C>hilbert</C> in <C>4ti2<C>


#! @Description
#! Computes a Hilbert basis of the cone or by a monoid
#! The input argument <A>a</A> is a list with structure: 
#! [ <type>, <list>, <type>, <list>, ... ] 
#! <type> is a text string, with possible values: 
#! "mat", "lat", "sign", "rel", "ub", to
#! describe (see documentation at http://www.4ti2.de) the contents of each 
#! matrix, given as a GAP <list>
#! @Returns a list with elements of the basis
#! @Arguments a
DeclareGlobalFunction("HilbertBasis4ti2");
