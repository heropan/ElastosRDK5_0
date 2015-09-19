
#ifndef __ELASTOS_DROID_NET_CNETWORKSTATE_H__
#define __ELASTOS_DROID_NET_CNETWORKSTATE_H__

#include "_Elastos_Droid_Net_CNetworkState.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkState)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ INetworkInfo* networkInfo,
        /* [in] */ ILinkProperties* linkProperties,
        /* [in] */ ILinkCapabilities* linkCapabilities);

    CARAPI constructor(
        /* [in] */ INetworkInfo* networkInfo,
        /* [in] */ ILinkProperties* linkProperties,
        /* [in] */ ILinkCapabilities* linkCapabilities,
        /* [in] */ const String& subscriberId,
        /* [in] */ const String& networkId);

    CARAPI GetNetworkInfo(
        /* [out] */ INetworkInfo** networkInfo);

    CARAPI GetLinkProperties(
        /* [out] */ ILinkProperties** linkProperties);

    CARAPI GetLinkCapabilities(
        /* [out] */ ILinkCapabilities** linkCapabilities);

    CARAPI GetSubscriberId(
        /* [out] */ String* subscriberId);

    CARAPI GetNetworkId(
        /* [out] */ String* networkId);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

public:
    AutoPtr<INetworkInfo> mNetworkInfo;
    AutoPtr<ILinkProperties> mLinkProperties;
    AutoPtr<ILinkCapabilities> mLinkCapabilities;
    /** Currently only used by testing. */
    String mSubscriberId;
    String mNetworkId;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CNETWORKSTATE_H__

