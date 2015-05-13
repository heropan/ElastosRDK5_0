
#ifndef __CWIFIMANAGERHELPER_H__
#define __CWIFIMANAGERHELPER_H__

#include "_CWifiManagerHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiManagerHelper)
{
public:
    CARAPI CalculateSignalLevel(
        /* [in] */ Int32 rssi,
        /* [in] */ Int32 numLevels,
        /* [out] */ Int32* level);

    CARAPI CompareSignalLevel(
        /* [in] */ Int32 rssiA,
        /* [in] */ Int32 rssiB,
        /* [out] */ Int32* result);
};

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //__CWIFIMANAGERHELPER_H__
