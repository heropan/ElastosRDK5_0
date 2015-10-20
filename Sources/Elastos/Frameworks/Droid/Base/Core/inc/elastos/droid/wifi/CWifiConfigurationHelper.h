
#ifndef __ELASTOS_DROID_NET_WIFI_CWIFICONFIGURATIONHELPER_H__
#define __ELASTOS_DROID_NET_WIFI_CWIFICONFIGURATIONHELPER_H__

#include "_Elastos_Droid_Net_Wifi_CWifiConfigurationHelper.h"

namespace Elastos {
namespace Droid {
namespace Wifi {

CarClass(CWifiConfigurationHelper)
{
public:
    CARAPI GetWepKeyVarName(
        /* [in] */ Int32 index,
        /* [out] */ String* wepKeyVarName);
};

} // namespace Wifi
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_NET_WIFI_CWIFICONFIGURATIONHELPER_H__
