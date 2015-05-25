
#include "CRSAOtherPrimeInfo.h"
#include "cmdef.h"

namespace Elastos {
namespace Security {
namespace Spec {


ECode CRSAOtherPrimeInfo::GetCrtCoefficient(
    /* [out] */ IBigInteger **crtCoefficient)
{
    VALIDATE_NOT_NULL(crtCoefficient)
    *crtCoefficient = mCrtCoefficient;
    INTERFACE_ADDREF(*crtCoefficient)
    return NOERROR;
}

ECode CRSAOtherPrimeInfo::GetPrime(
    /* [out] */ IBigInteger **prime)
{
    VALIDATE_NOT_NULL(prime)
    *prime = mPrime;
    INTERFACE_ADDREF(*prime)
    return NOERROR;
}

ECode CRSAOtherPrimeInfo::GetExponent(
    /* [out] */ IBigInteger **exponent)
{
    VALIDATE_NOT_NULL(exponent)
    *exponent = mPrimeExponent;
    INTERFACE_ADDREF(*exponent)
    return NOERROR;
}

ECode CRSAOtherPrimeInfo::constructor(
    /* [in] */ IBigInteger *prime,
    /* [in] */ IBigInteger *primeExponent,
    /* [in] */ IBigInteger *crtCoefficient)
{
    if (prime == NULL) {
        //throw new NullPointerException("prime == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (primeExponent == NULL) {
        //throw new NullPointerException("primeExponent == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (crtCoefficient == NULL) {
        //throw new NullPointerException("crtCoefficient == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    mPrime = prime;
    mPrimeExponent = primeExponent;
    mCrtCoefficient = crtCoefficient;
    return NOERROR;
}

}
}
}

