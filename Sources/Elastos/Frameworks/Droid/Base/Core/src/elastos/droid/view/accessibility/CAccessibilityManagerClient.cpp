
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/view/accessibility/CAccessibilityManagerClient.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

ECode CAccessibilityManagerClient::SetState(
    /* [in] */ Int32 stateFlags)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(
        CAccessibilityManager::DO_SET_STATE, stateFlags, 0, (IMessage**)&msg);
    Boolean result;
    return mHost->mHandler->SendMessage(msg, &result);
}

ECode CAccessibilityManagerClient::constructor(
    /* [in] */ Handle32 host)
{
    mHost = (CAccessibilityManager*)host;
    return NOERROR;
}

} // Accessibility
} // View
} // Droid
} // Elastos
