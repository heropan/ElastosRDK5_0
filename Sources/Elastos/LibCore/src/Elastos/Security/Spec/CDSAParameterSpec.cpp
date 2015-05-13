
#include "CDSAParameterSpec.h"
#include "cmdef.h"

namespace Elastos {
namespace Security {
namespace Spec {

ECode CDSAParameterSpec::GetG(
    /* [out] */ IBigInteger **base)
{
    VALIDATE_NOT_NULL(base)
    *base = mG;
    INTERFACE_ADDREF(*base)
    return NOERROR;
}

ECode CDSAParameterSpec::GetP(
    /* [out] */ IBigInteger **prime)
{
    VALIDATE_NOT_NULL(prime)
    *prime = mP;
    INTERFACE_ADDREF(*prime)
    return NOERROR;
}

ECode CDSAParameterSpec::GetQ(
    /* [out] */ IBigInteger **subprime)
{
    VALIDATE_NOT_NULL(subprime)
    *subprime = mQ;
    INTERFACE_ADDREF(*subprime)
    return NOERROR;
}

ECode CDSAParameterSpec::constructor(
    /* [in] */ IBigInteger *p,
    /* [in] */ IBigInteger *q,
    /* [in] */ IBigInteger *g)
{
    mP = p;
    mQ = q;
    mG = g;
    return NOERROR;
}

} // namespace Spec
} // namespace Security
} // namespace Security