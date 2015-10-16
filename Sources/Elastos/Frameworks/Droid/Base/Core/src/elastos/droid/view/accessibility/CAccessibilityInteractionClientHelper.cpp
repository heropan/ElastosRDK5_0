
#include "elastos/droid/view/accessibility/CAccessibilityInteractionClientHelper.h"
#include "elastos/droid/view/accessibility/CAccessibilityInteractionClient.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

/**
 * @return The client for the current thread.
 */
ECode CAccessibilityInteractionClientHelper::GetInstance(
    /* [out] */ IAccessibilityInteractionClient** client)
{
    VALIDATE_NOT_NULL(client);
    AutoPtr<IAccessibilityInteractionClient> c = CAccessibilityInteractionClient::GetInstance();
    *client = c;
    REFCOUNT_ADD(*client);
    return NOERROR;
}

/**
 * <strong>Note:</strong> We keep one instance per interrogating thread since
 * the instance contains state which can lead to undesired thread interleavings.
 * We do not have a thread local variable since other threads should be able to
 * look up the correct client knowing a thread id. See ViewRootImpl for details.
 *
 * @return The client for a given <code>threadId</code>.
 */
ECode CAccessibilityInteractionClientHelper::GetInstanceForThread(
    /* [in] */ Int64 threadId,
    /* [out] */ IAccessibilityInteractionClient** client)
{
    VALIDATE_NOT_NULL(client);
    AutoPtr<IAccessibilityInteractionClient> c = CAccessibilityInteractionClient::GetInstanceForThread(threadId);
    *client = c;
    REFCOUNT_ADD(*client);
    return NOERROR;
}

} // Accessibility
} // View
} // Droid
} // Elastos
