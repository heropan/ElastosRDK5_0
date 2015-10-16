
#ifndef __ELASTOS_DROID_WEBKIT_JSPROMPTRESULT_H__
#define __ELASTOS_DROID_WEBKIT_JSPROMPTRESULT_H__

#include "elastos/droid/webkit/JsResult.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Public class for handling JavaScript prompt requests. The WebChromeClient will receive a
 * {@link WebChromeClient#onJsPrompt(WebView, String, String, String, JsPromptResult)} call with a
 * JsPromptResult instance as a parameter. This parameter is used to return the result of this user
 * dialog prompt back to the WebView instance. The client can call cancel() to cancel the dialog or
 * confirm() with the user's input to confirm the dialog.
 */
class JsPromptResult : public JsResult
{
public:
    JsPromptResult()
    {}

    /**
     * @hide Only for use by WebViewProvider implementations
     */
    JsPromptResult(
        /* [in] */ IJsResultReceiver* receiver)
        : JsResult(receiver)
    {}

    /**
     * Handle a confirmation response from the user.
     */
    virtual CARAPI Confirm(
        /* [in] */ const String& result);

    /**
     * @hide Only for use by WebViewProvider implementations
     */
    virtual CARAPI_(String) GetStringResult();

protected:
    CARAPI Init(
        /* [in] */ IJsResultReceiver* receiver)
    {
        return JsResult::Init(receiver);
    }

private:
    // String result of the prompt
    String mStringResult;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_JSPROMPTRESULT_H__
