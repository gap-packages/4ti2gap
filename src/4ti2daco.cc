#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

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
#include "groebner/Vector.h"

#ifdef _4ti2_WITH_GMP_
// From NormalizInterface, with a minor change
bool _4ti2gap_GAPInt2IntegerType( Obj x, mpz_class& oval )
{
    if (IS_INTOBJ(x)) {
        oval = (int) INT_INTOBJ(x);
        return true;
    } else if (TNUM_OBJ(x) == T_INTPOS || TNUM_OBJ(x) == T_INTNEG) {
        mpz_ptr m = oval.get_mpz_t();
        UInt size = SIZE_INT(x);
        mpz_realloc2(m, size * GMP_NUMB_BITS);
        memcpy(m->_mp_d, ADDR_INT(x), sizeof(mp_limb_t) * size);
        m->_mp_size = (TNUM_OBJ(x) == T_INTPOS) ? (Int)size : - (Int)size;
        return true;
    }
    return false;
}
#elif defined(_4ti2_INT64_) || defined(_4ti2_INT32_)
// From NormalizInterface
bool _4ti2gap_GAPInt2IntegerType( Obj x, IntegerType &out )
{
    if (IS_INTOBJ(x)) {
        out = (int) INT_INTOBJ(x);
        return true;
    } else if (TNUM_OBJ(x) == T_INTPOS || TNUM_OBJ(x) == T_INTNEG) {
        UInt size = SIZE_INT(x);
        if (size == 1) {
            out = *ADDR_INT(x);
            if (out < 0)
                return false;   // overflow
            if (TNUM_OBJ(x) == T_INTNEG)
                out = -out;
            return true;
        }
    }
    return false;
}
#endif

Obj _4ti2gap_VectorArray2GAP( _4ti2_::VectorArray &mat )
{
    size_t numRows, numCols;

    Obj matGAP;

    numRows = mat.get_number();
    numCols = mat.get_size();

    matGAP = NEW_PLIST( T_PLIST, numRows );
    SET_LEN_PLIST( matGAP, numRows );
    for ( size_t i = 0; i < numRows; i++ ) {
        Obj lisGAP = NEW_PLIST( T_PLIST, numCols );
    	SET_LEN_PLIST( lisGAP, numCols );
        for ( size_t j = 0; j < numCols; j++ ) {
#ifdef _4ti2_WITH_GMP_
            SET_ELM_PLIST( lisGAP, j+1, MpzClassToGAP( mat[i][j] ) );
#else
            SET_ELM_PLIST( lisGAP, j+1, ObjInt_UInt( mat[i][j] ) );
#endif
            CHANGED_BAG( lisGAP );
        }
        SET_ELM_PLIST( matGAP, i+1, lisGAP );
        CHANGED_BAG( matGAP );
    }
    return matGAP;
}

#ifdef _4ti2_WITH_GMP_
Obj _4ti2gap_VectorArray_mpz2GAP( _4ti2_::VectorArray &mat )
{
    size_t numRows, numCols;

    Obj matGAP;

    numRows = mat.get_number();
    numCols = mat.get_size();

    matGAP = NEW_PLIST( T_PLIST, numRows );
    SET_LEN_PLIST( matGAP, numRows );
    for ( size_t i = 0; i < numRows; i++ ) {
        Obj lisGAP = NEW_PLIST( T_PLIST, numCols );
    	SET_LEN_PLIST( lisGAP, numCols );
        for ( size_t j = 0; j < numCols; j++ ) {
            SET_ELM_PLIST( lisGAP, j+1, MpzClassToGAP( mat[i][j] ) );
            CHANGED_BAG( lisGAP );
        }
        SET_ELM_PLIST( matGAP, i+1, lisGAP );
        CHANGED_BAG( matGAP );
    }
    return matGAP;
}
#endif


bool _4ti2gap_GAPMatrix2StdString( Obj listA, std::string& thst )
{
    size_t numRows = LEN_PLIST( listA );
    size_t numCols;
    std::ostringstream buildst;
    IntegerType av;
    Obj rowlist = ELM_PLIST( listA, 1 );
    if ( IS_PLIST( rowlist ) ) {
        numCols = LEN_PLIST( ELM_PLIST( listA, 1 ) );
        buildst << numRows << " " << numCols << "\n";

        for( size_t i = 1; i <= numRows; i++ ) {
            rowlist = ELM_PLIST( listA, i );

            size_t nele = LEN_PLIST( rowlist );
            if ( nele != numCols ) {
                std::cerr << "The number of elements must be the same for every row.\n";
                return false;
            }

            if ( !IS_LIST( rowlist ) )
                return false; //ErrorQuit( "A row was expected", 0, 0 );
            for( size_t j = 1; j <= numCols; j++ ) {
                Obj tmp =  ELM_PLIST( rowlist, j );
                if ( IS_STRING( tmp ) ) {
                    if ( GET_LEN_STRING( tmp ) > 1 ) {
                        std::cerr << "Invalid entry text " << CSTR_STRING( tmp ) << " present in input matrix." << std::endl;
                        return false;
                    }
                    buildst << CSTR_STRING( tmp ) << " ";
                }
                else {
                    if ( !_4ti2gap_GAPInt2IntegerType( tmp, av ) )
                        return false; //ErrorQuit( "Integer conversion error", 0, 0 );
                    buildst << av << " ";
                }
            }
            buildst << "\n";
        }
    }
    else {
        numCols = numRows;
        numRows = 1;
        buildst << numRows << " " << numCols << "\n";
        for( size_t i = 1; i <= numCols; i++ ) {
            Obj tmp =  ELM_PLIST( listA, i );
            if ( IS_STRING( tmp ) ) {
                if ( GET_LEN_STRING( tmp ) > 1 ) {
                    std::cerr << "Invalid entry text " << CSTR_STRING( tmp ) << " present in input vector." << std::endl;
                    return false;
                }
                buildst << CSTR_STRING( tmp ) << " ";
            }
            else {
                if ( !_4ti2gap_GAPInt2IntegerType( tmp, av ) )
                    return false; //ErrorQuit( "Integer conversion error", 0, 0 );
                buildst << av << " ";
            }
        }
        buildst << "\n";
    }
    thst = buildst.str();
    return true;
}

_4ti2_::VectorArray *_4ti2gap_GAPMatrix4Groebner( Obj list )
{
    std::string st;
    if ( !_4ti2gap_GAPMatrix2StdString( list, st ) ) {
        std::cerr << "Unable to proccess input data." << std::endl;
        return 0;
    }

    std::istringstream ists( st.c_str() );
    int m, n;
    ists >> m >> n;
    _4ti2_::VectorArray* vs_ptr = new _4ti2_::VectorArray( m, n );

    ists >> *vs_ptr;
    return vs_ptr;
}
