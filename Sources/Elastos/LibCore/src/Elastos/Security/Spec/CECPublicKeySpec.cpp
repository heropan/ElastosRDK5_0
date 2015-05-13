
#include "CECPublicKeySpec.h"
#include "cmdef.h"
#include "CECPoint.h"

namespace Elastos {
namespace Security {
namespace Spec {


ECode CECPublicKeySpec::GetParams(
    /* [out] */ IECParameterSpec **params)
{
    VALIDATE_NOT_NULL(params)
    *params = mParams;
    INTERFACE_ADDREF(*params)
    return NOERROR;
}

ECode CECPublicKeySpec::GetW(
    /* [out] */ IECPoint **w)
{
    VALIDATE_NOT_NULL(w)
    *w = mW;
    INTERFACE_ADDREF(*w)
    return NOERROR;
}

ECode CECPublicKeySpec::constructor(
    /* [in] */ IECPoint *w,
    /* [in] */ IECParameterSpec *params)
{
    mW = w;
    mParams = params;
    // throw NullPointerException if w or params is null
    if (mW == NULL || mParams == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    // throw IllegalArgumentException if w is point at infinity
    Boolean equal;
    mW->Equals(CECPoint::POINT_INFINITY.Get(), &equal);
    if (equal) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

}
}
}

