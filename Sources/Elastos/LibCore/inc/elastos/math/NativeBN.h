#ifndef __MATH_NATIVEBN_H__
#define __MATH_NATIVEBN_H__

#include <elastos.h>
#include <elquintet.h>
#include "Object.h"

// see java_math_NativeBN.cpp
//
namespace Elastos {
namespace Math {

class NativeBN : public Object
{
public:
    static Int32 ERR_get_error();

    static String ERR_error_string(
        /* [in] */ Int32 e);

    static Int32 BN_new();

    static void BN_free(
        /* [in] */ Int32 a);

    static Int32 BN_cmp(
        /* [in] */ Int32 a,
        /* [in] */ Int32 b);

    static Boolean BN_copy(
        /* [in] */ Int32 to,
        /* [in] */ Int32 from);

    static Boolean PutLongInt(
        /* [in] */ Int32 a,
        /* [in] */ Int64 dw);

    static Boolean PutULongInt(
        /* [in] */ Int32 a,
        /* [in] */ Int64 dw,
        /* [in] */ Boolean neg);

    static Int32 BN_dec2bn(
        /* [in] */ Int32 a,
        /* [in] */ const String& str);

    static Int32 BN_hex2bn(
        /* [in] */ Int32 a,
        /* [in] */ const String& str);

    static Boolean BN_bin2bn(
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Boolean neg,
        /* [in] */ Int32 ret);

    static Boolean LitEndInts2bn(
        /* [in] */ const ArrayOf<Int32>& ints,
        /* [in] */ Boolean neg,
        /* [in] */ Int32 ret);

    static Boolean TwosComp2bn(
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 ret);

    static Int64 LongInt(
        /* [in] */ Int32 a);

    static String BN_bn2dec(
        /* [in] */ Int32 a);

    static String BN_bn2h(
        /* [in] */ Int32 a);

    static AutoPtr<ArrayOf<Byte> > BN_bn2bin(
        /* [in] */ Int32 a);

    static AutoPtr<ArrayOf<Int32> > Bn2litEndInts(
        /* [in] */ Int32 a);

    // Returns -1, 0, 1 AND NOT Boolean.
    // #define BN_is_negative(a) ((a)->neg != 0)
    static Int32 Sign(
        /* [in] */ Int32 a);

    static void BN_set_negative(
        /* [in] */ Int32 b,
        /* [in] */ Int32 n);

    static Int32 BitLength(
        /* [in] */ Int32 a);

    static Boolean BN_is_bit_set(
        /* [in] */ Int32 a,
        /* [in] */ Int32 n);

    // Returns Boolean success.
    // op: 0 = reset; 1 = set; -1 = flip
    // uses BN_set_bit(), BN_clear_bit() and BN_is_bit_set()
    static Boolean ModifyBit(
        /* [in] */ Int32 a,
        /* [in] */ Int32 n,
        /* [in] */ Int32 op);

    static Boolean BN_shift(
        /* [in] */ Int32 r,
        /* [in] */ Int32 a,
        /* [in] */ Int32 n);

    static Boolean BN_add_word(
        /* [in] */ Int32 a,
        /* [in] */ Int32 w);

    static Boolean BN_sub_word(
        /* [in] */ Int32 a,
        /* [in] */ Int32 w);

    static Boolean BN_mul_word(
        /* [in] */ Int32 a,
        /* [in] */ Int32 w);

    static Int32 BN_div_word(
        /* [in] */ Int32 a,
        /* [in] */ Int32 w);

    static Int32 BN_mod_word(
        /* [in] */ Int32 a,
        /* [in] */ Int32 w);

    static Boolean BN_add(
        /* [in] */ Int32 r,
        /* [in] */ Int32 a,
        /* [in] */ Int32 b);

    static Boolean BN_sub(
        /* [in] */ Int32 r,
        /* [in] */ Int32 a,
        /* [in] */ Int32 b);

    static Boolean BN_gcd(
        /* [in] */ Int32 r,
        /* [in] */ Int32 a,
        /* [in] */ Int32 b);

    static Boolean BN_mul(
        /* [in] */ Int32 r,
        /* [in] */ Int32 a,
        /* [in] */ Int32 b);

    static Boolean BN_exp(
        /* [in] */ Int32 r,
        /* [in] */ Int32 a,
        /* [in] */ Int32 p);

    static Boolean BN_div(
        /* [in] */ Int32 dv,
        /* [in] */ Int32 rem,
        /* [in] */ Int32 m,
        /* [in] */ Int32 d);

    static Boolean BN_nnmod(
        /* [in] */ Int32 r,
        /* [in] */ Int32 a,
        /* [in] */ Int32 m);

    static Boolean BN_mod_exp(
        /* [in] */ Int32 r,
        /* [in] */ Int32 a,
        /* [in] */ Int32 p,
        /* [in] */ Int32 m);

    static Boolean BN_mod_inverse(
        /* [in] */ Int32 ret,
        /* [in] */ Int32 a,
        /* [in] */ Int32 n);

    static Boolean BN_generate_prime_(
        /* [in] */ Int32 ret,
        /* [in] */ Int32 bits,
        /* [in] */ Boolean safe,
        /* [in] */ Int32 add,
        /* [in] */ Int32 rem,
        /* [in] */ Int32 cb);

    static Boolean BN_is_prime_(
        /* [in] */ Int32 p,
        /* [in] */ Int32 nchecks,
        /* [in] */ Int32 cb);

private:
    static Boolean negBigEndianBytes2bn(
        /* [in] */ const unsigned char* bytes,
        /* [in] */ Int32 bytesLen,
        /* [in] */ Int32 ret);

    static char* leadingZerosTrimmed(
        /* [in] */ char* s);
};

} // namespace Math
} // namespace Elastos

#endif // __MATH_NATIVEBN_H__
