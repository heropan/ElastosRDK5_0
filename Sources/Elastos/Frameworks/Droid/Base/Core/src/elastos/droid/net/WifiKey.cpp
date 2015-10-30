
#include "elastos/droid/net/WifiKey.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(WifiKey, Object, IParcelable, IWifiKey)

WifiKey::WifiKey()
{}

ECode WifiKey::constructor(
    /* [in] */ const String& ssid,
    /* [in] */ const String& bssid)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!SSID_PATTERN.matcher(ssid).matches()) {
            throw new IllegalArgumentException("Invalid ssid: " + ssid);
        }
        if (!BSSID_PATTERN.matcher(bssid).matches()) {
            throw new IllegalArgumentException("Invalid bssid: " + bssid);
        }
        this.ssid = ssid;
        this.bssid = bssid;

#endif
}

WifiKey::WifiKey(
    /* [in] */ IParcel* in)
{
#if 0 // TODO: Translate codes below
        ssid = in.readString();
        bssid = in.readString();

#endif
}

ECode WifiKey::Equals(
    /* [in] */ IObject* o,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        WifiKey wifiKey = (WifiKey) o;
        return Objects.equals(ssid, wifiKey.ssid) && Objects.equals(bssid, wifiKey.bssid);

#endif
}

ECode WifiKey::HashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return Objects.hash(ssid, bssid);

#endif
}

ECode WifiKey::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return "WifiKey[SSID=" + ssid + ",BSSID=" + bssid + "]";

#endif
}

ECode WifiKey::ReadFromParcel(
        /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                @Override
                public WifiKey createFromParcel(Parcel in) {
                    return new WifiKey(in);
                }
                @Override
                public WifiKey[] newArray(int size) {
                    return new WifiKey[size];
                }

#endif
}

ECode WifiKey::WriteToParcel(
        /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                @Override
                public WifiKey createFromParcel(Parcel in) {
                    return new WifiKey(in);
                }
                @Override
                public WifiKey[] newArray(int size) {
                    return new WifiKey[size];
                }

#endif
}

ECode WifiKey::GetSsid(
        /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = SSID;
    return NOERROR;
}

ECode WifiKey::GetBssid(
        /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = BSSID;
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
