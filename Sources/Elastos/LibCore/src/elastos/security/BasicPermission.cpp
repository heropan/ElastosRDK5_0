
#include "BasicPermission.h"

namespace Elastos {
namespace Security {

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

ECode BasicPermission::Init(
    /* [in] */ const String& name)
{
    return Permission::Init(String(""));
}

ECode BasicPermission::Init(
    /* [in] */ const String& name,
    /* [in] */ const String& action)
{
    return Permission::Init(String(""));
}

} // namespace Security
} // namespace Elastos
