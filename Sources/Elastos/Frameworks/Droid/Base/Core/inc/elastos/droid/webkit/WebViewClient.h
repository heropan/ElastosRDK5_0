
#ifndef __ELASTOS_DROID_WEBKIT_WEBVIEWCLIENT_H__
#define __ELASTOS_DROID_WEBKIT_WEBVIEWCLIENT_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Net::Http::ISslError;
using Elastos::Droid::View::IInputEvent;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Webkit {

class HttpAuthHandler;

class WebViewClient
    : public Object
    , public IWebViewClient
{
public:
    // These ints must match up to the hidden values in EventHandler.
    /** Generic error */
    static const Int32 ERROR_UNKNOWN = -1;
    /** Server or proxy hostname lookup failed */
    static const Int32 ERROR_HOST_LOOKUP = -2;
    /** Unsupported authentication scheme (not basic or digest) */
    static const Int32 ERROR_UNSUPPORTED_AUTH_SCHEME = -3;
    /** User authentication failed on server */
    static const Int32 ERROR_AUTHENTICATION = -4;
    /** User authentication failed on proxy */
    static const Int32 ERROR_PROXY_AUTHENTICATION = -5;
    /** Failed to connect to the server */
    static const Int32 ERROR_CONNECT = -6;
    /** Failed to read or write to the server */
    static const Int32 ERROR_IO = -7;
    /** Connection timed out */
    static const Int32 ERROR_TIMEOUT = -8;
    /** Too many redirects */
    static const Int32 ERROR_REDIRECT_LOOP = -9;
    /** Unsupported URI scheme */
    static const Int32 ERROR_UNSUPPORTED_SCHEME = -10;
    /** Failed to perform SSL handshake */
    static const Int32 ERROR_FAILED_SSL_HANDSHAKE = -11;
    /** Malformed URL */
    static const Int32 ERROR_BAD_URL = -12;
    /** Generic file error */
    static const Int32 ERROR_FILE = -13;
    /** File not found */
    static const Int32 ERROR_FILE_NOT_FOUND = -14;
    /** Too many requests during this load */
    static const Int32 ERROR_TOO_MANY_REQUESTS = -15;

public:
    CAR_INTERFACE_DECL()

    CARAPI constructor();

    /**
     * Give the host application a chance to take over the control when a new
     * url is about to be loaded in the current WebView. If WebViewClient is not
     * provided, by default WebView will ask Activity Manager to choose the
     * proper handler for the url. If WebViewClient is provided, return true
     * means the host application handles the url, while return false means the
     * current WebView handles the url.
     *
     * @param view The WebView that is initiating the callback.
     * @param url The url to be loaded.
     * @return True if the host application wants to leave the current WebView
     *         and handle the url itself, otherwise return false.
     */
    CARAPI ShouldOverrideUrlLoading(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    /**
     * Notify the host application that a page has started loading. This method
     * is called once for each main frame load so a page with iframes or
     * framesets will call onPageStarted one time for the main frame. This also
     * means that onPageStarted will not be called when the contents of an
     * embedded frame changes, i.e. clicking a link whose target is an iframe.
     *
     * @param view The WebView that is initiating the callback.
     * @param url The url to be loaded.
     * @param favicon The favicon for this page if it already exists in the
     *            database.
     */
    CARAPI OnPageStarted(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [in] */ IBitmap* favicon);

    /**
     * Notify the host application that a page has finished loading. This method
     * is called only for main frame. When onPageFinished() is called, the
     * rendering picture may not be updated yet. To get the notification for the
     * new Picture, use {@link WebView.PictureListener#onNewPicture}.
     *
     * @param view The WebView that is initiating the callback.
     * @param url The url of the page.
     */
    CARAPI OnPageFinished(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url);

    /**
     * Notify the host application that the WebView will load the resource
     * specified by the given url.
     *
     * @param view The WebView that is initiating the callback.
     * @param url The url of the resource the WebView will load.
     */
    CARAPI OnLoadResource(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url);

    /**
     * Notify the host application of a resource request and allow the
     * application to return the data.  If the return value is null, the WebView
     * will continue to load the resource as usual.  Otherwise, the return
     * response and data will be used.  NOTE: This method is called by the
     * network thread so clients should exercise caution when accessing private
     * data.
     *
     * @param view The {@link android.webkit.WebView} that is requesting the
     *             resource.
     * @param url The raw url of the resource.
     * @return A {@link android.webkit.WebResourceResponse} containing the
     *         response information or null if the WebView should load the
     *         resource itself.
     */
    CARAPI ShouldInterceptRequest(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [out] */ IWebResourceResponse** wrr);

    /**
     * Notify the host application of a resource request and allow the
     * application to return the data.  If the return value is null, the WebView
     * will continue to load the resource as usual.  Otherwise, the return
     * response and data will be used.  NOTE: This method is called on a thread
     * other than the UI thread so clients should exercise caution
     * when accessing private data or the view system.
     *
     * @param view The {@link android.webkit.WebView} that is requesting the
     *             resource.
     * @param request Object containing the details of the request.
     * @return A {@link android.webkit.WebResourceResponse} containing the
     *         response information or null if the WebView should load the
     *         resource itself.
     */
    CARAPI ShouldInterceptRequest(
        /* [in] */ IWebView* view,
        /* [in] */ IWebResourceRequest* request,
        /* [out] */ IWebResourceResponse** wrr);

    /**
     * Notify the host application that there have been an excessive number of
     * HTTP redirects. As the host application if it would like to continue
     * trying to load the resource. The default behavior is to send the cancel
     * message.
     *
     * @param view The WebView that is initiating the callback.
     * @param cancelMsg The message to send if the host wants to cancel
     * @param continueMsg The message to send if the host wants to continue
     * @deprecated This method is no longer called. When the WebView encounters
     *             a redirect loop, it will cancel the load.
     */
    CARAPI OnTooManyRedirects(
        /* [in] */ IWebView* view,
        /* [in] */ IMessage* cancelMsg,
        /* [in] */ IMessage* continueMsg);

    /**
     * Report an error to the host application. These errors are unrecoverable
     * (i.e. the main resource is unavailable). The errorCode parameter
     * corresponds to one of the ERROR_* constants.
     * @param view The WebView that is initiating the callback.
     * @param errorCode The error code corresponding to an ERROR_* value.
     * @param description A String describing the error.
     * @param failingUrl The url that failed to load.
     */
    CARAPI OnReceivedError(
        /* [in] */ IWebView* view,
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& description,
        /* [in] */ const String& failingUrl);

    /**
     * As the host application if the browser should resend data as the
     * requested page was a result of a POST. The default is to not resend the
     * data.
     *
     * @param view The WebView that is initiating the callback.
     * @param dontResend The message to send if the browser should not resend
     * @param resend The message to send if the browser should resend data
     */
    CARAPI OnFormResubmission(
        /* [in] */ IWebView* view,
        /* [in] */ IMessage* dontResend,
        /* [in] */ IMessage* resend);

    /**
     * Notify the host application to update its visited links database.
     *
     * @param view The WebView that is initiating the callback.
     * @param url The url being visited.
     * @param isReload True if this url is being reloaded.
     */
    CARAPI DoUpdateVisitedHistory(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [in] */ Boolean isReload);

    /**
     * Notify the host application that an SSL error occurred while loading a
     * resource. The host application must call either handler.cancel() or
     * handler.proceed(). Note that the decision may be retained for use in
     * response to future SSL errors. The default behavior is to cancel the
     * load.
     *
     * @param view The WebView that is initiating the callback.
     * @param handler An SslErrorHandler object that will handle the user's
     *            response.
     * @param error The SSL error object.
     */
    CARAPI OnReceivedSslError(
        /* [in] */ IWebView* view,
        /* [in] */ ISslErrorHandler* handler,
        /* [in] */ ISslError* error);

    /**
     * Notify the host application to handle a SSL client certificate
     * request. The host application is responsible for showing the UI
     * if desired and providing the keys. There are three ways to
     * respond: proceed(), cancel() or ignore(). Webview remembers the
     * response if proceed() or cancel() is called and does not
     * call onReceivedClientCertRequest() again for the same host and port
     * pair. Webview does not remember the response if ignore() is called.
     *
     * This method is called on the UI thread. During the callback, the
     * connection is suspended.
     *
     * The default behavior is to cancel, returning no client certificate.
     *
     * @param view The WebView that is initiating the callback
     * @param request An instance of a {@link ClientCertRequest}
     *
     */
    CARAPI OnReceivedClientCertRequest(
        /* [in] */ IWebView* view,
        /* [in] */ IClientCertRequest* request);

    /**
     * Notifies the host application that the WebView received an HTTP
     * authentication request. The host application can use the supplied
     * {@link HttpAuthHandler} to set the WebView's response to the request.
     * The default behavior is to cancel the request.
     *
     * @param view the WebView that is initiating the callback
     * @param handler the HttpAuthHandler used to set the WebView's response
     * @param host the host requiring authentication
     * @param realm the realm for which authentication is required
     * @see Webview#getHttpAuthUsernamePassword
     */
    CARAPI OnReceivedHttpAuthRequest(
        /* [in] */ IWebView* view,
        /* [in] */ IHttpAuthHandler* handler,
        /* [in] */ const String& host,
        /* [in] */ const String& realm);

    /**
     * Give the host application a chance to handle the key event synchronously.
     * e.g. menu shortcut key events need to be filtered this way. If return
     * true, WebView will not handle the key event. If return false, WebView
     * will always handle the key event, so none of the super in the view chain
     * will see the key event. The default behavior returns false.
     *
     * @param view The WebView that is initiating the callback.
     * @param event The key event.
     * @return True if the host application wants to handle the key event
     *         itself, otherwise return false
     */
    CARAPI ShouldOverrideKeyEvent(
        /* [in] */ IWebView* view,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    /**
     * Notify the host application that a key was not handled by the WebView.
     * Except system keys, WebView always consumes the keys in the normal flow
     * or if shouldOverrideKeyEvent returns true. This is called asynchronously
     * from where the key is dispatched. It gives the host application a chance
     * to handle the unhandled key events.
     *
     * @param view The WebView that is initiating the callback.
     * @param event The key event.
     */
    CARAPI OnUnhandledKeyEvent(
        /* [in] */ IWebView* view,
        /* [in] */ IKeyEvent* event);

    /**
     * Notify the host application that a input event was not handled by the WebView.
     * Except system keys, WebView always consumes input events in the normal flow
     * or if shouldOverrideKeyEvent returns true. This is called asynchronously
     * from where the event is dispatched. It gives the host application a chance
     * to handle the unhandled input events.
     *
     * Note that if the event is a {@link android.view.MotionEvent}, then it's lifetime is only
     * that of the function call. If the WebViewClient wishes to use the event beyond that, then it
     * <i>must</i> create a copy of the event.
     *
     * It is the responsibility of overriders of this method to call
     * {@link #onUnhandledKeyEvent(WebView, KeyEvent)}
     * when appropriate if they wish to continue receiving events through it.
     *
     * @param view The WebView that is initiating the callback.
     * @param event The input event.
     */
    CARAPI OnUnhandledInputEvent(
        /* [in] */ IWebView* view,
        /* [in] */ IInputEvent* event);

    /**
     * Notify the host application that the scale applied to the WebView has
     * changed.
     *
     * @param view he WebView that is initiating the callback.
     * @param oldScale The old scale factor
     * @param newScale The new scale factor
     */
    CARAPI OnScaleChanged(
        /* [in] */ IWebView* view,
        /* [in] */ Float oldScale,
        /* [in] */ Float newScale);

    /**
     * Notify the host application that a request to automatically log in the
     * user has been processed.
     * @param view The WebView requesting the login.
     * @param realm The account realm used to look up accounts.
     * @param account An optional account. If not null, the account should be
     *                checked against accounts on the device. If it is a valid
     *                account, it should be used to log in the user.
     * @param args Authenticator specific arguments used to log in the user.
     */
    CARAPI OnReceivedLoginRequest(
        /* [in] */ IWebView* view,
        /* [in] */ const String& realm,
        /* [in] */ const String& account,
        /* [in] */ const String& args);

    CARAPI ToString(
        /* [out] */ String* info);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBVIEWCLIENT_H__
