#ifndef _4ti2GAPzsolve_H
#define _4ti2GAPzsolve_H

#include "4ti2gap.h"

Obj _4ti2zsolve_Hilbert( Obj self, Obj list );

#ifdef _4ti2_WITH_GMP_
Obj _4ti2zsolve_HilbertGMP( Obj self, Obj list );
#endif

Obj _4ti2zsolve_Graver( Obj self, Obj list );

#ifdef _4ti2_WITH_GMP_
Obj _4ti2zsolve_GraverGMP( Obj self, Obj list );
#endif

Obj _4ti2zsolve_ZSolve( Obj self, Obj list );

#ifdef _4ti2_WITH_GMP_
Obj _4ti2zsolve_ZSolveGMP( Obj self, Obj list );
#endif

#endif
