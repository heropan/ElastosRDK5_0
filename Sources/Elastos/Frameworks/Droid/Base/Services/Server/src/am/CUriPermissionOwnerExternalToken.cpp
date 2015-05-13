
#include <elastos/StringBuilder.h>
#include "am/CUriPermissionOwnerExternalToken.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

ECode CUriPermissionOwnerExternalToken::GetOwner(
    /* [out] */ Handle32 *owner)
{
    VALIDATE_NOT_NULL(owner)
    *owner = (Handle32)mUriOwner.Get();
    return NOERROR;
}

ECode CUriPermissionOwnerExternalToken::GetHashCode(
    /* [out] */ Int32 *hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)this;
    return NOERROR;
}

ECode CUriPermissionOwnerExternalToken::ToString(
    /* [out] */ String *str)
{
    VALIDATE_NOT_NULL(str);
    StringBuilder sb("UriPermissionOwner::ExternalToken: ");
    sb += (Int32)this;
    *str = sb.ToString();
    return NOERROR;
}

ECode CUriPermissionOwnerExternalToken::constructor(
    /* [in] */ Handle32 owner)
{
    mUriOwner = (UriPermissionOwner*)owner;
    return NOERROR;
}

} // namespace Am
} // namespace Droid
} // namespace Droid
} // namespace Elastos