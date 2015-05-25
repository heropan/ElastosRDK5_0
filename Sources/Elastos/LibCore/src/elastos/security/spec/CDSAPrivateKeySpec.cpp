
#include "CDSAPrivateKeySpec.h"
#include "cmdef.h"

namespace Elastos {
namespace Security {
namespace Spec {


ECode CDSAPrivateKeySpec::GetG(
    /* [out] */ IBigInteger **g)
{
    VALIDATE_NOT_NULL(g)
    *g = mG;
    INTERFACE_ADDREF(*g)
    return NOERROR;
}

ECode CDSAPrivateKeySpec::GetP(
    /* [out] */ IBigInteger **p)
{
    VALIDATE_NOT_NULL(p)
    *p = mP;
    INTERFACE_ADDREF(*p)
    return NOERROR;
}

ECode CDSAPrivateKeySpec::GetQ(
    /* [out] */ IBigInteger **q)
{
    VALIDATE_NOT_NULL(q)
    *q = mQ;
    INTERFACE_ADDREF(*q)
    return NOERROR;
}

ECode CDSAPrivateKeySpec::GetX(
    /* [out] */ IBigInteger **x)
{
    VALIDATE_NOT_NULL(x)
    *x = mX;
    INTERFACE_ADDREF(*x)
    return NOERROR;
}

ECode CDSAPrivateKeySpec::constructor(
    /* [in] */ IBigInteger *x,
    /* [in] */ IBigInteger *p,
    /* [in] */ IBigInteger *q,
    /* [in] */ IBigInteger *g)
{
    mX = x;
    mP = p;
    mQ = q;
    mG = g;
    return NOERROR;
}

}
}
}

