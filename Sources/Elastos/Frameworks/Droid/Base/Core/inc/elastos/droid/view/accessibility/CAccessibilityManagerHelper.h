
#ifndef __ELASTOS_DROID_VIEW_ACCESSIBILITY_CAccessibilityManager_H__
#define __ELASTOS_DROID_VIEW_ACCESSIBILITY_CAccessibilityManager_H__

#include "_Elastos_Droid_View_Accessibility_CAccessibilityManagerHelper.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CAccessibilityManagerHelper)
{
public:
    /**
     * Creates the singleton AccessibilityManager to be shared across users. This
     * has to be called before the local AccessibilityManager is created to ensure
     * it registers itself in the system correctly.
     * <p>
     * Note: Calling this method requires INTERACT_ACROSS_USERS_FULL or
     *       INTERACT_ACROSS_USERS permission.
     * </p>
     * @param context Context in which this manager operates.
     * @throws IllegalStateException if not called before the local
     *     AccessibilityManager is instantiated.
     *
     * @hide
     */
    CARAPI CreateAsSharedAcrossUsers(
        /* [in] */ IContext* context);

    /**
     * Get an AccessibilityManager instance (create one if necessary).
     *
     * @param context Context in which this manager operates.
     *
     * @hide
     */
    CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ IAccessibilityManager** manager);
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_VIEW_ACCESSIBILITY_CAccessibilityManager_H__
