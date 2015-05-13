
#include "PermissionCollection.h"
#include "cmdef.h"

namespace Elastos {
namespace Security {

ECode PermissionCollection::IsReadOnly(
    /* [out] */ Boolean *ret)
{
    VALIDATE_NOT_NULL(ret)
    *ret = TRUE;
    return NOERROR;
}

ECode PermissionCollection::SetReadOnly()
{
    return NOERROR;
}

} // namespace Security
} // namespace Elastos