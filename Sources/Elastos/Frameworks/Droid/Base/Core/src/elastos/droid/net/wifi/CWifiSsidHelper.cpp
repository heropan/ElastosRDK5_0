
#include "net/wifi/CWifiSsidHelper.h"
#include "net/wifi/CWifiSsid.h"

namespace Elastos {
namespace Droid {
namespace Net {
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
} // namespace Net
} // namespace Droid
} // namespace Elastos
