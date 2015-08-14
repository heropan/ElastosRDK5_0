
#ifndef __ELASTOS_DROID_NET_WIFI_CWIFISSID_H__
#define __ELASTOS_DROID_NET_WIFI_CWIFISSID_H__

#include "_CWifiSsid.h"

using Elastos::IO::IByteArrayOutputStream;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiSsid)
{
public:
    CARAPI constructor();

    CARAPI GetOctets(
        /* [out, callee] */ ArrayOf<Byte>** octets);

    CARAPI GetHexString(
        /* [out] */ String* hexString);

    CARAPI ToString(
        /* [out] */ String* value);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    static CARAPI CreateFromAsciiEncoded(
        /* [in] */ const String& asciiEncoded,
        /* [out] */ IWifiSsid** wifiSsid);

    static CARAPI CreateFromHex(
        /* [in] */ const String& hexStr,
        /* [out] */ IWifiSsid** wifiSsid);

private:
    /* This function is equivalent to printf_decode() at src/utils/common.c in
     * the supplicant */
    CARAPI_(void) ConvertToBytes(
        /* [in] */ const String& asciiEncoded);

    CARAPI_(Boolean) IsArrayAllZeroes(
        /* [in] */ const ArrayOf<Byte>* ssidBytes);

private:
    AutoPtr<IByteArrayOutputStream> mOctets;
};

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_WIFI_CWIFISSID_H__
