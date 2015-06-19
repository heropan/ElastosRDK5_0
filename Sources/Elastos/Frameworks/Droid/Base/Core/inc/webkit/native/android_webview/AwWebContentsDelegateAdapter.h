
#ifndef __AWWEBCONTENTSDELEGATEADAPTER_H__
#define __AWWEBCONTENTSDELEGATEADAPTER_H__

// import android.content.ContentResolver;
// import android.content.Context;
// import android.net.Uri;
// import android.os.AsyncTask;
// import android.os.Handler;
// import android.os.Message;
// import android.provider.MediaStore;
// import android.util.Log;
// import android.view.KeyEvent;
// import android.view.View;
// import android.webkit.ConsoleMessage;
// import android.webkit.ValueCallback;

// import org.chromium.base.ContentUriUtils;
// import org.chromium.base.ThreadUtils;
// import org.chromium.content.browser.ContentVideoView;
// import org.chromium.content.browser.ContentViewCore;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * Adapts the AwWebContentsDelegate interface to the AwContentsClient interface.
 * This class also serves a secondary function of routing certain callbacks from the content layer
 * to specific listener interfaces.
 */
class AwWebContentsDelegateAdapter : public AwWebContentsDelegate
{
private:
    class GetDisplayNameTask : public AsyncTask<Void, Void, String[]>
    {
    public:
        GetDisplayNameTask(
            /* [in] */ IContentResolver* contentResolver,
            /* [in] */ Int32 processId,
            /* [in] */ Int32 renderId,
            /* [in] */ Int32 modeFlags,
            /* [in] */ ArrayOf<String>* filePaths);

    public:
        const Int32 mProcessId;
        const Int32 mRenderId;
        const Int32 mModeFlags;
        const AutoPtr< ArrayOf<String> > mFilePaths;
        const AutoPtr<IContentResolver> mContentResolver;

    protected:
        //@Override
        CARAPI_(AutoPtr< ArrayOf<String> >) DoInBackground(/*Void...voids*/);

        //@Override
        CARAPI_(void) OnPostExecute(
            /* [in] */ ArrayOf<String>* result);

    private:
        /**
         * @return the display name of a path if it is a content URI and is present in the database
         * or an empty string otherwise.
         */
        CARAPI_(String) ResolveFileName(
            /* [in] */ String filePath);
    };

    class InnerHandler
        : public Object
        , public IHandler
    {
    public:
        InnerHandler(
            /* [in] */ AwWebContentsDelegateAdapter* owner,
            /* [in] */ ILooper* looper,
            /* [in] */ ContentViewCore* contentViewCore);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        AwWebContentsDelegateAdapter* mOwner
        ContentViewCore* mContentViewCore;
    };

    class InnerValueCallback
    {
    public:
        InnerValueCallback(
            /* [in] */ AwWebContentsDelegateAdapter* owner,
            /* [in] */ Int32 processId,
            /* [in] */ Int32 renderId,
            /* [in] */ Int32 modeFlags,
            /* [in] */ Boolean completed);

        CARAPI onReceiveValue(
            /* [in] */ IInterface* results);
    private:
        AwWebContentsDelegateAdapter* mOwner;
        Int32 mProcessId,
        Int32 mRenderId,
        Int32 mModeFlags,
        Boolean mCompleted
    };

public:
    const AutoPtr<AwContentsClient> mContentsClient;
    AutoPtr<IView> mContainerView;
    const AutoPtr<IContext> mContext;

public:
    AwWebContentsDelegateAdapter(
        /* [in] */ AwContentsClient* contentsClient,
        /* [in] */ IView* containerView,
        /* [in] */ IContext* context);

    CARAPI_(void) SetContainerView(
        /* [in] */ IView* containerView);

    //@Override
    CARAPI_(void) OnLoadProgressChanged(
        /* [in] */ Int32 progress);

    //@Override
    CARAPI_(void) HandleKeyboardEvent(
        /* [in] */ IKeyEvent* event);

    //@Override
    CARAPI_(Boolean) TakeFocus(
        /* [in] */ Boolean reverse);

    //@Override
    CARAPI_(Boolean) AddMessageToConsole(
        /* [in] */ Int32 level,
        /* [in] */ String message,
        /* [in] */ Int32 lineNumber,
        /* [in] */ String sourceId);

    //@Override
    CARAPI_(void) OnUpdateUrl(
        /* [in] */ String url);

    //@Override
    CARAPI_(void) OpenNewTab(
        /* [in] */ String url,
        /* [in] */ String extraHeaders,
        /* [in] */ ArrayOf<Byte>* postData,
        /* [in] */ Int32 disposition,
        /* [in] */ Boolean isRendererInitiated);

    //@Override
    CARAPI_(void) CloseContents();

    //@Override
    CARAPI_(void) ShowRepostFormWarningDialog(
        /* [in] */ const ContentViewCore* contentViewCore);

    //@Override
    CARAPI_(void) RunFileChooser(
        /* [in] */ Int32 processId,
        /* [in] */ Int32 renderId,
        /* [in] */ Int32 modeFlags,
        /* [in] */ String acceptTypes,
        /* [in] */ String title,
        /* [in] */ String defaultFilename,
        /* [in] */ Boolean capture);

    //@Override
    CARAPI_(Boolean) AddNewContents(
        /* [in] */ Boolean isDialog,
        /* [in] */ Boolean isUserGesture);

    //@Override
    CARAPI_(void) ActivateContents();

    //@Override
    CARAPI_(void) ToggleFullscreenModeForTab(
        /* [in] */ Boolean enterFullscreen);

private:
    static const String TAG;

    CARAPI_(Boolean) TryToMoveFocus(
        /* [in] */ Int32 direction);
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__AWWEBCONTENTSDELEGATEADAPTER_H__
