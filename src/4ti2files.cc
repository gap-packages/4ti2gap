//#include <string>
#include <fstream>

#include "4ti2gap.h"
#include "4ti2daco.h"

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

Obj _4ti2gap_MatFile2GAP( Obj self, Obj _filename )
{
    if ( IS_STRING( _filename ) ) {
        char *p_filename = CSTR_STRING( _filename );
        std::ifstream f( p_filename );
        if ( f.good() )
            f.close();
        else {
            f.close();
            ErrorQuit("File does not exists or cannot be accessed.", 0, 0);
        }
//        std::string filename ( p_filename );
        _4ti2_::VectorArray *_mat = _4ti2_::input_VectorArray( p_filename );
        _4ti2_::VectorArray mat( *_mat );
        delete _mat;
        #ifdef _4ti2_WITH_GMP_
        return _4ti2gap_VectorArray_mpz2GAP( mat );
        #else
        return _4ti2gap_VectorArray2GAP( mat );
        #endif
    }
    return (Obj) 0;
}
