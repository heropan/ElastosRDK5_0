
#ifndef __STATECHANGERESULT_H__
#define __STATECHANGERESULT_H__

#include "_CStateChangeResult.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CStateChangeResult)
{
public:
    CStateChangeResult();

    CARAPI constructor(
        /* [in] */ Int32 networkId,
        /* [in] */ IWifiSsid* wifiSsid,
        /* [in] */ const String& BSSID,
        /* [in] */ SupplicantState pState);

    CARAPI GetWifiSsid(
        /* [out] */ IWifiSsid** ssid);

    CARAPI SetWifiSsid(
        /* [in] */ IWifiSsid* ssid);

    CARAPI GetBSSID(
        /* [out] */ String* bssid);

    CARAPI SetBSSID(
        /* [in] */ const String& bssid);

    CARAPI GetNetworkId(
        /* [out] */ Int32* networkId);

    CARAPI SetNetworkId(
        /* [in] */ Int32 networkId);

    CARAPI GetSupplicantState(
        /* [out] */ SupplicantState* state);

    CARAPI SetSupplicantState(
        /* [in] */ SupplicantState state);

private:
    Int32 mNetworkId;
    AutoPtr<IWifiSsid> mWifiSsid;
    String mBSSID;
    SupplicantState mState;
};

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namepsace Elastos

#endif // __STATECHANGERESULT_H__
