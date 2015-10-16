
#include "elastos/droid/webkit/JsResult.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Handle the result if the user cancelled the dialog.
 */
ECode JsResult::Cancel()
{
    mResult = FALSE;
    WakeUp();
    return NOERROR;
}

/**
 * Handle a confirmation response from the user.
 */
ECode JsResult::Confirm()
{
    mResult = TRUE;
    WakeUp();
    return NOERROR;
}

/**
 * @hide Only for use by WebViewProvider implementations
 */
Boolean JsResult::GetResult()
{
    return mResult;
}

/* Notify the caller that the JsResult has completed */
void JsResult::WakeUp()
{
    assert(0);
//    mReceiver->OnJsResultComplete((IJsResult*)this->Probe(EIID_IJsResult));
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
