
#ifndef __ELASTOS_DROID_NET_WIFI_CSUPPLICANTSTATEHELPER_H__
#define __ELASTOS_DROID_NET_WIFI_CSUPPLICANTSTATEHELPER_H__

#include "_Elastos_Droid_Net_Wifi_CSupplicantStateHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CSupplicantStateHelper)
{
public:
    CARAPI IsValidState(
        /* [in] */ Elastos::Droid::Net::Wifi::SupplicantState state,
        /* [out] */ Boolean* isValidState);

    CARAPI IsHandshakeState(
        /* [in] */ Elastos::Droid::Net::Wifi::SupplicantState state,
        /* [out] */ Boolean* isHandshakeState);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

private:
    CARAPI IsConnecting(
        /* [in] */ Elastos::Droid::Net::Wifi::SupplicantState state,
        /* [out] */ Boolean* isConnecting);

    CARAPI IsDriverActive(
        /* [in] */ Elastos::Droid::Net::Wifi::SupplicantState state,
        /* [out] */ Boolean* isDriverActive);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_WIFI_CSUPPLICANTSTATEHELPER_H__
