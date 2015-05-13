#ifndef __MATH_CMATHCONTEXTHELPER_H__
#define __MATH_CMATHCONTEXTHELPER_H__

#include "_CMathContextHelper.h"

namespace Elastos {
namespace Math {

CarClass(CMathContextHelper)
{
public:

    CARAPI GetDECIMAL32(
        /* [out] */ IMathContext** context);

    CARAPI GetDECIMAL64(
        /* [out] */ IMathContext** context);

    CARAPI GetDECIMAL128(
        /* [out] */ IMathContext** context);

    CARAPI GetUNLIMITED(
        /* [out] */ IMathContext** context);
};

} // namespace Math
} // namespace Elastos

#endif // __MATH_CMATHCONTEXTHELPER_H__
