
#ifndef __ELASTOS_DROID_SERVER_AM_CURIPERMISSIONOWNEREXTERNALTOKEN_H__
#define __ELASTOS_DROID_SERVER_AM_CURIPERMISSIONOWNEREXTERNALTOKEN_H__

#include "_CUriPermissionOwnerExternalToken.h"
#include "UriPermissionOwner.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

//==============================================================================
// UriPermissionOwner::ExternalToken
//==============================================================================

CarClass(CUriPermissionOwnerExternalToken)
{
public:
    CARAPI GetOwner(
        /* [out] */ Handle32 *owner);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    CARAPI ToString(
        /* [out] */ String *str);

    CARAPI constructor(
        /* [in] */ Handle32 owner);

public:
    AutoPtr<UriPermissionOwner> mUriOwner;
};

} // namespace Am
} // namespace Droid
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_AM_CURIPERMISSIONOWNEREXTERNALTOKEN_H__
