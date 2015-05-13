
#ifndef __ALLPERMISSIONCOLLECTION_H__
#define __ALLPERMISSIONCOLLECTION_H__

#include "PermissionCollection.h"
#include "cmdef.h"

#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#else
#include "Elastos.Core.h"
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

    CARAPI Elements(
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
