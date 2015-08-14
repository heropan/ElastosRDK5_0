
#ifndef __ELASTOS_DROID_WEBKIT_JSRESULT_H__
#define __ELASTOS_DROID_WEBKIT_JSRESULT_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * An instance of this class is passed as a parameter in various {@link WebChromeClient} action
 * notifications. The object is used as a handle onto the underlying JavaScript-originated request,
 * and provides a means for the client to indicate whether this action should proceed.
 */
class JsResult
{
public:
    JsResult()
        : mResult(FALSE)
    {}

    /**
     * @hide Only for use by WebViewProvider implementations
     */
    JsResult(
        /* [in] */ IJsResultReceiver* receiver)
        : mReceiver(receiver)
        , mResult(FALSE)
    {}

    /**
     * Handle the result if the user cancelled the dialog.
     */
    virtual CARAPI Cancel();

    /**
     * Handle a confirmation response from the user.
     */
    virtual CARAPI Confirm();

    /**
     * @hide Only for use by WebViewProvider implementations
     */
    virtual CARAPI_(Boolean) GetResult();

protected:
    CARAPI Init(
       /* [in] */ IJsResultReceiver* receiver)
    {
        mReceiver = receiver;
        return NOERROR;
    }

    /* Notify the caller that the JsResult has completed */
    virtual CARAPI_(void) WakeUp() = 0;
    // This is the caller of the prompt and is the object that is waiting.
    AutoPtr<IJsResultReceiver> mReceiver;

private:
    // This is a basic result of a confirm or prompt dialog.
    Boolean mResult;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_JSRESULT_H__
