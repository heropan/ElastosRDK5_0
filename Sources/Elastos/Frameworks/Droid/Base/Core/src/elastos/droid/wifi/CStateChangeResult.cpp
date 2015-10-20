
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/net/wifi/CStateChangeResult.h"

namespace Elastos {
namespace Droid {
namespace Wifi {

CStateChangeResult::CStateChangeResult()
    : mNetworkId(0)
    , mState(SupplicantState_INVALID)
{}

ECode CStateChangeResult::constructor(
    /* [in] */ Int32 networkId,
    /* [in] */ IWifiSsid* wifiSsid,
    /* [in] */ const String& BSSID,
    /* [in] */ SupplicantState state)
{
    mNetworkId = networkId;
    mWifiSsid = wifiSsid;
    mBSSID = BSSID;
    mState = state;
    return NOERROR;
}

ECode CStateChangeResult::GetWifiSsid(
    /* [out] */ IWifiSsid** ssid)
{
    VALIDATE_NOT_NULL(ssid);
    *ssid = mWifiSsid;
    REFCOUNT_ADD(*ssid);
    return NOERROR;
}

ECode CStateChangeResult::SetWifiSsid(
    /* [in] */ IWifiSsid* ssid)
{
    mWifiSsid = ssid;
    return NOERROR;
}

ECode CStateChangeResult::GetBSSID(
    /* [out] */ String* bssid)
{
    VALIDATE_NOT_NULL(bssid);
    *bssid = mBSSID;
    return NOERROR;
}

ECode CStateChangeResult::SetBSSID(
    /* [in] */ const String& bssid)
{
    mBSSID = bssid;
    return NOERROR;
}

ECode CStateChangeResult::GetNetworkId(
    /* [out] */ Int32* networkId)
{
    VALIDATE_NOT_NULL(networkId);
    *networkId = mNetworkId;
    return NOERROR;
}

ECode CStateChangeResult::SetNetworkId(
    /* [in] */ Int32 networkId)
{
    mNetworkId = networkId;
    return NOERROR;
}

ECode CStateChangeResult::GetSupplicantState(
    /* [out] */ SupplicantState* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mState;
    return NOERROR;
}

ECode CStateChangeResult::SetSupplicantState(
    /* [in] */ SupplicantState state)
{
    mState = state;
    return NOERROR;
}

} // namespace Wifi
} // namespace Droid
} // namepsace Elastos
