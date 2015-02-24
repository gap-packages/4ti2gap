#include <iostream>
#include <sstream>
#include <cstring>
#include <new>

#include "4ti2gap.h"

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

bool GAPInt2IntegerType( Obj ival, IntegerType& oval )
{
    if ( IS_INTOBJ( ival ) ) 
        oval = INT_INTOBJ( ival );
    else if ( TNUM_OBJ( ival ) == T_INTPOS || TNUM_OBJ( ival ) == T_INTNEG ) {
        UInt size = SIZE_INT( ival );
        if ( size == 1 ) {
            oval = *ADDR_INT( ival );
            if ( oval < 0 ) {
                std::cout << "Cannot convert MP values, try another option." << std::endl;
                return false;   // overflow
            }
            if ( TNUM_OBJ( ival ) == T_INTNEG )
                oval = -oval;
        }
    }
    return true;
}

void GAPMatrix2VectorArray( Obj listA, _4ti2_::VectorArray &A )
{
    for( size_t i = 0; i < A.get_number(); i++ ) {
        Obj rowlist = ELM_PLIST( listA, i+1 );
	if ( !IS_LIST( rowlist ) )
            ErrorQuit( "A row was expected", 0, 0 );
        for( size_t j = 0; j < A.get_size(); j++ ) {
            Obj tmp =  ELM_PLIST( rowlist, j+1 );
            if ( !GAPInt2IntegerType( tmp, A[i][j] ) ) {
                ErrorQuit( "Integer conversion error", 0, 0 );
            }
        }
    }
}

bool GAPMatrix2StdString( Obj listA, std::string& thst )
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
                    if ( !GAPInt2IntegerType( tmp, av ) )
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
                if ( !GAPInt2IntegerType( tmp, av ) )
                    return false; //ErrorQuit( "Integer conversion error", 0, 0 );
                buildst << av << " ";
            }
        } 
        buildst << "\n";
    }
    thst = buildst.str();
    return true;
}

// It is assumed that listA is a list of lists with the same length.
Obj _4ti2_GroebnerBasisOrder( Obj self, Obj listA, Obj listO )
{
    size_t numRows = LEN_PLIST( listA );
    const size_t numCols = LEN_PLIST( ELM_PLIST( listA, 1 ) );

    _4ti2_::VectorArray A( numRows, numCols );

    GAPMatrix2VectorArray( listA, A );

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
        GAPMatrix2VectorArray( listO, cost );
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
            SET_ELM_PLIST( binGAP, j+1, ObjInt_Int( binomios[i][j] ) );
            CHANGED_BAG( binGAP );
        }
        SET_ELM_PLIST( binListGAP, i+1, binGAP );
        CHANGED_BAG( binListGAP );
    }
    return binListGAP;
}

Obj _4ti2_GroebnerBasis( Obj self, Obj listA )
{
    return _4ti2_GroebnerBasisOrder( self, listA, 0 );
}

Obj _4ti2_Hilbert( Obj self, Obj list )
{
    // This code is similar to NormalizInterface's input processing
    int n = LEN_PLIST( list );

    if ( n & 1 ) {
        std::cerr << "Input list must have even number of elements.\n" << std::endl;
        return Fail;
    }
    
    std::string st;

#ifdef _4ti2_INT64_
    _4ti2_zsolve_::HilbertAPI<_4ti2_int64_t> problem;
#endif

#ifdef _4ti2_INT32_
    _4ti2_zsolve_::HilbertAPI<_4ti2_int32_t> problem;
#endif

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
        if ( ! GAPMatrix2StdString( inel, st ) ) {
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
#ifdef _4ti2_INT64_
        char arg2[]="-p=64";
#endif
#ifdef _4ti2_INT32_
        char arg2[]="-p=32";
#endif
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
        std::cerr << "Please use the multiple precision version of this function!" << std::endl;
        return Fail;
    }
    catch ( _4ti2_zsolve_::IOException e )
    {
        std::cerr << e;
        return Fail;
    }

    _4ti2_zsolve_::VectorArrayAPI<_4ti2_int64_t> *hilbas =  (_4ti2_zsolve_::VectorArrayAPI<_4ti2_int64_t> *) problem.get_matrix( "zhom" );
    Obj hilListGAP;
    size_t numRows = hilbas->get_num_rows();
    size_t numCols = hilbas->get_num_cols();

    hilListGAP = NEW_PLIST( T_PLIST, numRows );
    SET_LEN_PLIST( hilListGAP, numRows );
    for ( size_t i = 0; i < numRows; i++ ) {
        Obj lisGAP = NEW_PLIST( T_PLIST, numCols );
    	SET_LEN_PLIST( lisGAP, numCols );
        for ( size_t j = 0; j < numCols; j++ ) {
            SET_ELM_PLIST( lisGAP, j+1, ObjInt_Int( hilbas->data[i][j] ) );
            CHANGED_BAG( lisGAP );
        }
        SET_ELM_PLIST( hilListGAP, i+1, lisGAP );
        CHANGED_BAG( hilListGAP );
    }
    return hilListGAP;
}

Obj _4ti2_Graver( Obj self, Obj list )
{
    // This code is similar to NormalizInterface's input processing
    int n = LEN_PLIST( list );

    if ( n & 1 ) {
        std::cerr << "Input list must have even number of elements.\n" << std::endl;
        return Fail;
    }
    
    std::string st;

#ifdef _4ti2_INT64_
    _4ti2_zsolve_::GraverAPI<_4ti2_int64_t> problem;
#endif

#ifdef _4ti2_INT32_
    _4ti2_zsolve_::GraverAPI<_4ti2_int32_t> problem;
#endif

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
        if ( ! GAPMatrix2StdString( inel, st ) ) {
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
#ifdef _4ti2_INT64_
        char arg2[]="-p=64";
#endif
#ifdef _4ti2_INT32_
        char arg2[]="-p=32";
#endif
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
        std::cerr << "Please use the multiple precision version of this function!" << std::endl;
        return Fail;
    }
    catch ( _4ti2_zsolve_::IOException e )
    {
        std::cerr << e;
        return Fail;
    }

#ifdef _4ti2_INT64_
    _4ti2_zsolve_::VectorArrayAPI<_4ti2_int64_t> *grabas =  (_4ti2_zsolve_::VectorArrayAPI<_4ti2_int64_t> *) problem.get_matrix( "gra" );
#endif
#ifdef _4ti2_INT32_
    _4ti2_zsolve_::VectorArrayAPI<_4ti2_int32_t> *grabas =  (_4ti2_zsolve_::VectorArrayAPI<_4ti2_int64_t> *) problem.get_matrix( "gra" );
#endif
    Obj graListGAP;
    size_t numRows = grabas->get_num_rows();
    size_t numCols = grabas->get_num_cols();

    graListGAP = NEW_PLIST( T_PLIST, numRows );
    SET_LEN_PLIST( graListGAP, numRows );
    for ( size_t i = 0; i < numRows; i++ ) {
        Obj lisGAP = NEW_PLIST( T_PLIST, numCols );
    	SET_LEN_PLIST( lisGAP, numCols );
        for ( size_t j = 0; j < numCols; j++ ) {
            SET_ELM_PLIST( lisGAP, j+1, ObjInt_Int( grabas->data[i][j] ) );
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
static StructGVarFunc GVarFuncs[] = {
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2_GroebnerBasisOrder, 2, "list, list"),
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2_GroebnerBasis, 1, "list"),
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2_Hilbert, 1, "list"),
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2_Graver, 1, "list"),
//    GVAR_FUNC_TABLE_ENTRY("4ti2hilbert.cc", _4ti2_gmp, 1, "x"),
    { 0 } /* Finish with an empty entry */

};

/******************************************************************************
*F  InitKernel( <module> )  . . . . . . . . initialise kernel data structures
*/
static Int InitKernel( StructInitInfo *module )
{
    /* init filters and functions                                          */
    InitHdlrFuncsFromTable( GVarFuncs );

    /* return success                                                      */
    return 0;
}

/******************************************************************************
*F  InitLibrary( <module> ) . . . . . . .  initialise library data structures
*/
static Int InitLibrary( StructInitInfo *module )
{
    /* init filters and functions */
    InitGVarFuncsFromTable( GVarFuncs );

    /* return success                                                      */
    return 0;
}

/******************************************************************************
*F  InitInfopl()  . . . . . . . . . . . . . . . . . table of init functions
*/
static StructInitInfo module = {
#ifdef FTITSTATIC
 /* type        = */ MODULE_STATIC,
#else
 /* type        = */ MODULE_DYNAMIC,
#endif
 /* name        = */ "4ti2gap",
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
  return &module;
}
#endif

extern "C"
StructInitInfo * Init__4ti2 ( void )
{
    return &module;
}

