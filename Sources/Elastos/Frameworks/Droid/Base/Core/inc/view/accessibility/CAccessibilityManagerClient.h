#ifndef __CACCESSIBILITYMANAGERCLIENT_H__
#define __CACCESSIBILITYMANAGERCLIENT_H__

#include "_CAccessibilityManagerClient.h"
#include "view/accessibility/CAccessibilityManager.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CAccessibilityManagerClient)
{
public:
    CARAPI SetState(
        /* [in] */ Int32 stateFlags);

    CARAPI constructor(
        /* [in] */ Handle32 host);

private:
    AutoPtr<CAccessibilityManager> mHost;
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__CACCESSIBILITYMANAGERCLIENT_H__
