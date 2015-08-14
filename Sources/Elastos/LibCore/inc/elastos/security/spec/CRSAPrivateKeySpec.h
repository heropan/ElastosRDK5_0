
#ifndef __ELASTOS_SECURITY_SPEC_CRSAPRIVATEKEYSPEC_H__
#define __ELASTOS_SECURITY_SPEC_CRSAPRIVATEKEYSPEC_H__

#include "_CRSAPrivateKeySpec.h"
#include "RSAPrivateKeySpec.h"

using Elastos::Math::IBigInteger;

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CRSAPrivateKeySpec), public RSAPrivateKeySpec
{
public:
    CARAPI GetModulus(
        /* [out] */ IBigInteger **modulus);

    CARAPI GetPrivateExponent(
        /* [out] */ IBigInteger **exponent);

    CARAPI constructor(
        /* [in] */IBigInteger *modulus,
        /* [in] */ IBigInteger *privateExponent);
};

}
}
}

#endif // __ELASTOS_SECURITY_SPEC_CRSAPRIVATEKEYSPEC_H__
