#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include "4ti2gap.h"
#include "4ti2daco.h"

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
#include "groebner/Vector.h"
#include "groebner/Feasible.h"
#include "groebner/GeneratingSet.h"
#include "groebner/GroebnerBasis.h"
#include "groebner/BasicOptions.h"

// It is assumed that listA is a list of lists with the same length.
Obj _4ti2groebner_GroebnerBasisOrder( Obj self, Obj listA, Obj listO )
{
//    _4ti2_::Globals::generation = _4ti2_::Globals::PROJECT_AND_LIFT;

    _4ti2_::VectorArray *A;

    if ( ! (A = _4ti2gap_GAPMatrix4Groebner( listA )) )
        ErrorQuit("Input data conversion failed.", 0, 0);

    std::streambuf *old = std::cout.rdbuf();
    std::stringstream ss;
    std::cout.rdbuf( ss.rdbuf() );

    _4ti2_::BasicOptions::instance()->output = _4ti2_::BasicOptions::SILENT;
    _4ti2_::Feasible feasible( 0, A );
    _4ti2_::VectorArray fbas = feasible.get_basis();
    if ( fbas.get_number() == 0 )
        ErrorQuit( "Could not compute the Groebner basis", 0, 0);

    _4ti2_::Globals::minimal = false;
    _4ti2_::GeneratingSet gs( feasible, 0 );

    _4ti2_::VectorArray binomios;
    if ( listO == 0 ) {
        _4ti2_::GroebnerBasis gb( gs, 0 );
        binomios = gb.get_groebner_basis();
    }
    else {
        _4ti2_::VectorArray *cost;
        if ( ! (cost = _4ti2gap_GAPMatrix4Groebner( listO )) ) {
            delete A;
            ErrorQuit("Input cost/order data conversion failed.", 0, 0);
        }
        _4ti2_::GroebnerBasis gb( gs, cost );
        binomios = gb.get_groebner_basis();
        delete cost;
    }

    delete A;
    std::cout.rdbuf( old );

    return _4ti2gap_VectorArray2GAP( binomios );
}

Obj _4ti2groebner_GroebnerBasis( Obj self, Obj listA )
{
    return _4ti2groebner_GroebnerBasisOrder( self, listA, 0 );
}
