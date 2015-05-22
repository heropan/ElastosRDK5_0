
#ifndef __PERMISSIONCOLLECTION_H__
#define __PERMISSIONCOLLECTION_H__

#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

using Elastos::Utility::IEnumeration;

namespace Elastos {
namespace Security {

class PermissionCollection {
public:
    virtual CARAPI Add(
        /* [in] */ IPermission *permission) = 0;

    virtual CARAPI Elements(
        /* [out] */ IEnumeration **elems) = 0;

    virtual CARAPI Implies(
        /* [in] */ IPermission *permission,
        /* [out] */ Boolean *ret) = 0;

    virtual CARAPI IsReadOnly(
        /* [out] */ Boolean *ret);

    virtual CARAPI SetReadOnly();
};

} // namespace Security
} // namespace Elastos

#endif // __PERMISSIONCOLLECTION_H__
