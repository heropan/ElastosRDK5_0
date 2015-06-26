
#ifndef __JSRESULTHANDLER_H__
#define __JSRESULTHANDLER_H__

// import org.chromium.base.ThreadUtils;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

class JsResultHandler
    : public JsResultReceiver
    , public JsPromptResultReceiver
{
private:
    class ConfirmRunnable
        : public Object
        , public IRunnable
    {
    public:
        ConfirmRunnable(
            /* [in] */ JsResultHandler* owner,
            /* [in] */ const String& promptResult);

        CARAPI Run();

    private:
        JsResultHandler* mOwner;
        const String& mPromptResult;
    };

    class CancelRunnable
        : public Object
        , public IRunnable
    {
    public:
        CancelRunnable(
            /* [in] */ JsResultHandler* owner);

        CARAPI Run();

    private:
        JsResultHandler* mOwner;
    };

public:
    JsResultHandler(
        /* [in] */ AwContentsClientBridge* bridge,
        /* [in] */ Int32 id);

    //@Override
    CARAPI_(void) Confirm();

    //@Override
    CARAPI_(void) Confirm(
        /* [in] */ const String& promptResult);

    //@Override
    CARAPI_(void) Cancel();

private:
    AutoPtr<AwContentsClientBridge> mBridge;
    const Int32 mId;
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__JSRESULTHANDLER_H__
