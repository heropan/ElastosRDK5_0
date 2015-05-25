
#ifndef __CAUTHPERMISSION_H__
#define __CAUTHPERMISSION_H__

#include "_CAuthPermission.h"
#include "BasicPermission.h"

using Elastos::Security::IPermissionCollection;
using Elastos::Security::IPermission;
using Elastos::Security::BasicPermission;

namespace Elastosx {
namespace Security {
namespace Auth {

CarClass(CAuthPermission), public BasicPermission
{
public:
    CARAPI CheckGuard(
        /* [in] */ IInterface *object);

    CARAPI GetName(
        /* [out] */ String *name);

    CARAPI NewPermissionCollection(
        /* [out] */ IPermissionCollection **permissions);

    CARAPI GetActions(
        /* [out] */ String *actions);

    CARAPI Implies(
        /* [in] */ IPermission *permission,
        /* [out] */ Boolean *result);

    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& actions);
};

}
}
}

#endif // __CAUTHPERMISSION_H__
