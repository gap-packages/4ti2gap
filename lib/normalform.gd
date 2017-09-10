#! @Chapter Functions
#! @Section Normalform4ti2
#! In this section we show how to interact with <C>normalform</C> in <C>4ti2<C>


#! @Description
#! Computes the normal form of a list of feasible points.
#! Input arguments (using 4ti2 specification for minimize):
#! A matrix (optional as an empty list [], only if lattice basis is given),
#! a lattice basis (optional as an empty list [], only if matrix is given),
#! the Groebner basis of the lattice,
#! a cost matrix (optional, default is degrevlex), ties are broken with degrevlex,
#! a list of integer feasible solutions,
#! the sign constraints of variables (1 o 0 for non-negative and free
#! variable, resp., optional as an empty list [] all variables non-negative).
#! Signature: Normalform4ti2(matrix, lattice, groebner, cost, feas, sign)
DeclareGlobalFunction("Normalform4ti2");
