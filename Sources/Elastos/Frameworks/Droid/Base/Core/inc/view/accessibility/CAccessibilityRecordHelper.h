#ifndef __CACCESSIBILITYRECORDHELPER_H__
#define __CACCESSIBILITYRECORDHELPER_H__

#include "_CAccessibilityRecordHelper.h"

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
    CARAPI ObtainEx(
        /* [out] */ IAccessibilityRecord** newInstance);
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__CACCESSIBILITYRECORDHELPER_H__
