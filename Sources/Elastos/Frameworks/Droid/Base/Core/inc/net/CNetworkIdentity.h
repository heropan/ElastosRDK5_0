
#ifndef __ELASTOS_DROID_NET_CNETWORKIDENTITY_H__
#define __ELASTOS_DROID_NET_CNETWORKIDENTITY_H__

#include "_CNetworkIdentity.h"
#include "os/Build.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::Build;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkIdentity)
{
public:
    CARAPI constructor(
        /* [in] */ Int32 nType,
        /* [in] */ Int32 subType,
        /* [in] */ const String& subscriberId,
        /* [in] */ const String& networkId,
        /* [in] */ Boolean roaming);

    CARAPI GetHashCode(
        /* [out] */ Int32* result);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result );

    CARAPI Equals(
        /* [in] */ INetworkIdentity* obj,
        /* [out] */ Boolean* result );

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI GetType(
        /* [out] */ Int32* result);

    CARAPI GetSubType(
        /* [out] */ Int32* result);

    CARAPI GetSubscriberId(
        /* [out] */ String* result);

    CARAPI GetNetworkId(
        /* [out] */ String* result);

    CARAPI GetRoaming(
        /* [out] */ Boolean* result);

    static CARAPI ScrubSubscriberId(
        /* [in] */ const String& subscriberId,
        /* [out] */ String* result);

    static CARAPI BuildNetworkIdentity(
        /* [in] */ IContext* context,
        /* [in] */ INetworkState* state,
        /* [out] */ INetworkIdentity** result);

public:
    /**
     * When enabled, combine all {@link #mSubType} together under
     * {@link #SUBTYPE_COMBINED}.
     */

    Int32 mType;
    Int32 mSubType;
    String mSubscriberId;
    String mNetworkId;
    Boolean mRoaming;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CNETWORKIDENTITY_H__

