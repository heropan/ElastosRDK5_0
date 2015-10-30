#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_JSRESULTHANDLER_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_JSRESULTHANDLER_H__
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/webkit/native/android_webview/JsResultReceiver.h"
#include "elastos/droid/webkit/native/android_webview/JsPromptResultReceiver.h"

using Elastos::Core::IRunnable;
// import org.chromium.base.ThreadUtils;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {
class AwContentsClientBridge;

class JsResultHandler
    : public Object
    , public JsResultReceiver
    , public JsPromptResultReceiver
{
private:
    class ConfirmRunnable
        : public Object
        , public IRunnable
    {
    public:
        CAR_INTERFACE_DECL();
        ConfirmRunnable(
            /* [in] */ JsResultHandler* owner,
            /* [in] */ const String& promptResult);

        CARAPI Run();

    private:
        JsResultHandler* mOwner;
        String mPromptResult;
    };

    class CancelRunnable
        : public Object
        , public IRunnable
    {
    public:
        CAR_INTERFACE_DECL();
        CancelRunnable(
            /* [in] */ JsResultHandler* owner);

        CARAPI Run();

    private:
        JsResultHandler* mOwner;
    };

public:
    CAR_INTERFACE_DECL();

    JsResultHandler(
        /* [in] */ AwContentsClientBridge* bridge,
        /* [in] */ Int32 id);

    //@Override
    CARAPI Confirm();

    //@Override
    CARAPI Confirm(
        /* [in] */ const String& promptResult);

    //@Override
    CARAPI Cancel();

private:
    JsResultHandler(const JsResultHandler&);
    JsResultHandler& operator=(const JsResultHandler&);
    AwContentsClientBridge* mBridge;
    const Int32 mId;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_JSRESULTHANDLER_H__
