
#ifndef __CSUBJECTHELPER_H__
#define __CSUBJECTHELPER_H__

#include "_CSubjectHelper.h"

using Elastos::Security::IPrivilegedAction;
using Elastos::Security::IAccessControlContext;
using Elastos::Security::IPrivilegedExceptionAction;
using Elastos::Security::IAccessControlContext;

namespace Elastosx {
namespace Security {
namespace Auth {

CarClass(CSubjectHelper)
{
public:
    CARAPI DoAs(
        /* [in] */ ISubject *subject,
        /* [in] */ IPrivilegedAction *action,
        /* [out] */ IInterface **obj);

    CARAPI DoAsPrivileged(
        /* [in] */ ISubject *subject,
        /* [in] */ IPrivilegedAction *action,
        /* [in] */ IAccessControlContext *context,
        /* [out] */ IInterface **obj);

    CARAPI DoAs(
        /* [in] */ ISubject *subject,
        /* [in] */ IPrivilegedExceptionAction *action,
        /* [out] */ IInterface **obj);

    CARAPI DoAsPrivileged(
        /* [in] */ ISubject *subject,
        /* [in] */ IPrivilegedExceptionAction *action,
        /* [in] */ IAccessControlContext *context,
        /* [out] */ IInterface **obj);

    CARAPI GetSubject(
        /* [in] */ IAccessControlContext *context,
        /* [out] */ ISubject **subject);
};

}
}
}

#endif // __CSUBJECTHELPER_H__
