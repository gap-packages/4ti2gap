#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include "4ti2gap.h"
#include "4ti2daco.h"

// #include <glpk.h>
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
#include "groebner/BasicOptions.h"
#include "groebner/LatticeBasis.h"

// Input objects should be prepared as homogeneous lists of lists (GAP matrices).
Obj _4ti2groebner_ZBasis( Obj self, Obj listM )
{
    // matrix argument
    _4ti2_::VectorArray *matrix = 0;
    if ( IS_LIST( listM ) && LEN_LIST( listM ) > 0 ) {
        if ( ! ( matrix = _4ti2gap_GAPMatrix4Groebner( listM ) ) )
            ErrorQuit("Input data conversion failed for matrix argument.", 0, 0);
    }

//     glp_term_out( GLP_OFF );
    
    std::streambuf *old = std::cout.rdbuf();
    std::stringstream ss;
    std::cout.rdbuf( ss.rdbuf() );

    _4ti2_::VectorArray zbasis( 0, matrix->get_size() );
    _4ti2_::lattice_basis( *matrix, zbasis );
 
    std::cout.rdbuf( old );

//    glp_term_out( GLP_OFF );

    if ( matrix ) delete matrix;

    return _4ti2gap_VectorArray2GAP( zbasis );
}
