#ifndef _4ti2GAPbasicxch_H
#define _4ti2GAPbasicxch_H

#include "4ti2gap.h"

#ifdef _4ti2_WITH_GMP_
#include "gmpxx.h"

Obj MpzClassToGAP( const mpz_class& x );
#endif


#endif
