#! @Chapter Functions
#! @Section MarkovBasis4ti2
#! In this section we show how to interact with <C>4ti2<C>'s <C>markov</C>

#! @Description
#! Computes a Markov basis (generating set) of the toric ideal
#! of a matrix or, more general, of the lattice ideal of a lattice.
#! Input arguments (using 4ti2 specification for markov):
#! A matrix (optional as an empty list [], only if lattice basis is given),
#! a lattice basis (optional as an empty list [], only if matrix is given),
#! the sign constraints of variables (1 o 0 for non-negative and free
#! variable, resp., optional as an empty list [] all variables non-negative), 
#! the weight vectors used for truncation (optional as an empty list []), 
#! the maximum weights used for truncation (is needed if weights is given),
#! an integer solution to specify a fiber (optional as an empty list [], 
#! if is used for truncation).
#! Signature: MarkovBasis4ti2(matrix, lattice, sign, weights, weights_max, zsol)
DeclareGlobalFunction("MarkovBasis4ti2");
