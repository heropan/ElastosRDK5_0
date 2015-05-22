
#ifndef __BASICPERMISSION_H__
#define __BASICPERMISSION_H__

#ifdef ELASTOS_CORE
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include "Permission.h"

namespace Elastos {
namespace Security {

class BasicPermission
    : public Permission {
public:
    virtual CARAPI GetActions(
        /* [out] */ String *actions);

    virtual CARAPI Implies(
        /* [in] */ IPermission *permission,
        /* [out] */ Boolean *ret);

    CARAPI Init(
        /* [in] */ const String& name);

    CARAPI Init(
        /* [in] */ const String& name,
        /* [in] */ const String& action);
};

} // namespace Security
} // namespace Elastos

#endif // __BASICPERMISSION_H__
