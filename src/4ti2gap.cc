#include "4ti2gap.h"
#include "4ti2groebner.h"
#include "4ti2minimize.h"
#include "4ti2files.h"
#include "4ti2normalform.h"
#include "4ti2markov.h"
#include "4ti2zsolve.h"

typedef Obj (* GVarFunc_4ti2gap)(/*arguments*/);

#define GVAR_FUNC_TABLE_ENTRY(srcfile, name, nparam, params) \
  {#name, nparam, \
   params, \
   (GVarFunc_4ti2gap)name, \
   srcfile ":Func" #name }

// Table of functions to export
static StructGVarFunc GVarFuncs[] = {
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2groebner_GroebnerBasisOrder, 2, "list, list"),
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2groebner_GroebnerBasis, 1, "list"),
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2groebner_Minimize, 5, "list"),
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2groebner_Normalform, 6, "list"),
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2groebner_MarkovBasis, 6, "list"),
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2gap_MatFile2GAP, 1, "list"),
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2zsolve_Hilbert, 1, "list"),
#ifdef _4ti2_WITH_GMP_
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2zsolve_HilbertGMP, 1, "list"),
#endif
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2zsolve_Graver, 1, "list"),
#ifdef _4ti2_WITH_GMP_
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2zsolve_GraverGMP, 1, "list"),
#endif
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2zsolve_ZSolve, 1, "list"),
#ifdef _4ti2_WITH_GMP_
    GVAR_FUNC_TABLE_ENTRY("4ti2gap.cc", _4ti2zsolve_ZSolveGMP, 1, "list"),
#endif
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
#if defined(__GNUC__) || defined(__GNUG__)
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
#else 
#ifdef FTITSTATIC
    .type = MODULE_STATIC,
#else
    .type = MODULE_DYNAMIC,
#endif
    .name = "4ti2gap",
    .initKernel = InitKernel,
    .initLibrary = InitLibrary,
#endif
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
