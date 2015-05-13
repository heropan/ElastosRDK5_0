
#include "content/CIntegerMapWrapper.h"

namespace Elastos {
namespace Droid {
namespace Content {

CIntegerMapWrapper::CIntegerMapWrapper()
    : mKey(0)
    , mValue(0)
{}

ECode CIntegerMapWrapper::GetKey(
    /* [out] */ Int32* key)
{
    VALIDATE_NOT_NULL(key);
    *key = mKey;
    return NOERROR;
}

ECode CIntegerMapWrapper::GetValue(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}

ECode CIntegerMapWrapper::constructor(
    /* [in] */ Int32 key,
    /* [in] */ Int32 value)
{
    mKey = key;
    mValue = value;
    return NOERROR;
}

}
}
}

