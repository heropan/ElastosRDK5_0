
#ifndef __CSCANRESULT_H__
#define __CSCANRESULT_H__

#include "_CScanResult.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CScanResult)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IWifiSsid* wifiSsid,
        /* [in] */ const String& BSSID,
        /* [in] */ const String& caps,
        /* [in] */ Int32 level,
        /* [in] */ Int32 frequency,
        /* [in] */ Int64 tsf);

    CARAPI constructor(
        /* [in] */ IScanResult* source);

    CARAPI GetSSID(
        /* [out] */ String* ssid);

    CARAPI SetSSID(
        /* [in] */ const String& ssid);

    CARAPI GetWifiSsid(
        /* [out] */ IWifiSsid** ssid);

    CARAPI SetWifiSsid(
        /* [in] */ IWifiSsid* ssid);

    CARAPI GetBSSID(
        /* [out] */ String* bssid);

    CARAPI SetBSSID(
        /* [in] */ const String& bssid);

    CARAPI GetCapabilities(
        /* [out] */ String* capabilities);

    CARAPI SetCapabilities(
        /* [in] */ const String& capabilities);

    CARAPI GetLevel(
        /* [out] */ Int32* level);

    CARAPI SetLevel(
        /* [in] */ Int32 level);

    CARAPI GetFrequency(
        /* [out] */ Int32* frequency);

    CARAPI SetFrequency(
        /* [in] */ Int32 frequency);

    CARAPI GetTimestamp(
        /* [out] */ Int64* timestamp);

    CARAPI SetTimestamp(
        /* [in] */ Int64 timestamp);

    CARAPI ToString(
        /* [out] */ String* value);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    String mSSID;
    AutoPtr<IWifiSsid> mWifiSsid;
    String mBSSID;
    String mCapabilities;
    Int32 mLevel;
    Int32 mFrequency;
    Int64 mTimestamp;
};

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __CSCANRESULT_H__
