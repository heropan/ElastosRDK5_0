
#include "elastos/droid/net/wifi/CWifiSsidHelper.h"
#include "elastos/droid/net/wifi/CWifiSsid.h"

namespace Elastos {
namespace Droid {
namespace Wifi {

ECode CWifiSsidHelper::CreateFromAsciiEncoded(
    /* [in] */ const String& asciiEncoded,
    /* [out] */ IWifiSsid** wifiSsid)
{
    return CWifiSsid::CreateFromAsciiEncoded(asciiEncoded, wifiSsid);
}

ECode CWifiSsidHelper::CreateFromHex(
    /* [in] */ const String& hexStr,
    /* [out] */ IWifiSsid** wifiSsid)
{
    return CWifiSsid::CreateFromHex(hexStr, wifiSsid);
}

} // namespace Wifi
} // namespace Droid
} // namespace Elastos
