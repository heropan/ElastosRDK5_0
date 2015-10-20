
#ifndef __ELASTOS_DROID_NET_WIFI_CWIFICONFIGURATIONPROTOCOL_H__
#define __ELASTOS_DROID_NET_WIFI_CWIFICONFIGURATIONPROTOCOL_H__

#include "_Elastos_Droid_Net_Wifi_CWifiConfigurationProtocol.h"

namespace Elastos {
namespace Droid {
namespace Wifi {

CarClass(CWifiConfigurationProtocol)
{
public:
    CARAPI GetStrings(
        /* [out, callee] */ ArrayOf<String>** strings);

    CARAPI ToString(
        /* [in] */ Int32 index,
        /* [out] */ String* strValue);

public:
    static const AutoPtr< ArrayOf<String> > sStrings;
};

} // namespace Wifi
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_WIFI_CWIFICONFIGURATIONPROTOCOL_H__
