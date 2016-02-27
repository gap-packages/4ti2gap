#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include "4ti2gap.h"
#include "4ti2daco.h"

#include <glpk.h>
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
#include "groebner/BitSet.h"
#include "groebner/Feasible.h"
#include "groebner/MinimizeOptions.h"
#include "groebner/Optimise.h"
#include "groebner/GroebnerBasis.h"
#include "groebner/BasicOptions.h"

// The initial processing mimics the input_Feasible code in FeasibleStream.cpp
// Input objects should be prepared as homogeneous lists of lists (GAP matrices).
Obj _4ti2groebner_Minimize( Obj self, Obj listA, Obj listL, Obj listC, Obj listZS, Obj listSG)
{
//    _4ti2_::Globals::generation = _4ti2_::Globals::PROJECT_AND_LIFT;

    _4ti2_::VectorArray *matrix = 0;

    if ( IS_LIST( listA ) && LEN_LIST( listA ) > 0 ) {
        if ( ! ( matrix = _4ti2gap_GAPMatrix4Groebner( listA ) ) )
            ErrorQuit("Input data conversion failed for matrix.", 0, 0);
    }

    _4ti2_::VectorArray *basis = 0;

    if ( IS_LIST( listL ) && LEN_LIST( listL ) > 0 ) {
        if ( ! ( basis = _4ti2gap_GAPMatrix4Groebner( listL ) ) ) {
            if ( matrix ) delete matrix;
            ErrorQuit("Input data conversion failed for lattice.", 0, 0);
        }
    }

    if ( basis == 0 && matrix == 0 )
        ErrorQuit("Input matrix and/or lattice is needed to proceed.", 0, 0);
    if ( basis != 0 && matrix != 0  && matrix->get_size() != basis->get_size() ) {
        delete basis; delete matrix;
        ErrorQuit("Input matrix and lattice size mismatch.", 0, 0);
    }

    int dim = ( matrix != 0 ? matrix->get_size() : basis->get_size() );
    _4ti2_::BitSet urs( dim );
    _4ti2_::VectorArray* sign = 0;
    if ( IS_LIST( listSG ) && LEN_LIST( listSG ) > 0 ) {
        if ( ! ( sign = _4ti2gap_GAPMatrix4Groebner( listSG ) ) ) {
            if ( matrix ) delete matrix; if ( basis ) delete basis;
            ErrorQuit("Input data conversion failed for sign.", 0, 0);
        }

        if ( sign->get_number() != 1 || sign->get_size() != dim ) {
            delete sign;
            if ( matrix ) delete matrix; if ( basis ) delete basis;
            ErrorQuit( "Input sign data should be a matrix/basis compatible sized single row matrix.", 0, 0 );
        }
        else {
            for (int i = 0; i < dim; ++i) {
                IntegerType value = (*sign)[0][i];
                if (value == 0) { urs.set(i); }
                else if (value == 1) { } // Nonnegative variable.
                else {
                    delete sign;
                    if ( matrix ) delete matrix; if ( basis ) delete basis;
                    ErrorQuit( "Input error in vector, just 1 or 0 values are allowed.", 0, 0 );
                }
            }
            delete sign;
        }
    }

    _4ti2_::VectorArray* tmp_rhs;
    if ( ! ( tmp_rhs = _4ti2gap_GAPMatrix4Groebner( listZS ) ) ) {
       if ( matrix ) delete matrix; if ( basis ) delete basis;
       ErrorQuit( "Input error, could not find a fiber file.", 0, 0 );
    }
    if ( tmp_rhs->get_number() != 1 || tmp_rhs->get_size() != dim ) {
        if ( matrix ) delete matrix; if ( basis ) delete basis;
        ErrorQuit( "Input integer solution should be a matrix/basis compatible sized single row matrix.", 0, 0 );
    }
    _4ti2_::Vector rhs( (*tmp_rhs)[0] );
    delete tmp_rhs;

    std::streambuf *old = std::cout.rdbuf();
    std::stringstream ss;
    std::cout.rdbuf( ss.rdbuf() );

    _4ti2_::BasicOptions::instance()->output = _4ti2_::BasicOptions::SILENT;
    _4ti2_::Feasible feasible( basis, matrix, &urs, &rhs );

    _4ti2_::VectorArray* cost;
    if ( ! ( cost = _4ti2gap_GAPMatrix4Groebner( listC ) ) ) {
        if ( matrix ) delete matrix; if ( basis ) delete basis;
        std::cout.rdbuf( old );
        ErrorQuit("Input data conversion failed for cost.", 0, 0);
    }
    if ( cost->get_number() != 1 || cost->get_size() != dim ) {
        delete cost;
        if ( matrix ) delete matrix; if ( basis ) delete basis;
        std::cout.rdbuf( old );
        ErrorQuit("Input data cost should be a single cost function, and its size should be matrix/basis compatible.", 0, 0);
    }

    _4ti2_::VectorArray sol( 1, rhs.get_size() );
    sol[0] = *(feasible.get_rhs());
    _4ti2_::Optimise opt;
    glp_term_out( GLP_OFF );
    opt.compute( feasible, (*cost)[0], sol[0] );
    glp_term_out( GLP_ON );
    std::cout.rdbuf( old );

    delete cost;
    if ( matrix ) delete matrix;
    if ( basis ) delete basis;

    return _4ti2gap_VectorArray2GAP( sol );
}
