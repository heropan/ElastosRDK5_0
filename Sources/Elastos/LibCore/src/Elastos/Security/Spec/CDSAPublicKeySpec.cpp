
#include "CDSAPublicKeySpec.h"
#include "cmdef.h"

namespace Elastos {
namespace Security {
namespace Spec {


ECode CDSAPublicKeySpec::GetG(
    /* [out] */ IBigInteger **g)
{
    VALIDATE_NOT_NULL(g)
    *g = mG;
    INTERFACE_ADDREF(*g)
    return NOERROR;
}

ECode CDSAPublicKeySpec::GetP(
    /* [out] */ IBigInteger **p)
{
    VALIDATE_NOT_NULL(p)
    *p = mP;
    INTERFACE_ADDREF(*p)
    return NOERROR;
}

ECode CDSAPublicKeySpec::GetQ(
    /* [out] */ IBigInteger **q)
{
    VALIDATE_NOT_NULL(q)
    *q = mQ;
    INTERFACE_ADDREF(*q)
    return NOERROR;
}

ECode CDSAPublicKeySpec::GetY(
    /* [out] */ IBigInteger **y)
{
    VALIDATE_NOT_NULL(y)
    *y = mY;
    INTERFACE_ADDREF(*y)
    return NOERROR;
}

ECode CDSAPublicKeySpec::constructor(
    /* [in] */ IBigInteger *y,
    /* [in] */ IBigInteger *p,
    /* [in] */ IBigInteger *q,
    /* [in] */ IBigInteger *g)
{
    mY = y;
    mP = p;
    mQ = q;
    mG = g;
    return NOERROR;
}

}
}
}

