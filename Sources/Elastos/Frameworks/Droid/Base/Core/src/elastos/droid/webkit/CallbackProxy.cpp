
#include "elastos/droid/webkit/CallbackProxy.h"
#include "elastos/droid/webkit/URLUtil.h"
#include "elastos/droid/webkit/BrowserFrame.h"
#include "elastos/droid/webkit/WebStorage.h"
#include "elastos/droid/webkit/CConsoleMessage.h"
#include "elastos/droid/webkit/CWebViewClassic.h"
#include "elastos/droid/webkit/CWebResourceResponse.h"
#include "elastos/droid/webkit/WebCoreThreadWatchdog.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/os/CMessageHelper.h"
#include "elastos/droid/app/CAlertDialogBuilder.h"
#include "elastos/droid/view/LayoutInflater.h"
#include "elastos/droid/R.h"
#include "elastos/droid/net/Uri.h"
#include <elastos/utility/logging/Logger.h>
//#include "elastos/droid/provider/CBrowser.h"
#include "elastos/droid/webkit/DebugFlags.h"

using Elastos::Utility::Logging::Logger;
using Elastos::Core::CBoolean;
using Elastos::Core::IBoolean;
using Elastos::Core::CInteger64;
using Elastos::Core::IInteger64;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::App::CAlertDialogBuilder;
using Elastos::Droid::App::IAlertDialogBuilder;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnCancelListener;
using Elastos::Droid::Os::CMessageHelper;
//using Elastos::Droid::Provider::IBrowser;//TODO the Browser no implementation
using Elastos::Droid::Net::Uri;
using Elastos::Net::IURL;
using Elastos::Utility::IObjectStringMap;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::LayoutInflater;
using Elastos::Utility::CObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//                CallbackProxy::ResultTransport
//===============================================================

CallbackProxy::ResultTransport::ResultTransport(
    /* [in] */ IInterface* defaultResult)
{
    mResult = defaultResult;
}

CAR_INTERFACE_IMPL_LIGHT(CallbackProxy::ResultTransport, IInterface);

void CallbackProxy::ResultTransport::SetResult(
    /* [in] */ IInterface* result)
{
    AutoLock lock(mLock);
    mResult = result;
}

AutoPtr<IInterface> CallbackProxy::ResultTransport::GetResult()
{
    AutoLock lock(mLock);
    return mResult;
}

ECode CallbackProxy::ResultTransport::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "CallbackProxy::ResultTransport";
    return NOERROR;
}

//===============================================================
//                 CallbackProxy::JsResultReceiver
//===============================================================

CAR_INTERFACE_IMPL_LIGHT(CallbackProxy::JsResultReceiver, IResultReceiver);

void CallbackProxy::JsResultReceiver::SetReady()
{
    mReady = TRUE;
    if (mTriedToNotifyBeforeReady) {
        NotifyCallbackProxy();
    }
}

/* Wake up the WebCore thread. */
ECode CallbackProxy::JsResultReceiver::OnJsResultComplete(
    /* [in] */ IJsResult* result)
{
    if (mReady) {
        NotifyCallbackProxy();
    }
    else {
        mTriedToNotifyBeforeReady = TRUE;
    }
    return NOERROR;
}

void CallbackProxy::JsResultReceiver::NotifyCallbackProxy()
{
    AutoLock lock(mOwner);
    mOwner->Notify();
}

ECode CallbackProxy::JsResultReceiver::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "CallbackProxy::JsResultReceiver";
    return NOERROR;
}

//===============================================================
//                 CallbackProxy::UploadFile
//===============================================================

CAR_INTERFACE_IMPL_LIGHT(CallbackProxy::UploadFile, IValueCallback);

ECode CallbackProxy::UploadFile::OnReceiveValue(
    /* [in] */ IInterface* value)
{
    mValue = IUri::Probe(value);
    {
        AutoLock lock(mOwner);
        mOwner->Notify();
    }
    return NOERROR;
}

AutoPtr<IUri> CallbackProxy::UploadFile::GetResult()
{
    return mValue;
}

ECode CallbackProxy::UploadFile::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "CallbackProxy::UploadFile";
    return NOERROR;
}

//===============================================================
//              CallbackProxy::UploadFileMessageData
//===============================================================

CallbackProxy::UploadFileMessageData::UploadFileMessageData(
    /* [in] */ CallbackProxy::UploadFile* uploadFile,
    /* [in] */ const String& acceptType,
    /* [in] */ const String& capture)
       : mCallback(uploadFile)
       , mAcceptType(acceptType)
       , mCapture(capture)
{
}

CAR_INTERFACE_IMPL_LIGHT(CallbackProxy::UploadFileMessageData, IInterface);

AutoPtr<CallbackProxy::UploadFile> CallbackProxy::UploadFileMessageData::GetUploadFile()
{
    return mCallback;
}

String CallbackProxy::UploadFileMessageData::GetAcceptType()
{
    return mAcceptType;
}

String CallbackProxy::UploadFileMessageData::GetCapture()
{
    return mCapture;
}

ECode CallbackProxy::UploadFileMessageData::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "CallbackProxy::UploadFileMessageData";
    return NOERROR;
}

//===============================================================
//           CallbackProxy::InnerDialogOnClickListener
//===============================================================

CallbackProxy::InnerDialogOnClickListener::InnerDialogOnClickListener(
    /* [in] */ IJsResult* res,
    /* [in] */ CallbackProxy* owner)
    : mRes(res)
    , mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(CallbackProxy::InnerDialogOnClickListener, IDialogInterfaceOnClickListener);

ECode CallbackProxy::InnerDialogOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    return mRes->Confirm();
}

ECode CallbackProxy::InnerDialogOnClickListener::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "CallbackProxy::InnerDialogOnClickListener";
    return NOERROR;
}

//===============================================================
//          CallbackProxy::InnerDialogOnClickListenerEx
//===============================================================

CallbackProxy::InnerDialogOnClickListenerEx::InnerDialogOnClickListener(
    /* [in] */ IJsPromptResult* res,
    /* [in] */ IEditText* text,
    /* [in] */ CallbackProxy* owner)
    : mRes(res)
    , mEditText(text)
    , mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(CallbackProxy::InnerDialogOnClickListenerEx, IDialogInterfaceOnClickListener);

ECode CallbackProxy::InnerDialogOnClickListenerEx::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    AutoPtr<ICharSequence> text;
    mEditText->GetText((ICharSequence**)&text);
    String str;
    text->ToString(&str);
    return mRes->Confirm(str);
}

ECode CallbackProxy::InnerDialogOnClickListenerEx::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "CallbackProxy::InnerDialogOnClickListenerEx";
    return NOERROR;
}

//===============================================================
//           CallbackProxy::InnerDialogOnCancelListener
//===============================================================

CallbackProxy::InnerDialogOnCancelListener::InnerDialogOnCancelListener(
    /* [in] */ IJsResult* res,
    /* [in] */ CallbackProxy* owner)
    : mRes(res)
    , mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(CallbackProxy::InnerDialogOnCancelListener, IDialogInterfaceOnCancelListener);

ECode CallbackProxy::InnerDialogOnCancelListener::OnCancel(
    /* [in] */ IDialogInterface* dialog)
{
    return mRes->Cancel();
}

ECode CallbackProxy::InnerDialogOnCancelListener::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "CallbackProxy::InnerDialogOnCancelListener";
    return NOERROR;
}

//===============================================================
//           CallbackProxy::CancelClickListener
//===============================================================

CallbackProxy::CancelClickListener::CancelClickListener(
    /* [in] */ IJsResult* res,
    /* [in] */ CallbackProxy* owner)
    : mRes(res)
    , mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(CallbackProxy::CancelClickListener, IDialogInterfaceOnClickListener);

ECode CallbackProxy::CancelClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    return mRes->Cancel();
}

ECode CallbackProxy::CancelClickListener::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "CallbackProxy::CancelClickListener";
    return NOERROR;
}

//===============================================================
//                         CallbackProxy
//===============================================================
// Performance probe
const Boolean CallbackProxy::PERF_PROBE  = FALSE;

// Logging tag
const String CallbackProxy::LOGTAG("CallbackProxy");

// Message IDs
const Int32 CallbackProxy::PAGE_STARTED;
const Int32 CallbackProxy::RECEIVED_ICON;
const Int32 CallbackProxy::RECEIVED_TITLE;
const Int32 CallbackProxy::OVERRIDE_URL;
const Int32 CallbackProxy::AUTH_REQUEST;
const Int32 CallbackProxy::SSL_ERROR;
const Int32 CallbackProxy::PROGRESS;
const Int32 CallbackProxy::UPDATE_VISITED;
const Int32 CallbackProxy::LOAD_RESOURCE;
const Int32 CallbackProxy::CREATE_WINDOW;
const Int32 CallbackProxy::CLOSE_WINDOW;
const Int32 CallbackProxy::SAVE_PASSWORD;
const Int32 CallbackProxy::JS_ALERT;
const Int32 CallbackProxy::JS_CONFIRM;
const Int32 CallbackProxy::JS_PROMPT;
const Int32 CallbackProxy::JS_UNLOAD;
const Int32 CallbackProxy::ASYNC_KEYEVENTS;
const Int32 CallbackProxy::DOWNLOAD_FILE;
const Int32 CallbackProxy::REPORT_ERROR;
const Int32 CallbackProxy::RESEND_POST_DATA;
const Int32 CallbackProxy::PAGE_FINISHED;
const Int32 CallbackProxy::REQUEST_FOCUS;
const Int32 CallbackProxy::SCALE_CHANGED;
const Int32 CallbackProxy::RECEIVED_CERTIFICATE;
const Int32 CallbackProxy::SWITCH_OUT_HISTORY;
const Int32 CallbackProxy::EXCEEDED_DATABASE_QUOTA;
const Int32 CallbackProxy::REACHED_APPCACHE_MAXSIZE;
const Int32 CallbackProxy::JS_TIMEOUT;
const Int32 CallbackProxy::ADD_MESSAGE_TO_CONSOLE;
const Int32 CallbackProxy::GEOLOCATION_PERMISSIONS_SHOW_PROMPT;
const Int32 CallbackProxy::GEOLOCATION_PERMISSIONS_HIDE_PROMPT;
const Int32 CallbackProxy::RECEIVED_TOUCH_ICON_URL;
const Int32 CallbackProxy::GET_VISITED_HISTORY;
const Int32 CallbackProxy::OPEN_FILE_CHOOSER;
const Int32 CallbackProxy::ADD_HISTORY_ITEM;
const Int32 CallbackProxy::HISTORY_INDEX_CHANGED;
const Int32 CallbackProxy::AUTH_CREDENTIALS;
const Int32 CallbackProxy::AUTO_LOGIN;
const Int32 CallbackProxy::CLIENT_CERT_REQUEST;
const Int32 CallbackProxy::PROCEEDED_AFTER_SSL_ERROR;

// Message triggered by the client to resume execution
const Int32 CallbackProxy::NOTIFY;

/**
 * Construct a new CallbackProxy.
 */
CallbackProxy::CallbackProxy(
    /* [in] */ IContext* context,
    /* [in] */ CWebViewClassic* w)
    : mWebCoreThreadTime(0)
    , mWebCoreIdleTime(0)
    , mProgressUpdatePending(FALSE)
    , mLatestProgress(100)
    , mBlockMessages(FALSE)
    , mContext(context)
    , mWebView(w)
{
    // Used to start a default activity.
    mBackForwardList = new WebBackForwardListClassic(this);
}

/**
 * Set the WebViewClient.
 * @param client An implementation of WebViewClient.
 */
void CallbackProxy::SetWebViewClient(
    /* [in] */ IWebViewClient* client)
{
    mWebViewClient = client;
}

/**
 * Get the WebViewClient.
 * @return the current WebViewClient instance.
 */
AutoPtr<IWebViewClient> CallbackProxy::GetWebViewClient()
{
    return mWebViewClient;
}

/**
 * Set the WebChromeClient.
 * @param client An implementation of WebChromeClient.
 */
void CallbackProxy::SetWebChromeClient(
    /* [in] */ IWebChromeClient* client)
{
    mWebChromeClient = client;
}

/**
 * Get the WebChromeClient.
 * @return the current WebChromeClient instance.
 */
AutoPtr<IWebChromeClient> CallbackProxy::GetWebChromeClient()
{
    return mWebChromeClient;
}

/**
 * Set the client DownloadListener.
 * @param client An implementation of DownloadListener.
 */
void CallbackProxy::SetDownloadListener(
    /* [in] */ IDownloadListener* client)
{
    mDownloadListener = client;
}

/**
 * Get the Back/Forward list to return to the user or to update the cached
 * history list.
 */
AutoPtr<WebBackForwardListClassic> CallbackProxy::GetBackForwardList()
{
    return mBackForwardList;
}

void CallbackProxy::SetWebBackForwardListClient(
    /* [in] */ IWebBackForwardListClient* client)
{
    mWebBackForwardListClient = client;
}

AutoPtr<IWebBackForwardListClient> CallbackProxy::GetWebBackForwardListClient()
{
    return mWebBackForwardListClient;
}

/**
 * Called by the UI side.  Calling overrideUrlLoading from the WebCore
 * side will post a message to call this method.
 */
Boolean CallbackProxy::UiOverrideUrlLoading(
    /* [in] */ const String& overrideUrl)
{
    if (overrideUrl.IsNullOrEmpty()) {
        return FALSE;
    }

    Boolean override = FALSE;

    if (mWebViewClient != NULL) {
        AutoPtr<IWebView> webView;
        mWebView->GetWebView((IWebView**)&webView);
        mWebViewClient->ShouldOverrideUrlLoading(webView,
                overrideUrl, &override);
    }
    else {
        AutoPtr<IIntent> intent;
        AutoPtr<IUri> uri;
        Uri::Parse(overrideUrl, (IUri**)&uri);
        CIntent::New(IIntent::ACTION_VIEW, uri, (IIntent**)&intent);
        if (!intent)
        {
            Logger::V(LOGTAG, "CallbackProxy::UiOverrideUrlLoading intent is null");
            return override;
        }
        intent->AddCategory(IIntent::CATEGORY_BROWSABLE);
        // If another application is running a WebView and launches the
        // Browser through this Intent, we want to reuse the same window if
        // possible.
        String pkgName;
        mContext->GetPackageName(&pkgName);
        Logger::V(LOGTAG, "TODO no implementation Uri CallbackProxy::UiOverrideUrlLoading, uri:%s, pkgName:%s",
                overrideUrl.string(), pkgName.string());
        //return override;//just return
        intent->PutExtra(String("com.android.browser.application_id"),//IBrowser::EXTRA_APPLICATION_ID,//TODO no implementation,just hardcode
                pkgName);
        //try {
            ECode result = mContext->StartActivity(intent);
            if (SUCCEEDED(result))
                override = TRUE;
        //} catch (ActivityNotFoundException ex) {
            // If no application can handle the URL, assume that the
            // browser can handle it.
        //}
    }

    return override;
}

/**
 * Called by UI side.
 */
Boolean CallbackProxy::UiOverrideKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    if (mWebViewClient != NULL) {
        Boolean bFlag = FALSE;
        AutoPtr<IWebView> webView;
        mWebView->GetWebView((IWebView**)&webView);
        mWebViewClient->ShouldOverrideKeyEvent(webView,
         event, &bFlag);
        return bFlag;
    }

    return FALSE;
}

ECode CallbackProxy::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    // We don't have to do synchronization because this function operates
    // in the UI thread. The WebViewClient and WebChromeClient functions
    // that check for a non-null callback are ok because java ensures atomic
    // 32-bit reads and writes.
    if (MessagesBlocked()) return NOERROR;
    Int32 what;
    msg->GetWhat(&what);
    if (DebugFlags::CALLBACK_PROXY) Logger::V(LOGTAG, "HandleMessage: %d , mWebViewClient:%p ", what, mWebViewClient.Get());
    switch (what) {
        case PAGE_STARTED: {
            AutoPtr<IBundle> bundle;
            msg->GetData((IBundle**)&bundle);
            String startedUrl;
            bundle->GetString(String("url"), &startedUrl);
            mWebView->OnPageStarted(startedUrl);
            if (mWebViewClient != NULL) {
                AutoPtr<IWebView> view;
                mWebView->GetWebView((IWebView**)&view);
                AutoPtr<IBitmap> obj;
                msg->GetObj((IInterface**)&obj);
                mWebViewClient->OnPageStarted(view, startedUrl, obj);
            }
            break;
        }
        case PAGE_FINISHED: {
            AutoPtr<ICharSequence> obj;
            msg->GetObj((IInterface**)&obj);
            String finishedUrl;
            obj->ToString(&finishedUrl);
            mWebView->OnPageFinished(finishedUrl);
            if (mWebViewClient != NULL) {
                AutoPtr<IWebView> webview;
                mWebView->GetWebView((IWebView**)&webview);
                mWebViewClient->OnPageFinished(webview, finishedUrl);
            }
            break;
        }
        case RECEIVED_ICON:
            if (mWebChromeClient != NULL) {
                AutoPtr<IBitmap> obj;
                msg->GetObj((IInterface**)&obj);
                AutoPtr<IWebView> webview;
                mWebView->GetWebView((IWebView**)&webview);
                mWebChromeClient->OnReceivedIcon(webview, obj);
            }
            break;
        case RECEIVED_TOUCH_ICON_URL:
            if (mWebChromeClient != NULL) {
                AutoPtr<IWebView> webView;
                mWebView->GetWebView((IWebView**)&webView);
                AutoPtr<ICharSequence> obj;
                msg->GetObj((IInterface**)&obj);
                String objStr;
                obj->ToString(&objStr);
                Int32 arg1;
                msg->GetArg1(&arg1);
                mWebChromeClient->OnReceivedTouchIconUrl(webView,
                        objStr, arg1 == 1);
            }
            break;
        case RECEIVED_TITLE:
            if (mWebChromeClient != NULL) {
                AutoPtr<IWebView> webView;
                mWebView->GetWebView((IWebView**)&webView);
                AutoPtr<ICharSequence> obj;
                msg->GetObj((IInterface**)&obj);
                String objStr;
                obj->ToString(&objStr);
                mWebChromeClient->OnReceivedTitle(webView, objStr);
            }
            break;
        case REPORT_ERROR:
            if (mWebViewClient != NULL) {
                AutoPtr<IWebView> webView;
                mWebView->GetWebView((IWebView**)&webView);
                Int32 reasonCode;
                msg->GetArg1(&reasonCode);
                AutoPtr<IBundle> bundle;
                msg->GetData((IBundle**)&bundle);
                String description;
                bundle->GetString(String("description"), &description);
                String failUrl;
                bundle->GetString(String("failingUrl"), &failUrl);
                mWebViewClient->OnReceivedError(webView, reasonCode,
                        description, failUrl);
            }
            break;
        case RESEND_POST_DATA: {
            AutoPtr<IBundle> bundle;
            msg->GetData((IBundle**)&bundle);
            AutoPtr<IParcelable> resend;
            bundle->GetParcelable(String("resend"), (IParcelable**)&resend);
            AutoPtr<IMessage> resendMsg = IMessage::Probe(resend);
            AutoPtr<IParcelable> dontResend;
            bundle->GetParcelable(String("dontResend"), (IParcelable**)&dontResend);
            AutoPtr<IMessage> dontResendMsg = IMessage::Probe(dontResend);
            if (mWebViewClient != NULL) {
                AutoPtr<IWebView> webView;
                mWebView->GetWebView((IWebView**)&webView);
                mWebViewClient->OnFormResubmission(webView, dontResendMsg,
                        resendMsg);
            }
            else {
                dontResendMsg->SendToTarget();
            }
            break;
        }
        case OVERRIDE_URL: {
            AutoPtr<IBundle> bundle;
            msg->GetData((IBundle**)&bundle);
            String overrideUrl;
            bundle->GetString(String("url"), &overrideUrl);
            Boolean override = UiOverrideUrlLoading(overrideUrl);
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            ResultTransport* result = (ResultTransport*)obj.Get();
            {
                AutoLock lock(this);
                AutoPtr<IBoolean> iOverride;
                CBoolean::New(override, (IBoolean**)&iOverride);
                result->SetResult(iOverride);
                Notify();
            }
            break;
        }
        case AUTH_REQUEST:
            if (mWebViewClient != NULL) {
                AutoPtr<IHttpAuthHandler> handler;
                msg->GetObj((IInterface**)&handler);
                AutoPtr<IBundle> bundle;
                msg->GetData((IBundle**)&bundle);
                String host;
                bundle->GetString(String("host"), &host);
                String realm;
                bundle->GetString(String("realm"), &realm);
                AutoPtr<IWebView> webView;
                mWebView->GetWebView((IWebView**)&webView);
                mWebViewClient->OnReceivedHttpAuthRequest(webView, handler,
                        host, realm);
            }
            break;
        case SSL_ERROR:
            if (mWebViewClient != NULL) {
                AutoPtr<IObjectStringMap> map;
                msg->GetObj((IInterface**)&map);
                AutoPtr<ISslErrorHandler> sslErrorHandler;
                map->Get(String("handler"), (IInterface**)&sslErrorHandler);
                AutoPtr<ISslError> sslError;
                map->Get(String("error"), (IInterface**)&sslError);
                AutoPtr<IWebView> webView;
                mWebView->GetWebView((IWebView**)&webView);
                mWebViewClient->OnReceivedSslError(webView, sslErrorHandler,
                        sslError);
            }
            break;
        case PROCEEDED_AFTER_SSL_ERROR: {
            AutoPtr<IWebViewClientClassicExt> webViewCCE;
            if (mWebViewClient != NULL &&
                (webViewCCE = IWebViewClientClassicExt::Probe(mWebViewClient)) != NULL) {
                AutoPtr<IWebView> webView;
                mWebView->GetWebView((IWebView**)&webView);
                AutoPtr<ISslError> sslError;
                msg->GetObj((IInterface**)&sslError);
                webViewCCE->OnProceededAfterSslError(webView, sslError);
            }
            break;
        }
        case CLIENT_CERT_REQUEST:
            if (mWebViewClient != NULL  && IWebViewClientClassicExt::Probe(mWebViewClient) != NULL) {
                AutoPtr<IObjectStringMap> map;
                msg->GetObj((IInterface**)&map);
                AutoPtr<IWebViewClientClassicExt> wvc = IWebViewClientClassicExt::Probe(mWebViewClient);
                AutoPtr<IWebView> wv;
                mWebView->GetWebView((IWebView**)&wv);
                AutoPtr<IClientCertRequestHandler> ccrh;
                map->Get(String("handler"), (IInterface**)&ccrh);
                AutoPtr<ICharSequence> hostAndPortCS;
                map->Get(String("host_and_port"), (IInterface**)&hostAndPortCS);
                String hostAndPort;
                hostAndPortCS->ToString(&hostAndPort);
                wvc->OnReceivedClientCertRequest(wv, ccrh, hostAndPort);
            }
            break;
        case PROGRESS:
            // Synchronize to ensure mLatestProgress is not modified after
            // setProgress is called and before mProgressUpdatePending is
            // changed.
            {
                AutoLock lock(this);
                if (mWebChromeClient != NULL) {
                    AutoPtr<IWebView> wv;
                    mWebView->GetWebView((IWebView**)&wv);
                    mWebChromeClient->OnProgressChanged(wv, mLatestProgress);
                }
                mProgressUpdatePending = FALSE;
            }
            break;

        case UPDATE_VISITED:
            if (mWebViewClient != NULL) {
                AutoPtr<IWebView> wv;
                mWebView->GetWebView((IWebView**)&wv);
                AutoPtr<ICharSequence> objCS;
                msg->GetObj((IInterface**)&objCS);
                String obj;
                objCS->ToString(&obj);
                Int32 arg1;
                msg->GetArg1(&arg1);
                mWebViewClient->DoUpdateVisitedHistory(wv, obj, arg1);
            }
            break;

        case LOAD_RESOURCE:
            if (mWebViewClient != NULL) {
                AutoPtr<IWebView> wv;
                mWebView->GetWebView((IWebView**)&wv);
                AutoPtr<ICharSequence> objCS;
                msg->GetObj((IInterface**)&objCS);
                String obj;
                objCS->ToString(&obj);
                mWebViewClient->OnLoadResource(wv, obj);
            }
            break;

        case DOWNLOAD_FILE:
            if (mDownloadListener != NULL) {
                AutoPtr<IBundle> bundle;
                msg->GetData((IBundle**)&bundle);
                String url;
                bundle->GetString(String("url"), &url);
                String userAgent;
                bundle->GetString(String("userAgent"), &userAgent);
                String contentDisposition;
                bundle->GetString(String("contentDisposition"), &contentDisposition);
                String mimetype;
                bundle->GetString(String("mimetype"), &mimetype);
                String referer;
                bundle->GetString(String("referer"), &referer);
                Int64 contentLength;
                bundle->GetInt64(String("contentLength"), &contentLength);

                AutoPtr<IBrowserDownloadListener> bdl = IBrowserDownloadListener::Probe(mDownloadListener);
                if (bdl != NULL) {
                    bdl->OnDownloadStart(url,
                         userAgent, contentDisposition, mimetype, referer, contentLength);
                }
                else {
                    mDownloadListener->OnDownloadStart(url, userAgent,
                         contentDisposition, mimetype, contentLength);
                }
            }
            break;

        case CREATE_WINDOW:
            if (mWebChromeClient != NULL) {
                AutoPtr<IWebView> wv;
                mWebView->GetWebView((IWebView**)&wv);
                Int32 arg1, arg2;
                msg->GetArg1(&arg1);
                msg->GetArg2(&arg2);
                AutoPtr<IMessage> msgObj;
                msg->GetObj((IInterface**)&msgObj);
                Boolean result = FALSE;
                mWebChromeClient->OnCreateWindow(wv, arg1, arg2, msgObj, &result);
                if (!result) {
                    AutoLock lock(this);
                    Notify();
                }
                mWebView->DismissZoomControl();
            }
            break;

        case REQUEST_FOCUS:
            if (mWebChromeClient != NULL) {
                AutoPtr<IWebView> wv;
                mWebView->GetWebView((IWebView**)&wv);
                mWebChromeClient->OnRequestFocus(wv);
            }
            break;

        case CLOSE_WINDOW:
            if (mWebChromeClient != NULL) {
                AutoPtr<IWebViewClassic> wvc;
                msg->GetObj((IInterface**)&wvc);
                AutoPtr<IWebView> wv;
                wvc->GetWebView((IWebView**)&wv);
                mWebChromeClient->OnCloseWindow(wv);
            }
            break;

        case SAVE_PASSWORD: {
                AutoPtr<IBundle> bundle;
                msg->GetData((IBundle**)&bundle);
                String schemePlusHost, username, password;
                bundle->GetString(String("host"), &schemePlusHost);
                bundle->GetString(String("username"), &username);
                bundle->GetString(String("password"), &password);
                // If the client returned false it means that the notify message
                // will not be sent and we should notify WebCore ourselves.
                AutoPtr<IMessage> msgObj;
                msg->GetObj((IInterface**)&msgObj);
                if (!mWebView->OnSavePassword(schemePlusHost, username, password, msgObj)) {
                    AutoLock lock(this);
                    Notify();
                }
            }
            break;

        case ASYNC_KEYEVENTS:
            if (mWebViewClient != NULL) {
                AutoPtr<IWebView> wv;
                mWebView->GetWebView((IWebView**)&wv);
                AutoPtr<IKeyEvent> keyEvent;
                msg->GetObj((IInterface**)&keyEvent);
                mWebViewClient->OnUnhandledKeyEvent(wv, keyEvent);
            }
            break;

        case EXCEEDED_DATABASE_QUOTA:
            if (mWebChromeClient != NULL) {
                AutoPtr<IObjectStringMap> map;
                msg->GetObj((IInterface**)&map);
                AutoPtr<ICharSequence> databaseIdentifierCS;
                map->Get(String("databaseIdentifier"), (IInterface**)&databaseIdentifierCS);
                String databaseIdentifier;
                databaseIdentifierCS->ToString(&databaseIdentifier);
                AutoPtr<ICharSequence> urlCS;
                map->Get(String("url"), (IInterface**)&urlCS);
                String url;
                urlCS->ToString(&url);
                AutoPtr<IInteger64> iQuota;
                map->Get(String("quota"), (IInterface**)&iQuota);
                Int64 quota;
                iQuota->GetValue(&quota);
                AutoPtr<IInteger64> iTotalQuota;
                map->Get(String("totalQuota"), (IInterface**)&iTotalQuota);
                Int64 totalQuota;
                iTotalQuota->GetValue(&totalQuota);
                AutoPtr<IInteger64> iEstimatedDatabaseSize;
                map->Get(String("estimatedDatabaseSize"), (IInterface**)&iEstimatedDatabaseSize);
                Int64 estimatedDatabaseSize;
                iEstimatedDatabaseSize->GetValue(&estimatedDatabaseSize);
                AutoPtr<IWebStorageQuotaUpdater> quotaUpdater;
                map->Get(String("quotaUpdater"), (IInterface**)&quotaUpdater);
                mWebChromeClient->OnExceededDatabaseQuota(url,
                        databaseIdentifier, quota, estimatedDatabaseSize,
                        totalQuota, quotaUpdater);
            }
            break;

        case REACHED_APPCACHE_MAXSIZE:
            if (mWebChromeClient != NULL) {
                AutoPtr<IObjectStringMap> map;
                msg->GetObj((IInterface**)&map);
                AutoPtr<IInteger64> iRequiredStorage;
                map->Get(String("requiredStorage"), (IInterface**)&iRequiredStorage);
                Int64 requiredStorage;
                iRequiredStorage->GetValue(&requiredStorage);
                AutoPtr<IInteger64> iQuota;
                map->Get(String("quota"), (IInterface**)&iQuota);
                Int64 quota;
                iQuota->GetValue(&quota);
                AutoPtr<IWebStorageQuotaUpdater> quotaUpdater;
                map->Get(String("quotaUpdater"), (IInterface**)&quotaUpdater);
                mWebChromeClient->OnReachedMaxAppCacheSize(requiredStorage,
                        quota, quotaUpdater);
            }
            break;

        case GEOLOCATION_PERMISSIONS_SHOW_PROMPT:
            if (mWebChromeClient != NULL) {
                AutoPtr<IObjectStringMap> map;
                msg->GetObj((IInterface**)&map);
                AutoPtr<ICharSequence> originCS;
                map->Get(String("origin"), (IInterface**)&originCS);
                String origin;
                originCS->ToString(&origin);
                AutoPtr<IGeolocationPermissionsCallback> callback;
                map->Get(String("callback"), (IInterface**)&callback);
                mWebChromeClient->OnGeolocationPermissionsShowPrompt(origin,
                        callback);
            }
            break;

        case GEOLOCATION_PERMISSIONS_HIDE_PROMPT:
            if (mWebChromeClient != NULL) {
                mWebChromeClient->OnGeolocationPermissionsHidePrompt();
            }
            break;

        case JS_ALERT:
            if (mWebChromeClient != NULL) {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                JsResultReceiver* receiver = (JsResultReceiver*)obj.Get();
                AutoPtr<IJsResult> res = receiver->mJsResult;
                AutoPtr<IBundle> bundle;
                msg->GetData((IBundle**)&bundle);
                String message;
                bundle->GetString(String("message"), &message);
                String url;
                bundle->GetString(String("url"), &url);
                AutoPtr<IWebView> wv;
                mWebView->GetWebView((IWebView**)&wv);
                Boolean result = FALSE;
                mWebChromeClient->OnJsAlert(wv, url, message, res, &result);
                if (!result) {
                    if (!CanShowAlertDialog()) {
                        res->Cancel();
                        receiver->SetReady();
                        break;
                    }

                    AutoPtr<IAlertDialogBuilder> builder;
                    CAlertDialogBuilder::New(mContext, (IAlertDialogBuilder**)&builder);
                    AutoPtr<ICharSequence> title;
                    CStringWrapper::New(GetJsDialogTitle(url), (ICharSequence**)&title);
                    builder->SetTitle(title);
                    AutoPtr<ICharSequence> messageCS;
                    CStringWrapper::New(message, (ICharSequence**)&messageCS);
                    builder->SetMessage(messageCS);
                    AutoPtr<IDialogInterfaceOnClickListener> clickListener = new InnerDialogOnClickListener(res, this);
                    builder->SetPositiveButton(R::string::ok, clickListener);
                    AutoPtr<IDialogInterfaceOnCancelListener> cancelListener = new InnerDialogOnCancelListener(res, this);
                    builder->SetOnCancelListener(cancelListener);
                    AutoPtr<IAlertDialog> alertDialog;
                    builder->Show((IAlertDialog**)&alertDialog);
                }
                receiver->SetReady();
            }
            break;

        case JS_CONFIRM:
            if (mWebChromeClient != NULL) {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                JsResultReceiver* receiver = (JsResultReceiver*)obj.Get();
                AutoPtr<IJsResult> res = receiver->mJsResult;
                AutoPtr<IBundle> bundle;
                msg->GetData((IBundle**)&bundle);
                String message;
                bundle->GetString(String("message"), &message);
                String url;
                bundle->GetString(String("url"), &url);
                AutoPtr<IWebView> wv;
                mWebView->GetWebView((IWebView**)&wv);
                Boolean result = FALSE;
                mWebChromeClient->OnJsConfirm(wv, url, message, res, &result);
                if (!result) {
                    if (!CanShowAlertDialog()) {
                        res->Cancel();
                        receiver->SetReady();
                        break;
                    }
                    AutoPtr<IAlertDialogBuilder> builder;
                    CAlertDialogBuilder::New(mContext, (IAlertDialogBuilder**)&builder);
                    AutoPtr<ICharSequence> title;
                    CStringWrapper::New(GetJsDialogTitle(url), (ICharSequence**)&title);
                    builder->SetTitle(title);
                    AutoPtr<ICharSequence> messageCS;
                    CStringWrapper::New(message, (ICharSequence**)&messageCS);
                    builder->SetMessage(messageCS);
                    AutoPtr<IDialogInterfaceOnClickListener> positive = new InnerDialogOnClickListener(res, this);
                    builder->SetPositiveButton(R::string::ok, positive);
                    AutoPtr<IDialogInterfaceOnClickListener> negative = new CancelClickListener(res, this);
                    builder->SetNegativeButton(R::string::cancel, negative);
                    AutoPtr<IDialogInterfaceOnCancelListener> cancelListener =  new InnerDialogOnCancelListener(res, this);
                    builder->SetOnCancelListener(cancelListener);
                    AutoPtr<IAlertDialog> alertDialog;
                    builder->Show((IAlertDialog**)&alertDialog);
                }
                // Tell the JsResult that it is ready for client
                // interaction.
                receiver->SetReady();
            }
            break;

        case JS_PROMPT:
            if (mWebChromeClient != NULL) {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                JsResultReceiver* receiver = (JsResultReceiver*)obj.Get();
                AutoPtr<IJsPromptResult> res = receiver->mJsResult;
                AutoPtr<IBundle> bundle;
                msg->GetData((IBundle**)&bundle);
                String message;
                bundle->GetString(String("message"), &message);
                String defaultVal;
                bundle->GetString(String("default"), &defaultVal);
                String url;
                bundle->GetString(String("url"), &url);
                AutoPtr<IWebView> wv;
                mWebView->GetWebView((IWebView**)&wv);
                Boolean result = FALSE;
                mWebChromeClient->OnJsPrompt(wv, url, message,
                            defaultVal, res, &result);
                if (!result) {
                    if (!CanShowAlertDialog()) {
                        res->Cancel();
                        receiver->SetReady();
                        break;
                    }
                    AutoPtr<ILayoutInflater> factory;
                    LayoutInflater::From(mContext, (ILayoutInflater**)&factory);
                    AutoPtr<IView> view;
                    factory->Inflate(R::layout::js_prompt, NULL, (IView**)&view);
                    AutoPtr<IEditText> v;
                    view->FindViewById(R::id::value, (IView**)&v);
                    AutoPtr<ICharSequence> defaultValCS;
                    CStringWrapper::New(defaultVal, (ICharSequence**)&defaultValCS);
                    v->SetText(defaultValCS);
                    AutoPtr<ITextView> textView;
                    view->FindViewById(R::id::message, (IView**)&textView);
                    AutoPtr<ICharSequence> messageCS;
                    CStringWrapper::New(message, (ICharSequence**)&messageCS);
                    textView->SetText(messageCS);
                    AutoPtr<IAlertDialogBuilder> builder;
                    CAlertDialogBuilder::New(mContext, (IAlertDialogBuilder**)&builder);
                    AutoPtr<ICharSequence> title;
                    CStringWrapper::New(GetJsDialogTitle(url), (ICharSequence**)&title);
                    builder->SetTitle(title);
                    builder->SetView(view);
                    AutoPtr<IDialogInterfaceOnClickListener> listener = new InnerDialogOnClickListener(res, v, this);
                    builder->SetPositiveButton(R::string::ok, listener);
                    AutoPtr<IDialogInterfaceOnClickListener> clickListener = new CancelClickListener(res, this);
                    builder->SetNegativeButton(R::string::cancel, clickListener);
                    AutoPtr<IDialogInterfaceOnCancelListener> cancelListener = new InnerDialogOnCancelListener(res, this);
                    builder->SetOnCancelListener(cancelListener);
                    AutoPtr<IAlertDialog> alertDialog;
                    builder->Show((IAlertDialog**)&alertDialog);
                }
                // Tell the JsResult that it is ready for client
                // interaction.
                receiver->SetReady();
            }
            break;

        case JS_UNLOAD:
            if (mWebChromeClient != NULL) {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                JsResultReceiver* receiver = (JsResultReceiver*)obj.Get();
                AutoPtr<IJsResult> res = receiver->mJsResult;
                AutoPtr<IBundle> bundle;
                msg->GetData((IBundle**)&bundle);
                String message;
                bundle->GetString(String("message"), &message);
                String url;
                bundle->GetString(String("url"), &url);
                AutoPtr<IWebView> wv;
                mWebView->GetWebView((IWebView**)&wv);
                Boolean result = FALSE;
                mWebChromeClient->OnJsBeforeUnload(wv, url, message, res, &result);
                if (!result) {
                    if (!CanShowAlertDialog()) {
                        res->Cancel();
                        receiver->SetReady();
                        break;
                    }
                    String m;
                    AutoPtr< ArrayOf<IInterface*> > args = ArrayOf<IInterface*>::Alloc(1);
                    AutoPtr<ICharSequence> messageCS;
                    CStringWrapper::New(message, (ICharSequence**)&messageCS);
                    args->Set(0, messageCS);
                    mContext->GetString(R::string::js_dialog_before_unload, args, &m);
                    AutoPtr<IAlertDialogBuilder> builder;
                    CAlertDialogBuilder::New(mContext, (IAlertDialogBuilder**)&builder);
                    AutoPtr<ICharSequence> _m;
                    CStringWrapper::New(m, (ICharSequence**)&_m);
                    builder->SetMessage(_m);
                    AutoPtr<IDialogInterfaceOnClickListener> listener = new InnerDialogOnClickListener(res, this);
                    builder->SetPositiveButton(R::string::ok, listener);
                    AutoPtr<IDialogInterfaceOnClickListener> clickListener = new CancelClickListener(res, this);
                    builder->SetNegativeButton(R::string::cancel, clickListener);
                    AutoPtr<IDialogInterfaceOnCancelListener> cancelListener = new InnerDialogOnCancelListener(res, this);
                    builder->SetOnCancelListener(cancelListener);
                    AutoPtr<IAlertDialog> alertDialog;
                    builder->Show((IAlertDialog**)&alertDialog);
                }
                receiver->SetReady();
            }
            break;

        case JS_TIMEOUT:
            if(mWebChromeClient != NULL) {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                JsResultReceiver* receiver = (JsResultReceiver*)obj.Get();
                AutoPtr<IJsResult> res = receiver->mJsResult;
                Boolean result = FALSE;
                mWebChromeClient->OnJsTimeout(&result);
                if(result) {
                    res->Confirm();
                }
                else {
                    res->Cancel();
                }
                receiver->SetReady();
            }
            break;

        case RECEIVED_CERTIFICATE: {
                AutoPtr<ISslCertificate> ssl;
                msg->GetObj((IInterface**)&ssl);
                mWebView->SetCertificate(ssl);
            }
            break;

        case NOTIFY: {
                AutoLock lock(this);
                Notify();
            }
            break;

        case SCALE_CHANGED:
            if (mWebViewClient != NULL) {
                AutoPtr<IWebView> wv;
                mWebView->GetWebView((IWebView**)&wv);
                AutoPtr<IBundle> bundle;
                msg->GetData((IBundle**)&bundle);
                Float oldValue;
                bundle->GetFloat(String("old"), &oldValue);
                Float newValue;
                bundle->GetFloat(String("new"), &newValue);
                mWebViewClient->OnScaleChanged(wv, oldValue, newValue);
            }
            break;

        case SWITCH_OUT_HISTORY:
            mWebView->SwitchOutDrawHistory();
            break;

        case ADD_MESSAGE_TO_CONSOLE: {
                if (mWebChromeClient == NULL) {
                    break;
                }
                AutoPtr<IBundle> bundle;
                msg->GetData((IBundle**)&bundle);
                String message;
                bundle->GetString(String("message"), &message);
                String sourceID;
                bundle->GetString(String("sourceID"), &sourceID);
                Int32 lineNumber;
                bundle->GetInt32(String("lineNumber"), &lineNumber);
                Int32 msgLevel;
                bundle->GetInt32(String("msgLevel"), &msgLevel);
                Int32 numberOfMessageLevels;// = ConsoleMessage.MessageLevel.values().length;
                // Sanity bounds check as we'll index an array with msgLevel
                if (msgLevel < 0 || msgLevel >= numberOfMessageLevels) {
                    msgLevel = 0;
                }

                MessageLevel messageLevel;// = ConsoleMessage.MessageLevel.values()[msgLevel];

                AutoPtr<IConsoleMessage> cm;
                CConsoleMessage::New(message, sourceID, lineNumber, messageLevel, (IConsoleMessage**)&cm);
                Boolean result = FALSE;
                mWebChromeClient->OnConsoleMessage(cm, &result);
                if (!result) {
                    // If false was returned the user did not provide their own console function so
                    //  we should output some default messages to the system log.
                    String logTag("Web Console");
                    String logMessage(message);
                    logMessage += " at ";
                    logMessage += sourceID;
                    logMessage += ":";
//                    logMessage += lineNumber;

                    switch (messageLevel) {
                        case TIP:
//                            Log.v(logTag, logMessage);
                            break;
                        case LOG:
//                            Log.i(logTag, logMessage);
                            break;
                        case WARNING:
//                            Log.w(logTag, logMessage);
                            break;
                        case ERROR:
//                            Log.e(logTag, logMessage);
                            break;
                        case DEBUG:
//                            Log.d(logTag, logMessage);
                            break;
                    }
                }
            }

            break;

        case GET_VISITED_HISTORY:
            if (mWebChromeClient != NULL) {
                AutoPtr<IValueCallback> value;
                msg->GetObj((IInterface**)&value);
                mWebChromeClient->GetVisitedHistory(value);
            }
            break;

        case OPEN_FILE_CHOOSER:
            if (mWebChromeClient != NULL) {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                UploadFileMessageData* data = (UploadFileMessageData*)obj.Get();
                mWebChromeClient->OpenFileChooser(data->GetUploadFile(), data->GetAcceptType(),
                        data->GetCapture());
            }
            break;

        case ADD_HISTORY_ITEM:
            if (mWebBackForwardListClient != NULL) {
                AutoPtr<IWebHistoryItem> item;
                msg->GetObj((IInterface**)&item);
                mWebBackForwardListClient->OnNewHistoryItem(item);
            }
            break;

        case HISTORY_INDEX_CHANGED:
            if (mWebBackForwardListClient != NULL) {
                AutoPtr<IWebHistoryItem> item;
                msg->GetObj((IInterface**)&item);
                Int32 arg1;
                msg->GetArg1(&arg1);
                mWebBackForwardListClient->OnIndexChanged(item, arg1);
            }
            break;
        case AUTH_CREDENTIALS: {
            AutoPtr<IBundle> bundle;
            msg->GetData((IBundle**)&bundle);
            String host;
            bundle->GetString(String("host"), &host);
            String realm;
            bundle->GetString(String("realm"), &realm);
            String username;
            bundle->GetString(String("username"), &username);
            String password;
            bundle->GetString(String("password"), &password);
            mWebView->SetHttpAuthUsernamePassword(
                    host, realm, username, password);
            break;
        }
        case AUTO_LOGIN: {
            if (mWebViewClient != NULL) {
                AutoPtr<IBundle> bundle;
                msg->GetData((IBundle**)&bundle);
                String realm;
                bundle->GetString(String("realm"), &realm);
                String account;
                bundle->GetString(String("account"), &account);
                String args;
                bundle->GetString(String("args"), &args);
                AutoPtr<IWebView> wv;
                mWebView->GetWebView((IWebView**)&wv);
                mWebViewClient->OnReceivedLoginRequest(wv, realm,
                        account, args);
            }
            break;
        }
    }

    return NOERROR;
}

/**
 * Return the latest progress.
 */
Int32 CallbackProxy::GetProgress()
{
    return mLatestProgress;
}

/**
 * Called by WebCore side to switch out of history Picture drawing mode
 */
void CallbackProxy::SwitchOutDrawHistory()
{
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, SWITCH_OUT_HISTORY, (IMessage**)&msg);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

//--------------------------------------------------------------------------
// WebViewClient functions.
// NOTE: shouldOverrideKeyEvent is never called from the WebCore thread so
// it is not necessary to include it here.
//--------------------------------------------------------------------------

void CallbackProxy::OnPageStarted(
    /* [in] */ const String& url,
    /* [in] */ IBitmap* favicon)
{
    // We need to send the message even if no WebViewClient is set, because we need to call
    // WebView.onPageStarted().

    // Performance probe
    if (PERF_PROBE) {
        assert(0);
//        mWebCoreThreadTime = SystemClock.currentThreadTimeMillis();
        mWebCoreIdleTime = 0;
        // un-comment this if PERF_PROBE is true
//            Looper.myQueue().setWaitCallback(mIdleCallback);
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, PAGE_STARTED, (IMessage**)&msg);
    msg->SetObj(favicon);
    AutoPtr<IBundle> bundle;
    msg->GetData((IBundle**)&bundle);
    bundle->PutString(String("url"), url);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

void CallbackProxy::OnPageFinished(
    /* [in] */ const String& url)
{
    // Performance probe
    if (PERF_PROBE) {
        // un-comment this if PERF_PROBE is true
//        Log.d("WebCore", "WebCore thread used " +
//                (SystemClock.currentThreadTimeMillis() - mWebCoreThreadTime)
//                + " ms and idled " + mWebCoreIdleTime + " ms");
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<ICharSequence> urlCS;
    CStringWrapper::New(url, (ICharSequence**)&urlCS);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, PAGE_FINISHED, urlCS, (IMessage**)&msg);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

// Because this method is public and because CallbackProxy is mistakenly
// party of the public classes, we cannot remove this method.
void CallbackProxy::OnTooManyRedirects(
    /* [in] */ IMessage* cancelMsg,
    /* [in] */ IMessage* continueMsg)
{
    // deprecated.
}

void CallbackProxy::OnReceivedError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        return;
    }
    if (DebugFlags::CALLBACK_PROXY) Logger::V(LOGTAG, "CallbackProxy::OnReceivedError");

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, REPORT_ERROR, (IMessage**)&msg);
    msg->SetArg1(errorCode);
    AutoPtr<IBundle> bundle;
    msg->GetData((IBundle**)&bundle);
    bundle->PutString(String("description"), description);
    bundle->PutString(String("failingUrl"), failingUrl);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

void CallbackProxy::OnFormResubmission(
    /* [in] */ IMessage* dontResend,
    /* [in] */ IMessage* resend)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        dontResend->SendToTarget();
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, RESEND_POST_DATA, (IMessage**)&msg);
    AutoPtr<IBundle> bundle;
    msg->GetData((IBundle**)&bundle);
    bundle->PutParcelable(String("resend"), IParcelable::Probe(resend));
    bundle->PutParcelable(String("dontResend"), IParcelable::Probe(dontResend));
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

/**
 * Called by the WebCore side
 */
Boolean CallbackProxy::ShouldOverrideUrlLoading(
    /* [in] */ const String& url)
{
    // We have a default behavior if no client exists so always send the
    // message.
    AutoPtr<IBoolean> value;
    CBoolean::New(FALSE, (IBoolean**)&value);
    AutoPtr<ResultTransport> res = new ResultTransport(value);
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, OVERRIDE_URL, (IMessage**)&msg);
    AutoPtr<IBundle> bundle;
    msg->GetData((IBundle**)&bundle);
    bundle->PutString(String("url"), url);
    msg->SetObj(res);
    SendMessageToUiThreadSync(msg);
    AutoPtr<IBoolean> result = IBoolean::Probe(res->GetResult());
    Boolean bValue;
    result->GetValue(&bValue);
    return bValue;
}

void CallbackProxy::OnReceivedHttpAuthRequest(
    /* [in] */ IHttpAuthHandler* handler,
    /* [in] */ const String& hostName,
    /* [in] */ const String& realmName)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        handler->Cancel();
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, AUTH_REQUEST, handler, (IMessage**)&msg);
    AutoPtr<IBundle> bundle;
    msg->GetData((IBundle**)&bundle);
    bundle->PutString(String("host"), hostName);
    bundle->PutString(String("realm"), realmName);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

void CallbackProxy::OnReceivedSslError(
    /* [in] */ ISslErrorHandler* handler,
    /* [in] */ ISslError* error)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        handler->Cancel();
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, SSL_ERROR, (IMessage**)&msg);
    AutoPtr<IObjectStringMap> map;
    CObjectStringMap::New((IObjectStringMap**)&map);
    map->Put(String("handler"), handler);
    map->Put(String("error"), error);
    msg->SetObj(map);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

void CallbackProxy::OnProceededAfterSslError(
    /* [in] */ ISslError* error)
{
    if (mWebViewClient == NULL || IWebViewClientClassicExt::Probe(mWebViewClient) == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, PROCEEDED_AFTER_SSL_ERROR, (IMessage**)&msg);
    msg->SetObj(error);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

void CallbackProxy::OnReceivedClientCertRequest(
    /* [in] */ ClientCertRequestHandler* handler,
    /* [in] */ const String& host_and_port)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL || IWebViewClientClassicExt::Probe(mWebViewClient) == NULL) {
        handler->Cancel();
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, CLIENT_CERT_REQUEST, (IMessage**)&msg);
    AutoPtr<IObjectStringMap> map;
    CObjectStringMap::New((IObjectStringMap**)&map);
    map->Put(String("handler"), handler);
    AutoPtr<ICharSequence> hostAndPort;
    CStringWrapper::New(host_and_port, (ICharSequence**)&hostAndPort);
    map->Put(String("host_and_port"), hostAndPort);
    msg->SetObj(map);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

void CallbackProxy::OnReceivedCertificate(
    /* [in] */ ISslCertificate* certificate)
{
    // here, certificate can be null (if the site is not secure)
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, RECEIVED_CERTIFICATE, certificate, (IMessage**)&msg);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

void CallbackProxy::DoUpdateVisitedHistory(
    /* [in] */ const String& url,
    /* [in] */ Boolean isReload)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<ICharSequence> urlCS;
    CStringWrapper::New(url, (ICharSequence**)&urlCS);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, UPDATE_VISITED, isReload ? 1 : 0, 0, urlCS, (IMessage**)&msg);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

AutoPtr<IWebResourceResponse> CallbackProxy::ShouldInterceptRequest(
    /* [in] */ const String& url)
{
if (mWebViewClient == NULL) {
        return NULL;
    }

    // Note: This method does _not_ send a message.
    AutoPtr<IWebResourceResponse> r;
    AutoPtr<IWebView> webView;
    mWebView->GetWebView((IWebView**)&webView);
    mWebViewClient->ShouldInterceptRequest(webView, url, (IWebResourceResponse**)&r);

    if (r == NULL) {
        AutoPtr<IMessageHelper> mh;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
        AutoPtr<ICharSequence> urlCS;
        CStringWrapper::New(url, (ICharSequence**)&urlCS);
        AutoPtr<IMessage> msg;
        mh->Obtain(this, LOAD_RESOURCE, urlCS, (IMessage**)&msg);
        Boolean result = FALSE;
        SendMessage(msg, &result);
    }

    return r;
}

void CallbackProxy::OnUnhandledKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, ASYNC_KEYEVENTS, event, (IMessage**)&msg);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

void CallbackProxy::OnScaleChanged(
    /* [in] */ Float oldScale,
    /* [in] */ Float newScale)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, SCALE_CHANGED, (IMessage**)&msg);
    AutoPtr<IBundle> bundle;
    msg->GetData((IBundle**)&bundle);
    bundle->PutFloat(String("old"), oldScale);
    bundle->PutFloat(String("new"), newScale);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

void CallbackProxy::OnReceivedLoginRequest(
    /* [in] */ const String& realm,
    /* [in] */ const String& account,
    /* [in] */ const String& args)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, AUTO_LOGIN, (IMessage**)&msg);
    AutoPtr<IBundle> bundle;
    msg->GetData((IBundle**)&bundle);
    bundle->PutString(String("realm"), realm);
    bundle->PutString(String("account"), account);
    bundle->PutString(String("args"), args);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

//--------------------------------------------------------------------------
// DownloadListener functions.
//--------------------------------------------------------------------------

/**
 * Starts a download if a download listener has been registered, otherwise
 * return false.
 */
Boolean CallbackProxy::OnDownloadStart(
    /* [in] */ const String& url,
    /* [in] */ const String& userAgent,
    /* [in] */ const String& contentDisposition,
    /* [in] */ const String& mimetype,
    /* [in] */ const String referer,
    /* [in] */ Int64 contentLength)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mDownloadListener == NULL) {
        // Cancel the download if there is no browser client.
        return FALSE;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, DOWNLOAD_FILE, (IMessage**)&msg);
    AutoPtr<IBundle> bundle;
    msg->GetData((IBundle**)&bundle);
    bundle->PutString(String("url"), url);
    bundle->PutString(String("userAgent"), userAgent);
    bundle->PutString(String("mimetype"), mimetype);
    bundle->PutString(String("referer"), referer);
    bundle->PutInt64(String("contentLength"), contentLength);
    bundle->PutString(String("contentDisposition"), contentDisposition);
    Boolean result = FALSE;
    SendMessage(msg, &result);

    return TRUE;
}

//--------------------------------------------------------------------------
// WebView specific functions that do not interact with a client. These
// functions just need to operate within the UI thread.
//--------------------------------------------------------------------------

Boolean CallbackProxy::OnSavePassword(
    /* [in] */ const String& schemePlusHost,
    /* [in] */ const String& username,
    /* [in] */ const String& password,
    /* [in] */ IMessage* resumeMsg)
{
    // resumeMsg should be null at this point because we want to create it
    // within the CallbackProxy.
//    if (DebugFlags.CALLBACK_PROXY) {
//        junit.framework.Assert.assertNull(resumeMsg);
//    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> _resumeMsg;
    mh->Obtain(this, NOTIFY, (IMessage**)&_resumeMsg);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, SAVE_PASSWORD, _resumeMsg, (IMessage**)&msg);
    AutoPtr<IBundle> bundle;
    msg->GetData((IBundle**)&bundle);
    bundle->PutString(String("host"), schemePlusHost);
    bundle->PutString(String("username"), username);
    bundle->PutString(String("password"), password);
    SendMessageToUiThreadSync(msg);
    // Doesn't matter here
    return FALSE;
}

void CallbackProxy::OnReceivedHttpAuthCredentials(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, AUTH_CREDENTIALS, (IMessage**)&msg);
    AutoPtr<IBundle> bundle;
    msg->GetData((IBundle**)&bundle);
    bundle->PutString(String("host"), host);
    bundle->PutString(String("realm"), realm);
    bundle->PutString(String("username"), username);
    bundle->PutString(String("password"), password);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

//--------------------------------------------------------------------------
// WebChromeClient methods
//--------------------------------------------------------------------------

void CallbackProxy::OnProgressChanged(
    /* [in] */ Int32 newProgress)
{
    AutoLock lock(this);
    // Synchronize so that mLatestProgress is up-to-date.
    // update mLatestProgress even mWebChromeClient is null as
    // WebView.getProgress() needs it
    if (mLatestProgress == newProgress) {
        return;
    }

    mLatestProgress = newProgress;
    if (mWebChromeClient == NULL) {
        return;
    }

    if (!mProgressUpdatePending) {
        Boolean result = FALSE;
        SendEmptyMessage(PROGRESS, &result);
        mProgressUpdatePending = TRUE;
    }
}

AutoPtr<BrowserFrame> CallbackProxy::CreateWindow(
    /* [in] */ Boolean dialog,
    /* [in] */ Boolean userGesture)
{
    assert(0);
#if 0
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == null) {
        return null;
    }

    WebView.WebViewTransport transport =
        mWebView.getWebView().new WebViewTransport();
    final Message msg = obtainMessage(NOTIFY);
    msg.obj = transport;
    sendMessageToUiThreadSync(obtainMessage(CREATE_WINDOW, dialog ? 1 : 0,
            userGesture ? 1 : 0, msg));
    WebViewClassic w = WebViewClassic.fromWebView(transport.getWebView());
    if (w != null) {
        WebViewCore core = w.getWebViewCore();
        // If WebView.destroy() has been called, core may be null.  Skip
        // initialization in that case and return null.
        if (core != null) {
            core.initializeSubwindow();
            return core.getBrowserFrame();
        }
    }
#endif

    return NULL;
}

void CallbackProxy::OnRequestFocus()
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return;
    }

    Boolean result = FALSE;
    SendEmptyMessage(REQUEST_FOCUS, &result);
}

void CallbackProxy::OnCloseWindow(
    /* [in] */ CWebViewClassic* window)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, CLOSE_WINDOW, IWebViewClassic::Probe(window), (IMessage**)&msg);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

void CallbackProxy::OnReceivedIcon(
    /* [in] */ IBitmap* icon)
{
    // The current item might be null if the icon was already stored in the
    // database and this is a new WebView.
    AutoPtr<IWebHistoryItem> item;
    AutoPtr<WebHistoryItemClassic> i;
    mBackForwardList->GetCurrentItem((IWebHistoryItem**)&item);
    assert(0);
//    i = (WebHistoryItemClassic*)(item->Probe(EIID_WebHistoryItemClassic));
    if (i != NULL) {
        i->SetFavicon(icon);
    }
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, RECEIVED_ICON, icon, (IMessage**)&msg);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

/* package */
void CallbackProxy::OnReceivedTouchIconUrl(
    /* [in] */ const String& url,
    /* [in] */ Boolean precomposed)
{
    // We should have a current item but we do not want to crash so check
    // for null.
    AutoPtr<IWebHistoryItem> item;
    AutoPtr<WebHistoryItemClassic> i;
    mBackForwardList->GetCurrentItem((IWebHistoryItem**)&item);
    assert(0);
//    i = (WebHistoryItemClassic*)(item->Probe(EIID_WebHistoryItemClassic));
    if (i != NULL) {
        i->SetTouchIconUrl(url, precomposed);
    }

    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<ICharSequence> urlCS;
    CStringWrapper::New(url, (ICharSequence**)&urlCS);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, RECEIVED_TOUCH_ICON_URL, precomposed ? 1 : 0, 0, urlCS, (IMessage**)&msg);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

void CallbackProxy::OnReceivedTitle(
    /* [in] */ const String& title)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<ICharSequence> titleCS;
    CStringWrapper::New(title, (ICharSequence**)&titleCS);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, RECEIVED_TITLE, titleCS, (IMessage**)&msg);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

void CallbackProxy::OnJsAlert(
    /* [in] */ const String& url,
    /* [in] */ const String& message)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return;
    }
    AutoPtr<JsResultReceiver> result = new JsResultReceiver(this);
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> alert;
    mh->Obtain(this, JS_ALERT, result, (IMessage**)&alert);
    AutoPtr<IBundle> bundle;
    alert->GetData((IBundle**)&bundle);
    bundle->PutString(String("message"), message);
    bundle->PutString(String("url"), url);
    SendMessageToUiThreadSync(alert);
}

Boolean CallbackProxy::OnJsConfirm(
    /* [in] */ const String& url,
    /* [in] */ const String& message)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return FALSE;
    }
    AutoPtr<JsResultReceiver> result = new JsResultReceiver(this);
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> confirm;
    mh->Obtain(this, JS_CONFIRM, result, (IMessage**)&confirm);
    AutoPtr<IBundle> bundle;
    confirm->GetData((IBundle**)&bundle);
    bundle->PutString(String("message"), message);
    bundle->PutString(String("url"), url);
    SendMessageToUiThreadSync(confirm);
    Boolean bValue = FALSE;
    result->mJsResult->GetResult(&bValue);
    return bValue;
}

String CallbackProxy::OnJsPrompt(
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [in] */ const String& defaultValue)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return String(NULL);
    }
    AutoPtr<JsResultReceiver> result = new JsResultReceiver(this);
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> prompt;
    mh->Obtain(this, JS_PROMPT, result, (IMessage**)&prompt);
    AutoPtr<IBundle> bundle;
    prompt->GetData((IBundle**)&bundle);
    bundle->PutString(String("message"), message);
    bundle->PutString(String("default"), defaultValue);
    bundle->PutString(String("url"), url);
    SendMessageToUiThreadSync(prompt);
    String stringResult;
    result->mJsResult->GetStringResult(&stringResult);
    return stringResult;
}

Boolean CallbackProxy::OnJsBeforeUnload(
    /* [in] */ const String& url,
    /* [in] */ const String& message)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return TRUE;
    }

    AutoPtr<JsResultReceiver> result = new JsResultReceiver(this);
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> confirm;
    mh->Obtain(this, JS_UNLOAD, result, (IMessage**)&confirm);
    AutoPtr<IBundle> bundle;
    confirm->GetData((IBundle**)&bundle);
    bundle->PutString(String("message"), message);
    bundle->PutString(String("url"), url);
    SendMessageToUiThreadSync(confirm);
    Boolean bResult = FALSE;
    result->mJsResult->GetResult(&bResult);
    return bResult;
}

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
void CallbackProxy::OnExceededDatabaseQuota(
    /* [in] */ const String& url,
    /* [in] */ const String& databaseIdentifier,
    /* [in] */ Int64 quota,
    /* [in] */ Int64 estimatedDatabaseSize,
    /* [in] */ Int64 totalQuota,
    /* [in] */ IWebStorageQuotaUpdater* quotaUpdater)
{
    if (mWebChromeClient == NULL) {
        // Native-side logic prevents the quota being updated to a smaller
        // value.
        quotaUpdater->UpdateQuota(quota);
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> exceededQuota;
    mh->Obtain(this, EXCEEDED_DATABASE_QUOTA, (IMessage**)&exceededQuota);
    AutoPtr<IObjectStringMap> map;
    CObjectStringMap::New((IObjectStringMap**)&map);
    AutoPtr<ICharSequence> databaseIdentifierCS;
    CStringWrapper::New(databaseIdentifier, (ICharSequence**)&databaseIdentifierCS);
    map->Put(String("databaseIdentifier"), databaseIdentifierCS);
    AutoPtr<ICharSequence> urlCS;
    CStringWrapper::New(url, (ICharSequence**)&urlCS);
    map->Put(String("url"), urlCS);
    AutoPtr<IInteger64> iQuota;
    CInteger64::New(quota, (IInteger64**)&iQuota);
    map->Put(String("quota"), iQuota);
    AutoPtr<IInteger64> iEstimatedDatabaseSize;
    CInteger64::New(estimatedDatabaseSize, (IInteger64**)&iEstimatedDatabaseSize);
    map->Put(String("estimatedDatabaseSize"), iEstimatedDatabaseSize);
    AutoPtr<IInteger64> iTotalQuota;
    CInteger64::New(totalQuota, (IInteger64**)&iTotalQuota);
    map->Put(String("totalQuota"), iTotalQuota);
    map->Put(String("quotaUpdater"), quotaUpdater);
    exceededQuota->SetObj(map);
    Boolean result = FALSE;
    SendMessage(exceededQuota, &result);
}

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
void CallbackProxy::OnReachedMaxAppCacheSize(
    /* [in] */ Int64 requiredStorage,
    /* [in] */ Int64 quota,
    /* [in] */ IWebStorageQuotaUpdater* quotaUpdater)
{
    if (mWebChromeClient == NULL) {
        // Native-side logic prevents the quota being updated to a smaller
        // value.
        quotaUpdater->UpdateQuota(quota);
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, REACHED_APPCACHE_MAXSIZE, (IMessage**)&msg);
    AutoPtr<IObjectStringMap> map;
    CObjectStringMap::New((IObjectStringMap**)&map);
    AutoPtr<IInteger64> iRequiredStorage;
    CInteger64::New(requiredStorage, (IInteger64**)&iRequiredStorage);
    map->Put(String("requiredStorage"), iRequiredStorage);
    AutoPtr<IInteger64> iQuota;
    CInteger64::New(quota, (IInteger64**)&iQuota);
    map->Put(String("quota"), iQuota);
    map->Put(String("quotaUpdater"), quotaUpdater);
    msg->SetObj(map);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

/**
 * Called by WebViewCore to instruct the browser to display a prompt to ask
 * the user to set the Geolocation permission state for the given origin.
 * @param origin The origin requesting Geolocation permsissions.
 * @param callback The callback to call once a permission state has been
 *     obtained.
 */
void CallbackProxy::OnGeolocationPermissionsShowPrompt(
    /* [in] */ const String& origin,
    /* [in] */ IGeolocationPermissionsCallback* callback)
{
    if (mWebChromeClient == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> showMessage;
    mh->Obtain(this, GEOLOCATION_PERMISSIONS_SHOW_PROMPT, (IMessage**)&showMessage);
    AutoPtr<IObjectStringMap> map;
    CObjectStringMap::New((IObjectStringMap**)&map);
    AutoPtr<ICharSequence> originCS;
    CStringWrapper::New(origin, (ICharSequence**)&originCS);
    map->Put(String("origin"), originCS);
    map->Put(String("callback"), callback);
    showMessage->SetObj(map);
    Boolean result = FALSE;
    SendMessage(showMessage, &result);
}

/**
 * Called by WebViewCore to instruct the browser to hide the Geolocation
 * permissions prompt.
 */
void CallbackProxy::OnGeolocationPermissionsHidePrompt()
{
    if (mWebChromeClient == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> hideMessage;
    mh->Obtain(this, GEOLOCATION_PERMISSIONS_HIDE_PROMPT, (IMessage**)&hideMessage);
    Boolean result = FALSE;
    SendMessage(hideMessage, &result);
}

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
void CallbackProxy::AddMessageToConsole(
    /* [in] */ const String& message,
    /* [in] */ Int32 lineNumber,
    /* [in] */ const String& sourceID,
    /* [in] */ Int32 msgLevel)
{
    if (mWebChromeClient == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, ADD_MESSAGE_TO_CONSOLE, (IMessage**)&msg);
    AutoPtr<IBundle> bundle;
    msg->GetData((IBundle**)&bundle);
    bundle->PutString(String("message"), message);
    bundle->PutString(String("sourceID"), sourceID);
    bundle->PutInt32(String("lineNumber"), lineNumber);
    bundle->PutInt32(String("msgLevel"), msgLevel);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

Boolean CallbackProxy::OnJsTimeout()
{
    //always interrupt timedout JS by default
    if (mWebChromeClient == NULL) {
        return TRUE;
    }
    AutoPtr<JsResultReceiver> result = new JsResultReceiver(this);
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> timeout;
    mh->Obtain(this, JS_TIMEOUT, result, (IMessage**)&timeout);
    SendMessageToUiThreadSync(timeout);
    Boolean bRet = FALSE;
    result->mJsResult->GetResult(&bRet);
    return bRet;
}

void CallbackProxy::GetVisitedHistory(
    /* [in] */ IValueCallback* callback)
{
    if (mWebChromeClient == NULL) {
        return;
    }
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, GET_VISITED_HISTORY, (IMessage**)&msg);
    msg->SetObj(callback);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

/**
 * Called by WebViewCore to open a file chooser.
 */
/* package */
AutoPtr<IUri> CallbackProxy::OpenFileChooser(
    /* [in] */ const String& acceptType,
    /* [in] */ const String& capture)
{
    if (mWebChromeClient == NULL) {
        return NULL;
    }
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> myMessage;
    mh->Obtain(this, OPEN_FILE_CHOOSER, (IMessage**)&myMessage);
    AutoPtr<UploadFile> uploadFile = new UploadFile(this);
    AutoPtr<UploadFileMessageData> data = new UploadFileMessageData(uploadFile, acceptType, capture);
    myMessage->SetObj(data);
    SendMessageToUiThreadSync(myMessage);
    return uploadFile->GetResult();
}

void CallbackProxy::OnNewHistoryItem(
    /* [in] */ IWebHistoryItem* item)
{
    if (mWebBackForwardListClient == NULL) {
        return;
    }
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, ADD_HISTORY_ITEM, item, (IMessage**)&msg);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

void CallbackProxy::OnIndexChanged(
    /* [in] */ IWebHistoryItem* item,
    /* [in] */ Int32 index)
{
    if (mWebBackForwardListClient == NULL) {
        return;
    }
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(this, HISTORY_INDEX_CHANGED, index, 0, item, (IMessage**)&msg);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

Boolean CallbackProxy::CanShowAlertDialog()
{
    // We can only display the alert dialog if mContext is
    // an Activity context.
    // FIXME: Should we display dialogs if mContext does
    // not have the window focus (e.g. if the user is viewing
    // another Activity when the alert should be displayed?
    // See bug 3166409
    return (IActivity::Probe(mContext) != NULL);
}

//synchronized
void CallbackProxy::BlockMessages()
{
    AutoLock lock(this);
    mBlockMessages = TRUE;
}

//synchronized
Boolean CallbackProxy::MessagesBlocked()
{
    AutoLock lock(this);
    return mBlockMessages;
}

void CallbackProxy::Shutdown()
{
//    RemoveCallbacksAndMessages(NULL);
    SetWebViewClient(NULL);
    SetWebChromeClient(NULL);
}

String CallbackProxy::GetJsDialogTitle(
    /* [in] */ const String& url)
{
    String title = url;
    if (URLUtil::IsDataUrl(url)) {
        // For data: urls, we just display 'JavaScript' similar to Safari.
        mContext->GetString(R::string::js_dialog_title_default, &title);
    }
    else {
        //try {
            AutoPtr<IURL> aUrl;
            assert(0);
//            CURL::New(url, (IURL**)&aUrl);
            // For example: "The page at 'http://www.mit.edu' says:"
//            title = mContext.getString(R.string.js_dialog_title,
//                    aUrl.getProtocol() + "://" + aUrl.getHost());
        //} catch (MalformedURLException ex) {
            // do nothing. just use the url as the title
        //}
    }

    return title;
}

//synchronized
void CallbackProxy::SendMessageToUiThreadSync(
    /* [in] */ IMessage* msg)
{
    AutoLock lock(this);
    Boolean result = FALSE;
    SendMessage(msg, &result);
    WebCoreThreadWatchdog::Pause();
    //try {
        Wait();
    //} catch (InterruptedException e) {
    //    Log.e(LOGTAG, "Caught exception waiting for synchronous UI message to be processed");
    //    Log.e(LOGTAG, Log.getStackTraceString(e));
    //}
    WebCoreThreadWatchdog::Resume();
}

ECode CallbackProxy::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "CallbackProxy";
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
