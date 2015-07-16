
#include "net/Uri.h"
#include "os/CMessageHelper.h"
#include "webkit/CertTool.h"
#include "webkit/CookieManager.h"
#include "webkit/DebugFlags.h"
#include "webkit/PluginManager.h"
#include "webkit/CPluginManager.h"
#include "webkit/CPluginManagerHelper.h"
#include "webkit/WebCoreElastosBridge.h"
#include "webkit/CWebViewClassic.h"
#include "webkit/CPluginManagerHelper.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::IBoolean;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Core::Mutex;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Os::IMessage;

namespace Elastos {
namespace Droid {
namespace Webkit {

const Int32 WebCoreElastosBridge::TIMER_MESSAGE;
const Int32 WebCoreElastosBridge::FUNCPTR_MESSAGE;
const String WebCoreElastosBridge::LOGTAG("WebCoreElastosBridge");
AutoPtr<IWeakReference> WebCoreElastosBridge::sCurrentMainWebView;
const Int32 WebCoreElastosBridge::REFRESH_PLUGINS;
Mutex WebCoreElastosBridge::sLock;

/**
 * Construct a new WebCoreElastosBridge to interface with
 * WebCore timers and cookies.
 */
WebCoreElastosBridge::WebCoreElastosBridge()
    :mNativeBridge(0),
     mHasInstantTimer(FALSE),
     mTimerPaused(FALSE),
     mHasDeferredTimers(FALSE)
{
    NativeConstructor();
}

WebCoreElastosBridge::~WebCoreElastosBridge()
{
    NativeFinalize();
}

PInterface WebCoreElastosBridge::Probe(
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

UInt32 WebCoreElastosBridge::AddRef()
{
    return HandlerBase::AddRef();
}

UInt32 WebCoreElastosBridge::Release()
{
    return HandlerBase::Release();
}

ECode WebCoreElastosBridge::GetInterfaceID(
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

ECode WebCoreElastosBridge::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

void WebCoreElastosBridge::SetActiveWebView(
    /* [in] */ CWebViewClassic* webview)
{
    Mutex::Autolock lock(sLock);

    AutoPtr<IInterface> obj;
    if (sCurrentMainWebView != NULL) {
        sCurrentMainWebView->Resolve(EIID_IWebViewClassic, (IInterface**)&obj);
    }
    if (obj != NULL) {
        // it is possible if there is a sub-WebView. Do nothing.
        return;
    }
    sCurrentMainWebView = NULL;
    webview->GetWeakReference((IWeakReference**)&sCurrentMainWebView);
}

//synchronized
void WebCoreElastosBridge::RemoveActiveWebView(
    /* [in] */ CWebViewClassic* webview)
{
    Mutex::Autolock lock(sLock);

    AutoPtr<IInterface> obj;
    if (sCurrentMainWebView != NULL) {
        sCurrentMainWebView->Resolve(EIID_IWebViewClassic, (IInterface**)&obj);
    }
    if (obj != (IWebViewClassic*)webview) {
        // it is possible if there is a sub-WebView. Do nothing.
        return;
    }
    sCurrentMainWebView = NULL;
}

ECode WebCoreElastosBridge::HandleMessage(
    /* [in] */ IMessage* msg)
{

    Int32 what;
    msg->GetWhat(&what);
    //if (DebugFlags::J_WEB_CORE_JAVA_BRIDGE) Logger::V(LOGTAG, "message %d, mTimerPaused:%d", what, mTimerPaused);
    switch (what) {
        case TIMER_MESSAGE: {
            if (mTimerPaused) {
                mHasDeferredTimers = TRUE;
            }
            else {
                FireSharedTimer();
            }
            break;
        }
        case FUNCPTR_MESSAGE:
            NativeServiceFuncPtrQueue();
            break;
        case REFRESH_PLUGINS: {
            AutoPtr< ArrayOf<String> > directories;
            AutoPtr<IPluginManagerHelper> pmh;
            CPluginManagerHelper::AcquireSingleton((IPluginManagerHelper**)&pmh);
            AutoPtr<CPluginManager> pluginManager;
            pmh->GetInstance(NULL, (IPluginManager**)&pluginManager);
            PluginManager *pm = (PluginManager*)(pluginManager.Get());
            directories = pm->GetPluginDirectories();
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            Boolean value;
            IBoolean::Probe(obj)->GetValue(&value);
            NativeUpdatePluginDirectories(directories, value);
            break;
        }
    }
    return NOERROR;
}

/**
 * Pause all timers.
 */
void WebCoreElastosBridge::Pause()
{
    if (!mTimerPaused) {
        mTimerPaused = TRUE;
        mHasDeferredTimers = FALSE;
    }
}

/**
 * Resume all timers.
 */
void WebCoreElastosBridge::Resume()
{
    if (mTimerPaused) {
       mTimerPaused = FALSE;
       if (mHasDeferredTimers) {
           mHasDeferredTimers = FALSE;
           FireSharedTimer();
       }
    }
}

/**
 * Set WebCore cache size.
 * @param bytes The cache size in bytes.
 */
void WebCoreElastosBridge::SetCacheSize(
    /* [in] */ Int32 bytes)
{
    Elastos_ElastosBridge_SetCacheSize(bytes);
}

void WebCoreElastosBridge::StoreFilePathForContentUri(
    /* [in] */ const String& path,
    /* [in] */ const String& contentUri)
{
    if (mContentUriToFilePathMap == NULL) {
        mContentUriToFilePathMap = new Elastos::Utility::HashMap<String, String>();
    }
    (*mContentUriToFilePathMap)[contentUri] = path;
}

void WebCoreElastosBridge::UpdateProxy(
    /* [in] */ IProxyProperties* proxyProperties)
{
    if (proxyProperties == NULL) {
        NativeUpdateProxy(String(""), String(""));
        return;
    }

    String host;
    proxyProperties->GetHost(&host);
    Int32 port;
    proxyProperties->GetPort(&port);
    if (port != 0) {
        host += String(":") + StringUtils::Int32ToString(port);
    }

    String exclusionList;
    proxyProperties->GetExclusionList(&exclusionList);
    NativeUpdateProxy(host, exclusionList);
}

void WebCoreElastosBridge::SetNetworkOnLine(
    /* [in] */ Boolean online)
{
    Elastos_ElastosBridge_SetNetworkOnLine(online);
}

void WebCoreElastosBridge::SetNetworkType(
    /* [in] */ const String& _type,
    /* [in] */ const String& _subtype)
{
    Elastos_ElastosBridge_SetNetworkType(_type, _subtype);
}

void WebCoreElastosBridge::AddPackageNames(
    /* [in] */ Elastos::Utility::HashSet<String>* packageNames)
{
    Elastos_ElastosBridge_AddPackageNames(packageNames);
}

void WebCoreElastosBridge::AddPackageName(
    /* [in] */ const String& packageName)
{
    Elastos_ElastosBridge_AddPackageName(packageName);
}

void WebCoreElastosBridge::RemovePackageName(
    /* [in] */ const String& packageName)
{
    Elastos_ElastosBridge_RemovePackageName(packageName);
}

void WebCoreElastosBridge::NativeUpdateProxy(
    /* [in] */ const String& newProxy,
    /* [in] */ const String& newExList)
{
    Elastos_ElastosBridge_UpdateProxy(newProxy, newExList);
}

/**
 * Call native timer callbacks.
 */
void WebCoreElastosBridge::FireSharedTimer()
{
    // clear the flag so that sharedTimerFired() can set a new timer
    mHasInstantTimer = FALSE;
    SharedTimerFired();
}

void WebCoreElastosBridge::SignalServiceFuncPtrQueue()
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(this, FUNCPTR_MESSAGE, (IMessage**)&msg);
    Boolean result;
    SendMessage(msg, &result);
}

void WebCoreElastosBridge::NativeServiceFuncPtrQueue()
{
    Elastos_ElastosBridge_ServiceFuncPtrQueue();
}

/**
 * Store a cookie string associated with a url.
 * @param url The url to be used as a key for the cookie.
 * @param value The cookie string to be stored.
 */
void WebCoreElastosBridge::SetCookies(
    /* [in] */ const String& url,
    /* [in] */ const String& _value)
{
    String value = _value;
    if (value.Contains("\r") || value.Contains("\n")) {
        // for security reason, filter out '\r' and '\n' from the cookie
        Int32 size = value.GetLength();
        StringBuilder buffer(size);
        Int32 i = 0;
        while (i != -1 && i < size) {
            Int32 ir = value.IndexOf('\r', i);
            Int32 in = value.IndexOf('\n', i);
            Int32 newi = (ir == -1) ? in : (in == -1 ? ir : (ir < in ? ir
                    : in));
            if (newi > i) {
                buffer.AppendString(value.Substring(i, newi));
            }
            else if (newi == -1) {
                buffer.AppendString(value.Substring(i, size));
                break;
            }
            i = newi + 1;
        }
        value = buffer.ToString();
    }
    CookieManager::GetInstance()->SetCookie(url, value);
}

/**
 * Retrieve the cookie string for the given url.
 * @param url The resource's url.
 * @return A String representing the cookies for the given resource url.
 */
String WebCoreElastosBridge::Cookies(
    /* [in] */ const String& url)
{
    String strRet;
    CookieManager::GetInstance()->GetCookie(url, &strRet);
    return strRet;
}

/**
 * Returns whether cookies are enabled or not.
 */
Boolean WebCoreElastosBridge::CookiesEnabled()
{
    Boolean bRet = FALSE;
    CookieManager::GetInstance()->AcceptCookie(&bRet);
    return bRet;
}

/**
 * Returns an array of plugin directoies
 */
AutoPtr< ArrayOf<String> > WebCoreElastosBridge::GetPluginDirectories()
{
    AutoPtr<IPluginManagerHelper> pmh;
    CPluginManagerHelper::AcquireSingleton((IPluginManagerHelper**)&pmh);
    AutoPtr<CPluginManager> pluginManager;
    pmh->GetInstance(NULL, (IPluginManager**)&pluginManager);
    PluginManager *pm = (PluginManager*)(pluginManager.Get());
    return pm->GetPluginDirectories();
}

/**
 * Returns the path of the plugin data directory
 */
String WebCoreElastosBridge::GetPluginSharedDataDirectory()
{
    AutoPtr<IPluginManagerHelper> pmh;
    CPluginManagerHelper::AcquireSingleton((IPluginManagerHelper**)&pmh);
    AutoPtr<CPluginManager> pluginManager;
    pmh->GetInstance(NULL, (IPluginManager**)&pluginManager);
    PluginManager *pm = (PluginManager*)(pluginManager.Get());
    return pm->GetPluginSharedDataDirectory();
}

/**
 * setSharedTimer
 * @param timemillis The relative time when the timer should fire
 */
void WebCoreElastosBridge::SetSharedTimer(
    /* [in] */ Int64 timemillis)
{
    //if (DebugFlags::J_WEB_CORE_JAVA_BRIDGE) Logger::W(LOGTAG, "setSharedTimer %lld", timemillis);

    if (timemillis <= 0) {
        // we don't accumulate the sharedTimer unless it is a delayed
        // request. This way we won't flood the message queue with
        // WebKit messages. This should improve the browser's
        // responsiveness to key events.
        if (mHasInstantTimer) {
            return;
        }
        else {
            mHasInstantTimer = TRUE;
            AutoPtr<IMessageHelper> helper;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
            AutoPtr<IMessage> msg;
            helper->Obtain(this, TIMER_MESSAGE, (IMessage**)&msg);
            Boolean result;
            SendMessageDelayed(msg, timemillis, &result);
        }
    }
    else {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(this, TIMER_MESSAGE, (IMessage**)&msg);
        Boolean result;
        SendMessageDelayed(msg, timemillis, &result);
        //if (DebugFlags::J_WEB_CORE_JAVA_BRIDGE) Logger::W(LOGTAG, "SendMessageDelayed, TIMER_MESSAGE, time:%lld", timemillis);
    }
}

/**
 * Stop the shared timer.
 */
void WebCoreElastosBridge::StopSharedTimer()
{
    //if (DebugFlags::J_WEB_CORE_JAVA_BRIDGE) { Logger::V(LOGTAG, "stopSharedTimer removing all timers"); }
    RemoveMessages(TIMER_MESSAGE);
    mHasInstantTimer = FALSE;
    mHasDeferredTimers = FALSE;
}

AutoPtr< ArrayOf<String> > WebCoreElastosBridge::GetKeyStrengthList()
{
    return CertTool::GetKeyStrengthList();
}

String WebCoreElastosBridge::GetSignedPublicKey(
    /* [in] */ Int32 index,
    /* [in] */ const String& challenge,
    /* [in] */ const String& url)
{
    Mutex::Autolock lock(mLock);

    AutoPtr<IWebViewClassic> obj;
    if (sCurrentMainWebView != NULL) {
        sCurrentMainWebView->Resolve(EIID_IWebViewClassic, (IInterface**)&obj);
    }
    AutoPtr<CWebViewClassic> current = (CWebViewClassic*)obj.Get();
    if (current != NULL) {
        // generateKeyPair expects organizations which we don't have. Ignore
        // url.
        AutoPtr<IContext> context = current->GetContext();
        return CertTool::GetSignedPublicKey(
                context, index, challenge);
    }
    else {
        Logger::E(LOGTAG, "There is no active WebView for getSignedPublicKey");
        return String("");
    }
}

// Called on the WebCore thread through JNI.
String WebCoreElastosBridge::ResolveFilePathForContentUri(
    /* [in] */ const String& uri)
{
    if (mContentUriToFilePathMap != NULL) {
        String fileName;
        Elastos::Utility::HashMap<String, String>::Iterator it =
                mContentUriToFilePathMap->Find(uri);
        if (it != mContentUriToFilePathMap->End()) {
            fileName = it->mSecond;
        }
        if (!fileName.IsNull()) {
            return fileName;
        }
    }

    // Failsafe fallback to just use the last path segment.
    // (See OpenableColumns documentation in the SDK)
    AutoPtr<IUri> jUri;
    Uri::Parse(uri, (IUri**)&jUri);
    String str;
    jUri->GetLastPathSegment(&str);
    return str;
}

void WebCoreElastosBridge::NativeConstructor()
{
    //prepare the callback functions
    struct ElaElastosBridgeCallback elaElastosBridgeCallback = {
        0,
        &WebCoreElastosBridge::_SetSharedTimer,
        &WebCoreElastosBridge::_StopSharedTimer,
        &WebCoreElastosBridge::_SetCookies,
        &WebCoreElastosBridge::_Cookies,
        &WebCoreElastosBridge::_CookiesEnabled,
        &WebCoreElastosBridge::_GetPluginDirectories,
        &WebCoreElastosBridge::_GetPluginSharedDataDirectory,
        &WebCoreElastosBridge::_SignalServiceFuncPtrQueue,
        &WebCoreElastosBridge::_GetKeyStrengthList,
        &WebCoreElastosBridge::_GetSignedPublicKey,
        &WebCoreElastosBridge::_ResolveFilePathForContentUri
    };

    IWeakReferenceSource* source = IWeakReferenceSource::Probe((IWeakReferenceSource *)this);
    AutoPtr<IWeakReference> wr;
    source->GetWeakReference((IWeakReference**)&wr);
    elaElastosBridgeCallback.obj = wr;
    if (DebugFlags::J_WEB_CORE_JAVA_BRIDGE)
        Logger::V(LOGTAG, "this:%p, callback:%p, obj %p, _SetSharedTimer:%p",
                this,  &elaElastosBridgeCallback, wr.Get(), &WebCoreElastosBridge::_SetSharedTimer);
    mNativeBridge = Elastos_ElastosBridge_Constructor((Int32)&elaElastosBridgeCallback);
}

void WebCoreElastosBridge::NativeFinalize()
{
    Logger::V(LOG_TAG, "webcore_javabridge::nativeFinalize(%p)\n", mNativeBridge);
    Elastos_ElastosBridge_Finalize(mNativeBridge);
    mNativeBridge = 0;
}

void WebCoreElastosBridge::SharedTimerFired()
{
    Elastos_ElastosBridge_SharedTimerFired();
}

void WebCoreElastosBridge::NativeUpdatePluginDirectories(
    /* [in] */ ArrayOf<String>* array,
    /* [in] */ Boolean reload)
{
    Elastos_ElastosBridge_UpdatePluginDirectories(array, reload);
}

void WebCoreElastosBridge::_SetSharedTimer(
    /* [in] */ IInterface* obj,
    /* [in] */ Int64 timemillis)
{
    //if (DebugFlags::J_WEB_CORE_JAVA_BRIDGE) Logger::V(LOGTAG, "obj %p, timemillis:%lld", obj, timemillis);
    WebCoreElastosBridge* webCoreElastosBridge = (WebCoreElastosBridge*)(IHandler*)(obj);
    webCoreElastosBridge->SetSharedTimer(timemillis);
}

void WebCoreElastosBridge::_StopSharedTimer(
    /* [in] */ IInterface* obj)
{
    WebCoreElastosBridge* webCoreElastosBridge = (WebCoreElastosBridge*)(IHandler*)(obj);
    webCoreElastosBridge->StopSharedTimer();
}

void WebCoreElastosBridge::_SetCookies(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [in] */ const String& value)
{
    WebCoreElastosBridge* webCoreElastosBridge = (WebCoreElastosBridge*)(IHandler*)(obj);
    webCoreElastosBridge->SetCookies(url, value);
}

String WebCoreElastosBridge::_Cookies(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url)
{
    WebCoreElastosBridge* webCoreElastosBridge = (WebCoreElastosBridge*)(IHandler*)(obj);
    return webCoreElastosBridge->Cookies(url);
}

Boolean WebCoreElastosBridge::_CookiesEnabled(
    /* [in] */ IInterface* obj)
{
    WebCoreElastosBridge* webCoreElastosBridge = (WebCoreElastosBridge*)(IHandler*)(obj);
    return webCoreElastosBridge->CookiesEnabled();
}

AutoPtr< ArrayOf<String> > WebCoreElastosBridge::_GetPluginDirectories(
    /* [in] */ IInterface* obj)
{
    WebCoreElastosBridge* webCoreElastosBridge = (WebCoreElastosBridge*)(IHandler*)(obj);
    return webCoreElastosBridge->GetPluginDirectories();
}

String WebCoreElastosBridge::_GetPluginSharedDataDirectory(
    /* [in] */ IInterface* obj)
{
    WebCoreElastosBridge* webCoreElastosBridge = (WebCoreElastosBridge*)(IHandler*)(obj);
    return webCoreElastosBridge->GetPluginSharedDataDirectory();
}
void WebCoreElastosBridge::_SignalServiceFuncPtrQueue(
    /* [in] */ IInterface* obj)
{
    WebCoreElastosBridge* webCoreElastosBridge = (WebCoreElastosBridge*)(IHandler*)(obj);
    webCoreElastosBridge->SignalServiceFuncPtrQueue();
}

AutoPtr< ArrayOf<String> > WebCoreElastosBridge::_GetKeyStrengthList(
    /* [in] */ IInterface* obj)
{
    WebCoreElastosBridge* webCoreElastosBridge = (WebCoreElastosBridge*)(IHandler*)(obj);
    return webCoreElastosBridge->GetKeyStrengthList();
}

String WebCoreElastosBridge::_GetSignedPublicKey(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 index,
    /* [in] */ const String& challenge,
    /* [in] */ const String& url)
{
    WebCoreElastosBridge* webCoreElastosBridge = (WebCoreElastosBridge*)(IHandler*)(obj);
    return webCoreElastosBridge->GetSignedPublicKey(index, challenge, url);
}

String WebCoreElastosBridge::_ResolveFilePathForContentUri(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& uri)
{
    WebCoreElastosBridge* webCoreElastosBridge = (WebCoreElastosBridge*)(IHandler*)(obj);
    return webCoreElastosBridge->ResolveFilePathForContentUri(uri);
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
