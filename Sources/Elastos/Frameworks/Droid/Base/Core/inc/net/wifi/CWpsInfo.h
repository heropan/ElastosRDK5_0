
#ifndef __ELASTOS_DROID_NET_WIFI_CWPSINFO_H__
#define __ELASTOS_DROID_NET_WIFI_CWPSINFO_H__

#include "_CWpsInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWpsInfo)
{
public:
    CARAPI GetSetup(
        /* [out] */ Int32* setup);

    CARAPI SetSetup(
        /* [in] */ Int32 setup);

    CARAPI GetBSSID(
        /* [out] */ String* BSSID);

    CARAPI SetBSSID(
        /* [in] */ const String& BSSID);

    CARAPI GetPin(
        /* [out] */ String* pin);

    CARAPI SetPin(
        /* [in] */ const String& pin);

    CARAPI ToString(
        /* [out] */ String* value);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IWpsInfo* source);

private:
    Int32 mSetup;
    String mBSSID;
    String mPin;

friend class WifiConfigStore;
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_WIFI_CWPSINFO_H__
