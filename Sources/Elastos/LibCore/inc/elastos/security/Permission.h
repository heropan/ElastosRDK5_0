
#ifndef __PERMISSION_H__
#define __PERMISSION_H__

#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

namespace Elastos {
namespace Security {

class Permission {
public:
    CARAPI Init(
        /* [in] */ const String& name);

    virtual CARAPI GetName(
        /* [out] */ String *name) const;

    virtual CARAPI CheckGuard(
        /* [in] */ IInterface *obj);

    virtual CARAPI NewPermissionCollection(
        /* [out] */ IPermissionCollection **coll);

    virtual CARAPI GetActions(
        /* [out] */ String *actions) = 0;

    virtual CARAPI Implies(
        /* [in] */ IPermission *permission,
        /* [out] */ Boolean *ret) = 0;
};

} // namespace Security
} // namespace Elastos

#endif // __PERMISSION_H__
