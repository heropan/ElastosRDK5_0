
#include "webkit/JsPromptResult.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Handle a confirmation response from the user.
 */
ECode JsPromptResult::ConfirmEx(
    /* [in] */ const String& result)
{
    mStringResult = result;
    return JsResult::Confirm();
}

/**
 * @hide Only for use by WebViewProvider implementations
 */
String JsPromptResult::GetStringResult()
{
    return mStringResult;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos