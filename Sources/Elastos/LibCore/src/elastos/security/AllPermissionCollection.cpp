
#include "AllPermissionCollection.h"

namespace Elastos {
namespace Security {

CAR_INTERFACE_IMPL(AllPermissionCollection, IPermissionCollection)

ECode AllPermissionCollection::Add(
    /* [in] */ IPermission *permission)
{
    return NOERROR;
}

ECode AllPermissionCollection::GetElements(
    /* [out] */ IEnumeration **elems)
{
    return NOERROR;
}

ECode AllPermissionCollection::Implies(
    /* [in] */ IPermission *permission,
    /* [out] */ Boolean *ret)
{
    VALIDATE_NOT_NULL(ret)
    *ret = TRUE;
    return NOERROR;
}

ECode AllPermissionCollection::IsReadOnly(
    /* [out] */ Boolean *ret)
{
    return PermissionCollection::IsReadOnly(ret);
}

ECode AllPermissionCollection::SetReadOnly()
{
    return PermissionCollection::SetReadOnly();
}

} // namespace Security
} // namespace Elastos
