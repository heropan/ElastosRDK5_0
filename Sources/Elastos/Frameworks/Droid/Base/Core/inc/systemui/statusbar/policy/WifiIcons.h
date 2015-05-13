
#ifndef __WIFIICONS_H__
#define __WIFIICONS_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Policy {

class WifiIcons
{
public:
    static const Int32 WIFI_SIGNAL_STRENGTH[2][5];

    static const Int32 QS_WIFI_SIGNAL_STRENGTH[2][5];

    static const Int32 WIFI_LEVEL_COUNT;

private:
    WifiIcons();
    ~WifiIcons();
};


}// namespace Policy
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__WIFIICONS_H__
