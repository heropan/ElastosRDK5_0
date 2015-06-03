
#include "CKeyStoreCallbackHandlerProtection.h"

namespace Elastos {
namespace Security {

ECode CKeyStoreCallbackHandlerProtection::GetCallbackHandler(
    /* [out] */ ICallbackHandler **handler)
{
    VALIDATE_NOT_NULL(handler)
    *handler = mCallbackHandler;
    REFCOUNT_ADD(*handler)
    return NOERROR;
}

ECode CKeyStoreCallbackHandlerProtection::constructor(
    /* [in] */ ICallbackHandler *handler)
{
    if (handler == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mCallbackHandler = handler;
    return NOERROR;
}

}
}

