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
#include "groebner/WalkAlgorithm.h"
#include "groebner/BasicOptions.h"

// The initial processing mimics the input_Feasible code in FeasibleStream.cpp
// Input objects should be prepared as homogeneous lists of lists (GAP matrices).
Obj _4ti2groebner_Walk( Obj self, Obj listArgs ) //, Obj listL, Obj listGS, Obj listCS, 
//                        Obj listC, Obj listZS, Obj listSG)
{
    int dim;

    Obj listM = ELM_PLIST( listArgs, 1 );
    Obj listL = ELM_PLIST( listArgs, 2 );
    Obj listGS = ELM_PLIST( listArgs, 3 );
    Obj listCS = ELM_PLIST( listArgs, 4 ); 
    Obj listC = ELM_PLIST( listArgs, 5 );
    Obj listZS = ELM_PLIST( listArgs, 6 );
    Obj listSG = ELM_PLIST( listArgs, 7 );
//    std::cout << "MATRIX LIST is ";
//    PrintObj(listM); std::cout << std::endl;
//    std::cout << "LATTICE LIST is ";
//    PrintObj(listL); std::cout << std::endl;
//    std::cout << "GS LIST is ";
//    PrintObj(listGS); std::cout << std::endl;
//    std::cout << "COST START LIST is ";
//    PrintObj(listCS); std::cout << std::endl;
//    std::cout << "COST LIST is ";
//    PrintObj(listC); std::cout << std::endl;
//    std::cout << "ZSOL LIST is ";
//    PrintObj(listZS); std::cout << std::endl;
//    std::cout << "SG LIST is ";
//    PrintObj(listSG); std::cout << std::endl;

    // matrix argument
    _4ti2_::VectorArray *matrix = 0;
//    PrintObj(listM);
    if ( IS_LIST( listM ) && LEN_LIST( listM ) > 0 ) {
        if ( ! ( matrix = _4ti2gap_GAPMatrix4Groebner( listM ) ) )
            ErrorQuit("Input data conversion failed for matrix argument.", 0, 0);
        dim = matrix->get_size();
    }

    // lattice argument
    _4ti2_::VectorArray *basis = 0;
    if ( IS_LIST( listL ) && LEN_LIST( listL ) > 0 ) {
        if ( ! ( basis = _4ti2gap_GAPMatrix4Groebner( listL ) ) ) {
            if ( matrix ) delete matrix;
            ErrorQuit("Input data conversion failed for lattice argument.", 0, 0);
        }
        dim = basis->get_size();
    }

    // lattice and matrix argument checking
    if ( basis == 0 && matrix == 0 )
        ErrorQuit("Input matrix and/or lattice is needed to proceed.", 0, 0);
    if ( basis != 0 && matrix != 0  && matrix->get_size() != basis->get_size() ) {
        delete basis; delete matrix;
        ErrorQuit("Input matrix and lattice size mismatch.", 0, 0);
    }

    // gro_start argument
    _4ti2_::VectorArray* gro_start = 0;
    if ( ! ( gro_start = _4ti2gap_GAPMatrix4Groebner( listGS ) ) ) {
        if ( matrix ) delete matrix; if ( basis ) delete basis;
        ErrorQuit( "Input data conversion failed for gro_start argument.", 0, 0);
    }

    // cost_start argument
    _4ti2_::VectorArray *cost_start = 0;
    if ( IS_LIST( listCS ) && LEN_LIST( listCS ) > 0 ) {
        if ( ! ( cost_start = _4ti2gap_GAPMatrix4Groebner( listCS ) ) ) {
            if ( gro_start ) delete gro_start;  
            if ( matrix ) delete matrix; if ( basis ) delete basis;
            ErrorQuit( "Input data conversion failed for cost_start argument.", 0, 0);
        }
    }
    else 
        cost_start = new _4ti2_::VectorArray(0, dim);

    // cost argument
    _4ti2_::VectorArray *cost = 0;
    if ( ! ( cost = _4ti2gap_GAPMatrix4Groebner( listC ) ) ) {
        if ( cost_start ) delete cost_start;  
        if ( gro_start ) delete gro_start;  
        if ( matrix ) delete matrix; if ( basis ) delete basis;
        ErrorQuit( "Input data conversion failed for cost argument.", 0, 0);
    }

    // zsol argument
    _4ti2_::Vector *zsol = 0;
    _4ti2_::VectorArray* tmp_zsol;
    if ( IS_LIST( listZS ) && LEN_LIST( listZS ) > 0 ) {
        if ( ! ( tmp_zsol = _4ti2gap_GAPMatrix4Groebner( listZS ) ) ) {
            if ( cost ) delete cost; if ( cost_start ) delete cost_start;
            if ( gro_start ) delete gro_start;
            if ( matrix ) delete matrix; if ( basis ) delete basis;
            ErrorQuit( "Input data conversion failed for zsol argument.", 0, 0 );
        }
        if ( tmp_zsol->get_number() != 1 || tmp_zsol->get_size() != dim ) {
            if ( tmp_zsol ) delete tmp_zsol;
            if ( cost ) delete cost; if ( cost_start ) delete cost_start;
            if ( gro_start ) delete gro_start;
            if ( matrix ) delete matrix; if ( basis ) delete basis;
            ErrorQuit( "Input integer solution zsol should be a matrix/basis compatible sized single row matrix.", 0, 0 );
        }
        zsol = new _4ti2_::Vector( (*tmp_zsol)[0] );
        delete tmp_zsol;
    }

    // sign argument
    _4ti2_::BitSet urs( dim );
    _4ti2_::VectorArray* sign = 0;
    if ( IS_LIST( listSG ) && LEN_LIST( listSG ) > 0 ) {
        if ( ! ( sign = _4ti2gap_GAPMatrix4Groebner( listSG ) ) ) {
            if ( cost ) delete cost; if ( cost_start ) delete cost_start;
            if ( gro_start ) delete gro_start; if ( zsol ) delete zsol;
            if ( matrix ) delete matrix; if ( basis ) delete basis;
            ErrorQuit("Input data conversion failed for sign argument.", 0, 0);
        }

        if ( sign->get_number() != 1 || sign->get_size() != dim ) {
            delete sign;
            if ( cost ) delete cost; if ( cost_start ) delete cost_start;
            if ( gro_start ) delete gro_start; if ( zsol ) delete zsol;
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
                    if ( cost ) delete cost; if ( cost_start ) delete cost_start;
                    if ( gro_start ) delete gro_start; if ( zsol ) delete zsol;
                    if ( matrix ) delete matrix; if ( basis ) delete basis;
                    ErrorQuit( "Input error in sign argument, just 1 or 0 values are allowed.", 0, 0 );
                }
            }
            delete sign;
        }
    }

    glp_term_out( GLP_OFF );
    
    std::streambuf *old = std::cout.rdbuf();
    std::stringstream ss;
    std::cout.rdbuf( ss.rdbuf() );

    _4ti2_::BasicOptions::instance()->output = _4ti2_::BasicOptions::SILENT;
    _4ti2_::Feasible *feasible = new _4ti2_::Feasible( basis, matrix, &urs, zsol );
    
    _4ti2_::WalkAlgorithm algorithm;
    algorithm.compute( *feasible, *cost_start, *gro_start, 
                        *cost );

    delete feasible;

    std::cout.rdbuf( old );

    glp_term_out( GLP_ON );

    _4ti2_::VectorArray mar = *gro_start;

    if ( cost ) delete cost; if ( cost_start ) delete cost_start;
    if ( gro_start ) delete gro_start; if ( zsol ) delete zsol;
    if ( matrix ) delete matrix; if ( basis ) delete basis;

    return _4ti2gap_VectorArray2GAP( mar );
}
