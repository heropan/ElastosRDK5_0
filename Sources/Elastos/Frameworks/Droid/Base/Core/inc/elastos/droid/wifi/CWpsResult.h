
#ifndef __ELASTOS_DROID_NET_WIFI_CWPSRESULT_H__
#define __ELASTOS_DROID_NET_WIFI_CWPSRESULT_H__

#include "_Elastos_Droid_Net_Wifi_CWpsResult.h"

namespace Elastos {
namespace Droid {
namespace Wifi {

CarClass(CWpsResult)
{
public:
    CARAPI GetStatus(
        /* [out] */ WpsResultStatus* status);

    CARAPI SetStatus(
        /* [in] */ WpsResultStatus status);

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
        /* [in] */ WpsResultStatus status);

    CARAPI constructor(
        /* [in] */ IWpsResult* source);

private:
    WpsResultStatus mStatus;
    String mPin;

friend class WifiConfigStore;
};

}
}
}

#endif // __ELASTOS_DROID_NET_WIFI_CWPSRESULT_H__
