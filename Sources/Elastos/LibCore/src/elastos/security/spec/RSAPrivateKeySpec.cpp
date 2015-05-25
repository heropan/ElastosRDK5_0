
#include "RSAPrivateKeySpec.h"
#include "cmdef.h"

namespace Elastos {
namespace Security {
namespace Spec {


ECode RSAPrivateKeySpec::GetModulus(
    /* [out] */ IBigInteger **modulus)
{
    VALIDATE_NOT_NULL(modulus)
    *modulus = mModulus;
    INTERFACE_ADDREF(*modulus)
    return NOERROR;
}

ECode RSAPrivateKeySpec::GetPrivateExponent(
    /* [out] */ IBigInteger **exponent)
{
    VALIDATE_NOT_NULL(exponent)
    *exponent = mPrivateExponent;
    INTERFACE_ADDREF(*exponent)
    return NOERROR;
}

ECode RSAPrivateKeySpec::Init(
    /* [in] */ IBigInteger *modulus,
    /* [in] */ IBigInteger *privateExponent)
{
    mModulus = modulus;
    mPrivateExponent = privateExponent;
    return NOERROR;
}

}
}
}

