#! @Chapter Functions
#! @Section Walk4ti2
#! In this section we show how to interact with <C>4ti2<C>'s <C>walk</C>


#! @Description
#! Computes the minimal solution of an integer linear program
#! or, more general, a lattice program using a Groebner basis.
#! Input arguments (using 4ti2 specification for walk):
#! A matrix (optional as an empty list [], only if lattice basis is given), 
#! a lattice basis (optional as an empty list [], only if matrix is given),
#! a starting Groebner basis (needed), the starting cost vector (optional ,
#! as an empty list [], default is degrevlex, ties are broken with degrevlex), 
#! the target cost vector (optional as an empty list []), 
#! an integer solution to specify a fiber (needed),
#! the sign constraints of variables (1 o 0 for non-negative and free
#! variable, resp., optional as an empty list [] all variables non-negative).
#! Signature: Walk4ti2(matrix, lattice, gro_start, cost_start, cost, zsol, sign)
DeclareGlobalFunction("Walk4ti2");
