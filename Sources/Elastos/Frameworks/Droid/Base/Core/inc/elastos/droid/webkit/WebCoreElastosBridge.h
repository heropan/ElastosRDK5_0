#ifndef __ELASTOS_DROID_WEBKIT_WEBCOREELASTOSBRIDGE_H__
#define __ELASTOS_DROID_WEBKIT_WEBCOREELASTOSBRIDGE_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/HandlerBase.h"
#include <elastos/utility/etl/HashSet.h>
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashSet;
using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Net::IProxyProperties;
using Elastos::Droid::Os::HandlerBase;

namespace Elastos {
namespace Droid {
namespace Webkit {

//callback functions for the webkit layer
struct ElaElastosBridgeCallback
{
    AutoPtr<IWeakReference> obj;//point to the WebCoreElastosBridge
    void (*setSharedTimer)(IInterface* obj, Int64 timemillis);
    void (*stopSharedTimer)(IInterface* obj);
    void (*setCookies)(IInterface* obj, const String& url, const String& value);
    String (*cookies)(IInterface* obj, const String& url);
    Boolean (*cookiesEnabled)(IInterface* obj);
    AutoPtr< ArrayOf<String> > (*getPluginDirectories)(IInterface* obj);
    String (*getPluginSharedDataDirectory)(IInterface* obj);
    void (*signalServiceFuncPtrQueue)(IInterface* obj);
    AutoPtr< ArrayOf<String> > (*getKeyStrengthList)(IInterface* obj);
    String (*getSignedPublicKey)(IInterface* obj, Int32 index, const String& challenge, const String& url);
    String (*resolveFilePathForContentUri)(IInterface* obj, const String& uri);
};


#ifdef __cplusplus
extern "C"
{
#endif
    extern int Elastos_ElastosBridge_Constructor(Int32 callbacks);
    extern void Elastos_ElastosBridge_Finalize(Int32 obj);
    extern void Elastos_ElastosBridge_SharedTimerFired();
    extern void Elastos_ElastosBridge_SetCacheSize(Int32 bytes);
    extern void Elastos_ElastosBridge_SetNetworkOnLine(Boolean online);
    extern void Elastos_ElastosBridge_SetNetworkType(const Elastos::String& type, const Elastos::String& subtype);
    extern void Elastos_ElastosBridge_ServiceFuncPtrQueue();
    extern void Elastos_ElastosBridge_UpdatePluginDirectories(ArrayOf<String>* array, Boolean reload);
    extern void Elastos_ElastosBridge_AddPackageNames(Elastos::Utility::HashSet<String>* packageNames);
    extern void Elastos_ElastosBridge_AddPackageName(const Elastos::String& packageName);
    extern void Elastos_ElastosBridge_RemovePackageName(const Elastos::String& packageName);
    extern void Elastos_ElastosBridge_UpdateProxy(const String& newProxy, const String& newExList);
#ifdef __cplusplus
}
#endif

class CWebViewClassic;

class WebCoreElastosBridge
    : public HandlerBase
{
public:
    /**
     * Construct a new JWebCoreJavaBridge to interface with
     * WebCore timers and cookies.
     */
    WebCoreElastosBridge();

    ~WebCoreElastosBridge();

    CAR_INTERFACE_DECL();

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    static CARAPI_(void) SetActiveWebView(
        /* [in] */ CWebViewClassic* webview);

    static CARAPI_(void) RemoveActiveWebView(
        /* [in] */ CWebViewClassic* webview);

    /**
     * handleMessage
     * @param msg The dispatched message.
     *
     * The only accepted message currently is TIMER_MESSAGE
     */
    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

    /**
     * Pause all timers.
     */
    CARAPI_(void) Pause();

    /**
     * Resume all timers.
     */
    CARAPI_(void) Resume();

    /**
     * Set WebCore cache size.
     * @param bytes The cache size in bytes.
     */
    CARAPI_(void) SetCacheSize(
        /* [in] */ Int32 bytes);

    CARAPI_(void) StoreFilePathForContentUri(
        /* [in] */ const String& path,
        /* [in] */ const String& contentUri);

    CARAPI_(void) UpdateProxy(
        /* [in] */ IProxyProperties* proxyProperties);

    CARAPI_(void) SetNetworkOnLine(
        /* [in] */ Boolean online);

    CARAPI_(void) SetNetworkType(
        /* [in] */ const String& type,
        /* [in] */ const String& subtype);

    CARAPI_(void) AddPackageNames(
        /* [in] */ Elastos::Utility::HashSet<String>* packageNames);

    CARAPI_(void) AddPackageName(
        /* [in] */ const String& packageName);

    CARAPI_(void) RemovePackageName(
        /* [in] */ const String& packageName);

    CARAPI_(void) NativeUpdateProxy(
        /* [in] */ const String& newProxy,
        /* [in] */ const String& exclusionList);

private:
    /**
     * Call native timer callbacks.
     */
    CARAPI_(void) FireSharedTimer();

    // called from JNI side
    CARAPI_(void) SignalServiceFuncPtrQueue();

    CARAPI_(void) NativeServiceFuncPtrQueue();

    /**
     * Store a cookie string associated with a url.
     * @param url The url to be used as a key for the cookie.
     * @param value The cookie string to be stored.
     */
    CARAPI_(void) SetCookies(
        /* [in] */ const String& url,
        /* [in] */ const String& value);

    /**
     * Retrieve the cookie string for the given url.
     * @param url The resource's url.
     * @return A String representing the cookies for the given resource url.
     */
    CARAPI_(String) Cookies(
        /* [in] */ const String& url);

    /**
     * Returns whether cookies are enabled or not.
     */
    CARAPI_(Boolean) CookiesEnabled();

    /**
     * Returns an array of plugin directoies
     */
    CARAPI_(AutoPtr< ArrayOf<String> >) GetPluginDirectories();

    /**
     * Returns the path of the plugin data directory
     */
    CARAPI_(String) GetPluginSharedDataDirectory();

    /**
     * setSharedTimer
     * @param timemillis The relative time when the timer should fire
     */
    CARAPI_(void) SetSharedTimer(
        /* [in] */ Int64 timemillis);

    /**
     * Stop the shared timer.
     */
    CARAPI_(void) StopSharedTimer();

    CARAPI_(AutoPtr< ArrayOf<String> >) GetKeyStrengthList();

    CARAPI_(String) GetSignedPublicKey(
        /* [in] */ Int32 index,
        /* [in] */ const String& challenge,
        /* [in] */ const String& url);

    // Called on the WebCore thread through JNI.
    CARAPI_(String) ResolveFilePathForContentUri(
        /* [in] */ const String& uri);

    CARAPI_(void) NativeConstructor();

    CARAPI_(void) NativeFinalize();

    CARAPI_(void) SharedTimerFired();

    CARAPI_(void) NativeUpdatePluginDirectories(
        /* [in] */ ArrayOf<String>* directories,
        /* [in] */ Boolean reload);

    static CARAPI_(void) _SetSharedTimer(
        /* [in] */ IInterface* obj,
        /* [in] */ Int64 timemillis);

    static CARAPI_(void) _StopSharedTimer(
        /* [in] */ IInterface* obj);

    static CARAPI_(void) _SetCookies(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url,
        /* [in] */ const String& value);

    static CARAPI_(String) _Cookies(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url);

    static CARAPI_(Boolean) _CookiesEnabled(
        /* [in] */ IInterface* obj);

    static CARAPI_(AutoPtr< ArrayOf<String> >) _GetPluginDirectories(
        /* [in] */ IInterface* obj);

    static CARAPI_(String) _GetPluginSharedDataDirectory(
        /* [in] */ IInterface* obj);

    static CARAPI_(void) _SignalServiceFuncPtrQueue(
        /* [in] */ IInterface* obj);

    static CARAPI_(AutoPtr< ArrayOf<String> >) _GetKeyStrengthList(
        /* [in] */ IInterface* obj);

    static CARAPI_(String) _GetSignedPublicKey(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 index,
        /* [in] */ const String& challenge,
        /* [in] */ const String& url);

    static CARAPI_(String) _ResolveFilePathForContentUri(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& uri);

public:
    /* package */
    static const Int32 REFRESH_PLUGINS = 100;

private:
    // Identifier for the timer message.
    static const Int32 TIMER_MESSAGE = 1;
    // ID for servicing functionptr queue
    static const Int32 FUNCPTR_MESSAGE = 2;
    // Log system identifier.
    static const String LOGTAG;

    // Native object pointer for interacting in native code.
    Int32 mNativeBridge;
    // Instant timer is used to implement a timer that needs to fire almost
    // immediately.
    Boolean mHasInstantTimer;

    Boolean mTimerPaused;
    Boolean mHasDeferredTimers;

    // keep track of the main WebViewClassic attached to the current window so that we
    // can get the proper Context.
    static AutoPtr<IWeakReference> sCurrentMainWebView;

    AutoPtr< HashMap<String, String> > mContentUriToFilePathMap;

    static Object sLock;
    Object mLock;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBCOREELASTOSBRIDGE_H__
