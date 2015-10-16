
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/net/wifi/CScanResult.h"
#include "elastos/droid/net/wifi/CWifiSsid.h"
#include <elastos/core/StringBuffer.h>

using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

ECode CScanResult::constructor()
{
    return NOERROR;
}

ECode CScanResult::constructor(
    /* [in] */ IWifiSsid* wifiSsid,
    /* [in] */ const String& BSSID,
    /* [in] */ const String& caps,
    /* [in] */ Int32 level,
    /* [in] */ Int32 frequency,
    /* [in] */ Int64 tsf)
{
    mWifiSsid = wifiSsid;
    if (wifiSsid != NULL) {
        wifiSsid->ToString(&mSSID);
    }
    else {
        mSSID = IWifiSsid::NONE;
    }
    mBSSID = BSSID;
    mCapabilities = caps;
    mLevel = level;
    mFrequency = frequency;
    mTimestamp = tsf;
    return NOERROR;
}

ECode CScanResult::constructor(
    /* [in] */ IScanResult* source)
{
    if (source != NULL) {
        source->GetWifiSsid((IWifiSsid**)&mWifiSsid);
        source->GetSSID(&mSSID);
        source->GetBSSID(&mBSSID);
        source->GetCapabilities(&mCapabilities);
        source->GetLevel(&mLevel);
        source->GetFrequency(&mFrequency);
        source->GetTimestamp(&mTimestamp);
    }
    return NOERROR;
}

ECode CScanResult::GetSSID(
    /* [out] */ String* ssid)
{
    VALIDATE_NOT_NULL(ssid)
    *ssid = mSSID;
    return NOERROR;
}

ECode CScanResult::SetSSID(
    /* [in] */ const String& ssid)
{
    mSSID = ssid;
    return NOERROR;
}

ECode CScanResult::GetWifiSsid(
    /* [out] */ IWifiSsid** ssid)
{
    VALIDATE_NOT_NULL(ssid)
    *ssid = mWifiSsid;
    REFCOUNT_ADD(*ssid)
    return NOERROR;
}

ECode CScanResult::SetWifiSsid(
    /* [in] */ IWifiSsid* ssid)
{
    mWifiSsid = ssid;
    return NOERROR;
}

ECode CScanResult::GetBSSID(
    /* [out] */ String* bssid)
{
    VALIDATE_NOT_NULL(bssid)
    *bssid = mBSSID;
    return NOERROR;
}

ECode CScanResult::SetBSSID(
    /* [in] */ const String& bssid)
{
    mBSSID = bssid;
    return NOERROR;
}

ECode CScanResult::GetCapabilities(
    /* [out] */ String* capabilities)
{
    VALIDATE_NOT_NULL(capabilities)
    *capabilities = mCapabilities;
    return NOERROR;
}

ECode CScanResult::SetCapabilities(
    /* [in] */ const String& capabilities)
{
    mCapabilities = capabilities;
    return NOERROR;
}

ECode CScanResult::GetLevel(
    /* [out] */ Int32* level)
{
    VALIDATE_NOT_NULL(level)
    *level = mLevel;
    return NOERROR;
}

ECode CScanResult::SetLevel(
    /* [in] */ Int32 level)
{
    mLevel = level;
    return NOERROR;
}

ECode CScanResult::GetFrequency(
    /* [out] */ Int32* frequency)
{
    VALIDATE_NOT_NULL(frequency)
    *frequency = mFrequency;
    return NOERROR;
}

ECode CScanResult::SetFrequency(
    /* [in] */ Int32 frequency)
{
    mFrequency = frequency;
    return NOERROR;
}

ECode CScanResult::GetTimestamp(
    /* [out] */ Int64* timestamp)
{
    VALIDATE_NOT_NULL(timestamp)
    *timestamp = mTimestamp;
    return NOERROR;
}

ECode CScanResult::SetTimestamp(
    /* [in] */ Int64 timestamp)
{
    mTimestamp = timestamp;
    return NOERROR;
}

ECode CScanResult::ToString(
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    StringBuffer sb;
    String none("<none>");

    sb.Append("SSID: ");
    if (mWifiSsid == NULL) {
        sb.Append(IWifiSsid::NONE);
    }
    else {
        String temp;
        mWifiSsid->ToString(&temp);
        sb.Append(temp);
    }

    sb.Append(", BSSID: ");
    sb.Append(mBSSID.IsNull() ? none : mBSSID);
    sb.Append(", capabilities: ");
    sb.Append(mCapabilities.IsNull() ? none : mCapabilities);
    sb.Append(", level: ");
    sb.Append(mLevel);
    sb.Append(", frequency: ");
    sb.Append(mFrequency);
    sb.Append(", timestamp: ");
    sb.Append(mTimestamp);
    *value = sb.ToString();
    return NOERROR;
}

ECode CScanResult::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    AutoPtr<IInterface> obj;
    source->ReadInterfacePtr((Handle32*)&obj);
    mWifiSsid = IWifiSsid::Probe(obj);
    if (mWifiSsid != NULL) {
        mWifiSsid->ToString(&mSSID);
    }
    else mSSID = IWifiSsid::NONE;
    source->ReadString(&mBSSID);
    source->ReadString(&mCapabilities);
    source->ReadInt32(&mLevel);
    source->ReadInt32(&mFrequency);
    source->ReadInt64(&mTimestamp);
    return NOERROR;
}

ECode CScanResult::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInterfacePtr(mWifiSsid.Get());
    dest->WriteString(mBSSID);
    dest->WriteString(mCapabilities);
    dest->WriteInt32(mLevel);
    dest->WriteInt32(mFrequency);
    dest->WriteInt64(mTimestamp);
    return NOERROR;
}

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos
