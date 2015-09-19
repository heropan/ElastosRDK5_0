
#include "net/CUriHelper.h"
//#include "net/WebAddress.h"
//#include "net/http/CErrorStrings.h"
#include "utility/CTypedValue.h"
#include "view/ViewRootImpl.h"
#include "webkit/CallbackProxy.h"
#include "webkit/CCacheManager.h"
#include "webkit/CCookieSyncManagerHelper.h"
#include "webkit/CWebResourceResponse.h"
#include "webkit/BrowserFrame.h"
#include "webkit/CPluginManagerHelper.h"
#include "webkit/URLUtil.h"
#include "webkit/DebugFlags.h"
#include "webkit/SslCertLookupTable.h"
#include "webkit/WebTextView.h"
#include "webkit/CWebViewCore.h"
#include "webkit/CWebViewClassic.h"
#include "webkit/WebViewDatabaseClassic.h"
#include "webkit/WebSettingsClassic.h"
#include "webkit/CPluginManagerHelper.h"
#include "R.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>
#include "webkit/MimeTypeMap.h"
#include "webkit/CMimeTypeMapHelper.h"
#include "webkit/L10nUtils.h"
#include "webkit/CertTool.h"

using Elastos::Core::CStringWrapper;
using Elastos::Core::StringUtils;
using Elastos::Net::IURLEncoder;
using Elastos::Utility::IMap;
using Elastos::Utility::ISet;
using Elastos::Utility::IIterator;
using Elastos::Utility::IMapEntry;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::App::IActivityManager;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::EIID_IComponentCallbacks;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::IWebAddress;
//using Elastos::Droid::Net::WebAddress;
//using Elastos::Droid::Net::Http::CErrorStrings;
//using Elastos::Droid::Net::Http::IErrorStrings;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::ISurface;
using Elastos::Droid::View::ViewRootImpl;
using Elastos::Droid::Webkit::IMimeTypeMap;
using Elastos::Droid::Webkit::IMimeTypeMapHelper;
using Elastos::Droid::Webkit::CMimeTypeMapHelper;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//                  BrowserFrame::JSObject
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(BrowserFrame::JSObject, IInterface);


//===============================================================
//                  BrowserFrame::ConfigCallback
//===============================================================
BrowserFrame::ConfigCallback::ConfigCallback(
    /* [in] */ IWindowManager* wm)
    : mWindowManager(wm)
{}

CAR_INTERFACE_IMPL_LIGHT(BrowserFrame::ConfigCallback, IComponentCallbacks);

void BrowserFrame::ConfigCallback::AddHandler(
    /* [in] */ IHandler* h)
{
    AutoLock lock(mLock);
    // No need to ever remove a Handler. If the BrowserFrame is
    // destroyed, it will be collected and the WeakReference set to
    // null. If it happens to still be around during a configuration
    // change, the message will be ignored.
    AutoPtr<IWeakReference> ref;
    IWeakReferenceSource::Probe(h)->GetWeakReference((IWeakReference**)&ref);
    mHandlers.PushBack(ref);
}

ECode BrowserFrame::ConfigCallback::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    if (mHandlers.IsEmpty()) {
        return NOERROR;
    }
    AutoPtr<IDisplay> display;
    mWindowManager->GetDefaultDisplay((IDisplay**)&display);
    Int32 orientation;
    display->GetOrientation(&orientation);
    switch (orientation) {
        case ISurface::ROTATION_90:
            orientation = 90;
            break;
        case ISurface::ROTATION_180:
            orientation = 180;
            break;
        case ISurface::ROTATION_270:
            orientation = -90;
            break;
        case ISurface::ROTATION_0:
            orientation = 0;
            break;
        default:
            break;
    }
    {
        AutoLock lock(mLock);

        // Create a list of handlers to remove. Go ahead and make it
        // the same size to avoid resizing.
        List< AutoPtr<IWeakReference> > handlersToRemove;
        List< AutoPtr<IWeakReference> >::Iterator it;
        for (it = mHandlers.Begin(); it != mHandlers.End(); ++it) {
            AutoPtr<IWeakReference> wh = *it;
            AutoPtr<IHandler> h;
            wh->Resolve(EIID_IHandler, (IInterface**)&h);
            if (h != NULL) {
                AutoPtr<IMessage> msg;
                h->ObtainMessage(ORIENTATION_CHANGED,
                    orientation, 0, (IMessage**)&msg);
                Boolean result;
                h->SendMessage(msg, &result);
            }
            else {
                handlersToRemove.PushBack(wh);
            }
        }
        // Now remove all the null references.
        for (it = handlersToRemove.Begin(); it != handlersToRemove.End(); ++it) {
            mHandlers.Remove(*it);
        }
    }
    return NOERROR;
}

ECode BrowserFrame::ConfigCallback::OnLowMemory()
{
    return NOERROR;
}


//===============================================================
//                  BrowserFrame::MyHttpAuthHandler
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(BrowserFrame::MyHttpAuthHandler, IHttpAuthHandler);

IHANDLER_METHODS_IMPL(BrowserFrame::MyHttpAuthHandler, HttpAuthHandler);

ECode BrowserFrame::MyHttpAuthHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    return NOERROR;
}

ECode BrowserFrame::MyHttpAuthHandler::UseHttpAuthUsernamePassword(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mUseCachedCredentials;
    return NOERROR;
}

ECode BrowserFrame::MyHttpAuthHandler::Proceed(
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    mOwner->NativeAuthenticationProceed(mHandle, username, password);
    return NOERROR;
}

ECode BrowserFrame::MyHttpAuthHandler::Cancel()
{
    mOwner->NativeAuthenticationCancel(mHandle);
    return NOERROR;
}

ECode BrowserFrame::MyHttpAuthHandler::SuppressDialog(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSuppressDialog;
    return NOERROR;
}


//===============================================================
//       BrowserFrame::MySslErrorHandler::ProceedRunnable
//===============================================================
BrowserFrame::MySslErrorHandler::ProceedRunnable::ProceedRunnable(
    /* [in] */ MySslErrorHandler* owner)
    : mOwner(owner)
{}

ECode BrowserFrame::MySslErrorHandler::ProceedRunnable::Run()
{
    mOwner->mOwner->NativeSslCertErrorProceed(mOwner->mHandle);
    return NOERROR;
}


//===============================================================
//       BrowserFrame::MySslErrorHandler::CancelRunnable
//===============================================================
BrowserFrame::MySslErrorHandler::CancelRunnable::CancelRunnable(
    /* [in] */ MySslErrorHandler* owner)
    : mOwner(owner)
{}

ECode BrowserFrame::MySslErrorHandler::CancelRunnable::Run()
{
    mOwner->mOwner->NativeSslCertErrorCancel(mOwner->mHandle, mOwner->mCertError);
    return NOERROR;
}


//===============================================================
//                  BrowserFrame::MySslErrorHandler
//===============================================================
CAR_INTERFACE_IMPL(BrowserFrame::MySslErrorHandler, ISslErrorHandler);

IHANDLER_METHODS_IMPL(BrowserFrame::MySslErrorHandler, SslErrorHandler);

ECode BrowserFrame::MySslErrorHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    return NOERROR;
}

ECode BrowserFrame::MySslErrorHandler::Proceed()
{
    SslCertLookupTable::GetInstance()->SetIsAllowed(mSslError);
    AutoPtr<IRunnable> runnable = new ProceedRunnable(this);
    Boolean result = FALSE;
    Post(runnable, &result);
    return NOERROR;
}

ECode BrowserFrame::MySslErrorHandler::Cancel()
{
    AutoPtr<IRunnable> runnable = new CancelRunnable(this);
    Boolean result = FALSE;
    Post(runnable, &result);
    return NOERROR;
}


//===============================================================
//                          BrowserFrame
//===============================================================
const String BrowserFrame::LOGTAG("BrowserFrame");
/**
 * Cap the number of LoadListeners that will be instantiated, so
 * we don't blow the GREF count.  Attempting to queue more than
 * this many requests will prompt an error() callback on the
 * request's LoadListener
 */
const Int32 BrowserFrame::MAX_OUTSTANDING_REQUESTS;

// message ids
// a message posted when a frame loading is completed
const Int32 BrowserFrame::FRAME_COMPLETED;
// orientation change message
const Int32 BrowserFrame::ORIENTATION_CHANGED;
// a message posted when the user decides the policy
const Int32 BrowserFrame::POLICY_FUNCTION;

// Note: need to keep these in sync with FrameLoaderTypes.h in native
const Int32 BrowserFrame::FRAME_LOADTYPE_STANDARD;
const Int32 BrowserFrame::FRAME_LOADTYPE_BACK;
const Int32 BrowserFrame::FRAME_LOADTYPE_FORWARD;
const Int32 BrowserFrame::FRAME_LOADTYPE_INDEXEDBACKFORWARD;
const Int32 BrowserFrame::FRAME_LOADTYPE_RELOAD;
const Int32 BrowserFrame::FRAME_LOADTYPE_RELOADALLOWINGSTALEDATA;
const Int32 BrowserFrame::FRAME_LOADTYPE_SAME;
const Int32 BrowserFrame::FRAME_LOADTYPE_REDIRECT;
const Int32 BrowserFrame::FRAME_LOADTYPE_REPLACE;

// A progress threshold to switch from history Picture to live Picture
const Int32 BrowserFrame::TRANSITION_SWITCH_THRESHOLD;

// Static instance of a WebCoreElastosBridge to handle timer and cookie
// requests from WebCore.
AutoPtr<WebCoreElastosBridge> BrowserFrame::sElastosBridge;

AutoPtr<BrowserFrame::ConfigCallback> BrowserFrame::sConfigCallback;
struct ElaWebFrameCallback BrowserFrame::sWebFrameCallback =
{
    NULL,
    NULL,
    NULL,
    &BrowserFrame::MaybeSavePassword,
    &BrowserFrame::ShouldInterceptRequest,
    &BrowserFrame::LoadStarted,
    &BrowserFrame::TransitionToCommitted,
    &BrowserFrame::LoadFinished,
    &BrowserFrame::ReportError,
    &BrowserFrame::SetTitle,
    &BrowserFrame::WindowObjectCleared,
    &BrowserFrame::SetProgress,
    &BrowserFrame::DidReceiveIcon,
    &BrowserFrame::DidReceiveTouchIconUrl,
    &BrowserFrame::UpdateVisitedHistory,
    &BrowserFrame::HandleUrl,
    &BrowserFrame::CreateWindow,
    &BrowserFrame::CloseWindow,
    &BrowserFrame::DecidePolicyForFormResubmission,
    &BrowserFrame::RequestFocus,
    &BrowserFrame::GetRawResFilename,
    &BrowserFrame::Density,
    &BrowserFrame::GetFileSize,
    &BrowserFrame::GetFile,
    &BrowserFrame::DidReceiveAuthenticationChallenge,
    &BrowserFrame::ReportSslCertError,
    &BrowserFrame::RequestClientCert,
    &BrowserFrame::DownloadStart,
    &BrowserFrame::DidReceiveData,
    &BrowserFrame::DidFinishLoading,
    &BrowserFrame::SetCertificate,
    &BrowserFrame::ShouldSaveFormData,
    &BrowserFrame::SaveFormData,
    &BrowserFrame::AutoLogin,
    &BrowserFrame::SetFrameField,
    &BrowserFrame::GetFrameField,
    &CWebViewCore::sElaWebViewCoreCallback
};

// XXX: Must match PolicyAction in FrameLoaderTypes.h in webcore
const Int32 BrowserFrame::POLICY_USE;
const Int32 BrowserFrame::POLICY_IGNORE;

// These ids need to be in sync with enum rawResId in PlatformBridge.h
const Int32 BrowserFrame::NODOMAIN;
const Int32 BrowserFrame::LOADERROR;
/* package */ const Int32 BrowserFrame::DRAWABLEDIR;
const Int32 BrowserFrame::FILE_UPLOAD_LABEL;
const Int32 BrowserFrame::RESET_LABEL;
const Int32 BrowserFrame::SUBMIT_LABEL;
const Int32 BrowserFrame::FILE_UPLOAD_NO_FILE_CHOSEN;

/**
 * Create a new BrowserFrame to be used in an application.
 * @param context An application context to use when retrieving assets.
 * @param w A WebViewCore used as the view for this frame.
 * @param proxy A CallbackProxy for posting messages to the UI thread and
 *              querying a client for information.
 * @param settings A WebSettings object that holds all settings.
 * XXX: Called by WebCore thread.
 */
BrowserFrame::BrowserFrame(
    /* [in] */ IContext* context,
    /* [in] */ CWebViewCore* w,
    /* [in] */ CallbackProxy* proxy,
    /* [in] */ WebSettingsClassic* settings,
    /* [in] */ IMap* javascriptInterfaces)
    : mLoadInitFromElastos(FALSE)
    , mNativeFrame(0)
    , mLoadType(0)
    , mFirstLayoutDone(TRUE)
    , mCommitted(TRUE)
    , mBlockMessages(FALSE)
    , mOrientation(-1)
    , mIsMainFrame(FALSE)
{
    AutoPtr<IContext> appContext;
    context->GetApplicationContext((IContext**)&appContext);

    // Create a global WebCoreElastosBridge to handle timers and
    // cookies in the WebCore thread.
    if (sElastosBridge == NULL) {
        sElastosBridge = new WebCoreElastosBridge();
        // set WebCore native cache size
        AutoPtr<IActivityManager> am;
        context->GetSystemService(IContext::ACTIVITY_SERVICE, (IInterface**)&am);
        Int32 size = 0;
        am->GetMemoryClass(&size);
        if (size > 16) {
            sElastosBridge->SetCacheSize(8 * 1024 * 1024);
        }
        else {
            sElastosBridge->SetCacheSize(4 * 1024 * 1024);
        }
        // initialize CacheManager
        CCacheManager::Init(appContext);
        // create CookieSyncManager with current Context
        AutoPtr<ICookieSyncManagerHelper> helper;
        CCookieSyncManagerHelper::AcquireSingleton((ICookieSyncManagerHelper**)&helper);
        AutoPtr<ICookieSyncManager> csm;
        helper->CreateInstance(appContext, (ICookieSyncManager**)&csm);
        // create PluginManager with current Context
        AutoPtr<IPluginManagerHelper> pmh;
        CPluginManagerHelper::AcquireSingleton((IPluginManagerHelper**)&pmh);
        AutoPtr<CPluginManager> pluginManager;
        pmh->GetInstance(appContext, (IPluginManager**)&pluginManager);
    }

    if (sConfigCallback == NULL) {
        AutoPtr<IWindowManager> wm;
        appContext->GetSystemService(
                IContext::WINDOW_SERVICE, (IInterface**)&wm);
        sConfigCallback = new ConfigCallback(wm);
        ViewRootImpl::AddConfigCallback(sConfigCallback);
    }
    sConfigCallback->AddHandler((IHandler*)this);

    AddJavaScriptObjects(javascriptInterfaces);

    mSettings = settings;
    mContext = context;
    mCallbackProxy = proxy;
    mDatabase = WebViewDatabaseClassic::GetInstance(appContext);
    mWebViewCore = w;

    AutoPtr<IAssetManager> am;
    context->GetAssets((IAssetManager**)&am);

    AddRef();
    NativeCreateFrame(w, am, (IWebBackForwardList*)proxy->GetBackForwardList().Get());

    if (DebugFlags::BROWSER_FRAME) {
        Logger::V(LOGTAG, "BrowserFrame constructor: this=%p", this);
    }
}

BrowserFrame::~BrowserFrame()
{}

PInterface BrowserFrame::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IHandler *)this;
    }
    else if (riid == EIID_IHandler) {
        return (IHandler *)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource *)this;
    }
    return NULL;
}

UInt32 BrowserFrame::AddRef()
{
    return HandlerBase::AddRef();
}

UInt32 BrowserFrame::Release()
{
    return HandlerBase::Release();
}

ECode BrowserFrame::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IHandler *)this) {
        *iid = EIID_IHandler;
    }
    else if (object == (IInterface*)(IWeakReferenceSource *)this) {
        *iid = EIID_IWeakReferenceSource;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Load a url from the network or the filesystem into the main frame.
 * Following the same behaviour as Safari, javascript: URLs are not passed
 * to the main frame, instead they are evaluated immediately.
 * @param url The url to load.
 * @param extraHeaders The extra headers sent with this url. This should not
 *            include the common headers like "user-agent". If it does, it
 *            will be replaced by the intrinsic value of the WebView.
 */
void BrowserFrame::LoadUrl(
    /* [in] */ const String& url,
    /* [in] */ IMap* extraHeaders)
{
    if (DebugFlags::BROWSER_FRAME)
        Logger::D(LOGTAG, "BrowserFrame::LoadUrl %s", url.string());
    mLoadInitFromElastos = TRUE;
    if (URLUtil::IsJavaScriptUrl(url)) {
        // strip off the scheme and evaluate the string
        StringByEvaluatingJavaScriptFromString(
                url.Substring(CString("javascript:").GetLength()));
    }
    else {
        NativeLoadUrl(url, extraHeaders);
    }
    mLoadInitFromElastos = FALSE;
}

/**
 * Load a url with "POST" method from the network into the main frame.
 * @param url The url to load.
 * @param data The data for POST request.
 */
void BrowserFrame::PostUrl(
    /* [in] */ const String& url,
    /* [in] */ ArrayOf<Byte>* data)
{
    mLoadInitFromElastos = TRUE;
    NativePostUrl(url, data);
    mLoadInitFromElastos = FALSE;
}

/**
 * Load the content as if it was loaded by the provided base URL. The
 * historyUrl is used as the history entry for the load data.
 *
 * @param baseUrl Base URL used to resolve relative paths in the content
 * @param data Content to render in the browser
 * @param mimeType Mimetype of the data being passed in
 * @param encoding Character set encoding of the provided data.
 * @param historyUrl URL to use as the history entry.
 */
void BrowserFrame::LoadData(
    /* [in] */ const String& _baseUrl,
    /* [in] */ const String& _data,
    /* [in] */ const String& _mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ const String& _historyUrl)
{
    String baseUrl = _baseUrl;
    String data = _data;
    String mimeType = _mimeType;
    String historyUrl = _historyUrl;

    mLoadInitFromElastos = TRUE;
    if (historyUrl.IsNullOrEmpty()) {
        historyUrl = "about:blank";
    }
    if (data.IsNull()) {
        data = "";
    }

    // Setup defaults for missing values. These defaults where taken from
    // WebKit's WebFrame.mm
    if (baseUrl.IsNullOrEmpty()) {
        baseUrl = "about:blank";
    }
    if (mimeType.IsNullOrEmpty()) {
        mimeType = "text/html";
    }
    NativeLoadData(baseUrl, data, mimeType, encoding, historyUrl);
    mLoadInitFromElastos = FALSE;
}

/**
 * Saves the contents of the frame as a web archive.
 *
 * @param basename The filename where the archive should be placed.
 * @param autoname If false, takes filename to be a file. If true, filename
 *                 is assumed to be a directory in which a filename will be
 *                 chosen according to the url of the current page.
 */
/* package */
String BrowserFrame::SaveWebArchive(
    /* [in] */ const String& basename,
    /* [in] */ Boolean autoname)
{
    return NativeSaveWebArchive(basename, autoname);
}

/**
 * Go back or forward the number of steps given.
 * @param steps A negative or positive number indicating the direction
 *              and number of steps to move.
 */
void BrowserFrame::GoBackOrForward(
    /* [in] */ Int32 steps)
{
    mLoadInitFromElastos = TRUE;
    NativeGoBackOrForward(steps);
    mLoadInitFromElastos = FALSE;
}

/**
 * native callback
 * Report an error to an activity.
 * @param errorCode The HTTP error code.
 * @param description Optional human-readable description. If no description
 *     is given, we'll use a standard localized error message.
 * @param failingUrl The URL that was being loaded when the error occurred.
 * TODO: Report all errors including resource errors but include some kind
 * of domain identifier. Change errorCode to an enum for a cleaner
 * interface.
 */
void BrowserFrame::ReportError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& _description,
    /* [in] */ const String& failingUrl)
{
    if (DebugFlags::BROWSER_FRAME)
        Logger::V(LOGTAG,"BrowserFrame::ReportError: errorCode:%d,_description:%s, failingUrl:%d",
                errorCode,_description.string(), failingUrl.string());
    String description = _description;
    // As this is called for the main resource and loading will be stopped
    // after, reset the state variables.
    ResetLoadingStates();
    if (description.IsNullOrEmpty()) {
        if (DebugFlags::BROWSER_FRAME)
            Logger::E(LOGTAG,"no implementation BrowserFrame::ReportError:depend on net/http which not ready,\
                    default:\"There was a network error.\", errorCode:%d", errorCode);
        description = String("There was a network error.");
        //assert(0);//TODO depend on net/http which is not ready
        //AutoPtr<IErrorStrings> es;
        //CErrorStrings::AcquireSingleton((IErrorStrings**)&es);
        //es->GetString(errorCode, mContext, &description);
    }
    mCallbackProxy->OnReceivedError(errorCode, description, failingUrl);
}

void BrowserFrame::ResetLoadingStates()
{
    mCommitted = TRUE;
    mFirstLayoutDone = TRUE;
}

/* package */
Boolean BrowserFrame::Committed()
{
    return mCommitted;
}

/* package */
Boolean BrowserFrame::FirstLayoutDone()
{
    return mFirstLayoutDone;
}

/* package */
Int32 BrowserFrame::LoadType()
{
    return mLoadType;
}

/* package */
void BrowserFrame::DidFirstLayout()
{
    if (!mFirstLayoutDone) {
        mFirstLayoutDone = TRUE;
        // ensure {@link WebViewCore#webkitDraw} is called as we were
        // blocking the update in {@link #loadStarted}
        mWebViewCore->ContentDraw();
    }
}

/**
 * native callback
 * Indicates the beginning of a new load.
 * This method will be called once for the main frame.
 */
void BrowserFrame::LoadStarted(
    /* [in] */ const String& url,
    /* [in] */ IBitmap* favicon,
    /* [in] */ Int32 loadType,
    /* [in] */ Boolean isMainFrame)
{
    mIsMainFrame = isMainFrame;
    if (DebugFlags::BROWSER_FRAME) {
        Logger::V(LOGTAG, "LoadStarted : url=%s, loadType, %d, isMainFrame:%d ", url.string(),loadType,isMainFrame);
    }

    if (isMainFrame || loadType == FRAME_LOADTYPE_STANDARD) {
        mLoadType = loadType;

        if (isMainFrame) {
            // Call onPageStarted for main frames.
            mCallbackProxy->OnPageStarted(url, favicon);
            // as didFirstLayout() is only called for the main frame, reset
            // mFirstLayoutDone only for the main frames
            mFirstLayoutDone = FALSE;
            mCommitted = FALSE;
            // remove pending draw to block update until mFirstLayoutDone is
            // set to true in didFirstLayout()
            mWebViewCore->ClearContent();
            mWebViewCore->RemoveMessages(CWebViewCore::EventHub::WEBKIT_DRAW);
        }
    }
}

void BrowserFrame::SaveFormData(
    /* [in] */ IMap* data)
{
    if (mSettings->GetSaveFormData()) {
        AutoPtr<IWebHistoryItem> h;
        mCallbackProxy->GetBackForwardList()->GetCurrentItem((IWebHistoryItem**)&h);
        if (h != NULL) {
            String _url;
            h->GetUrl(&_url);
            String url = WebTextView::UrlForAutoCompleteData(_url);
            if (!url.IsNull()) {
                mDatabase->SetFormData(url, data);
            }
        }
    }
}

Boolean BrowserFrame::ShouldSaveFormData()
{
    if (mSettings->GetSaveFormData()) {
        AutoPtr<IWebHistoryItem> h;
        mCallbackProxy->GetBackForwardList()
                ->GetCurrentItem((IWebHistoryItem**)&h);
        String url;
        return h != NULL && (h->GetUrl(&url), !url.IsNull());
    }
    return FALSE;
}

/**
 * native callback
 * Indicates the WebKit has committed to the new load
 */
void BrowserFrame::TransitionToCommitted(
    /* [in] */ Int32 loadType,
    /* [in] */ Boolean isMainFrame)
{
    // loadType is not used yet
    if (isMainFrame) {
        mCommitted = TRUE;
        mWebViewCore->GetWebViewClassic()->mViewManager->PostResetStateAll();
    }
}

/**
 * native callback
 * <p>
 * Indicates the end of a new load.
 * This method will be called once for the main frame.
 */
void BrowserFrame::LoadFinished(
    /* [in] */ const String& url,
    /* [in] */ Int32 loadType,
    /* [in] */ Boolean isMainFrame)
{
    // mIsMainFrame and isMainFrame are better be equal!!!

    if (isMainFrame || loadType == FRAME_LOADTYPE_STANDARD) {
        if (isMainFrame) {
            ResetLoadingStates();
            mCallbackProxy->SwitchOutDrawHistory();
            mCallbackProxy->OnPageFinished(url);
        }
    }
}

/**
 * Destroy all native components of the BrowserFrame.
 */
void BrowserFrame::Destroy()
{
    NativeDestroyFrame();
    mBlockMessages = TRUE;
    RemoveCallbacksAndMessages(NULL);
}

/**
 * Handle messages posted to us.
 * @param msg The message to handle.
 */
CARAPI BrowserFrame::HandleMessage(
    /* [in] */ IMessage* msg)
{
    if (mBlockMessages) {
        return NOERROR;
    }
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case FRAME_COMPLETED: {
            if (mSettings->GetSavePassword() && HasPasswordField()) {
                AutoPtr<IWebHistoryItem> item;
                mCallbackProxy->GetBackForwardList()
                        ->GetCurrentItem((IWebHistoryItem**)&item);
                if (item != NULL) {
                    String strUrl;
                    item->GetUrl(&strUrl);
                    assert(0);
//                    WebAddress uri(strUrl);
                    String schemePlusHost;// = uri.GetScheme() + uri.GetHost();
                    AutoPtr< ArrayOf<String> > up = mDatabase->GetUsernamePassword(
                            schemePlusHost);
                    if (up != NULL && !(*up)[0].IsNull()) {
                        SetUsernamePassword((*up)[0], (*up)[1]);
                    }
                }
            }
            break;
        }

        case POLICY_FUNCTION: {
            Int32 arg1, arg2;
            msg->GetArg1(&arg1);
            msg->GetArg2(&arg2);
            NativeCallPolicyFunction(arg1, arg2);
            break;
        }

        case ORIENTATION_CHANGED: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            if (mOrientation != arg1) {
                mOrientation = arg1;
                NativeOrientationChanged(arg1);
            }
            break;
        }

        default:
            break;
    }
    return NOERROR;
}

/**
 * Punch-through for WebCore to set the document
 * title. Inform the Activity of the new title.
 * @param title The new title of the document.
 */
void BrowserFrame::SetTitle(
    /* [in] */ const String& title)
{
    // FIXME: The activity must call getTitle (a native method) to get the
    // title. We should try and cache the title if we can also keep it in
    // sync with the document.
    mCallbackProxy->OnReceivedTitle(title);
}

/**
 * Retrieves the render tree of this frame and puts it as the object for
 * the message and sends the message.
 * @param callback the message to use to send the render tree
 */
void BrowserFrame::ExternalRepresentation(
    /* [in] */ IMessage* callback)
{
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(ExternalRepresentation(), (ICharSequence**)&cs);
    callback->SetObj(cs);
    callback->SendToTarget();
}

/**
 * Return the render tree as a string
 */
String BrowserFrame::ExternalRepresentation()
{
    return Elastos_WebFrame_ExternalRepresentation((Int32)this);
}

/**
 * Retrieves the visual text of the frames, puts it as the object for
 * the message and sends the message.
 * @param callback the message to use to send the visual text
 */
void BrowserFrame::DocumentAsText(
    /* [in] */ IMessage* callback)
{
    StringBuilder text;
    Int32 arg1, arg2;
    callback->GetArg1(&arg1);
    callback->GetArg2(&arg2);
    if (arg1 != 0) {
        // Dump top frame as text.
        text.AppendString(DocumentAsText());
    }
    if (arg2 != 0) {
        // Dump child frames as text.
        text.AppendString(ChildFramesAsText());
    }
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(text.ToString(), (ICharSequence**)&cs);
    callback->SetObj(cs);
    callback->SendToTarget();
}

/**
 * Return the text drawn on the screen as a string
 */
String BrowserFrame::DocumentAsText()
{
    return Elastos_WebFrame_DocumentAsText((Int32)this);
}

/**
 * Return the text drawn on the child frames as a string
 */
String BrowserFrame::ChildFramesAsText()
{
    return Elastos_WebFrame_ChildFramesAsText((Int32)this);
}

/*
 * This method is called by WebCore to inform the frame that
 * the Javascript window object has been cleared.
 * We should re-attach any attached js interfaces.
 */
void BrowserFrame::WindowObjectCleared(
    /* [in] */ Int32 nativeFramePointer)
{
    HashMap<String, AutoPtr<JSObject> >::Iterator it = mJavaScriptObjects.Begin();
    while (it != mJavaScriptObjects.End()) {
        String interfaceName = it->mFirst;
        AutoPtr<JSObject> jsobject = it->mSecond;
        if (jsobject != NULL && jsobject->mObject != NULL) {
            NativeAddJavascriptInterface(nativeFramePointer,
                    jsobject->mObject, interfaceName, jsobject->mRequireAnnotation);
        }
        ++it;
    }
    mRemovedJavaScriptObjects.Clear();
}

/*
 * Add javascript objects to the internal list of objects. The default behavior
 * is to allow access to inherited methods (no annotation needed). This is only
 * used when js objects are passed through a constructor (via a hidden constructor).
 *
 * @TODO change the default behavior to be compatible with the public addjavascriptinterface
 */
void BrowserFrame::AddJavaScriptObjects(
    /* [in] */ IMap* javascriptInterfaces)
{
    // TODO in a separate CL provide logic to enable annotations for API level JB_MR1 and above.
    if (javascriptInterfaces == NULL) return;
    AutoPtr<ISet> entrys;
    javascriptInterfaces->EntrySet((ISet**)&entrys);
    AutoPtr<IIterator> it;
    entrys->GetIterator((IIterator**)&it);
    Boolean hasNext;
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<IMapEntry> entry;
        it->Next((IInterface**)&entry);
        AutoPtr<ICharSequence> key;
        entry->GetKey((IInterface**)&key);
        String interfaceName;
        key->ToString(&interfaceName);
        AutoPtr<IInterface> object;
        entry->GetValue((IInterface**)&object);
        if (object != NULL) {
            mJavaScriptObjects[interfaceName] = new JSObject(object, FALSE);
        }
    }
}

/**
 * This method is called by WebCore to check whether application
 * wants to hijack url loading
 */
Boolean BrowserFrame::HandleUrl(
    /* [in] */ const String& url)
{
    if (mLoadInitFromElastos == TRUE) {
        return FALSE;
    }
    if (mCallbackProxy->ShouldOverrideUrlLoading(url)) {
        // if the url is hijacked, reset the state of the BrowserFrame
        DidFirstLayout();
        return TRUE;
    }
    else {
        return FALSE;
    }
}

void BrowserFrame::AddJavascriptInterface(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& interfaceName,
    /* [in] */ Boolean requireAnnotation)
{
    assert(obj != NULL);
    RemoveJavascriptInterface(interfaceName);
    AutoPtr<JSObject> jsobj = new JSObject(obj, requireAnnotation);
    mJavaScriptObjects[interfaceName] = jsobj;
}

void BrowserFrame::RemoveJavascriptInterface(
    /* [in] */ const String& interfaceName)
{
    // We keep a reference to the removed object because the native side holds only a weak
    // reference and we need to allow the object to continue to be used until the page has been
    // navigated.
    HashMap<String, AutoPtr<JSObject> >::Iterator it = mJavaScriptObjects.Find(interfaceName);
    if (it != mJavaScriptObjects.End()) {
        mRemovedJavaScriptObjects.Insert((IInterface*)it->mSecond.Get());
        mJavaScriptObjects.Erase(it);
    }
}

/**
 * Called by JNI.  Given a URI, find the associated file and return its size
 * @param uri A String representing the URI of the desired file.
 * @return Int32 The size of the given file.
 */
Int32 BrowserFrame::GetFileSize(
    /* [in] */ const String& uri)
{
    Int32 size = 0;
//    try {
    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    AutoPtr<IUri> uriObj;
    helper->Parse(uri, (IUri**)&uriObj);
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<IInputStream> stream;
    cr->OpenInputStream(uriObj, (IInputStream**)&stream);
    stream->Available(&size);
    stream->Close();
//    } catch (Exception e) {}
    return size;
}

/**
 * Called by JNI.  Given a URI, a buffer, and an offset into the buffer,
 * copy the resource into buffer.
 * @param uri A String representing the URI of the desired file.
 * @param buffer The byte array to copy the data into.
 * @param offset The offet into buffer to place the data.
 * @param expectedSize The size that the buffer has allocated for this file.
 * @return Int32 The size of the given file, or zero if it fails.
 */
Int32 BrowserFrame::GetFile(
    /* [in] */ const String& uri,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 expectedSize)
{
    Int32 size = 0;
//    try {
    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    AutoPtr<IUri> uriObj;
    helper->Parse(uri, (IUri**)&uriObj);
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<IInputStream> stream;
    cr->OpenInputStream(uriObj, (IInputStream**)&stream);
    stream->Available(&size);
    if (size <= expectedSize && buffer != NULL
            && buffer->GetLength() - offset >= size) {
        Int32 number;
        stream->ReadBytes(buffer, offset, size, &number);
    }
    else {
        size = 0;
    }
    stream->Close();
    //} catch (java.io.FileNotFoundException e) {
    //    Log.e(LOGTAG, "FileNotFoundException:" + e);
    //    size = 0;
    //} catch (java.io.IOException e2) {
    //    Log.e(LOGTAG, "IOException: " + e2);
    //    size = 0;
    //}
    return size;
}

/**
 * Get the InputStream for an Android resource
 * There are three different kinds of android resources:
 * - file:///android_res
 * - file:///android_asset
 * - content://
 * @param url The url to load.
 * @return An InputStream to the android resource
 */
AutoPtr<IInputStream> BrowserFrame::InputStreamForAndroidResource(
    /* [in] */ const String& _url)
{
    String url;
    String ANDROID_ASSET = IURLUtil::ASSET_BASE;
    String ANDROID_RESOURCE = IURLUtil::RESOURCE_BASE;
    String ANDROID_CONTENT = IURLUtil::CONTENT_BASE;

    if (_url.StartWith(ANDROID_RESOURCE)) {
        StringUtils::ReplaceFirst(_url, ANDROID_RESOURCE, String(""), &url);
        if (url.IsNullOrEmpty()) {
            Logger::E(LOGTAG, "url has length 0 %s", url.string());
            return NULL;
        }
        Int32 slash = url.IndexOf('/');
        Int32 dot = url.IndexOf('.', slash);
        if (slash == -1 || dot == -1) {
            Logger::E(LOGTAG, "Incorrect res path: %s", url.string());
            return NULL;
        }
        String subClassName = url.Substring(0, slash);
        String fieldName = url.Substring(slash + 1, dot);
        String errorMsg;
        //try {
            AutoPtr<IContext> context;
            mContext->GetApplicationContext((IContext**)&context);
            String pkgName;
            mContext->GetPackageName(&pkgName);
            StringBuilder builder;
            builder.AppendString(pkgName);
            builder.AppendCStr(".R$");
            builder.AppendString(subClassName);
            AutoPtr<IClassLoader> classLoader;
            context->GetClassLoader((IClassLoader**)&classLoader);
            AutoPtr<IClassInfo> klass;
            classLoader->LoadClass(builder.ToString(), (IClassInfo**)&klass);
            Logger::E(LOGTAG, "TODO:InputStreamForAndroidResource, the uri maybe null,url:%s", url.string());
            assert(0);
//            java.lang.reflect.Field field = d.getField(fieldName);
            Int32 id;// = field.getInt(null);
            AutoPtr<ITypedValue> value;
            CTypedValue::New((ITypedValue**)&value);
            AutoPtr<IResources> res;
            mContext->GetResources((IResources**)&res);
            res->GetValue(id, value, TRUE);
            Int32 type;
            value->GetType(&type);
            if (type == ITypedValue::TYPE_STRING) {
                Int32 assetCookie;
                value->GetAssetCookie(&assetCookie);
                AutoPtr<ICharSequence> cs;
                value->GetString((ICharSequence**)&cs);
                String str;
                cs->ToString(&str);
                AutoPtr<IAssetManager> assets;
                mContext->GetAssets((IAssetManager**)&assets);
                AutoPtr<IInputStream> inputStream;
                assets->OpenNonAsset(assetCookie, str,
                        IAssetManager::ACCESS_STREAMING, (IInputStream**)&inputStream);
                return inputStream;
            }
            else {
                // Old stack only supports TYPE_STRING for res files
                Logger::E(LOGTAG, "not of type string: %s", url.string());
                return NULL;
            }
        //} catch (Exception e) {
        //    Log.e(LOGTAG, "Exception: " + url);
        //    return null;
        //}
    }
    else if (_url.StartWith(ANDROID_ASSET)) {
        StringUtils::ReplaceFirst(_url, ANDROID_ASSET, String(""), &url);
        //try {
            AutoPtr<IAssetManager> assets;
            mContext->GetAssets((IAssetManager**)&assets);
            AutoPtr<IUriHelper> helper;
            CUriHelper::AcquireSingleton((IUriHelper**)&helper);
            AutoPtr<IUri> uri;
            helper->Parse(url, (IUri**)&uri);
            if (uri == NULL)
            {
                Logger::E(LOGTAG, "TODO::InputStreamForAndroidResource, the uri maybe null,url:%s", url.string());
                return NULL;
            }
            String path;
            uri->GetPath(&path);
            AutoPtr<IInputStream> inputStream;
            assets->Open(path, IAssetManager::ACCESS_STREAMING,
                    (IInputStream**)&inputStream);
            return inputStream;
        //} catch (IOException e) {
        //    return null;
        //}
    }
    else if (mSettings->GetAllowContentAccess() &&
               _url.StartWith(ANDROID_CONTENT)) {
        //try {
            // Strip off MIME type. If we don't do this, we can fail to
            // load Gmail attachments, because the URL being loaded doesn't
            // exactly match the URL we have permission to read.
            Int32 mimeIndex = _url.LastIndexOf('?');
            if (mimeIndex != -1) {
                url = _url.Substring(0, mimeIndex);
            }
            else
            {
                url = _url;
            }
            AutoPtr<IUriHelper> helper;
            CUriHelper::AcquireSingleton((IUriHelper**)&helper);
            AutoPtr<IUri> uri;
            helper->Parse(url, (IUri**)&uri);
            AutoPtr<IContentResolver> cr;
            mContext->GetContentResolver((IContentResolver**)&cr);
            AutoPtr<IInputStream> inputStream;
            cr->OpenInputStream(uri, (IInputStream**)&inputStream);
            return inputStream;
        //} catch (Exception e) {
        //    Log.e(LOGTAG, "Exception: " + url);
        //    return null;
        //}
    }
    else {
        return NULL;
    }
}

/**
 * If this looks like a POST request (form submission) containing a username
 * and password, give the user the option of saving them. Will either do
 * nothing, or block until the UI interaction is complete.
 *
 * Called directly by WebKit.
 *
 * @param postData The data about to be sent as the body of a POST request.
 * @param username The username entered by the user (sniffed from the DOM).
 * @param password The password entered by the user (sniffed from the DOM).
 */
void BrowserFrame::MaybeSavePassword(
    /* [in] */ ArrayOf<Byte>* postData,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    if (postData == NULL
            || username.IsNullOrEmpty()
            || password.IsNullOrEmpty()) {
        return; // No password to save.
    }

    if (!mSettings->GetSavePassword()) {
        return; // User doesn't want to save passwords.
    }

//    try {
        if (DebugFlags::BROWSER_FRAME) {
            AutoPtr<IWebHistoryItem> h;
            mCallbackProxy->GetBackForwardList()->GetCurrentItem((IWebHistoryItem**)&h);
            assert(h != NULL);
        }
        AutoPtr<IWebHistoryItem> h;
        mCallbackProxy->GetBackForwardList()->GetCurrentItem((IWebHistoryItem**)&h);
        String url;
        h->GetUrl(&url);
        AutoPtr<IWebAddress> uri;
        Logger::E(LOGTAG, "TODO: need implementation");
        assert(0);
//        CWebAddress::New(url, (IWebAddress**)&uri);
        String scheme, host;
//        uri->GetScheme(&scheme);
//        uri->GetHost(&host);
        String schemePlusHost = scheme + host;
        // Check to see if the username & password appear in
        // the post data (there could be another form on the
        // page and that was posted instead.
        String postString((const char*)postData->GetPayload());
        AutoPtr<IURLEncoder> encoder;
//        CURLEncoder::AcquireSingleton((IURLEncoder**)&encoder);
        String encodeUsername, encodePassword;
        encoder->Encode(username, &encodeUsername);
        encoder->Encode(password, &encodePassword);
        if (postString.Contains(encodeUsername) &&
                postString.Contains(encodePassword)) {
            AutoPtr< ArrayOf<String> > saved = mDatabase->GetUsernamePassword(
                    schemePlusHost);
            if (saved != NULL) {
                // null username implies that user has chosen not to
                // save password
                if (!(*saved)[0].IsNull()) {
                    // non-null username implies that user has
                    // chosen to save password, so update the
                    // recorded password
                    mDatabase->SetUsernamePassword(schemePlusHost, username, password);
                }
            }
            else {
                // CallbackProxy will handle creating the resume
                // message
                mCallbackProxy->OnSavePassword(schemePlusHost, username,
                        password, NULL);
            }
        }
    //} catch (ParseException ex) {
        // if it is bad uri, don't save its password
    //}
}

// Called by jni from the chrome network stack.
AutoPtr<IWebResourceResponse> BrowserFrame::ShouldInterceptRequest(
    /* [in] */ const String& url)
{
    AutoPtr<IInputStream> androidResource = InputStreamForAndroidResource(url);
    if (androidResource != NULL) {
        //if (DebugFlags::BROWSER_FRAME) {
        //    Logger::V(LOGTAG, "BrowserFrame::ShouldInterceptRequest, got android Resource:%s", url.string());
        //}
        AutoPtr<IWebResourceResponse> wr;
        CWebResourceResponse::New(String(NULL), String(NULL), androidResource,
                (IWebResourceResponse**)&wr);
        return wr;
    }

    // Note that we check this after looking for an android_asset or
    // android_res URL, as we allow those even if file access is disabled.
    if (!mSettings->GetAllowFileAccess() && url.StartWith("file://")) {
        AutoPtr<IWebResourceResponse> wr;
        CWebResourceResponse::New(String(NULL), String(NULL), NULL, (IWebResourceResponse**)&wr);
        return wr;
    }

    AutoPtr<IWebResourceResponse> response = mCallbackProxy->ShouldInterceptRequest(url);
    if (response == NULL && url.Equals("browser:incognito")) {
        //try {
            AutoPtr<IResources> res;
            mContext->GetResources((IResources**)&res);
            AutoPtr<IInputStream> ins;
            res->OpenRawResource(R::raw::incognito_mode_start_page,
                    (IInputStream**)&ins);
            CWebResourceResponse::New(String("text/html"), String("utf8"),
                          ins, (IWebResourceResponse**)&response);
        //} catch (NotFoundException ex) {
            // This shouldn't happen, but try and gracefully handle it jic
        //    Log.w(LOGTAG, "Failed opening raw.incognito_mode_start_page", ex);
        //}
    }
    return response;
}

/**
 * Set the progress for the browser activity.  Called by native code.
 * Uses a delay so it does not happen too often.
 * @param newProgress An Int32 between zero and one hundred representing
 *                    the current progress percentage of loading the page.
 */
void BrowserFrame::SetProgress(
    /* [in] */ Int32 newProgress)
{
    mCallbackProxy->OnProgressChanged(newProgress);
    if (newProgress == 100) {
        AutoPtr<IMessage> msg;
        ObtainMessage(FRAME_COMPLETED, (IMessage**)&msg);
        Boolean result;
        SendMessageDelayed(msg, 100, &result);
    }
    // FIXME: Need to figure out a better way to switch out of the history
    // drawing mode. Maybe we can somehow compare the history picture with
    // the current picture, and switch when it contains more content.
    if (mFirstLayoutDone && newProgress > TRANSITION_SWITCH_THRESHOLD) {
        mCallbackProxy->SwitchOutDrawHistory();
    }
}

/**
 * Send the icon to the activity for display.
 * @param icon A Bitmap representing a page's favicon.
 */
void BrowserFrame::DidReceiveIcon(
    /* [in] */ IBitmap* icon)
{
    mCallbackProxy->OnReceivedIcon(icon);
}

// Called by JNI when an apple-touch-icon attribute was found.
void BrowserFrame::DidReceiveTouchIconUrl(
    /* [in] */ const String& url,
    /* [in] */ Boolean precomposed)
{
    mCallbackProxy->OnReceivedTouchIconUrl(url, precomposed);
}

/**
 * Request a new window from the client.
 * @return The BrowserFrame object stored in the new WebView.
 */
AutoPtr<BrowserFrame> BrowserFrame::CreateWindow(
    /* [in] */ Boolean dialog,
    /* [in] */ Boolean userGesture)
{
    return mCallbackProxy->CreateWindow(dialog, userGesture);
}

/**
 * Try to focus this WebView.
 */
void BrowserFrame::RequestFocus()
{
    mCallbackProxy->OnRequestFocus();
}

/**
 * Close this frame and window.
 */
void BrowserFrame::CloseWindow(
    /* [in] */ CWebViewCore* w)
{
    mCallbackProxy->OnCloseWindow(w->GetWebViewClassic());
}

void BrowserFrame::DecidePolicyForFormResubmission(
    /* [in] */ Int32 policyFunction)
{
    AutoPtr<IMessage> dontResend;
    ObtainMessage(POLICY_FUNCTION, policyFunction,
        POLICY_IGNORE, (IMessage**)&dontResend);
    AutoPtr<IMessage> resend;
    ObtainMessage(POLICY_FUNCTION, policyFunction,
        POLICY_USE, (IMessage**)&resend);
    mCallbackProxy->OnFormResubmission(dontResend, resend);
}

/**
 * Tell the activity to update its global history.
 */
void BrowserFrame::UpdateVisitedHistory(
    /* [in] */ const String& url,
    /* [in] */ Boolean isReload)
{
    mCallbackProxy->DoUpdateVisitedHistory(url, isReload);
}

/**
 * Get the CallbackProxy for sending messages to the UI thread.
 */
/* package */
AutoPtr<CallbackProxy> BrowserFrame::GetCallbackProxy()
{
    return mCallbackProxy;
}

/**
 * Returns the User Agent used by this frame
 */
String BrowserFrame::GetUserAgentString()
{
    return mSettings->GetUserAgentString();
}

String BrowserFrame::GetRawResFilename(
    /* [in] */ Int32 id)
{
    return GetRawResFilename(id, mContext);
}

/* package */
String BrowserFrame::GetRawResFilename(
    /* [in] */ Int32 id,
    /* [in] */ IContext* context)
{
    Int32 resid;
    switch (id) {
        case NODOMAIN:
            resid = R::raw::nodomain;
            break;

        case LOADERROR:
            resid = R::raw::loaderror;
            break;

        case DRAWABLEDIR:
            // use one known resource to find the drawable directory
            resid = R::drawable::btn_check_off;
            break;

        case FILE_UPLOAD_LABEL: {
            AutoPtr<IResources> res;
            context->GetResources((IResources**)&res);
            String str;
            res->GetString(R::string::upload_file, &str);
            return str;
        }
        case RESET_LABEL: {
            AutoPtr<IResources> res;
            context->GetResources((IResources**)&res);
            String str;
            res->GetString(R::string::reset, &str);
            return str;
        }
        case SUBMIT_LABEL: {
            AutoPtr<IResources> res;
            context->GetResources((IResources**)&res);
            String str;
            res->GetString(R::string::submit, &str);
            return str;
        }
        case FILE_UPLOAD_NO_FILE_CHOSEN: {
            AutoPtr<IResources> res;
            context->GetResources((IResources**)&res);
            String str;
            res->GetString(R::string::no_file_chosen, &str);
            return str;
        }
        default:
            Logger::E(LOGTAG, "getRawResFilename got incompatible resource ID");
            return String("");
    }
    AutoPtr<ITypedValue> value;
    CTypedValue::New((ITypedValue**)&value);
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetValue(resid, value, TRUE);
    if (id == DRAWABLEDIR) {
        AutoPtr<ICharSequence> cs;
        value->GetString((ICharSequence**)&cs);
        String path;
        cs->ToString(&path);
        Int32 index = path.LastIndexOf('/');
        if (index < 0) {
            Logger::E(LOGTAG, "Can't find drawable directory.");
            return String("");
        }
        return path.Substring(0, index + 1);
    }
    AutoPtr<ICharSequence> cs;
    value->GetString((ICharSequence**)&cs);
    String str;
    cs->ToString(&str);
    return str;
}

Float BrowserFrame::Density()
{
    return CWebViewCore::GetFixedDisplayDensity(mContext);
}

/**
 * Called by JNI when the native HTTP stack gets an authentication request.
 *
 * We delegate the request to CallbackProxy, and route its response to
 * {@link #nativeAuthenticationProceed(int, String, String)} or
 * {@link #nativeAuthenticationCancel(int)}.
 *
 * We don't care what thread the callback is invoked on. All threading is
 * handled on the C++ side, because the WebKit thread may be blocked on a
 * synchronous call and unable to pump our MessageQueue.
 */
void BrowserFrame::DidReceiveAuthenticationChallenge(
    /* [in] */ const Int32 handle,
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [in] */ const Boolean useCachedCredentials,
    /* [in] */ const Boolean suppressDialog)
{
    AutoPtr<IHttpAuthHandler> handler = (IHttpAuthHandler*)(new MyHttpAuthHandler(
            handle, useCachedCredentials, suppressDialog, this));
    mCallbackProxy->OnReceivedHttpAuthRequest(handler, host, realm);
}

/**
 * Called by JNI when the Chromium HTTP stack gets an invalid certificate chain.
 *
 * We delegate the request to CallbackProxy, and route its response to
 * {@link #nativeSslCertErrorProceed(int)} or
 * {@link #nativeSslCertErrorCancel(int, int)}.
 */
void BrowserFrame::ReportSslCertError(
    /* [in] */ const Int32 handle,
    /* [in] */ const Int32 certError,
    /* [in] */ ArrayOf<Byte>* certDER,
    /* [in] */ const String& url)
{
    Logger::E(LOGTAG, "TODO no implementation BrowserFrame::ReportSslCertError");
    AutoPtr<ISslError> sslError;
    assert(0);
    //TODO
//    try {
//        X509Certificate cert = new X509CertImpl(certDER);
//        SslCertificate sslCert = new SslCertificate(cert);
//        sslError = SslError.SslErrorFromChromiumErrorCode(certError, sslCert, url);
//    } catch (IOException e) {
//        // Can't get the certificate, not much to do.
//        Log.e(LOGTAG, "Can't get the certificate from WebKit, canceling");
//        nativeSslCertErrorCancel(handle, certError);
//        return;
//    }

    if (SslCertLookupTable::GetInstance()->IsAllowed(sslError)) {
        NativeSslCertErrorProceed(handle);
        mCallbackProxy->OnProceededAfterSslError(sslError);
        return;
    }

    AutoPtr<ISslErrorHandler> handler = (ISslErrorHandler*)new MySslErrorHandler(
            handle, certError, sslError, this);
    mCallbackProxy->OnReceivedSslError(handler, sslError);
}

/**
 * Called by JNI when the native HTTPS stack gets a client
 * certificate request.
 *
 * We delegate the request to CallbackProxy, and route its response to
 * {@link #nativeSslClientCert(int, X509Certificate)}.
 */
void BrowserFrame::RequestClientCert(
    /* [in] */ Int32 handle,
    /* [in] */ const String& hostAndPort)
{
    if (DebugFlags::BROWSER_FRAME) {
        Logger::V(LOGTAG, "TODO no implementation BrowserFrame::RequestClientCert");
    }
    assert(0);
    // TODO
//    AutoPtr<SslClientCertLookupTable> table = SslClientCertLookupTable::GetInstance();
//    if (table.IsAllowed(hostAndPort)) {
//        // previously allowed
//        PrivateKey pkey = table.PrivateKey(hostAndPort);
//        if (pkey instanceof OpenSSLRSAPrivateKey) {
//            nativeSslClientCert(handle,
//                                ((OpenSSLRSAPrivateKey)pkey).getPkeyContext(),
//                                table.CertificateChain(hostAndPort));
//        }
//        else if (pkey instanceof OpenSSLDSAPrivateKey) {
//            nativeSslClientCert(handle,
//                                ((OpenSSLDSAPrivateKey)pkey).getPkeyContext(),
//                                table.CertificateChain(hostAndPort));
//        }
//        else {
//            nativeSslClientCert(handle,
//                                pkey.getEncoded(),
//                                table.CertificateChain(hostAndPort));
//        }
//    }
//    else if (table.IsDenied(hostAndPort)) {
//        // previously denied
//        nativeSslClientCert(handle, 0, null);
//    }
//    else {
//        // previously ignored or new
//        mCallbackProxy.onReceivedClientCertRequest(
//                new ClientCertRequestHandler(this, handle, hostAndPort, table), hostAndPort);
//    }
}

/**
 * Called by JNI when the native HTTP stack needs to download a file.
 *
 * We delegate the request to CallbackProxy, which owns the current app's
 * DownloadListener.
 */
void BrowserFrame::DownloadStart(
    /* [in] */ const String& url,
    /* [in] */ const String& userAgent,
    /* [in] */ const String& contentDisposition,
    /* [in] */ const String& _mimeType,
    /* [in] */ const String& referer,
    /* [in] */ Int64 contentLength)
{
    if (DebugFlags::BROWSER_FRAME) {
        Logger::V(LOGTAG, "::ownloadStart,url:%s, mimeType:%s,contentDis:%s",
                url.string(), _mimeType.string(),contentDisposition.string());
    }
    String mimeType = _mimeType;
    // This will only work if the url ends with the filename
    if (mimeType.IsNullOrEmpty()) {
        //try {
            String extension = url.Substring(url.LastIndexOf('.') + 1);
            AutoPtr<Elastos::Net::IMimeUtils> mimeUtils;
            Elastos::Net::CMimeUtils::AcquireSingleton((Elastos::Net::IMimeUtils**)&mimeUtils);
            mimeUtils->GuessMimeTypeFromExtension(extension, &mimeType);
            // MimeUtils might return null, not sure if downloadmanager is happy with that
            if (mimeType.IsNull())
                mimeType = "";
        //} catch(IndexOutOfBoundsException exception) {
            // mimeType string end with a '.', not much to do
        //}
    }
//    mimeType = MimeTypeMap.getSingleton().remapGenericMimeType(
//            mimeType, url, contentDisposition);
    AutoPtr<IMimeTypeMapHelper> mimeTypeHelper;
    CMimeTypeMapHelper::AcquireSingleton((IMimeTypeMapHelper**)&mimeTypeHelper);
    AutoPtr<IMimeTypeMap> pMtm;
    ECode ec = mimeTypeHelper->GetSingleton((IMimeTypeMap**)&pMtm);
    if (SUCCEEDED(ec))
    {
       MimeTypeMap* mimeTypeMap = (MimeTypeMap*)pMtm.Get();
       mimeTypeMap->RemapGenericMimeType(mimeType, url, contentDisposition);
    }

    String certType = CertTool::GetCertType(mimeType);
    if (!certType.IsNullOrEmpty()) {
        mKeyStoreHandler = new KeyStoreHandler(mimeType);
    }
    else {
        mCallbackProxy->OnDownloadStart(url, userAgent,
            contentDisposition, mimeType, referer, contentLength);
    }
}

/**
 * Called by JNI for Chrome HTTP stack when the Java side needs to access the data.
 */
void BrowserFrame::DidReceiveData(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 size)
{
    if (mKeyStoreHandler != NULL) mKeyStoreHandler->DidReceiveData(data, size);
}

void BrowserFrame::DidFinishLoading()
{
    if (mKeyStoreHandler != NULL) {
        mKeyStoreHandler->InstallCert(mContext);
        mKeyStoreHandler = NULL;
    }
}

/**
 * Called by JNI when we recieve a certificate for the page's main resource.
 * Used by the Chromium HTTP stack only.
 */
void BrowserFrame::SetCertificate(
    /* [in] */ ArrayOf<Byte>* cert_der)
{
    Logger::V(LOGTAG, "TODO no implementation BrowserFrame::SetCertificate");
    assert(0);
    // TODO
//    try {
//        X509Certificate cert = new X509CertImpl(cert_der);
//        mCallbackProxy.onReceivedCertificate(new SslCertificate(cert));
//    } catch (IOException e) {
//        // Can't get the certificate, not much to do.
//        Log.e(LOGTAG, "Can't get the certificate from WebKit, canceling");
//        return;
//    }
}

/**
 * Called by JNI when processing the X-Auto-Login header.
 */
void BrowserFrame::AutoLogin(
    /* [in] */ const String& realm,
    /* [in] */ const String& account,
    /* [in] */ const String& args)
{
    mCallbackProxy->OnReceivedLoginRequest(realm, account, args);
}

/**
 * Create a new native frame for a given WebView
 * @param w     A WebView that the frame draws into.
 * @param am    AssetManager to use to get assets.
 * @param list  The native side will add and remove items from this list as
 *              the native list changes.
 */
void BrowserFrame::NativeCreateFrame(
    /* [in] */ CWebViewCore* javaview,
    /* [in] */ IAssetManager* jAssetManager,
    /* [in] */ IWebBackForwardList* historyList)
{
    AutoPtr<IWeakReference> jObj;
    IWeakReferenceSource::Probe((IWeakReferenceSource *)this)->GetWeakReference((IWeakReference**)&jObj);
    AutoPtr<IWeakReference> jView;
    IWeakReferenceSource::Probe((IWeakReferenceSource *)javaview)->GetWeakReference((IWeakReference**)&jView);
    AutoPtr<IWeakReference> jHL;
    IWeakReferenceSource::Probe((IWeakReferenceSource *)historyList)->GetWeakReference((IWeakReference**)&jHL);

    if (DebugFlags::BROWSER_FRAME) {
        Logger::V(LOGTAG, "BrowserFrame constructor: this=%p, callback, %p ", this, &BrowserFrame::sWebFrameCallback);
        Logger::V(LOGTAG, "startLoadingResource:%p", BrowserFrame::sWebFrameCallback.startLoadingResource);
        Logger::V(LOGTAG, "shouldInterceptRequest:%p", BrowserFrame::sWebFrameCallback.shouldInterceptRequest);
        Logger::V(LOGTAG, "loadStarted:%p", BrowserFrame::sWebFrameCallback.loadStarted);
        Logger::V(LOGTAG, "transitionToCommitted:%p", BrowserFrame::sWebFrameCallback.transitionToCommitted);
        Logger::V(LOGTAG, "loadFinished:%p", BrowserFrame::sWebFrameCallback.loadFinished);
        Logger::V(LOGTAG, "reportError:%p", BrowserFrame::sWebFrameCallback.reportError);
        Logger::V(LOGTAG, "setTitle:%p", BrowserFrame::sWebFrameCallback.setTitle);
        Logger::V(LOGTAG, "windowObjectCleared:%p", BrowserFrame::sWebFrameCallback.windowObjectCleared);
        Logger::V(LOGTAG, "setProgress:%p", BrowserFrame::sWebFrameCallback.setProgress);
        Logger::V(LOGTAG, "didReceiveIcon:%p", BrowserFrame::sWebFrameCallback.didReceiveIcon);
        Logger::V(LOGTAG, "didReceiveTouchIconUrl:%p", BrowserFrame::sWebFrameCallback.didReceiveTouchIconUrl);
        Logger::V(LOGTAG, "updateVisitedHistory:%p", BrowserFrame::sWebFrameCallback.updateVisitedHistory);
        Logger::V(LOGTAG, "handleUrl:%p", BrowserFrame::sWebFrameCallback.handleUrl);
        Logger::V(LOGTAG, "createWindow:%p", BrowserFrame::sWebFrameCallback.createWindow);
        Logger::V(LOGTAG, "closeWindow:%p", BrowserFrame::sWebFrameCallback.closeWindow);
        Logger::V(LOGTAG, "decidePolicyForFormResubmission:%p", BrowserFrame::sWebFrameCallback.decidePolicyForFormResubmission);
        Logger::V(LOGTAG, "requestFocus:%p", BrowserFrame::sWebFrameCallback.requestFocus);
        Logger::V(LOGTAG, "getRawResFilename:%p", BrowserFrame::sWebFrameCallback.getRawResFilename);
        Logger::V(LOGTAG, "density:%p", BrowserFrame::sWebFrameCallback.density);
        Logger::V(LOGTAG, "getFileSize:%p", BrowserFrame::sWebFrameCallback.getFileSize);
        Logger::V(LOGTAG, "getFile:%p", BrowserFrame::sWebFrameCallback.getFile);
        Logger::V(LOGTAG, "didReceiveAuthenticationChallenge:%p", BrowserFrame::sWebFrameCallback.didReceiveAuthenticationChallenge);
        Logger::V(LOGTAG, "reportSslCertError:%p", BrowserFrame::sWebFrameCallback.reportSslCertError);
        Logger::V(LOGTAG, "requestClientCert:%p", BrowserFrame::sWebFrameCallback.requestClientCert);
        Logger::V(LOGTAG, "downloadStart:%p", BrowserFrame::sWebFrameCallback.downloadStart);
        Logger::V(LOGTAG, "didReceiveData:%p", BrowserFrame::sWebFrameCallback.didReceiveData);
        Logger::V(LOGTAG, "didFinishLoading:%p", BrowserFrame::sWebFrameCallback.didFinishLoading);
        Logger::V(LOGTAG, "setCertificate:%p", BrowserFrame::sWebFrameCallback.setCertificate);
        Logger::V(LOGTAG, "shouldSaveFormData:%p", BrowserFrame::sWebFrameCallback.shouldSaveFormData);
        Logger::V(LOGTAG, "saveFormData:%p", BrowserFrame::sWebFrameCallback.saveFormData);
        Logger::V(LOGTAG, "autoLogin:%p", BrowserFrame::sWebFrameCallback.autoLogin);
        Logger::V(LOGTAG, "setFrameField:%p", BrowserFrame::sWebFrameCallback.setFrameField);
        Logger::V(LOGTAG, "getFrameField:%p", BrowserFrame::sWebFrameCallback.getFrameField);
    }
    Elastos_WebFrame_CreateFrame(
        (Int32)(&BrowserFrame::sWebFrameCallback),
        jObj,
        jView,
        (Int32)(jAssetManager),
        jHL
        );
}

/**
 * Destroy the native frame.
 */
void BrowserFrame::NativeDestroyFrame()
{
    Elastos_WebFrame_DestroyFrame((Int32)this);
    mNativeFrame = 0;
}

void BrowserFrame::NativeCallPolicyFunction(
    /* [in] */ Int32 func,
    /* [in] */ Int32 decision)
{
    Elastos_WebFrame_CallPolicyFunction((Int32)this, func, decision);
}

/**
 * Reload the current main frame.
 */
void BrowserFrame::Reload(
    /* [in] */ Boolean allowStale)
{
    Elastos_WebFrame_Reload((Int32)this, allowStale);
}

/**
 * Go back or forward the number of steps given.
 * @param steps A negative or positive number indicating the direction
 *              and number of steps to move.
 */
void BrowserFrame::NativeGoBackOrForward(
    /* [in] */ Int32 pos)
{
    Elastos_WebFrame_GoBackOrForward((Int32)this, pos);
}

/**
 * stringByEvaluatingJavaScriptFromString will execute the
 * JS passed in in the context of this browser frame.
 * @param script A javascript string to execute
 *
 * @return string result of execution or null
 */
String BrowserFrame::StringByEvaluatingJavaScriptFromString(
    /* [in] */ const String& script)
{
    return Elastos_WebFrame_StringByEvaluatingJavaScriptFromString((Int32)this, script);
}

/**
 * Add a javascript interface to the main frame.
 */
void BrowserFrame::NativeAddJavascriptInterface(
    /* [in] */ Int32 nativeFramePointer,
    /* [in] */ IInterface* javascriptObj,
    /* [in] */ const String& interfaceName,
    /* [in] */ Boolean requireAnnotation)
{
    Elastos_WebFrame_AddJavascriptInterface((Int32)this, nativeFramePointer, javascriptObj, interfaceName, requireAnnotation);
}

void BrowserFrame::ClearCache()
{
    Elastos_WebFrame_ClearCache((Int32)this);
}
/**
 * Returns false if the url is bad.
 */
void BrowserFrame::NativeLoadUrl(
    /* [in] */ const String& url,
    /* [in] */ IMap* headers)
{
    if (DebugFlags::BROWSER_FRAME)
        Logger::D(LOGTAG, "enter BrowserFrame::NativeLoadUrl:%s", url.string());
    Elastos_WebFrame_LoadUrl((Int32)this, url, headers);
}

void BrowserFrame::NativePostUrl(
    /* [in] */ const String& url,
    /* [in] */ ArrayOf<Byte>* postData)
{
    Elastos_WebFrame_PostUrl((Int32)this, url, postData);
}

void BrowserFrame::NativeLoadData(
    /* [in] */ const String& baseUrl,
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ const String& failUrl)
{
    Elastos_WebFrame_LoadData((Int32)this, baseUrl, data, mimeType, encoding, failUrl);
}

/**
 * Stop loading the current page.
 */
void BrowserFrame::StopLoading()
{
    if (mIsMainFrame) {
        ResetLoadingStates();
    }
    NativeStopLoading();
}

void BrowserFrame::NativeStopLoading()
{
    Elastos_WebFrame_StopLoading((Int32)this);
}

/**
 * Return true if the document has images.
 */
Boolean BrowserFrame::DocumentHasImages()
{
   return Elastos_WebFrame_DocumentHasImages((Int32)this);
}

/**
 * @return TRUE if there is a password field in the current frame
 */
Boolean BrowserFrame::HasPasswordField()
{
    return Elastos_WebFrame_HasPasswordField((Int32)this);
}

/**
 * Get username and password in the current frame. If found, String[0] is
 * username and String[1] is password. Otherwise return NULL.
 * @return String[]
 */
AutoPtr< ArrayOf<String> > BrowserFrame::GetUsernamePassword()
{
    return Elastos_WebFrame_GetUsernamePassword((Int32)this);
}

/**
 * Set username and password to the proper fields in the current frame
 * @param username
 * @param password
 */
void BrowserFrame::SetUsernamePassword(
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    Elastos_WebFrame_SetUsernamePassword((Int32)this, username, password);
}

String BrowserFrame::NativeSaveWebArchive(
    /* [in] */ const String& basename,
    /* [in] */ Boolean autoname)
{
    return Elastos_WebFrame_SaveWebArchive((Int32)this, basename, autoname);
}

void BrowserFrame::NativeOrientationChanged(
    /* [in] */ Int32 orientation)
{
    Elastos_WebFrame_OrientationChanged((Int32)this, orientation);
}

void BrowserFrame::NativeAuthenticationProceed(
    /* [in] */ Int32 handle,
    /* [in] */ const String& jUsername,
    /* [in] */ const String& jPassword)
{
    Elastos_WebFrame_AuthenticationProceed((Int32)this, handle, jUsername, jPassword);
}

void BrowserFrame::NativeAuthenticationCancel(
    /* [in] */ Int32 handle)
{
    Elastos_WebFrame_AuthenticationCancel((Int32)this, handle);
}

void BrowserFrame::NativeSslCertErrorProceed(
    /* [in] */ Int32 handle)
{
    Elastos_WebFrame_SslCertErrorProceed((Int32)this, handle);
}

void BrowserFrame::NativeSslCertErrorCancel(
    /* [in] */ Int32 handle,
    /* [in] */ Int32 certError)
{
    Elastos_WebFrame_SslCertErrorCancel((Int32)this, handle, certError);
}

void BrowserFrame::NativeSslClientCert(
    /* [in] */ Int32 handle,
    /* [in] */ Int32 ctx,
    /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* chain)
{
    Elastos_WebFrame_SslClientCertCtx((Int32)this, handle, ctx, chain);
}

void BrowserFrame::NativeSslClientCert(
    /* [in] */ Int32 handle,
    /* [in] */ ArrayOf<Byte>* pkey,
    /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* chain)
{
    Elastos_WebFrame_SslClientCertPKCS8((Int32)this, handle, pkey, chain);
}

/**
 * Returns true when the contents of the frame is an RTL or vertical-rl
 * page. This is used for determining whether a frame should be initially
 * scrolled right-most as opposed to left-most.
 * @return true when the frame should be initially scrolled right-most
 * based on the text direction and writing mode.
 */
/* package */
Boolean BrowserFrame::GetShouldStartScrolledRight()
{
    return NativeGetShouldStartScrolledRight(mNativeFrame);
}

Boolean BrowserFrame::NativeGetShouldStartScrolledRight(
    /* [in] */ Int32 browserFrame)
{
    return Elastos_WebFrame_GetShouldStartScrolledRight((Int32)this, browserFrame);
}

ECode BrowserFrame::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}
// start the callback functions definition
void BrowserFrame::MaybeSavePassword(
    /* [in] */ IInterface* obj,
    /* [in] */ ArrayOf<Byte>* postData,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->MaybeSavePassword(postData, username, password);
}

AutoPtr<IInterface> BrowserFrame::ShouldInterceptRequest(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [out] */ Int32** callback)
{
    *callback = (Int32*)(&CWebResourceResponse::sWebResourceResponseCallback);
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    return browserFrame->ShouldInterceptRequest(url);
}

void BrowserFrame::LoadStarted(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [in] */ IInterface* favicon,
    /* [in] */ Int32 loadType,
    /* [in] */ Boolean isMainFrame)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    IBitmap* bitmap = IBitmap::Probe(favicon);
    browserFrame->LoadStarted(url, bitmap, loadType, isMainFrame);
}

void BrowserFrame::TransitionToCommitted(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 loadType,
    /* [in] */ Boolean isMainFrame)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->TransitionToCommitted(loadType, isMainFrame);
}

void BrowserFrame::LoadFinished(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [in] */ Int32 loadType,
    /* [in] */ Boolean isMainFrame)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->LoadFinished(url, loadType, isMainFrame);
}

void BrowserFrame::ReportError(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->ReportError(errorCode, description, failingUrl);
}

void BrowserFrame::SetTitle(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& title)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->SetTitle(title);
}

void BrowserFrame::WindowObjectCleared(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 nativeFramePointer)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->WindowObjectCleared(nativeFramePointer);
}

void BrowserFrame::SetProgress(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 newProgress)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->SetProgress(newProgress);
}

void BrowserFrame::DidReceiveIcon(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* icon)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    IBitmap* bitmap = IBitmap::Probe(icon);
    browserFrame->DidReceiveIcon(bitmap);
}

void BrowserFrame::DidReceiveTouchIconUrl(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [in] */ Boolean precomposed)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->DidReceiveTouchIconUrl(url, precomposed);
}

void BrowserFrame::UpdateVisitedHistory(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [in] */ Boolean isReload)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->UpdateVisitedHistory(url, isReload);
}

Boolean BrowserFrame::HandleUrl(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    return browserFrame->HandleUrl(url);
}

Int32 BrowserFrame::CreateWindow(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean dialog,
    /* [in] */ Boolean userGesture)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    return (Int32)(browserFrame->CreateWindow(dialog, userGesture).Get());
}

void BrowserFrame::CloseWindow(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* w)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    //CWebViewCore* webViewCore = w->Probe(EIID_IWebViewCore);
    //Is this OK?
    CWebViewCore* webViewCore = (CWebViewCore*)IWebViewCore::Probe(w);
    browserFrame->CloseWindow(webViewCore);
}

void BrowserFrame::DecidePolicyForFormResubmission(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 policyFunction)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->DecidePolicyForFormResubmission(policyFunction);
}

void BrowserFrame::RequestFocus(
    /* [in] */ IInterface* obj)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->RequestFocus();
}

String BrowserFrame::GetRawResFilename(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 id)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    return browserFrame->GetRawResFilename(id);
}

Float BrowserFrame::Density(
    /* [in] */ IInterface* obj)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    return browserFrame->Density();
}

Int32 BrowserFrame::GetFileSize(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& uri)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    return browserFrame->GetFileSize(uri);
}

Int32 BrowserFrame::GetFile(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& uri,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 expectedSize)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    return browserFrame->GetFile(uri, buffer, offset, expectedSize);
}

void BrowserFrame::DidReceiveAuthenticationChallenge(
    /* [in] */ IInterface* obj,
    /* [in] */ const Int32 handle,
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [in] */ const Boolean useCachedCredentials,
    /* [in] */ const Boolean suppressDialog)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->DidReceiveAuthenticationChallenge(handle, host, realm, useCachedCredentials, suppressDialog);
}

void BrowserFrame::ReportSslCertError(
    /* [in] */ IInterface* obj,
    /* [in] */ const Int32 handle,
    /* [in] */ const Int32 certError,
    /* [in] */ ArrayOf<Byte>* certDER,
    /* [in] */ const String& url)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->ReportSslCertError(handle, certError, certDER, url);
}

void BrowserFrame::RequestClientCert(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 handle,
    /* [in] */ const String& hostAndPort)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->RequestClientCert(handle, hostAndPort);
}

void BrowserFrame::DownloadStart(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [in] */ const String& userAgent,
    /* [in] */ const String& contentDisposition,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& referer,
    /* [in] */ Int64 contentLength)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->DownloadStart(url, userAgent, contentDisposition, mimeType, referer, contentLength);
}

void BrowserFrame::DidReceiveData(
    /* [in] */ IInterface* obj,
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 size)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->DidReceiveData(data, size);
}

void BrowserFrame::DidFinishLoading(
    /* [in] */ IInterface* obj)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->DidFinishLoading();
}

void BrowserFrame::SetCertificate(
    /* [in] */ IInterface* obj,
    /* [in] */ ArrayOf<Byte>* cert_der)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->SetCertificate(cert_der);
}

Boolean BrowserFrame::ShouldSaveFormData(
    /* [in] */ IInterface* obj)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    return browserFrame->ShouldSaveFormData();
}

void BrowserFrame::SaveFormData(
    /* [in] */ IInterface* obj,
    /* [in] */ IMap* data)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->SaveFormData(data);
}

void BrowserFrame::AutoLogin(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& realm,
    /* [in] */ const String& account,
    /* [in] */ const String& args)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->AutoLogin(realm, account, args);
}

void BrowserFrame::SetFrameField(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 frame)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(IHandler *)obj;
    browserFrame->mNativeFrame = frame;
}

Int32 BrowserFrame::GetFrameField(
    /* [in] */ Int32 obj)
{
    BrowserFrame* browserFrame = (BrowserFrame*)(obj);
    return browserFrame->mNativeFrame;
}

// end the callback functions definition
} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
