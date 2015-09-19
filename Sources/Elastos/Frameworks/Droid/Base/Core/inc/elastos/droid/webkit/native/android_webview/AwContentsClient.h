
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTSCLIENT_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTSCLIENT_H__

// import android.content.pm.ActivityInfo;
// import android.graphics.Bitmap;
// import android.graphics.Picture;
// import android.net.http.SslError;
// import android.os.Looper;
// import android.os.Message;
// import android.util.ArrayMap;
// import android.view.KeyEvent;
// import android.view.View;
// import android.webkit.ConsoleMessage;
// import android.webkit.GeolocationPermissions;
// import android.webkit.ValueCallback;
// import android.webkit.WebChromeClient;

// import org.chromium.android_webview.permission.AwPermissionRequest;
// import org.chromium.content.browser.ContentViewCore;
// import org.chromium.content.browser.WebContentsObserverAndroid;
// import org.chromium.net.NetError;

// import java.security.Principal;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * Base-class that an AwContents embedder derives from to receive callbacks.
 * This extends ContentViewClient, as in many cases we want to pass-thru ContentViewCore
 * callbacks right to our embedder, and this setup facilities that.
 * For any other callbacks we need to make transformations of (e.g. adapt parameters
 * or perform filtering) we can provide final overrides for methods here, and then introduce
 * new abstract methods that the our own client must implement.
 * i.e.: all methods in this class should either be final, or abstract.
 */
class AwContentsClient
{
public:
    class AwWebContentsObserver : public WebContentsObserverAndroid
    {
    public:
        AwWebContentsObserver(
            /* [in] */ AwContentsClient* owner,
            /* [in] */ ContentViewCore* contentViewCore);

        //@Override
        CARAPI_(void) DidFinishLoad(
            /* [in] */ Int64 frameId,
            /* [in] */ String validatedUrl,
            /* [in] */ Boolean isMainFrame);

        //@Override
        CARAPI_(void) DidFailLoad(
            /* [in] */ Boolean isProvisionalLoad,
            /* [in] */ Boolean isMainFrame,
            /* [in] */ Int32 errorCode,
            /* [in] */ String description,
            /* [in] */ String failingUrl);

        //@Override
        CARAPI_(void) DidNavigateMainFrame(
            /* [in] */ String url,
            /* [in] */ String baseUrl,
            /* [in] */ Boolean isNavigationToDifferentPage,
            /* [in] */ Boolean isFragmentNavigation);

        //@Override
        CARAPI_(void) DidNavigateAnyFrame(
            /* [in] */ String url,
            /* [in] */ String baseUrl,
            /* [in] */ Boolean isReload);

    private:
        AwContentsClient* mOwner;
    };

    //--------------------------------------------------------------------------------------------
    //             WebView specific methods that map directly to WebViewClient / WebChromeClient
    //--------------------------------------------------------------------------------------------

    /**
     * Parameters for the {@link AwContentsClient#showFileChooser} method.
     */
    struct FileChooserParams
    {
        FileChooserParams()
            : mode(0)
            , capture(FALSE)
        {
        }

        Int32 mode;
        String acceptTypes;
        String title;
        String defaultFilename;
        Boolean capture;
    };

    /**
     * Parameters for the {@link AwContentsClient#shouldInterceptRequest} method.
     */
    struct ShouldInterceptRequestParams
    {
        ShouldInterceptRequestParams()
            : isMainFrame(FALSE)
            , hasUserGesture(FALSE)
        {
        }

        // Url of the request.
        String url;
        // Is this for the main frame or a child iframe?
        Boolean isMainFrame;
        // Was a gesture associated with the request? Don't trust can easily be spoofed.
        Boolean hasUserGesture;
        // Method used (GET/POST/OPTIONS)
        String method;
        // Headers that would have been sent to server.
        ArrayMap<String, String> requestHeaders;
    };

public:
    AwContentsClient();

    // Alllow injection of the callback thread, for testing.
    AwContentsClient(
        /* [in] */ ILooper* looper);

    virtual CARAPI_(void) InstallWebContentsObserver(
        /* [in] */ ContentViewCore* contentViewCore);

    virtual CARAPI_(AutoPtr<AwContentsClientCallbackHelper>) GetCallbackHelper();

    virtual CARAPI_(Int32) GetCachedRendererBackgroundColor();

    virtual CARAPI_(Boolean) IsCachedRendererBackgroundColorValid();

    virtual CARAPI_(void) OnBackgroundColorChanged(
        /* [in] */ Int32 color);

    virtual CARAPI_(void) GetVisitedHistory(
        /* [in] */ IValueCallback* callback) = 0;

    virtual CARAPI_(void) DoUpdateVisitedHistory(
        /* [in] */ String url,
        /* [in] */ Boolean isReload) = 0;

    virtual CARAPI_(void) OnProgressChanged(
        /* [in] */ Int32 progress) = 0;

    virtual CARAPI_(AutoPtr<AwWebResourceResponse>) ShouldInterceptRequest(
        /* [in] */ ShouldInterceptRequestParams* params) = 0;

    virtual CARAPI_(Boolean) ShouldOverrideKeyEvent(
        /* [in] */ IKeyEvent* event) = 0;

    virtual CARAPI_(Boolean) ShouldOverrideUrlLoading(
        /* [in] */ String url) = 0;

    virtual CARAPI_(void) OnLoadResource(
        /* [in] */ String url) = 0;

    virtual CARAPI_(void) OnUnhandledKeyEvent(
        /* [in] */ IKeyEvent* event) = 0;

    virtual CARAPI_(Boolean) OnConsoleMessage(
        /* [in] */ IConsoleMessage* consoleMessage) = 0;

    virtual CARAPI_(void) OnReceivedHttpAuthRequest(
        /* [in] */ AwHttpAuthHandler* handler,
        /* [in] */ String host,
        /* [in] */ String realm) = 0;

    virtual CARAPI_(void) OnReceivedSslError(
        /* [in] */ IValueCallback* callback,
        /* [in] */ ISslError* error) = 0;

    // TODO(sgurun): Make abstract once this has rolled in downstream.
    virtual CARAPI_(void) OnReceivedClientCertRequest(
        /* [in] */ const AwContentsClientBridge::ClientCertificateRequestCallback* callback,
        /* [in] */ const ArrayOf<String>* keyTypes,
        /* [in] */ const ArrayOf<IPrincipal>* principals,
        /* [in] */ const String& host,
        /* [in] */ Int32 port);

    virtual CARAPI_(void) OnReceivedLoginRequest(
        /* [in] */ String realm,
        /* [in] */ String account,
        /* [in] */ String args) = 0;

    virtual CARAPI_(void) OnFormResubmission(
        /* [in] */ IMessage* dontResend,
        /* [in] */ IMessage* resend) = 0;

    virtual CARAPI_(void) OnDownloadStart(
        /* [in] */ String url,
        /* [in] */ String userAgent,
        /* [in] */ String contentDisposition,
        /* [in] */ String mimeType,
        /* [in] */ Int64 contentLength) = 0;

    // TODO(joth): Make abstract once this has rolled in downstream.
    virtual CARAPI_(void) ShowFileChooser(
        /* [in] */ IValueCallback* uploadFilePathsCallback,
        /* [in] */ IFileChooserParams* fileChooserParams);

    virtual CARAPI_(void) OnGeolocationPermissionsShowPrompt(
        /* [in] */ String origin,
        /* [in] */ IGeolocationPermissionsCallback* callback) = 0;

    virtual CARAPI_(void) OnGeolocationPermissionsHidePrompt() = 0;

    // TODO(michaelbai): Change the abstract once merged
    virtual CARAPI_(void) OnPermissionRequest(
        /* [in] */ AwPermissionRequest* awPermissionRequest);

    // TODO(michaelbai): Change the abstract once merged
    virtual CARAPI_(void) OnPermissionRequestCanceled(
        /* [in] */ AwPermissionRequest* awPermissionRequest);

    virtual CARAPI_(void) OnScaleChangedScaled(
        /* [in] */ Float oldScale,
        /* [in] */ Float newScale) = 0;

    virtual CARAPI_(void) OnReceivedTouchIconUrl(
        /* [in] */ String url,
        /* [in] */ Boolean precomposed) = 0;

    virtual CARAPI_(void) OnReceivedIcon(
        /* [in] */ IBitmap* bitmap) = 0;

    virtual CARAPI_(void) OnReceivedTitle(
        /* [in] */ String title) = 0;

    virtual CARAPI_(void) OnPageStarted(
        /* [in] */ String url) = 0;

    virtual CARAPI_(void) OnPageFinished(
        /* [in] */ String url) = 0;

    virtual CARAPI_(void) OnReceivedError(
        /* [in] */ Int32 errorCode,
        /* [in] */ String description,
        /* [in] */ String failingUrl) = 0;

    // TODO (michaelbai): Remove this method once the same method remove from
    // WebViewContentsClientAdapter.
    virtual CARAPI_(void) OnShowCustomView(
        /* [in] */ IView* view,
        /* [in] */ Int32 requestedOrientation,
        /* [in] */ IWebChromeClientCustomViewCallback* callback);

    // TODO (michaelbai): This method should be abstract, having empty body here
    // makes the merge to the Android easy.
    virtual CARAPI_(void) OnShowCustomView(
        /* [in] */ IView* view,
        /* [in] */ IWebChromeClientCustomViewCallback* callback);

    virtual CARAPI_(void) onHideCustomView() = 0;

    virtual CARAPI_(AutoPtr<IBitmap>) GetDefaultVideoPoster() = 0;

    //--------------------------------------------------------------------------------------------
    //                              Other WebView-specific methods
    //--------------------------------------------------------------------------------------------
    //
    virtual CARAPI_(void) OnFindResultReceived(
        /* [in] */ Int32 activeMatchOrdinal,
        /* [in] */ Int32 numberOfMatches,
        /* [in] */ Boolean isDoneCounting) = 0;

    /**
     * Called whenever there is a new content picture available.
     * @param picture New picture.
     */
    virtual CARAPI_(void) OnNewPicture(
        /* [in] */ IPicture* picture) = 0;

protected:
    virtual CARAPI_(void) HandleJsAlert(
        /* [in] */ String url,
        /* [in] */ String message,
        /* [in] */ JsResultReceiver* receiver);

    virtual CARAPI_(void) HandleJsBeforeUnload(
        /* [in] */ String url,
        /* [in] */ String message,
        /* [in] */ JsResultReceiver* receiver);

    virtual CARAPI_(void) HandleJsConfirm(
        /* [in] */ String url,
        /* [in] */ String message,
        /* [in] */ JsResultReceiver* receiver);

    virtual CARAPI_(void) HandleJsPrompt(
        /* [in] */ String url,
        /* [in] */ String message,
        /* [in] */ String defaultValue,
        /* [in] */ JsPromptResultReceiver* receiver);

    virtual CARAPI_(Boolean) OnCreateWindow(
        /* [in] */ Boolean isDialog,
        /* [in] */ Boolean isUserGesture);

    virtual CARAPI_(void) OnCloseWindow();

    virtual CARAPI_(void) OnRequestFocus();

    virtual CARAPI_(AutoPtr<IView>) GetVideoLoadingProgressView();

private:
    CARAPI_(void) Init(
        /* [in] */ ILooper* looper);

    AutoPtr<AwContentsClientCallbackHelper> mCallbackHelper;

    AutoPtr<AwWebContentsObserver> mWebContentsObserver;

    // Last background color reported from the renderer. Holds the sentinal value INVALID_COLOR
    // if not valid.
    Int32 mCachedRendererBackgroundColor;

    static const Int32 INVALID_COLOR = 0;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTSCLIENT_H__
