#! @Chapter Functions
#! @Section Minimize4ti2
#! In this section we show how to interact with <C>minimize</C> in <C>4ti2<C>


#! @Description
#! Computes the minimal solution of an integer linear program
#! or, more general, a lattice program, using a Groebner basis.
#! Input arguments (using 4ti2 specification for minimize):
#! A matrix (optional as an empty list [], only if lattice basis is given),
#! a lattice basis (optional as an empty list [], only if matrix is given),
#! a cost vector (as a matrix with 1 row or as a flat list),
#! an integer solution to specify a fiber (same format as cost vector),
#! the sign constraints of variables (1 o 0 for non-negative and free
#! variable, resp., optional as an empty list []).
#! Signature: Minimize4ti2(matrix, lattice, cost, sol, sign)
DeclareGlobalFunction("Minimize4ti2");
