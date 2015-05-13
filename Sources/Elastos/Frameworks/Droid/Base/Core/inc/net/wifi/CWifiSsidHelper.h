
#ifndef __CWIFISSIDHELPER_H__
#define __CWIFISSIDHELPER_H__

#include "_CWifiSsidHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiSsidHelper)
{
public:
    CARAPI CreateFromAsciiEncoded(
        /* [in] */ const String& asciiEncoded,
        /* [out] */ IWifiSsid** wifiSsid);

    CARAPI CreateFromHex(
        /* [in] */ const String& hexStr,
        /* [out] */ IWifiSsid** wifiSsid);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}

#endif // __CWIFISSIDHELPER_H__
