
#include "CECGenParameterSpec.h"

namespace Elastos {
namespace Security {
namespace Spec {


ECode CECGenParameterSpec::GetName(
    /* [out] */ String *name)
{
    VALIDATE_NOT_NULL(name)
    *name = mName;
    return NOERROR;
}

ECode CECGenParameterSpec::constructor(
    /* [in] */ const String& name)
{
    mName = name;
    if (mName.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    return NOERROR;
}

}
}
}

