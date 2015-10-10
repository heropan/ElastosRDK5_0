
#ifndef __ELASTOS_DROID_WEBKIT_CALLBACKPROXY_H__
#define __ELASTOS_DROID_WEBKIT_CALLBACKPROXY_H__

#include "elastos/droid/ext/frameworkext.h"
#include "os/HandlerBase.h"
#include "webkit/ClientCertRequestHandler.h"
#include "webkit/WebBackForwardListClassic.h"
#include "webkit/BrowserFrame.h"
#include "webkit/CJsPromptResult.h"
#include <elastos/Core/Object.h>

using Elastos::Core::Object;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::IDialogInterfaceOnCancelListener;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Net::Http::ISslError;
using Elastos::Droid::Net::Http::ISslCertificate;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::Widget::IEditText;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewClassic;

/**
 * This class is a proxy class for handling WebCore -> UI thread messaging. All
 * the callback functions are called from the WebCore thread and messages are
 * posted to the UI thread for the actual client callback.
 */
/*
 * This class is created in the UI thread so its handler and any private classes
 * that extend Handler will operate in the UI thread.
 */
class CallbackProxy
    : public HandlerBase
    , public Object
{
private:
    // Result transportation object for returning results across thread
    // boundaries.
    class ResultTransport
        : public ElLightRefBase
        , public IInterface
    {
    public:
        ResultTransport(
            /* [in] */ IInterface* defaultResult);

        CAR_INTERFACE_DECL();

        CARAPI_(void) SetResult(
            /* [in] */ IInterface* result);

        CARAPI_(AutoPtr<IInterface>) GetResult();

    private:
        // Private result object
        AutoPtr<IInterface> mResult;
        Object mLock;
    };

    class JsResultReceiver
        : public ElLightRefBase
        , public IJsResultReceiver
    {
    public:
        JsResultReceiver(
            /* [in] */ CallbackProxy* owner)
            : mReady(FALSE)
            , mTriedToNotifyBeforeReady(FALSE)
            , mOwner(owner)
        {
            CJsPromptResult::New(this, (IJsPromptResult**)&mJsResult);
        }

        CAR_INTERFACE_DECL();

        CARAPI_(void) SetReady();

        /* Wake up the WebCore thread. */
        CARAPI OnJsResultComplete(
            /* [in] */ IJsResult* result);

    private:
        CARAPI_(void) NotifyCallbackProxy();

    public:
        AutoPtr<IJsPromptResult> mJsResult;

    private:
        // This prevents a user from interacting with the result before WebCore is
        // ready to handle it.
        Boolean mReady;
        // Tells us if the user tried to confirm or cancel the result before WebCore
        // is ready.
        Boolean mTriedToNotifyBeforeReady;
        CallbackProxy* mOwner;
    };

    class UploadFile
        : public ElLightRefBase
        , public IValueCallback
    {
    public:
        UploadFile(
            /* [in] */ CallbackProxy* owner)
            : mOwner(owner)
        {}

        CAR_INTERFACE_DECL();

        CARAPI OnReceiveValue(
            /* [in] */ IInterface* value);

        CARAPI_(AutoPtr<IUri>) GetResult();

    private:
        AutoPtr<IUri> mValue;
        CallbackProxy* mOwner;
    };

    class UploadFileMessageData
        : public ElLightRefBase
        , public IInterface
    {
    public:
        UploadFileMessageData(
            /* [in] */ UploadFile* uploadFile,
            /* [in] */ const String& acceptType,
            /* [in] */ const String& capture);

        CAR_INTERFACE_DECL();

        CARAPI_(AutoPtr<UploadFile>) GetUploadFile();

        CARAPI_(String) GetAcceptType();

        CARAPI_(String) GetCapture();

    private:
        AutoPtr<UploadFile> mCallback;
        String mAcceptType;
        String mCapture;
    };

    class InnerDialogOnClickListener
        : public ElLightRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        InnerDialogOnClickListener(
            /* [in] */ IJsResult* res,
            /* [in] */ CallbackProxy* owner);

        CAR_INTERFACE_DECL();

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        AutoPtr<IJsResult> mRes;
        CallbackProxy* mOwner;
    };

    class InnerDialogOnClickListenerEx
        : public ElLightRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        InnerDialogOnClickListener(
            /* [in] */ IJsPromptResult* res,
            /* [in] */ IEditText* text,
            /* [in] */ CallbackProxy* owner);

        CAR_INTERFACE_DECL();

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        AutoPtr<IJsPromptResult> mRes;
        AutoPtr<IEditText> mEditText;
        CallbackProxy* mOwner;
    };

    class InnerDialogOnCancelListener
        : public ElLightRefBase
        , public IDialogInterfaceOnCancelListener
    {
    public:
        InnerDialogOnCancelListener(
            /* [in] */ IJsResult* res,
            /* [in] */ CallbackProxy* owner);

        CAR_INTERFACE_DECL();

        CARAPI OnCancel(
            /* [in] */ IDialogInterface* dialog);

    private:
        AutoPtr<IJsResult> mRes;
        CallbackProxy* mOwner;
    };

    class CancelClickListener
        : public ElLightRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        CancelClickListener(
            /* [in] */ IJsResult* res,
            /* [in] */ CallbackProxy* owner);

        CAR_INTERFACE_DECL();

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        AutoPtr<IJsResult> mRes;
        CallbackProxy* mOwner;
    };

public:
    /**
     * Construct a new CallbackProxy.
     */
    CallbackProxy(
        /* [in] */ IContext* context,
        /* [in] */ CWebViewClassic* w);

    CARAPI_(void) BlockMessages();

    CARAPI_(void) Shutdown();

    /**
     * Set the WebViewClient.
     * @param client An implementation of WebViewClient.
     */
    CARAPI_(void) SetWebViewClient(
        /* [in] */ IWebViewClient* client);

    /**
     * Get the WebViewClient.
     * @return the current WebViewClient instance.
     */
    CARAPI_(AutoPtr<IWebViewClient>) GetWebViewClient();

    /**
     * Set the WebChromeClient.
     * @param client An implementation of WebChromeClient.
     */
    CARAPI_(void) SetWebChromeClient(
        /* [in] */ IWebChromeClient* client);

    /**
     * Get the WebChromeClient.
     * @return the current WebChromeClient instance.
     */
    CARAPI_(AutoPtr<IWebChromeClient>) GetWebChromeClient();

    /**
     * Set the client DownloadListener.
     * @param client An implementation of DownloadListener.
     */
    CARAPI_(void) SetDownloadListener(
        /* [in] */ IDownloadListener* client);

    /**
     * Get the Back/Forward list to return to the user or to update the cached
     * history list.
     */
    CARAPI_(AutoPtr<WebBackForwardListClassic>) GetBackForwardList();

    CARAPI_(void) SetWebBackForwardListClient(
        /* [in] */ IWebBackForwardListClient* client);

    CARAPI_(AutoPtr<IWebBackForwardListClient>) GetWebBackForwardListClient();

    /**
     * Called by the UI side.  Calling overrideUrlLoading from the WebCore
     * side will post a message to call this method.
     */
    CARAPI_(Boolean) UiOverrideUrlLoading(
        /* [in] */ const String& overrideUrl);

    /**
     * Called by UI side.
     */
    CARAPI_(Boolean) UiOverrideKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

    /**
     * Return the latest progress.
     */
    CARAPI_(Int32) GetProgress();

    /**
     * Called by WebCore side to switch out of history Picture drawing mode
     */
    CARAPI_(void) SwitchOutDrawHistory();

    //--------------------------------------------------------------------------
    // WebViewClient functions.
    // NOTE: shouldOverrideKeyEvent is never called from the WebCore thread so
    // it is not necessary to include it here.
    //--------------------------------------------------------------------------

    CARAPI_(void) OnPageStarted(
        /* [in] */ const String& url,
        /* [in] */ IBitmap* favicon);

    CARAPI_(void) OnPageFinished(
        /* [in] */ const String& url);

    // Because this method is public and because CallbackProxy is mistakenly
    // party of the public classes, we cannot remove this method.
    CARAPI_(void) OnTooManyRedirects(
        /* [in] */ IMessage* cancelMsg,
        /* [in] */ IMessage* continueMsg);

    CARAPI_(void) OnReceivedError(
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& description,
        /* [in] */ const String& failingUrl);

    CARAPI_(void) OnFormResubmission(
        /* [in] */ IMessage* dontResend,
        /* [in] */ IMessage* resend);

    /**
     * Called by the WebCore side
     */
    CARAPI_(Boolean) ShouldOverrideUrlLoading(
        /* [in] */ const String& url);

    CARAPI_(void) OnReceivedHttpAuthRequest(
        /* [in] */ IHttpAuthHandler* handler,
        /* [in] */ const String& hostName,
        /* [in] */ const String& realmName);

    CARAPI_(void) OnReceivedSslError(
        /* [in] */ ISslErrorHandler* handler,
        /* [in] */ ISslError* error);

    CARAPI_(void) OnProceededAfterSslError(
        /* [in] */ ISslError* error);

    CARAPI_(void) OnReceivedClientCertRequest(
        /* [in] */ ClientCertRequestHandler* handler,
        /* [in] */ const String& host_and_port);

    CARAPI_(void) OnReceivedCertificate(
        /* [in] */ ISslCertificate* certificate);

    CARAPI_(void) DoUpdateVisitedHistory(
        /* [in] */ const String& url,
        /* [in] */ Boolean isReload);

    CARAPI_(AutoPtr<IWebResourceResponse>) ShouldInterceptRequest(
        /* [in] */ const String& url);

    CARAPI_(void) OnUnhandledKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI_(void) OnScaleChanged(
        /* [in] */ Float oldScale,
        /* [in] */ Float newScale);

    CARAPI_(void) OnReceivedLoginRequest(
        /* [in] */ const String& realm,
        /* [in] */ const String& account,
        /* [in] */ const String& args);

    //--------------------------------------------------------------------------
    // DownloadListener functions.
    //--------------------------------------------------------------------------

    /**
     * Starts a download if a download listener has been registered, otherwise
     * return false.
     */
    CARAPI_(Boolean) OnDownloadStart(
        /* [in] */ const String& url,
        /* [in] */ const String& userAgent,
        /* [in] */ const String& contentDisposition,
        /* [in] */ const String& mimetype,
        /* [in] */ const String referer,
        /* [in] */ Int64 contentLength);

    //--------------------------------------------------------------------------
    // WebView specific functions that do not interact with a client. These
    // functions just need to operate within the UI thread.
    //--------------------------------------------------------------------------

    CARAPI_(Boolean) OnSavePassword(
        /* [in] */ const String& schemePlusHost,
        /* [in] */ const String& username,
        /* [in] */ const String& password,
        /* [in] */ IMessage* resumeMsg);

    CARAPI_(void) OnReceivedHttpAuthCredentials(
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    //--------------------------------------------------------------------------
    // WebChromeClient methods
    //--------------------------------------------------------------------------

    CARAPI_(void) OnProgressChanged(
        /* [in] */ Int32 newProgress);

    CARAPI_(AutoPtr<BrowserFrame>) CreateWindow(
        /* [in] */ Boolean dialog,
        /* [in] */ Boolean userGesture);

    CARAPI_(void) OnRequestFocus();

    CARAPI_(void) OnCloseWindow(
        /* [in] */ CWebViewClassic* window);

    CARAPI_(void) OnReceivedIcon(
        /* [in] */ IBitmap* icon);

    /* package */
    CARAPI_(void) OnReceivedTouchIconUrl(
        /* [in] */ const String& url,
        /* [in] */ Boolean precomposed);

    CARAPI_(void) OnReceivedTitle(
        /* [in] */ const String& title);

    CARAPI_(void) OnJsAlert(
        /* [in] */ const String& url,
        /* [in] */ const String& message);

    CARAPI_(Boolean) OnJsConfirm(
        /* [in] */ const String& url,
        /* [in] */ const String& message);

    CARAPI_(String) OnJsPrompt(
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [in] */ const String& defaultValue);

    CARAPI_(Boolean) OnJsBeforeUnload(
        /* [in] */ const String& url,
        /* [in] */ const String& message);

    /**
     * Called by WebViewCore to inform the Java side that the current origin
     * has overflowed it's database quota. Called in the WebCore thread so
     * posts a message to the UI thread that will prompt the WebChromeClient
     * for what to do. On return back to C++ side, the WebCore thread will
     * sleep pending a new quota value.
     * @param url The URL that caused the quota overflow.
     * @param databaseIdentifier The identifier of the database that the
     *     transaction that caused the overflow was running on.
     * @param quota The current quota the origin is allowed.
     * @param estimatedDatabaseSize The estimated size of the database.
     * @param totalQuota is the sum of all origins' quota.
     * @param quotaUpdater An instance of a class encapsulating a callback
     *     to WebViewCore to run when the decision to allow or deny more
     *     quota has been made.
     */
    CARAPI_(void) OnExceededDatabaseQuota(
        /* [in] */ const String& url,
        /* [in] */ const String& databaseIdentifier,
        /* [in] */ Int64 quota,
        /* [in] */ Int64 estimatedDatabaseSize,
        /* [in] */ Int64 totalQuota,
        /* [in] */ IWebStorageQuotaUpdater* quotaUpdater);

    /**
     * Called by WebViewCore to inform the Java side that the appcache has
     * exceeded its max size.
     * @param requiredStorage is the amount of storage, in bytes, that would be
     * needed in order for the last appcache operation to succeed.
     * @param quota is the current quota (for all origins).
     * @param quotaUpdater An instance of a class encapsulating a callback
     * to WebViewCore to run when the decision to allow or deny a bigger
     * app cache size has been made.
     */
    CARAPI_(void) OnReachedMaxAppCacheSize(
        /* [in] */ Int64 requiredStorage,
        /* [in] */ Int64 quota,
        /* [in] */ IWebStorageQuotaUpdater* quotaUpdater);

    /**
     * Called by WebViewCore to instruct the browser to display a prompt to ask
     * the user to set the Geolocation permission state for the given origin.
     * @param origin The origin requesting Geolocation permsissions.
     * @param callback The callback to call once a permission state has been
     *     obtained.
     */
    CARAPI_(void) OnGeolocationPermissionsShowPrompt(
        /* [in] */ const String& origin,
        /* [in] */ IGeolocationPermissionsCallback* callback);

    /**
     * Called by WebViewCore to instruct the browser to hide the Geolocation
     * permissions prompt.
     */
    CARAPI_(void) OnGeolocationPermissionsHidePrompt();

    /**
     * Called by WebViewCore when we have a message to be added to the JavaScript
     * error console. Sends a message to the Java side with the details.
     * @param message The message to add to the console.
     * @param lineNumber The lineNumber of the source file on which the error
     *     occurred.
     * @param sourceID The filename of the source file in which the error
     *     occurred.
     * @param msgLevel The message level, corresponding to the MessageLevel enum in
     *     WebCore/page/Console.h
     */
    CARAPI_(void) AddMessageToConsole(
        /* [in] */ const String& message,
        /* [in] */ Int32 lineNumber,
        /* [in] */ const String& sourceID,
        /* [in] */ Int32 msgLevel);

    CARAPI_(Boolean) OnJsTimeout();

    CARAPI_(void) GetVisitedHistory(
        /* [in] */ IValueCallback* callback);

    /**
     * Called by WebViewCore to open a file chooser.
     */
    /* package */
    CARAPI_(AutoPtr<IUri>) OpenFileChooser(
        /* [in] */ const String& acceptType,
        /* [in] */ const String& capture);

    CARAPI_(void) OnNewHistoryItem(
        /* [in] */ IWebHistoryItem* item);

    CARAPI_(void) OnIndexChanged(
        /* [in] */ IWebHistoryItem* item,
        /* [in] */ Int32 index);

    CARAPI_(Boolean) CanShowAlertDialog();

protected:
    CARAPI_(Boolean) MessagesBlocked();

private:
    CARAPI_(String) GetJsDialogTitle(
        /* [in] */ const String& url);

    CARAPI_(void) SendMessageToUiThreadSync(
        /* [in] */ IMessage* msg);

private:
    // Logging tag
    static const String LOGTAG;
    // Instance of WebViewClient that is the client callback.
    AutoPtr<IWebViewClient> mWebViewClient;
    // Instance of WebChromeClient for handling all chrome functions.
    AutoPtr<IWebChromeClient> mWebChromeClient;
    // Instance of WebViewClassic for handling UI requests.
    CWebViewClassic* mWebView;
    // Client registered callback listener for download events
    AutoPtr<IDownloadListener> mDownloadListener;
    // Keep track of multiple progress updates.
    Boolean mProgressUpdatePending;
    // Keep track of the last progress amount.
    // Start with 100 to indicate it is not in load for the empty page.
    Int32 mLatestProgress;
    // Back/Forward list
    AutoPtr<WebBackForwardListClassic> mBackForwardList;
    // Back/Forward list client
    AutoPtr<IWebBackForwardListClient> mWebBackForwardListClient;
    // Used to call startActivity during url override.
    AutoPtr<IContext> mContext;
    // block messages flag for destroy
    Boolean mBlockMessages;

    // Message IDs
    static const Int32 PAGE_STARTED                         = 100;
    static const Int32 RECEIVED_ICON                        = 101;
    static const Int32 RECEIVED_TITLE                       = 102;
    static const Int32 OVERRIDE_URL                         = 103;
    static const Int32 AUTH_REQUEST                         = 104;
    static const Int32 SSL_ERROR                            = 105;
    static const Int32 PROGRESS                             = 106;
    static const Int32 UPDATE_VISITED                       = 107;
    static const Int32 LOAD_RESOURCE                        = 108;
    static const Int32 CREATE_WINDOW                        = 109;
    static const Int32 CLOSE_WINDOW                         = 110;
    static const Int32 SAVE_PASSWORD                        = 111;
    static const Int32 JS_ALERT                             = 112;
    static const Int32 JS_CONFIRM                           = 113;
    static const Int32 JS_PROMPT                            = 114;
    static const Int32 JS_UNLOAD                            = 115;
    static const Int32 ASYNC_KEYEVENTS                      = 116;
    static const Int32 DOWNLOAD_FILE                        = 118;
    static const Int32 REPORT_ERROR                         = 119;
    static const Int32 RESEND_POST_DATA                     = 120;
    static const Int32 PAGE_FINISHED                        = 121;
    static const Int32 REQUEST_FOCUS                        = 122;
    static const Int32 SCALE_CHANGED                        = 123;
    static const Int32 RECEIVED_CERTIFICATE                 = 124;
    static const Int32 SWITCH_OUT_HISTORY                   = 125;
    static const Int32 EXCEEDED_DATABASE_QUOTA              = 126;
    static const Int32 REACHED_APPCACHE_MAXSIZE             = 127;
    static const Int32 JS_TIMEOUT                           = 128;
    static const Int32 ADD_MESSAGE_TO_CONSOLE               = 129;
    static const Int32 GEOLOCATION_PERMISSIONS_SHOW_PROMPT  = 130;
    static const Int32 GEOLOCATION_PERMISSIONS_HIDE_PROMPT  = 131;
    static const Int32 RECEIVED_TOUCH_ICON_URL              = 132;
    static const Int32 GET_VISITED_HISTORY                  = 133;
    static const Int32 OPEN_FILE_CHOOSER                    = 134;
    static const Int32 ADD_HISTORY_ITEM                     = 135;
    static const Int32 HISTORY_INDEX_CHANGED                = 136;
    static const Int32 AUTH_CREDENTIALS                     = 137;
    static const Int32 AUTO_LOGIN                           = 140;
    static const Int32 CLIENT_CERT_REQUEST                  = 141;
    static const Int32 PROCEEDED_AFTER_SSL_ERROR            = 144;

    // Message triggered by the client to resume execution
    static const Int32 NOTIFY                               = 200;

    // Performance probe
    static const Boolean PERF_PROBE;
    Int64 mWebCoreThreadTime;
    Int64 mWebCoreIdleTime;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_CALLBACKPROXY_H__
