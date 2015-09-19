#ifndef __ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYMANAGERCLIENT_H__
#define __ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYMANAGERCLIENT_H__

#include "_Elastos_Droid_View_Accessibility_CAccessibilityManagerClient.h"
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

#endif //__ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYMANAGERCLIENT_H__
