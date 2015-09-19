#ifndef __ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYRECORDHELPER_H__
#define __ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYRECORDHELPER_H__

#include "_Elastos_Droid_View_Accessibility_CAccessibilityRecordHelper.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CAccessibilityRecordHelper)
{
public:
    /**
     * Returns a cached instance if such is available or a new one is
     * instantiated. The instance is initialized with data from the
     * given record.
     *
     * @return An instance.
     */
    CARAPI Obtain(
        /* [in] */ IAccessibilityRecord* record,
        /* [out] */ IAccessibilityRecord** newInstance);

    /**
     * Returns a cached instance if such is available or a new one is
     * instantiated.
     *
     * @return An instance.
     */
    CARAPI Obtain(
        /* [out] */ IAccessibilityRecord** newInstance);
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYRECORDHELPER_H__
