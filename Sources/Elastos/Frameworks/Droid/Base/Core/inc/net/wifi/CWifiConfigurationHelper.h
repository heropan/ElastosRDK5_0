
#ifndef __ELASTOS_DROID_NET_WIFI_CWIFICONFIGURATIONHELPER_H__
#define __ELASTOS_DROID_NET_WIFI_CWIFICONFIGURATIONHELPER_H__

#include "_CWifiConfigurationHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiConfigurationHelper)
{
public:
    CARAPI GetWepKeyVarName(
        /* [in] */ Int32 index,
        /* [out] */ String* wepKeyVarName);
};

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_NET_WIFI_CWIFICONFIGURATIONHELPER_H__
