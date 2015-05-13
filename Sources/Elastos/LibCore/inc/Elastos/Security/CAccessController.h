
#ifndef __CACCESSCONTROLLER_H__
#define __CACCESSCONTROLLER_H__

#include "_CAccessController.h"

namespace Elastos {
namespace Security {

CarClass(CAccessController)
{
public:
    CARAPI DoPrivileged(
        /* [in] */ IPrivilegedAction *action,
        /* [out] */ IInterface **result);

    CARAPI DoPrivilegedEx(
        /* [in] */ IPrivilegedAction *action,
        /* [in] */ IAccessControlContext *context,
        /* [out] */ IInterface **result);

    CARAPI DoPrivilegedEx2(
        /* [in] */ IPrivilegedExceptionAction *action,
        /* [out] */ IInterface **result);

    CARAPI DoPrivilegedEx3(
        /* [in] */ IPrivilegedExceptionAction *action,
        /* [in] */ IAccessControlContext *context,
        /* [out] */ IInterface **result);

    CARAPI DoPrivilegedWithCombiner(
        /* [in] */ IPrivilegedAction *action,
        /* [out] */ IInterface **result);

    CARAPI DoPrivilegedWithCombinerEx(
        /* [in] */ IPrivilegedExceptionAction *action,
        /* [out] */ IInterface **result);

    CARAPI CheckPermission(
        /* [in] */ IPermission *permission);

    CARAPI GetContext(
        /* [out] */ IAccessControlContext **context);
};

}
}

#endif // __CACCESSCONTROLLER_H__
