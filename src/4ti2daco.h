#ifndef _4ti2GAP4ti2daco_H
#define _4ti2GAP4ti2daco_H

#include "4ti2gap.h"
#include "basicxch.h"

// If 4ti2 has gmp, disable int(64,32) in groebner
#ifdef _4ti2_WITH_GMP_
#include <gmpxx.h>
#define _4ti2_GMP_
#ifdef SYS_IS_64_BIT
#undef _4ti2_INT64_
#else
#undef _4ti2_INT32_
#endif
#endif

#include "groebner/DataType.h"
#include "groebner/Globals.h"
#include "groebner/VectorArray.h"
#include "groebner/VectorArrayStream.h"
// #include "groebner/Vector.h"

Obj _4ti2gap_VectorArray2GAP( _4ti2_::VectorArray &mat );

#ifdef _4ti2_WITH_GMP_
Obj _4ti2gap_VectorArray_mpz2GAP( _4ti2_::VectorArray &mat );
#endif

_4ti2_::VectorArray *_4ti2gap_GAPMatrix4Groebner( Obj list );
#endif
