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
#include "groebner/Minimize.h"
#include "groebner/GroebnerBasis.h"
#include "groebner/BasicOptions.h"

// The initial processing mimics the input_Feasible code in FeasibleStream.cpp
// Input objects should be prepared as homogeneous lists of lists (GAP matrices).
Obj _4ti2groebner_MarkovBasis( Obj self, Obj listM, Obj listL, Obj listSG, Obj listW,
                              Obj listWM, Obj listZS)
{
    // matrix argument
    _4ti2_::VectorArray *matrix = 0;
    if ( IS_LIST( listM ) && LEN_LIST( listM ) > 0 ) {
        if ( ! ( matrix = _4ti2gap_GAPMatrix4Groebner( listM ) ) )
            ErrorQuit("Input data conversion failed for matrix argument.", 0, 0);
    }

    // lattice argument
    _4ti2_::VectorArray *basis = 0;
    if ( IS_LIST( listL ) && LEN_LIST( listL ) > 0 ) {
        if ( ! ( basis = _4ti2gap_GAPMatrix4Groebner( listL ) ) ) {
            if ( matrix ) delete matrix;
            ErrorQuit("Input data conversion failed for lattice argument.", 0, 0);
        }
    }

    // lattice and matrix argument checking
    if ( basis == 0 && matrix == 0 )
        ErrorQuit("Input matrix and/or lattice is needed to proceed.", 0, 0);
    if ( basis != 0 && matrix != 0  && matrix->get_size() != basis->get_size() ) {
        delete basis; delete matrix;
        ErrorQuit("Input matrix and lattice size mismatch.", 0, 0);
    }

    // sign argument
    int dim = ( matrix != 0 ? matrix->get_size() : basis->get_size() );
    _4ti2_::BitSet urs( dim );
    _4ti2_::VectorArray* sign = 0;
    if ( IS_LIST( listSG ) && LEN_LIST( listSG ) > 0 ) {
        if ( ! ( sign = _4ti2gap_GAPMatrix4Groebner( listSG ) ) ) {
            if ( matrix ) delete matrix; if ( basis ) delete basis;
            ErrorQuit("Input data conversion failed for sign argument.", 0, 0);
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
                    ErrorQuit( "Input error in sign argument, just 1 or 0 values are allowed.", 0, 0 );
                }
            }
            delete sign;
        }
    }

    // weights argument
    _4ti2_::VectorArray* weights = 0;
    if ( IS_LIST( listW ) && LEN_LIST( listW ) > 0 ) {
        if ( ! ( weights = _4ti2gap_GAPMatrix4Groebner( listW ) ) ) {
            if ( matrix ) delete matrix; if ( basis ) delete basis;
            ErrorQuit( "Input data conversion failed for weights argument.", 0, 0);
        }
    }

    // weights_max argument
    _4ti2_::Vector *weights_max = 0;
    _4ti2_::VectorArray* tmp_weights_max;
    if ( IS_LIST( listWM ) && LEN_LIST( listWM ) > 0 ) {
        if ( ! ( tmp_weights_max = _4ti2gap_GAPMatrix4Groebner( listWM ) ) ) {
            if ( weights ) delete weights;  
            if ( matrix ) delete matrix; if ( basis ) delete basis;
            ErrorQuit( "Input data conversion failed for weights_max argument.", 0, 0);
        }
        if ( tmp_weights_max->get_number() != 1 || tmp_weights_max->get_size() != dim ) {
            if ( tmp_weights_max ) delete tmp_weights_max;
            if ( weights ) delete weights; if ( weights_max ) delete weights_max;
            if ( matrix ) delete matrix; if ( basis ) delete basis;
            ErrorQuit( "weights_max should be a matrix/basis compatible sized single row matrix.", 0, 0 );
        }
        weights_max = new _4ti2_::Vector( (*tmp_weights_max)[0] );
        delete tmp_weights_max;
    }

    // weights and weights_max argument checking
    if ( (weights != 0 && weights_max == 0) ||
         (weights == 0 && weights_max != 0) ) {
        if ( weights ) delete weights;
        if ( weights_max ) delete weights_max;
        if ( matrix ) delete matrix; if ( basis ) delete basis;
        ErrorQuit( "weights_max and weights are both required or none should be given.", 0, 0);
    }

    // zsol argument
    _4ti2_::Vector *zsol = 0;
    _4ti2_::VectorArray* tmp_zsol;
    if ( IS_LIST( listZS ) && LEN_LIST( listZS ) > 0 ) {
        if ( ! ( tmp_zsol = _4ti2gap_GAPMatrix4Groebner( listZS ) ) ) {
            if ( weights ) delete weights;
            if ( weights_max ) delete weights_max;
            if ( matrix ) delete matrix; if ( basis ) delete basis;
            ErrorQuit( "Input data conversion failed for zsol argument.", 0, 0 );
        }
        if ( tmp_zsol->get_number() != 1 || tmp_zsol->get_size() != dim ) {
            if ( tmp_zsol ) delete tmp_zsol;
            if ( weights ) delete weights; if ( weights_max ) delete weights_max;
            if ( matrix ) delete matrix; if ( basis ) delete basis;
            ErrorQuit( "Input integer solution zsol should be a matrix/basis compatible sized single row matrix.", 0, 0 );
        }
        zsol = new _4ti2_::Vector( (*tmp_zsol)[0] );
        delete tmp_zsol;
    }
   
    glp_term_out( GLP_OFF );
    
    std::streambuf *old = std::cout.rdbuf();
    std::stringstream ss;
    std::cout.rdbuf( ss.rdbuf() );

    _4ti2_::BasicOptions::instance()->output = _4ti2_::BasicOptions::SILENT;
    _4ti2_::Feasible *feasible = new _4ti2_::Feasible( basis, matrix, &urs, zsol, weights, weights_max );
    
    _4ti2_::GeneratingSet gs( *feasible, 0 );
    gs.standardise();

    _4ti2_::VectorArray mar = gs.get_generating_set();
 
    std::cout.rdbuf( old );

    glp_term_out( GLP_ON );

    if ( zsol ) delete zsol;
    if ( weights ) delete weights; if ( weights_max ) delete weights_max;
    if ( matrix ) delete matrix; if ( basis ) delete basis;

    return _4ti2gap_VectorArray2GAP( mar );
}
