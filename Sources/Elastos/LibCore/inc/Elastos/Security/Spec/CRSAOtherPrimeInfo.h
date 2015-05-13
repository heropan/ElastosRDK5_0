
#ifndef __CRSAOTHERPRIMEINFO_H__
#define __CRSAOTHERPRIMEINFO_H__

#include "_CRSAOtherPrimeInfo.h"

using Elastos::Math::IBigInteger;

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CRSAOtherPrimeInfo)
{
public:
    CARAPI GetCrtCoefficient(
        /* [out] */ IBigInteger **crtCoefficient);

    CARAPI GetPrime(
        /* [out] */ IBigInteger **prime);

    CARAPI GetExponent(
        /* [out] */ IBigInteger **exponent);

    CARAPI constructor(
        /* [in] */ IBigInteger *prime,
        /* [in] */ IBigInteger *primeExponent,
        /* [in] */ IBigInteger *crtCoefficient);

private:
    // Prime
    AutoPtr<IBigInteger> mPrime;
    // Prime Exponent
    AutoPtr<IBigInteger> mPrimeExponent;
    // CRT Coefficient
    AutoPtr<IBigInteger> mCrtCoefficient;
};

}
}
}

#endif // __CRSAOTHERPRIMEINFO_H__
