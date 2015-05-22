
#ifndef __RSAPRIVATEKEYSPEC_H__
#define __RSAPRIVATEKEYSPEC_H__

#ifdef ELASTOS_CORE
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

using Elastos::Math::IBigInteger;

namespace Elastos {
namespace Security {
namespace Spec {

class RSAPrivateKeySpec
{
public:
    CARAPI GetModulus(
        /* [out] */ IBigInteger **modulus);

    CARAPI GetPrivateExponent(
        /* [out] */ IBigInteger **exponent);

    CARAPI Init(
        /* [in] */IBigInteger *modulus,
        /* [in] */ IBigInteger *privateExponent);

private:
    // Modulus
    AutoPtr<IBigInteger> mModulus;
    // Private Exponent
    AutoPtr<IBigInteger> mPrivateExponent;
};

}
}
}

#endif // __CRSAPRIVATEKEYSPEC_H__
