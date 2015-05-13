
#ifndef __ACCESSIBILITYCONTENTDESCRIPTIONS_H__
#define __ACCESSIBILITYCONTENTDESCRIPTIONS_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Policy {

class AccessibilityContentDescriptions
{
public:
    static const Int32 PHONE_SIGNAL_STRENGTH[5];

    static const Int32 DATA_CONNECTION_STRENGTH[5];

    static const Int32 WIFI_CONNECTION_STRENGTH[5];

    static const Int32 WIMAX_CONNECTION_STRENGTH[5];

private:
    AccessibilityContentDescriptions();
    ~AccessibilityContentDescriptions();
};


}// namespace Policy
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__ACCESSIBILITYCONTENTDESCRIPTIONS_H__
