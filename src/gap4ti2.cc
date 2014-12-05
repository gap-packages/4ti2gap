#include <iostream>
#include <sstream>

#include "gap4ti2.h"

#include "groebner/Globals.h"
#include "groebner/VectorArray.h"
#include "groebner/VectorArrayStream.h"
#include "groebner/Vector.h"
#include "groebner/Feasible.h"
#include "groebner/GeneratingSet.h"
#include "groebner/GroebnerBasis.h"
#include "groebner/DataType.h"
#include "groebner/BasicOptions.h"

bool GAPInt2IntegerType( Obj ival, IntegerType& oval )
{
    if ( !IS_INTOBJ( ival ) ) 
        return false;
    oval = INT_INTOBJ( ival );
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

//Obj OrderMatrix( Obj order )
//{
//    if ( !IS_STRING_REP(order) ) {
//    }
//}

Obj _4ti2_GroebnerBasisOrder( Obj self, Obj listA, Obj listO )
{
// The next lines are commented because listA is checked previously
// as a matrix.
//    if ( !IS_PLIST( listA ) || !IS_DENSE_LIST( listA ) )
//        ErrorQuit( "Input argument must be a list", 0, 0);
//
    size_t numRows = LEN_PLIST( listA );
//    if ( numRows <= 0 )
//        ErrorQuit( "Input argument must be a nonempty list", 0, 0);
//
    const size_t numCols = LEN_PLIST( ELM_PLIST( listA, 1 ) );
//    if ( numCols <= 0 )
//        ErrorQuit( "Input argument must be a list of nonempty list", 0, 0);

    _4ti2_::VectorArray A( numRows, numCols );
//    for( size_t i = 0; i < numRows; i++ ) {
//        Obj rowlist = ELM_PLIST( listA, i+1 );
//	if ( !IS_LIST( rowlist ) )
//            ErrorQuit( "A row was expected", 0, 0 );
//        for( size_t j = 0; j < numCols; j++ ) {
//            Obj tmp =  ELM_PLIST( rowlist, j+1 );
//            if ( !GAPInt2IntegerType( tmp, A[i][j] ) ) {
//                ErrorQuit( "Integer conversion error", 0, 0 );
//            }
//        }
//    }

    GAPMatrix2VectorArray( listA, A );

    std::streambuf *old = std::cout.rdbuf();
    std::stringstream ss;
    std::cout.rdbuf( ss.rdbuf() );

    _4ti2_::BasicOptions::instance()->output = _4ti2_::BasicOptions::SILENT;
    _4ti2_::Feasible feasible( 0, &A );
    _4ti2_::VectorArray fbas = feasible.get_basis(); 
    if ( fbas.get_number() == 0 ) {
        ErrorQuit( "Could not compute the Groebner basis", 0, 0);
    }

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
    
// std::cout << binomios << std::endl;

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

typedef Obj (* GVarFunc)(/*arguments*/);

#define GVAR_FUNC_TABLE_ENTRY(srcfile, name, nparam, params) \
  {#name, nparam, \
   params, \
   (GVarFunc)name, \
   srcfile ":Func" #name }

// Table of functions to export
static StructGVarFunc GVarFuncs[] = {
    GVAR_FUNC_TABLE_ENTRY("gap4ti2.cc", _4ti2_GroebnerBasisOrder, 2, "list, list"),
    GVAR_FUNC_TABLE_ENTRY("gap4ti2.cc", _4ti2_GroebnerBasis, 1, "list"),
    { 0 } /* Finish with an empty entry */

};

/******************************************************************************
*F  InitKernel( <module> )  . . . . . . . . initialise kernel data structures
*/
static Int InitKernel( StructInitInfo *module )
{
    /* init filters and functions                                          */
    InitHdlrFuncsFromTable( GVarFuncs );

//    InitCopyGVar( "TheTypeNormalizLongIntCone", &TheTypeNormalizLongIntCone );
//    InitCopyGVar( "TheTypeNormalizGMPCone", &TheTypeNormalizGMPCone );

//    InfoBags[T_NORMALIZ].name = "NormalizCone";
//    InitMarkFuncBags(T_NORMALIZ, &MarkNoSubBags);
//    InitFreeFuncBag(T_NORMALIZ, &NormalizFreeFunc);
//    TypeObjFuncs[T_NORMALIZ] = &NormalizTypeFunc;

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
 /* name        = */ "4ti2",
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

