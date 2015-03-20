#include <iostream>
#include <sstream>
#include <cstring>

#include "4ti2gap.h"
#include "basicxch.h"

#ifdef _4ti2_WITH_GMP_
#include <gmpxx.h>
#endif

#include "zsolve/Exception.h"
#include "zsolve/VectorArrayAPI.hpp"
#include "zsolve/HilbertAPI.hpp"
#include "zsolve/GraverAPI.hpp"

// From NormalizInterface
template<typename Integer>
bool GAPIntTo4ti2( Obj x, Integer &out )
{
    return Integer::unimplemented_function;
//    return false;
}

// From NormalizInterface
#ifdef _4ti2_INT64_
template<>
bool GAPIntTo4ti2(Obj x, _4ti2_int64_t &out)
#elif defined(_4ti2_INT32_)
template<>
bool GAPIntTo4ti2(Obj x, _4ti2_int32_t &out)
#endif
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
        }
    }
    return false;
}

// From NormalizInterface

#ifdef _4ti2_WITH_GMP_
template<>
bool GAPIntTo4ti2(Obj x, mpz_class &out)
{
    if (IS_INTOBJ(x)) {
        out = (int) INT_INTOBJ(x);
        return true;
    } else if (TNUM_OBJ(x) == T_INTPOS || TNUM_OBJ(x) == T_INTNEG) {
        mpz_ptr m = out.get_mpz_t();
        UInt size = SIZE_INT(x);
        mpz_realloc2(m, size * GMP_NUMB_BITS);
        memcpy(m->_mp_d, ADDR_INT(x), sizeof(mp_limb_t) * size);
        m->_mp_size = (TNUM_OBJ(x) == T_INTPOS) ? (Int)size : - (Int)size;
        return true;
    }
    return false;
}
#endif

bool GAPInt2StdString( Obj ival, std::string& oval )
{
    if ( IS_INTOBJ( ival ) )
        oval = std::to_string( (int) INT_INTOBJ (ival ) );
#ifdef _4ti2_WITH_GMP_
    else {
        mpz_class aux;
        if ( GAPIntTo4ti2( ival, aux ) ) {
            oval = aux.get_str();
        }
        else
            return false;
    }
#else
    else {
        std::cerr << "Not integer value, but GMP is not available.\n";
        return false;
    }
#endif
    return true;
}

// This function makes the conversion of a GAP matrix to a string 
// formated as a valid input for 4ti2. If the input matrix is a 
// vector it is transformed to a row vector.
template<typename Integer>
bool GAPMatrix2StdString( Obj listA, std::string& thst )
{
    size_t numRows = LEN_PLIST( listA );
    size_t numCols;
    std::ostringstream buildst;
    Integer av;
//    std::string valst;
    Obj rowlist = ELM_PLIST( listA, 1 );
    if ( IS_PLIST( rowlist ) ) {
        numCols = LEN_PLIST( ELM_PLIST( listA, 1 ) );
        buildst << numRows << " " << numCols << "\n";
        for( size_t i = 1; i <= numRows; i++ ) {
            rowlist = ELM_PLIST( listA, i );
            if ( !IS_LIST( rowlist ) ) {
                std::cerr << "Bad formed input.\n";
                return false; //ErrorQuit( "A row was expected", 0, 0 );
            }
            size_t nele = LEN_PLIST( rowlist );
            if ( nele != numCols ) {
                std::cerr << "The number of elements must be the same for every row.\n";
                return false;
            }

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
                    if ( !GAPIntTo4ti2<Integer>( tmp, av ) )
//                    if ( !GAPInt2StdString( tmp, valst ) )
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
                if ( !GAPIntTo4ti2<Integer>( tmp, av ) )
//                if ( !GAPInt2StdString( tmp, valst ) )
                    return false; //ErrorQuit( "Integer conversion error", 0, 0 );
                buildst << av << " ";
            }
        } 
        buildst << "\n";
    }
    thst = buildst.str();
    return true;
}

template<typename Integer>
Obj _4ti2_IntegerType2GAP( Integer x )
{
    return Integer::unimplemented_function;
}

#ifdef _4ti2_INT64_
template<>
Obj _4ti2_IntegerType2GAP( _4ti2_int64_t x )
#elif defined(_4ti2_INT32_)
template<>
Obj _4ti2_IntegerType2GAP( _4ti2_int32_t x )
#endif
{
    return ObjInt_Int(x);
}

#ifdef _4ti2_WITH_GMP_
template<>
Obj _4ti2_IntegerType2GAP( mpz_class x )
{
    return MpzClassToGAP(x);
}
#endif

template<typename Integer>
Obj _4ti2_zsolve_VectorArrayAPI2GAP( _4ti2_zsolve_::VectorArrayAPI<Integer> &mat )
{
    Obj matGAP;
    size_t numRows = mat.get_num_rows();
    size_t numCols = mat.get_num_cols();

    matGAP = NEW_PLIST( T_PLIST, numRows );
    SET_LEN_PLIST( matGAP, numRows );
    for ( size_t i = 0; i < numRows; i++ ) {
        Obj lisGAP = NEW_PLIST( T_PLIST, numCols );
    	SET_LEN_PLIST( lisGAP, numCols );
        for ( size_t j = 0; j < numCols; j++ ) {
            SET_ELM_PLIST( lisGAP, j+1, _4ti2_IntegerType2GAP( mat.data[i][j] ) );
            CHANGED_BAG( lisGAP );
        }
        SET_ELM_PLIST( matGAP, i+1, lisGAP );
        CHANGED_BAG( matGAP );
    }
    return matGAP;
}

template<typename Integer>
bool getZSolveInput( Obj list, _4ti2_zsolve_::ZSolveAPI<Integer> &problem )
{
// This code is similar to NormalizInterface's input processing
    int n = LEN_PLIST( list );

    if ( n & 1 ) {
        std::cerr << "Input list must have even number of elements.\n" << std::endl;
        return Fail;
    }
    std::string st;
    for (int i = 0; i < n; i += 2) {
        Obj type = ELM_PLIST( list, i+1 );
        if ( ! IS_STRING( type ) ) {
            std::cerr << "Element " << i+1 << " of the input list must be a type string" << std::endl;
            return false;
        }
        std::string stinty( CSTR_STRING(type) );

        Obj inel = ELM_PLIST( list, i+2 );
        if ( !IS_LIST( inel ) ) {
            std::cerr << "Element " << i+2 << " must be a vector or a matrix.\n";
            return false;
        }
        if ( ! GAPMatrix2StdString<Integer>( inel, st ) ) {
            std::cerr << "Unable to proccess element " << i+2 << " of the input list" << std::endl;
            return false;
        }
        std::istringstream ists( st );
        // ists.str( st );
        try {
            problem.create_matrix( ists, stinty.c_str() );
        }
         catch (_4ti2_zsolve_::IOException e) {
            std::cerr << "4ti2 can not create the input matrix: " << stinty.c_str() << std::endl;
            std::cerr << st;
            return false;
        }
    }

    return true;
}

template<typename Integer>
bool buildZSolveProblem( Obj list, _4ti2_zsolve_::ZSolveAPI<Integer> &problem )
{
//    return Integer::unimplemented_function; 
    return false;
}

#ifdef _4ti2_INT64_
template<>
bool buildZSolveProblem( Obj list, _4ti2_zsolve_::ZSolveAPI<_4ti2_int64_t> &problem )
#elif defined(_4ti2_INT32_) 
template<>
bool buildZSolveProblem( Obj list, _4ti2_zsolve_::ZSolveAPI<_4ti2_int32_t> &problem )
#endif
{
#ifdef _4ti2_INT64_    
    if ( getZSolveInput<_4ti2_int64_t>( list, problem ) ) {
#elif defined(_4ti2_INT32_)
    if ( getZSolveInput<_4ti2_int32_t>( list, problem ) ) {
#endif
        char arg1[]="-q";
#ifdef _4ti2_INT64_
        char arg2[]="-p=64";
#elif defined(_4ti2_INT32_)
        char arg2[]="-p=32";
#endif
        char **argopt;
    	argopt = new char* [3];
        argopt[1] = arg1;
        argopt[2] = arg2;
        problem.set_options( 3, argopt );
        delete[] argopt;
        return true;
    }
    return false;
}

#ifdef _4ti2_WITH_GMP_
template<>
bool buildZSolveProblem( Obj list, _4ti2_zsolve_::ZSolveAPI<mpz_class> &problem )
{
    if ( getZSolveInput<mpz_class>( list, problem ) ) {
        char arg1[]="-q";
        char arg2[]="-p=gmp";
        char **argopt;
    	argopt = new char* [3];
        argopt[1] = arg1;
        argopt[2] = arg2;
        problem.set_options( 3, argopt );
        delete[] argopt;
        return true;
    }
    return false;
}
#endif

template<typename Integer>
Obj _4ti2_zsolve_HilbertResult2GAP( _4ti2_zsolve_::HilbertAPI<Integer>& problem )
{
    Obj zsolGAP;

    _4ti2_zsolve_::VectorArrayAPI<Integer> *zhom =  (_4ti2_zsolve_::VectorArrayAPI<Integer> *) problem.get_matrix( "zhom" );
    _4ti2_zsolve_::VectorArrayAPI<Integer> *zfree =  (_4ti2_zsolve_::VectorArrayAPI<Integer> *) problem.get_matrix( "zfree" );

    _4ti2_zsolve_::VectorArrayAPI<Integer> empty( 0, 0 );
    
    if ( zfree && zfree->data.height() > 0 ) {
        zsolGAP = NEW_PLIST( T_PLIST, 2 );
        SET_LEN_PLIST( zsolGAP, 2 );
    }
    else {
        zsolGAP = NEW_PLIST( T_PLIST, 1 );
        SET_LEN_PLIST( zsolGAP, 1 );
    }
    
    if ( zhom ) {
        Obj elem =  _4ti2_zsolve_VectorArrayAPI2GAP<Integer>( *zhom );
        SET_ELM_PLIST( zsolGAP, 1, elem );
    }
    else
        SET_ELM_PLIST( zsolGAP, 1, _4ti2_zsolve_VectorArrayAPI2GAP<Integer>( empty ) );
    CHANGED_BAG( zsolGAP );
    
    if ( zfree && zfree->data.height() > 0) {
        Obj elem =  _4ti2_zsolve_VectorArrayAPI2GAP<Integer>( *zfree );
        SET_ELM_PLIST( zsolGAP, 2, elem );
        CHANGED_BAG( zsolGAP );
    }

    return zsolGAP;
}

Obj _4ti2zsolve_Hilbert( Obj self, Obj list )
{
#ifdef _4ti2_INT64_
    _4ti2_zsolve_::HilbertAPI<_4ti2_int64_t> problem;
#elif defined(_4ti2_INT32_)
    _4ti2_zsolve_::HilbertAPI<_4ti2_int32_t> problem;
#endif

    if ( !buildZSolveProblem( list, problem ) ) {
        return Fail;
    }
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
    return _4ti2_zsolve_HilbertResult2GAP<_4ti2_int64_t>( problem ); 
#elif defined(_4ti2_INT32_)
    return _4ti2_zsolve_HilbertResult2GAP<_4ti2_int32_t>( problem ); 
#endif
}

#ifdef _4ti2_WITH_GMP_
Obj _4ti2zsolve_HilbertGMP( Obj self, Obj list )
{
    _4ti2_zsolve_::HilbertAPI<mpz_class> problem;

    if ( !buildZSolveProblem( list, problem ) ) {
        return Fail;
    }
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

    return _4ti2_zsolve_HilbertResult2GAP<mpz_class>( problem ); 
    
}
#endif

Obj _4ti2zsolve_Graver( Obj self, Obj list )
{
#ifdef _4ti2_INT64_
    _4ti2_zsolve_::GraverAPI<_4ti2_int64_t> problem;
#elif defined(_4ti2_INT32_)
    _4ti2_zsolve_::GraverAPI<_4ti2_int32_t> problem;
#endif

    if ( !buildZSolveProblem( list, problem ) ) {
        return Fail;
    }
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
    return _4ti2_zsolve_VectorArrayAPI2GAP<_4ti2_int64_t>( *grabas ); 
#elif defined(_4ti2_INT32_)
    _4ti2_zsolve_::VectorArrayAPI<_4ti2_int32_t> *grabas =  (_4ti2_zsolve_::VectorArrayAPI<_4ti2_int32_t> *) problem.get_matrix( "gra" );
    return _4ti2_zsolve_VectorArrayAPI2GAP<_4ti2_int32_t>( *grabas ); 
#endif
}

#ifdef _4ti2_WITH_GMP_
Obj _4ti2zsolve_GraverGMP( Obj self, Obj list )
{
    _4ti2_zsolve_::GraverAPI<mpz_class> problem;

    if ( !buildZSolveProblem( list, problem ) ) {
        return Fail;
    }
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

    _4ti2_zsolve_::VectorArrayAPI<mpz_class> *grabas =  (_4ti2_zsolve_::VectorArrayAPI<mpz_class> *) problem.get_matrix( "gra" );

    return _4ti2_zsolve_VectorArrayAPI2GAP<mpz_class>( *grabas ); 
}
#endif

template<typename Integer>
Obj _4ti2_zsolve_ZSolveResult2GAP( _4ti2_zsolve_::ZSolveAPI<Integer>& problem )
{
    Obj zsolGAP;

    _4ti2_zsolve_::VectorArrayAPI<Integer> *zinhom =  (_4ti2_zsolve_::VectorArrayAPI<Integer> *) problem.get_matrix( "zinhom" );
    _4ti2_zsolve_::VectorArrayAPI<Integer> *zhom =  (_4ti2_zsolve_::VectorArrayAPI<Integer> *) problem.get_matrix( "zhom" );
    _4ti2_zsolve_::VectorArrayAPI<Integer> *zfree =  (_4ti2_zsolve_::VectorArrayAPI<Integer> *) problem.get_matrix( "zfree" );

    _4ti2_zsolve_::VectorArrayAPI<Integer> empty( 0, 0 );
    
    if ( zfree && zfree->data.height() > 0 ) {
        zsolGAP = NEW_PLIST( T_PLIST, 3 );
        SET_LEN_PLIST( zsolGAP, 3 );
    }
    else {
        zsolGAP = NEW_PLIST( T_PLIST, 2 );
        SET_LEN_PLIST( zsolGAP, 2 );
    }
    
    if ( zinhom ) {
        Obj elem =  _4ti2_zsolve_VectorArrayAPI2GAP<Integer>( *zinhom );
        SET_ELM_PLIST( zsolGAP, 1, elem );
    }
    else
        SET_ELM_PLIST( zsolGAP, 1, _4ti2_zsolve_VectorArrayAPI2GAP<Integer>( empty ) );
    CHANGED_BAG( zsolGAP );
    
    if ( zhom ) {
        Obj elem =  _4ti2_zsolve_VectorArrayAPI2GAP<Integer>( *zhom );
        SET_ELM_PLIST( zsolGAP, 2, elem );
    }
    else
        SET_ELM_PLIST( zsolGAP, 2, _4ti2_zsolve_VectorArrayAPI2GAP<Integer>( empty ) );
    CHANGED_BAG( zsolGAP );

    if ( zfree && zfree->data.height() > 0 ) {
        Obj elem =  _4ti2_zsolve_VectorArrayAPI2GAP<Integer>( *zfree );
        SET_ELM_PLIST( zsolGAP, 3, elem );
        CHANGED_BAG( zsolGAP );
    }
    
    return zsolGAP;
}

Obj _4ti2zsolve_ZSolve( Obj self, Obj list )
{
#ifdef _4ti2_INT64_
    _4ti2_zsolve_::ZSolveAPI<_4ti2_int64_t> problem;
#elif defined(_4ti2_INT32_)
    _4ti2_zsolve_::ZSolveAPI<_4ti2_int32_t> problem;
#endif

    if ( !buildZSolveProblem( list, problem ) ) {
        return Fail;
    }
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
    return _4ti2_zsolve_ZSolveResult2GAP<_4ti2_int64_t>( problem ); 
#elif defined(_4ti2_INT32_)
    return _4ti2_zsolve_ZSolveResult2GAP<_4ti2_int32_t>( problem ); 
#endif
}

#ifdef _4ti2_WITH_GMP_
Obj _4ti2zsolve_ZSolveGMP( Obj self, Obj list )
{
    _4ti2_zsolve_::ZSolveAPI<mpz_class> problem;

    if ( !buildZSolveProblem( list, problem ) ) {
        return Fail;
    }
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
    return _4ti2_zsolve_ZSolveResult2GAP<mpz_class>( problem ); 
}
#endif
