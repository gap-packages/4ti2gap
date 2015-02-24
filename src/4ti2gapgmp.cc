#include <iostream>
#include <string>
#include <sstream>
#include <new>
// #include <cstring>

#include "4ti2gapgmp.h"

#include "groebner/Globals.h"
#include "groebner/VectorArray.h"
#include "groebner/Vector.h"
#include "groebner/Feasible.h"
#include "groebner/GeneratingSet.h"
#include "groebner/GroebnerBasis.h"
#include "groebner/DataType.h"
#include "groebner/BasicOptions.h"

#include "zsolve/Exception.h"
#include "zsolve/VectorArrayAPI.hpp"
#include "zsolve/HilbertAPI.hpp"
#include "zsolve/GraverAPI.hpp"

bool GAPInt2IntegerTypeMpz( Obj ival, IntegerType& oval )
{
    if ( IS_INTOBJ( ival ) ) {
        oval = mpz_class( (int) INT_INTOBJ (ival ) );
        return true;
    } 
    else if ( TNUM_OBJ( ival ) == T_INTPOS || TNUM_OBJ( ival ) == T_INTNEG) {
        mpz_ptr m = oval.get_mpz_t();
        UInt size = SIZE_INT( ival );
        mpz_realloc2(m, size * GMP_NUMB_BITS);
        memcpy(m->_mp_d, ADDR_INT( ival ), sizeof(mp_limb_t) * size);
        m->_mp_size = (TNUM_OBJ( ival ) == T_INTPOS) ? (Int)size : - (Int)size;
        return true;
    }
    return false;
}

static bool GAPInt2StdString( Obj ival, std::string& oval )
{
    if ( IS_INTOBJ( ival ) )
        oval = std::to_string( (int) INT_INTOBJ (ival ) );
    else {
        IntegerType aux;
        if ( GAPInt2IntegerTypeMpz( ival, aux ) )
            oval = aux.get_str();
        else
            return false;
    }
    return true;
//     } else if ( TNUM_OBJ( ival ) == T_INTPOS || TNUM_OBJ( ival ) == T_INTNEG) {
//         mpz_class aux;
//         mpz_ptr m = aux.get_mpz_t();
//         UInt size = SIZE_INT( ival );
//         mpz_realloc2(m, size * GMP_NUMB_BITS);
//         memcpy(m->_mp_d, ADDR_INT( ival ), sizeof(mp_limb_t) * size);
//         m->_mp_size = (TNUM_OBJ( ival ) == T_INTPOS) ? (Int)size : - (Int)size;
//         oval = aux.get_str();
//         return true;
//     }
//     return false;
}

void GAPMatrix2VectorArrayMpz( Obj listA, _4ti2_::VectorArray &A )
{
    for( size_t i = 0; i < A.get_number(); i++ ) {
        Obj rowlist = ELM_PLIST( listA, i+1 );
	if ( !IS_LIST( rowlist ) )
            ErrorQuit( "A row was expected", 0, 0 );
        for( size_t j = 0; j < A.get_size(); j++ ) {
            Obj tmp =  ELM_PLIST( rowlist, j+1 );
            if ( !GAPInt2IntegerTypeMpz( tmp, A[i][j] ) ) {
                ErrorQuit( "Integer conversion error", 0, 0 );
            }
        }
    }
}

bool GAPMatrixMpz2StdString( Obj listA, std::string& thst )
{
    size_t numRows = LEN_PLIST( listA );
    size_t numCols;
    std::ostringstream buildst;
    std::string av;
    Obj rowlist = ELM_PLIST( listA, 1 );
    if ( IS_PLIST( rowlist ) ) {
        numCols = LEN_PLIST( ELM_PLIST( listA, 1 ) );
        buildst << numRows << " " << numCols << "\n";
        for( size_t i = 1; i <= numRows; i++ ) {
            rowlist = ELM_PLIST( listA, i );
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
                    if ( !GAPInt2StdString( tmp, av ) )
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
                if ( !GAPInt2StdString( tmp, av ) )
                    return false; //ErrorQuit( "Integer conversion error", 0, 0 );
                buildst << av << " ";
            }
        } 
        buildst << "\n";
    }
    thst = buildst.str();
    return true;
}

Obj Mpz2GAP( const mpz_t x )
{
    Obj res;
    int size = x->_mp_size;
    int sign;
    if (size == 0) {
        return INTOBJ_INT(0);
    } else if (size < 0) {
        size = -size;
        sign = -1;
    } else {
        sign = +1;
    }
#ifdef SYS_IS_64_BIT
    if (size == 1) {
        if (sign > 0)
            return ObjInt_UInt(x->_mp_d[0]);
        else
            return AInvInt(ObjInt_UInt(x->_mp_d[0]));
    }
#endif
    size = sizeof(mp_limb_t) * size;
    if (sign > 0)
        res = NewBag(T_INTPOS, size);
    else
        res = NewBag(T_INTNEG, size);
    memcpy(ADDR_INT(res), x->_mp_d, size);
    return res;
}

inline Obj IntegerTypeMpz2GAP( const IntegerType& x )
{
    return Mpz2GAP( x.get_mpz_t() );
}

// It is assumed that listA is a list of lists with the same length.
Obj _4ti2_GroebnerBasisOrderGMP( Obj self, Obj listA, Obj listO )
{
    size_t numRows = LEN_PLIST( listA );
    const size_t numCols = LEN_PLIST( ELM_PLIST( listA, 1 ) );

    _4ti2_::VectorArray A( numRows, numCols );

    GAPMatrix2VectorArrayMpz( listA, A );

    std::streambuf *old = std::cout.rdbuf();
    std::stringstream ss;
    std::cout.rdbuf( ss.rdbuf() );

    _4ti2_::BasicOptions::instance()->output = _4ti2_::BasicOptions::SILENT;
    _4ti2_::Feasible feasible( 0, &A );
    _4ti2_::VectorArray fbas = feasible.get_basis(); 
    if ( fbas.get_number() == 0 )
        ErrorQuit( "Could not compute the Groebner basis", 0, 0);

    _4ti2_::Globals::minimal = true;
    _4ti2_::GeneratingSet gs( feasible, 0 );

    _4ti2_::VectorArray binomios;
    if ( listO == 0 ) {
        _4ti2_::GroebnerBasis gb( gs, 0 );
        binomios = gb.get_groebner_basis();
    }
    else {
        size_t numRowsO = LEN_PLIST( listO );
        const size_t numColsO = LEN_PLIST( ELM_PLIST( listO, 1 ) );

        _4ti2_::VectorArray cost( numRowsO, numColsO );
        GAPMatrix2VectorArrayMpz( listO, cost );
        _4ti2_::GroebnerBasis gb( gs, &cost );
        binomios = gb.get_groebner_basis();
    }

    std::cout.rdbuf( old );
    
    Obj binListGAP;

    numRows = binomios.get_number();
    binListGAP = NEW_PLIST( T_PLIST, numRows );
    SET_LEN_PLIST( binListGAP, numRows );
    for ( size_t i = 0; i < numRows; i++ ) {
        Obj binGAP = NEW_PLIST( T_PLIST, numCols );
    	SET_LEN_PLIST( binGAP, numCols );
        for ( size_t j = 0; j < numCols; j++ ) {
            SET_ELM_PLIST( binGAP, j+1, IntegerTypeMpz2GAP( binomios[i][j] ) );
            CHANGED_BAG( binGAP );
        }
        SET_ELM_PLIST( binListGAP, i+1, binGAP );
        CHANGED_BAG( binListGAP );
    }
    return binListGAP;
}

Obj _4ti2_GroebnerBasisGMP( Obj self, Obj listA )
{
    return _4ti2_GroebnerBasisOrderGMP( self, listA, 0 );
}

Obj _4ti2_HilbertGMP( Obj self, Obj list )
{
    // This code is similar to NormalizInterface's input processing
    int n = LEN_PLIST( list );

    if ( n & 1 ) {
        std::cerr << "Input list must have even number of elements.\n" << std::endl;
        return Fail;
    }
    
    std::string st;
    _4ti2_zsolve_::HilbertAPI<mpz_class> problem;

    for (int i = 0; i < n; i += 2) {
        Obj type = ELM_PLIST( list, i+1 );
        if ( ! IS_STRING( type ) ) {
            std::cerr << "Element " << i+1 << " of the input list must be a type string" << std::endl;
            return Fail;
        }
        std::string stinty( CSTR_STRING(type) );

        Obj inel = ELM_PLIST( list, i+2 );
        if ( !IS_LIST( inel ) ) {
            std::cerr << "Element " << i+2 << " must be a vector or a matrix.\n";
            return Fail;
        }
        if ( ! GAPMatrixMpz2StdString( inel, st ) ) {
            std::cerr << "Unable to proccess element " << i+2 << " of the input list" << std::endl;
            return Fail;
        }
        std::istringstream ists( st );
        // ists.str( st );
        
        if ( ! problem.create_matrix( ists, stinty.c_str() ) ) {
            std::cerr << "4ti2 can not create the input matrix: " << stinty.c_str() << std::endl;
            std::cerr << st;
            return Fail;
        }
    }

        char arg1[]="-q";
        char arg2[]="-p=gmp";
        char **argopt;
	argopt = new char* [3];
        argopt[1] = arg1;
        argopt[2] = arg2;
        problem.set_options( 3, argopt );
std::cout << argopt[1] << " --- y --- " << argopt[2] << std::endl;
        delete[] argopt;
    try {
        problem.compute();
    }
    catch ( _4ti2_zsolve_::PrecisionException e )
    {
        std::cerr << "Results were near maximum precision (" << e.precision () << "bit).\n";
        std::cerr << "Please restart with higher precision!" << std::endl;
        std::cerr << "...., but wait, this is not available as an option yet. We are wonking on it..." << std::endl;
        return Fail;
    }
    catch ( _4ti2_zsolve_::IOException e )
    {
        std::cerr << e;
        return Fail;
    }

    _4ti2_zsolve_::VectorArrayAPI<mpz_class> *hilbas = (_4ti2_zsolve_::VectorArrayAPI<mpz_class> *) problem.get_matrix( "zhom" );
    Obj hilListGAP;
    size_t numRows = hilbas->get_num_rows();
    size_t numCols = hilbas->get_num_cols();

    hilListGAP = NEW_PLIST( T_PLIST, numRows );
    SET_LEN_PLIST( hilListGAP, numRows );
    for ( size_t i = 0; i < numRows; i++ ) {
        Obj lisGAP = NEW_PLIST( T_PLIST, numCols );
    	SET_LEN_PLIST( lisGAP, numCols );
        for ( size_t j = 0; j < numCols; j++ ) {
            SET_ELM_PLIST( lisGAP, j+1, IntegerTypeMpz2GAP( hilbas->data[i][j] ) );
            CHANGED_BAG( lisGAP );
        }
        SET_ELM_PLIST( hilListGAP, i+1, lisGAP );
        CHANGED_BAG( hilListGAP );
    }
    return hilListGAP;
}

Obj _4ti2_GraverGMP( Obj self, Obj list )
{
    // This code is similar to NormalizInterface's input processing
    int n = LEN_PLIST( list );

    if ( n & 1 ) {
        std::cerr << "Input list must have even number of elements.\n" << std::endl;
        return Fail;
    }

    
    std::string st;
    _4ti2_zsolve_::GraverAPI<mpz_class> problem;

    for (int i = 0; i < n; i += 2) {
        Obj type = ELM_PLIST( list, i+1 );
        if ( ! IS_STRING( type ) ) {
            std::cerr << "Element " << i+1 << " of the input list must be a type string" << std::endl;
            return Fail;
        }
        std::string stinty( CSTR_STRING(type) );

        Obj inel = ELM_PLIST( list, i+2 );
        if ( !IS_LIST( inel ) ) {
            std::cerr << "Element " << i+2 << " must be a vector or a matrix.\n";
            return Fail;
        }
        if ( ! GAPMatrixMpz2StdString( inel, st ) ) {
            std::cerr << "Unable to proccess element " << i+2 << " of the input list" << std::endl;
            return Fail;
        }
        std::istringstream ists( st );
        // ists.str( st );
        
        if ( ! problem.create_matrix( ists, stinty.c_str() ) ) {
            std::cerr << "4ti2 can not create the input matrix: " << stinty.c_str() << std::endl;
            std::cerr << st;
            return Fail;
        }
    }
        char arg1[]="-q";
        char arg2[]="-p=gmp";
        char **argopt;
	argopt = new char* [3];
        argopt[1] = arg1;
        argopt[2] = arg2;
        problem.set_options( 3, argopt );
        delete[] argopt;

    try {
        problem.compute();
    }
    catch ( _4ti2_zsolve_::PrecisionException e )
    {
        std::cerr << "Results were near maximum precision (" << e.precision () << "bit).\n";
        std::cerr << "Please restart with higher precision!" << std::endl;
        std::cerr << "...., but wait, this is not available as an option yet. We are wonking on it..." << std::endl;
        return Fail;
    }
    catch ( _4ti2_zsolve_::IOException e )
    {
        std::cerr << e;
        return Fail;
    }

    _4ti2_zsolve_::VectorArrayAPI<mpz_class> *grabas =  (_4ti2_zsolve_::VectorArrayAPI<mpz_class> *) problem.get_matrix( "gra" );
    Obj graListGAP;
    size_t numRows = grabas->get_num_rows();
    size_t numCols = grabas->get_num_cols();

    graListGAP = NEW_PLIST( T_PLIST, numRows );
    SET_LEN_PLIST( graListGAP, numRows );
    for ( size_t i = 0; i < numRows; i++ ) {
        Obj lisGAP = NEW_PLIST( T_PLIST, numCols );
    	SET_LEN_PLIST( lisGAP, numCols );
        for ( size_t j = 0; j < numCols; j++ ) {
            SET_ELM_PLIST( lisGAP, j+1, IntegerTypeMpz2GAP( grabas->data[i][j] ) );
            CHANGED_BAG( lisGAP );
        }
        SET_ELM_PLIST( graListGAP, i+1, lisGAP );
        CHANGED_BAG( graListGAP );
    }
    return graListGAP;
}

typedef Obj (* GVarFunc)(/*arguments*/);

#define GVAR_FUNC_TABLE_ENTRY(srcfile, name, nparam, params) \
  {#name, nparam, \
   params, \
   (GVarFunc)name, \
   srcfile ":Func" #name }

// Table of functions to export
static StructGVarFunc GVarFuncsgmp[] = {
    GVAR_FUNC_TABLE_ENTRY("4ti2gapgmp.cc", _4ti2_GroebnerBasisOrderGMP, 2, "list, list"),
    GVAR_FUNC_TABLE_ENTRY("4ti2gapgmp.cc", _4ti2_GroebnerBasisGMP, 1, "list"),
    GVAR_FUNC_TABLE_ENTRY("4ti2gapgmp.cc", _4ti2_HilbertGMP, 1, "list"),
    GVAR_FUNC_TABLE_ENTRY("4ti2gapgmp.cc", _4ti2_GraverGMP, 1, "list"),
    { 0 } /* Finish with an empty entry */

};

/******************************************************************************
*F  InitKernel( <module> )  . . . . . . . . initialise kernel data structures
*/
static Int InitKernel( StructInitInfo *module )
{
    /* init filters and functions                                          */
    InitHdlrFuncsFromTable( GVarFuncsgmp );

    /* return success                                                      */
    return 0;
}

/******************************************************************************
*F  InitLibrary( <module> ) . . . . . . .  initialise library data structures
*/
static Int InitLibrary( StructInitInfo *module )
{
    /* init filters and functions */
    InitGVarFuncsFromTable( GVarFuncsgmp );

    /* return success                                                      */
    return 0;
}

/******************************************************************************
*F  InitInfopl()  . . . . . . . . . . . . . . . . . table of init functions
*/
static StructInitInfo modulegmp = {
#ifdef FTITSTATIC
 /* type        = */ MODULE_STATIC,
#else
 /* type        = */ MODULE_DYNAMIC,
#endif
 /* name        = */ "4ti2gapgmp",
 /* revision_c  = */ 0,
 /* revision_h  = */ 0,
 /* version     = */ 0,
 /* crc         = */ 0,
 /* initKernel  = */ InitKernel,
 /* initLibrary = */ InitLibrary,
 /* checkInit   = */ 0,
 /* preSave     = */ 0,
 /* postSave    = */ 0,
 /* postRestore = */ 0
};

#ifndef FTITSTATIC
extern "C"
StructInitInfo * Init__Dynamic ( void )
{
  return &modulegmp;
}
#endif

extern "C"
StructInitInfo * Init__4ti2gmp ( void )
{
    return &modulegmp;
}

