
#ifndef __ALLPERMISSIONCOLLECTION_H__
#define __ALLPERMISSIONCOLLECTION_H__

#include "PermissionCollection.h"

#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

using Elastos::Utility::IEnumeration;

namespace Elastos {
namespace Security {

class AllPermissionCollection
    : public IPermissionCollection
    , public PermissionCollection
    , public ElRefBase {
public:
    CAR_INTERFACE_DECL()
    CARAPI Add(
        /* [in] */ IPermission *permission);

    CARAPI GetElements(
        /* [out] */ IEnumeration **elems);

    CARAPI Implies(
        /* [in] */ IPermission *permission,
        /* [out] */ Boolean *ret);

    CARAPI IsReadOnly(
        /* [out] */ Boolean *ret);

    CARAPI SetReadOnly();
};

} // namespace Security
} // namespace Elastos

#endif // __ALLPERMISSIONCOLLECTION_H__
