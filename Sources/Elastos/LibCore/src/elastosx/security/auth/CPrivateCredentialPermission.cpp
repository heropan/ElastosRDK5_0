
#include "CPrivateCredentialPermission.h"
#include "cmdef.h"

namespace Elastosx {
namespace Security {
namespace Auth {


ECode CPrivateCredentialPermission::CheckGuard(
    /* [in] */ IInterface *object)
{
    return Permission::CheckGuard(object);
}

ECode CPrivateCredentialPermission::GetName(
    /* [out] */ String *name)
{
    return Permission::GetName(name);
}

ECode CPrivateCredentialPermission::NewPermissionCollection(
    /* [out] */ IPermissionCollection **permissions)
{
    return Permission::NewPermissionCollection(permissions);
}

ECode CPrivateCredentialPermission::GetActions(
    /* [out] */ String *actions)
{
    VALIDATE_NOT_NULL(actions)
    *actions = String();
    return NOERROR;
}

ECode CPrivateCredentialPermission::Implies(
    /* [in] */ IPermission *permission,
    /* [out] */ Boolean *result)
{
    VALIDATE_NOT_NULL(result)
    *result = TRUE;
    return NOERROR;
}

ECode CPrivateCredentialPermission::GetPrincipals(
    /* [out, callee] */ ArrayOf<IObjectContainer*> **principals)
{
    return NOERROR;
}

ECode CPrivateCredentialPermission::GetCredentialClass(
    /* [out] */ String *credential)
{
    VALIDATE_NOT_NULL(credential)
    *credential = String();
    return NOERROR;
}

ECode CPrivateCredentialPermission::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& action)
{
    return Permission::Init(String(""));
}

}
}
}

