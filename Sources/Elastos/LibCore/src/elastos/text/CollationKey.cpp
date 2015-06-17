#include "CollationKey.h"

namespace Elastos {
namespace Text {

CAR_INTERFACE_IMPL(CollationKey, Object, ICollationKey)

CollationKey::CollationKey()
{}

CollationKey:: ~CollationKey()
{}

ECode CollationKey::constructor(
    /* [in] */ const String& source)
{
    mSource = String(source);
    return NOERROR;
}

ECode CollationKey::GetSourceString(
    /* [out] */ String* sourceString)
{
    VALIDATE_NOT_NULL(sourceString);
    *sourceString = mSource;
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
