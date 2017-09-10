#! @Chapter Functions
#! @Section Gröbner bases
#! In this section we show how to interact with <C>groebner</C> in <C>4ti2<C>


#! @Description
#! Computes a Groebner basis of the Ideal associated to the monoid whose
#! generators are the columns of <A>a</A>
#! If a second argument <A>m</A> is given, then it must be a matrix specifying the
#! order to compute the Groebner basis
#! @Returns a list containing the differences of the exponents of the binomials
#! @Arguments a[,m]
DeclareGlobalFunction("GroebnerBasis4ti2");
