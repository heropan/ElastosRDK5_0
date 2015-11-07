
#include "elastos/droid/view/accessibility/CAccessibilityInteractionClientHelper.h"
#include "elastos/droid/view/accessibility/CAccessibilityInteractionClient.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CAR_INTERFACE_IMPL(CAccessibilityInteractionClientHelper, Singleton, IAccessibilityInteractionClientHelper)

CAR_SINGLETON_IMPL(CAccessibilityInteractionClientHelper)

ECode CAccessibilityInteractionClientHelper::GetInstance(
    /* [out] */ IAccessibilityInteractionClient** client)
{
    VALIDATE_NOT_NULL(client);
    AutoPtr<IAccessibilityInteractionClient> c = CAccessibilityInteractionClient::GetInstance();
    *client = c;
    REFCOUNT_ADD(*client);
    return NOERROR;
}

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
