
#ifndef __CPRIVATECREDENTIALPERMISSION_H__
#define __CPRIVATECREDENTIALPERMISSION_H__

#include "_CPrivateCredentialPermission.h"
#include "Permission.h"

using Elastos::Security::IPermissionCollection;
using Elastos::Security::IPermission;
using Elastos::Security::Permission;

namespace Elastosx {
namespace Security {
namespace Auth {

CarClass(CPrivateCredentialPermission), public Permission
{
public:
    CARAPI CheckGuard(
        /* [in] */ IInterface *object);

    CARAPI GetName(
        /* [out] */ String *name);

    CARAPI NewPermissionCollection(
        /* [out] */ IPermissionCollection **permissions);

    CARAPI GetPrincipals(
        /* [out, callee] */ ArrayOf<IObjectContainer*> **principals);

    CARAPI GetCredentialClass(
        /* [out] */ String *credential);

    CARAPI GetActions(
        /* [out] */ String *actions);

    CARAPI Implies(
        /* [in] */ IPermission *permission,
        /* [out] */ Boolean *result);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& action);
};

}
}
}

#endif // __CPRIVATECREDENTIALPERMISSION_H__
