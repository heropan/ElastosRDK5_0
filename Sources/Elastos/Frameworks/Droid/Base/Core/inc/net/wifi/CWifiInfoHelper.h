
#ifndef __ELASTOS_DROID_NET_WIFI_CWIFIINFOHELPER_H__
#define __ELASTOS_DROID_NET_WIFI_CWIFIINFOHELPER_H__

#include "_CWifiInfoHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiInfoHelper)
{
public:
    CARAPI GetDetailedStateOf(
        /* [in] */ SupplicantState suppState,
        /* [out] */ NetworkInfoDetailedState* detailedState);

    CARAPI RemoveDoubleQuotes(
        /* [in] */ const String& str,
        /* [out] */ String* value);
};

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_NET_WIFI_CWIFIINFOHELPER_H__
