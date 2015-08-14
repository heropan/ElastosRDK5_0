#ifndef __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PPROVDISCEVENT_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PPROVDISCEVENT_H__

#include "_CWifiP2pProvDiscEvent.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {

CarClass(CWifiP2pProvDiscEvent)
{
public:
    CWifiP2pProvDiscEvent();

    ~CWifiP2pProvDiscEvent();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& string);

    CARAPI GetEvent(
        /* [out] */ Int32* event);

    CARAPI SetEvent(
        /* [in] */ Int32 event);

    CARAPI GetDevice(
        /* [out] */ IWifiP2pDevice** device);

    CARAPI SetDevice(
        /* [in] */ IWifiP2pDevice* device);

    CARAPI GetPin(
        /* [out] */ String* pin);

    CARAPI SetPin(
        /* [in] */ const String& pin);

    CARAPI ToString(
        /* [out] */ String* string);

public:
    static const String TAG;

    /* One of PBC_REQ, PBC_RSP, ENTER_PIN or SHOW_PIN */
    Int32 mEvent;

    AutoPtr<IWifiP2pDevice> mDevice;

    /* Valid when event = SHOW_PIN */
    String mPin;
};

}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PPROVDISCEVENT_H__
