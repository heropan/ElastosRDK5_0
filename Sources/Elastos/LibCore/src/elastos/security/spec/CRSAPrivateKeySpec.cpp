
#include "CRSAPrivateKeySpec.h"

namespace Elastos {
namespace Security {
namespace Spec {


ECode CRSAPrivateKeySpec::GetModulus(
    /* [out] */ IBigInteger **modulus)
{
    return RSAPrivateKeySpec::GetModulus(modulus);
}

ECode CRSAPrivateKeySpec::GetPrivateExponent(
    /* [out] */ IBigInteger **exponent)
{
    return RSAPrivateKeySpec::GetPrivateExponent(exponent);
}

ECode CRSAPrivateKeySpec::constructor(
    /* [in] */ IBigInteger *modulus,
    /* [in] */ IBigInteger *privateExponent)
{
    return RSAPrivateKeySpec::Init(modulus, privateExponent);
}

}
}
}

