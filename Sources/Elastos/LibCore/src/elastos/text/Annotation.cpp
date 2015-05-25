#include "Annotation.h"

namespace Elastos {
namespace Text {

ECode Annotation::Init(
    /* [in] */ IInterface* attribute)
{
    mValue = attribute;
    return NOERROR;
}

ECode Annotation::GetValue(
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    INTERFACE_ADDREF(*value);
    return NOERROR;
}

ECode Annotation::ToString(
    /* [out] */ String * str)
{
    VALIDATE_NOT_NULL(str)
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
