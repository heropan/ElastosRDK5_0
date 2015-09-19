#ifndef __ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYINTERACTIONCLIENTHELPER_H__
#define __ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYINTERACTIONCLIENTHELPER_H__

#include "ext/frameworkext.h"
#include "_Elastos_Droid_View_Accessibility_CAccessibilityInteractionClientHelper.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CAccessibilityInteractionClientHelper)
{
public:
    /**
     * @return The client for the current thread.
     */
    CARAPI GetInstance(
        /* [out] */ IAccessibilityInteractionClient** client);

    /**
     * <strong>Note:</strong> We keep one instance per interrogating thread since
     * the instance contains state which can lead to undesired thread interleavings.
     * We do not have a thread local variable since other threads should be able to
     * look up the correct client knowing a thread id. See ViewRootImpl for details.
     *
     * @return The client for a given <code>threadId</code>.
     */
    CARAPI GetInstanceForThread(
        /* [in] */ Int64 threadId,
        /* [out] */ IAccessibilityInteractionClient** client);
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYINTERACTIONCLIENTHELPER_H__
