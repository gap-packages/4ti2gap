
#include "4ti2gap.h"

#ifdef _4ti2_WITH_GMP_
#include "gmpxx.h"
#endif

// The next functions are taken from NormalizInterface GAP4 package
// by Sebastian Gutsche, Max Horn, Christof Söger.

Obj MpzToGAP( const mpz_t x )
{
    Obj res;
    Int size = x->_mp_size;
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

Obj MpzClassToGAP( const mpz_class& x )
{
    return MpzToGAP(x.get_mpz_t());
}

