
#ifndef __ELASTOS_SECURITY_SPEC_CRSAPUBLICKEYSPEC_H__
#define __ELASTOS_SECURITY_SPEC_CRSAPUBLICKEYSPEC_H__

#include "_CRSAPublicKeySpec.h"

using Elastos::Math::IBigInteger;

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CRSAPublicKeySpec)
{
public:
    CARAPI GetModulus(
        /* [out] */ IBigInteger **modulus);

    CARAPI GetPublicExponent(
        /* [out] */ IBigInteger **exponent);

    CARAPI constructor(
        /* [in] */ IBigInteger *modulus,
        /* [in] */ IBigInteger *publicExponent);

private:
    // Modulus
    AutoPtr<IBigInteger> mModulus;
    // Public Exponent
    AutoPtr<IBigInteger> mPublicExponent;
};

}
}
}

#endif // __ELASTOS_SECURITY_SPEC_CRSAPUBLICKEYSPEC_H__
