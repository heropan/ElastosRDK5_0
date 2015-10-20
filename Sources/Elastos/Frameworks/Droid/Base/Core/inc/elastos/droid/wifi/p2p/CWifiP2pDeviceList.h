
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PDEVICELIST_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PDEVICELIST_H__

#include "_Elastos_Droid_Net_Wifi_P2p_CWifiP2pDeviceList.h"
#include "CWifiP2pDevice.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;

namespace Elastos {
namespace Droid {
namespace Wifi {
namespace P2p {

/**
 * A class representing a Wi-Fi P2p device list.
 *
 * Note that the operations are not thread safe.
 * {@see WifiP2pManager}
 */
CarClass(CWifiP2pDeviceList)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IWifiP2pDeviceList* source);

    CARAPI constructor(
        /* [in] */ ArrayOf<IWifiP2pDevice*>* devices);

    CARAPI Clear(
        /* [out] */ Boolean* ret);

    CARAPI Update(
        /* [in] */ IWifiP2pDevice* device);

    CARAPI UpdateGroupCapability(
        /* [in] */ const String& deviceAddress,
        /* [in] */ Int32 groupCapab);

    CARAPI UpdateStatus(
        /* [in] */ const String& deviceAddress,
        /* [in] */ Int32 status);

    CARAPI Get(
        /* [in] */ const String& deviceAddress,
        /* [out] */ IWifiP2pDevice** device);

    CARAPI Remove(
        /* [in] */ IWifiP2pDevice* device,
        /* [out] */ Boolean* ret);

    CARAPI Remove(
        /* [in] */ IWifiP2pDeviceList* list,
        /* [out] */ Boolean* ret);

    CARAPI GetDeviceList(
        /* [out, callee] */ ArrayOf<IWifiP2pDevice*>** list);

    CARAPI IsGroupOwner(
        /* [in] */ const String& deviceAddress,
        /* [out] */ Boolean* isGroupOwner);

    CARAPI ToString(
        /* [out] */ String* string);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    HashMap<String, AutoPtr<IWifiP2pDevice> > mDevices;
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PDEVICELIST_H__
