#ifndef _4ti2GAPgroebner_H
#define _4ti2GAPgroebner_H
#include "4ti2gap.h"

// It is assumed that listA is a list of lists with the same length.
Obj _4ti2groebner_GroebnerBasisOrder( Obj self, Obj listA, Obj listO );

Obj _4ti2groebner_GroebnerBasis( Obj self, Obj listA );

#endif
