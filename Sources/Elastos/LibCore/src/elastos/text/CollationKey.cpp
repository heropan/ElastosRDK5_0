#include "CollationKey.h"

namespace Elastos {
namespace Text {

CAR_INTERFACE_IMPL(CollationKey, Object, ICollationKey)

ECode CollationKey::Init(
    /* [in] */ const String& source)
{
    this->source = String(source);
    return NOERROR;
}

ECode CollationKey::GetSourceString(
    /* [out] */ String* sourceString)
{
    VALIDATE_NOT_NULL(sourceString);
    *sourceString = String(source);
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
