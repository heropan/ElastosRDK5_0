#ifndef __MATH_CBIGINTEGERHELPER_H__
#define __MATH_CBIGINTEGERHELPER_H__

#include "_CBigIntegerHelper.h"

namespace Elastos {
namespace Math {

CarClass(CBigIntegerHelper)
{
public:
    /** Returns a {@code BigInteger} whose value is equal to {@code value}. */
    CARAPI ValueOf(
        /* [in] */ Int64 value,
        /* [out] */ IBigInteger** result);

    CARAPI GetMINUSOne(
        /* [out] */ IBigInteger** result);

    CARAPI GetZERO(
        /* [out] */ IBigInteger** result);

    CARAPI GetONE(
        /* [out] */ IBigInteger** result);

    CARAPI GetTEN(
        /* [out] */ IBigInteger** result);
};

} // namespace Math
} // namespace Elastos

#endif // __MATH_CBIGINTEGERHELPER_H__
