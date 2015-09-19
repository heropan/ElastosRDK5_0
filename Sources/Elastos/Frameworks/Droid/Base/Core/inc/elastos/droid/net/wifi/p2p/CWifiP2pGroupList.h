
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PGROUPLIST_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PGROUPLIST_H__

#include "_Elastos_Droid_Net_Wifi_P2p_CWifiP2pGroupList.h"
#include "utility/LruCache.h"

using Elastos::Core::IInteger32;
using Elastos::Droid::Utility::LruCache;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {

CarClass(CWifiP2pGroupList)
{
private:
    class GroupLruCache
        : public LruCache< AutoPtr<IInteger32>, AutoPtr<IWifiP2pGroup> >
    {
    public:
        GroupLruCache(
            /* [in] */ Int32 size,
            /* [in] */ CWifiP2pGroupList* host);

        virtual CARAPI_(void) EntryRemoved(
            /* [in] */ Boolean evicted,
            /* [in] */ IInteger32* netId,
            /* [in] */ AutoPtr<IWifiP2pGroup> oldValue,
            /* [in] */ AutoPtr<IWifiP2pGroup> newValue);
    private:
        CWifiP2pGroupList* mHost;
    };

public:
    CWifiP2pGroupList();

    ~CWifiP2pGroupList();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IWifiP2pGroupList* source,
        /* [in] */ IGroupDeleteListener* listener);

    CARAPI GetGroupList(
        /* [out, callee] */ ArrayOf<IWifiP2pGroup*>** list);

    CARAPI Add(
        /* [in] */ IWifiP2pGroup* group);

    CARAPI Remove(
        /* [in] */ Int32 netId);

    CARAPI Remove(
        /* [in] */ const String& deviceAddress);

    CARAPI Clear(
        /* [out] */ Boolean* ret);

    CARAPI GetNetworkId(
        /* [in] */ const String& deviceAddress,
        /* [out] */ Int32* networkId);

    CARAPI GetNetworkId(
        /* [in] */ const String& deviceAddress,
        /* [in] */ const String& ssid,
        /* [out] */ Int32* networkId);

    CARAPI GetOwnerAddr(
        /* [in] */ Int32 netId,
        /* [out] */ String* addr);

    CARAPI Contains(
        /* [in] */ Int32 netId,
        /* [out] */ Boolean* ret);

    CARAPI ToString(
        /* [out] */ String* string);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    static const Int32 CREDENTIAL_MAX_NUM;
    GroupLruCache* mGroups;

    AutoPtr<IGroupDeleteListener> mListener;

    Boolean mIsClearCalled;
};

}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PGROUPLIST_H__
