
#include "BasicPermission.h"

namespace Elastos {
namespace Security {

BasicPermission::BasicPermission(
    /* [in] */ const String& name)
    : Permission(String(""))
{
}

BasicPermission::BasicPermission(
    /* [in] */ const String& name,
    /* [in] */ const String& action)
    : Permission(String(""))
{
}

ECode BasicPermission::GetActions(
    /* [out] */ String *actions)
{
    VALIDATE_NOT_NULL(actions)
    *actions = String();
    return NOERROR;
}

ECode BasicPermission::Implies(
    /* [in] */ IPermission *permission,
    /* [out] */ Boolean *ret)
{
    VALIDATE_NOT_NULL(ret)
    *ret = TRUE;
    return NOERROR;
}

ECode BasicPermission::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    return Permission::GetName(name);
}

ECode BasicPermission::NewPermissionCollection(
    /* [out] */ IPermissionCollection** permissions)
{
    VALIDATE_NOT_NULL(permissions)
    return Permission::NewPermissionCollection(permissions);
}

ECode BasicPermission::constructor(
    /* [in] */ const String& name)
{
    return Permission::Init(String(""));
}

ECode BasicPermission::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& action)
{
    return Permission::Init(String(""));
}

} // namespace Security
} // namespace Elastos
