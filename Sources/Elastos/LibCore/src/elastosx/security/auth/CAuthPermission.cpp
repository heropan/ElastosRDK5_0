
#include "CAuthPermission.h"
#include "AllPermissionCollection.h"

using Elastos::Security::AllPermissionCollection;

namespace Elastosx {
namespace Security {
namespace Auth {

ECode CAuthPermission::CheckGuard(
    /* [in] */ IInterface *object)
{
    return NOERROR;
}

ECode CAuthPermission::GetName(
    /* [out] */ String *name)
{
    VALIDATE_NOT_NULL(name)
    *name = String();
    return NOERROR;
}

ECode CAuthPermission::NewPermissionCollection(
    /* [out] */ IPermissionCollection **permissions)
{
    VALIDATE_NOT_NULL(permissions)
    *permissions = new AllPermissionCollection();
    REFCOUNT_ADD(*permissions)
    return NOERROR;
}

ECode CAuthPermission::GetActions(
    /* [out] */ String *actions)
{
    return BasicPermission::GetActions(actions);
}

ECode CAuthPermission::Implies(
    /* [in] */ IPermission *permission,
    /* [out] */ Boolean *result)
{
    return BasicPermission::Implies(permission, result);
}

ECode CAuthPermission::constructor(
    /* [in] */ const String& name)
{
    return BasicPermission::Init(String(""));
}

ECode CAuthPermission::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& actions)
{
    return BasicPermission::Init(String(""), String(""));
}

}
}
}

