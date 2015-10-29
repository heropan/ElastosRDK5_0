
#include "elastos/droid/webkit/webview/chromium/WebViewChromiumFactoryProvider.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

//=====================================================================
//    WebViewChromiumFactoryProvider::InnerSyncATraceStateRunnable
//=====================================================================
WebViewChromiumFactoryProvider::InnerSyncATraceStateRunnable::InnerSyncATraceStateRunnable(
    /* [in] */ WebViewChromiumFactoryProvider* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromiumFactoryProvider::InnerSyncATraceStateRunnable::Run()
{
    // ==================before translated======================
    // syncATraceState();
    assert(0);
    return NOERROR;
}

//=====================================================================
//  WebViewChromiumFactoryProvider::InnerStartChromiumLockedRunnable
//=====================================================================
WebViewChromiumFactoryProvider::InnerStartChromiumLockedRunnable::InnerStartChromiumLockedRunnable(
    /* [in] */ WebViewChromiumFactoryProvider* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromiumFactoryProvider::InnerStartChromiumLockedRunnable::Run()
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     startChromiumLocked();
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
// WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics
//=====================================================================
WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics::InnerWebViewFactoryProviderStatics(
    /* [in] */ WebViewChromiumFactoryProvider* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

String WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics::FindAddress(
    /* [in] */ const String& addr)
{
    // ==================before translated======================
    // return ContentViewStatics.findAddress(addr);
    assert(0);
    return String("");
}

String WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics::GetDefaultUserAgent(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // return AwSettings.getDefaultUserAgent();
    assert(0);
    return String("");
}

ECode WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics::SetWebContentsDebuggingEnabled(
    /* [in] */ Boolean enable)
{
    // ==================before translated======================
    // // Web Contents debugging is always enabled on debug builds.
    // if (!Build.IS_DEBUGGABLE) {
    //     WebViewChromiumFactoryProvider.this.
    //             setWebContentsDebuggingEnabled(enable);
    // }
    assert(0);
    return NOERROR;
}

ECode WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics::ClearClientCertPreferences(
    /* [in] */ IRunnable* onCleared)
{
    VALIDATE_NOT_NULL(onCleared);
    // ==================before translated======================
    // AwContentsStatics.clearClientCertPreferences(onCleared);
    assert(0);
    return NOERROR;
}

ECode WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics::FreeMemoryForTests()
{
    // ==================before translated======================
    // if (ActivityManager.isRunningInTestHarness()) {
    //     MemoryPressureListener.maybeNotifyMemoryPresure(
    //             ComponentCallbacks2.TRIM_MEMORY_COMPLETE);
    // }
    assert(0);
    return NOERROR;
}

ECode WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics::EnableSlowWholeDocumentDraw()
{
    // ==================before translated======================
    // WebViewChromium.enableSlowWholeDocumentDraw();
    assert(0);
    return NOERROR;
}

AutoPtr< ArrayOf< AutoPtr<IUri> > > WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics::ParseFileChooserResult(
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* intent)
{
    // ==================before translated======================
    // return FileChooserParamsAdapter.parseFileChooserResult(resultCode, intent);
    assert(0);
    AutoPtr< ArrayOf< AutoPtr<IUri> > > empty;
    return empty;
}

//=====================================================================
//                    WebViewChromiumFactoryProvider
//=====================================================================
const String WebViewChromiumFactoryProvider::TAG("WebViewChromiumFactoryProvider");
const String WebViewChromiumFactoryProvider::CHROMIUM_PREFS_NAME("WebViewChromiumPrefs");
const String WebViewChromiumFactoryProvider::VERSION_CODE_PREF("lastVersionCodeUsed");
const String WebViewChromiumFactoryProvider::COMMAND_LINE_FILE("/data/local/tmp/webview-command-line");

WebViewChromiumFactoryProvider::WebViewChromiumFactoryProvider()
{
    // ==================before translated======================
    // ThreadUtils.setWillOverrideUiThread();
    // // Load chromium library.
    // Trace.traceBegin(Trace.TRACE_TAG_WEBVIEW, "AwBrowserProcess.loadLibrary()");
    // AwBrowserProcess.loadLibrary();
    // Trace.traceEnd(Trace.TRACE_TAG_WEBVIEW);
    // // Load glue-layer support library.
    // System.loadLibrary("webviewchromium_plat_support");
    //
    // // TODO: temporary try/catch while framework builds catch up with WebView builds.
    // // Remove this.
    // try {
    //     // Use shared preference to check for package downgrade.
    //     mWebViewPrefs = ActivityThread.currentApplication().getSharedPreferences(
    //                         CHROMIUM_PREFS_NAME, IContext.MODE_PRIVATE);
    //     int lastVersion = mWebViewPrefs.getInt(VERSION_CODE_PREF, 0);
    //     int currentVersion = WebViewFactory.getLoadedPackageInfo().versionCode;
    //     if (lastVersion > currentVersion) {
    //         // The WebView package has been downgraded since we last ran in this application.
    //         // Delete the WebView data directory's contents.
    //         String dataDir = PathUtils.getDataDirectory(ActivityThread.currentApplication());
    //         Log.i(TAG, "WebView package downgraded from " + lastVersion + " to " + currentVersion +
    //                    "; deleting contents of " + dataDir);
    //         FileUtils.deleteContents(new File(dataDir));
    //     }
    //     if (lastVersion != currentVersion) {
    //         mWebViewPrefs.edit().putInt(VERSION_CODE_PREF, currentVersion).apply();
    //     }
    // } catch (NoSuchMethodError e) {
    //     Log.w(TAG, "Not doing version downgrade check as framework is too old.");
    // }
    //
    // // Now safe to use WebView data directory.
}

Boolean WebViewChromiumFactoryProvider::HasStarted()
{
    // ==================before translated======================
    // return mStarted;
    assert(0);
    return FALSE;
}

ECode WebViewChromiumFactoryProvider::StartYourEngines(
    /* [in] */ Boolean onMainThread)
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     ensureChromiumStartedLocked(onMainThread);
    //
    // }
    assert(0);
    return NOERROR;
}

AutoPtr<AwBrowserContext> WebViewChromiumFactoryProvider::GetBrowserContext()
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     return getBrowserContextLocked();
    // }
    assert(0);
    AutoPtr<AwBrowserContext> empty;
    return empty;
}

AutoPtr<IInterface/*IWebViewFactoryProvider::IStatics*/> WebViewChromiumFactoryProvider::GetStatics()
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     if (mStaticMethods == null) {
    //         // TODO: Optimization potential: most these methods only need the native library
    //         // loaded and initialized, not the entire browser process started.
    //         // See also http://b/7009882
    //         ensureChromiumStartedLocked(true);
    //         mStaticMethods = new WebViewFactoryProvider.Statics() {
    //             @Override
    //             public String findAddress(String addr) {
    //                 return ContentViewStatics.findAddress(addr);
    //             }
    //
    //             @Override
    //             public String getDefaultUserAgent(IContext context) {
    //                 return AwSettings.getDefaultUserAgent();
    //             }
    //
    //             @Override
    //             public void setWebContentsDebuggingEnabled(boolean enable) {
    //                 // Web Contents debugging is always enabled on debug builds.
    //                 if (!Build.IS_DEBUGGABLE) {
    //                     WebViewChromiumFactoryProvider.this.
    //                             setWebContentsDebuggingEnabled(enable);
    //                 }
    //             }
    //
    //             // TODO enable after L release to AOSP
    //             //@Override
    //             public void clearClientCertPreferences(Runnable onCleared) {
    //                 AwContentsStatics.clearClientCertPreferences(onCleared);
    //             }
    //
    //             @Override
    //             public void freeMemoryForTests() {
    //                 if (ActivityManager.isRunningInTestHarness()) {
    //                     MemoryPressureListener.maybeNotifyMemoryPresure(
    //                             ComponentCallbacks2.TRIM_MEMORY_COMPLETE);
    //                 }
    //             }
    //
    //             // TODO: Add @Override.
    //             public void enableSlowWholeDocumentDraw() {
    //                 WebViewChromium.enableSlowWholeDocumentDraw();
    //             }
    //
    //             @Override
    //             public Uri[] parseFileChooserResult(int resultCode, Intent intent) {
    //                 return FileChooserParamsAdapter.parseFileChooserResult(resultCode, intent);
    //             }
    //         };
    //     }
    // }
    // return mStaticMethods;
    assert(0);
    AutoPtr<IInterface/*IWebViewFactoryProvider::IStatics*/> empty;
    return empty;
}

AutoPtr<IInterface/*IWebViewProvider*/> WebViewChromiumFactoryProvider::CreateWebView(
    /* [in] */ IInterface/*WebView*/* webView,
    /* [in] */ IInterface/*WebView::PrivateAccess*/* privateAccess)
{
    // ==================before translated======================
    // WebViewChromium wvc = new WebViewChromium(this, webView, privateAccess);
    //
    // synchronized (mLock) {
    //     if (mWebViewsToStart != null) {
    //         mWebViewsToStart.add(new WeakReference<WebViewChromium>(wvc));
    //     }
    // }
    //
    // return wvc;
    assert(0);
    AutoPtr<IInterface/*IWebViewProvider*/> empty;
    return empty;
}

AutoPtr<IInterface/*IGeolocationPermissions*/> WebViewChromiumFactoryProvider::GetGeolocationPermissions()
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     if (mGeolocationPermissions == null) {
    //         ensureChromiumStartedLocked(true);
    //         mGeolocationPermissions = new GeolocationPermissionsAdapter(
    //                 getBrowserContextLocked().getGeolocationPermissions());
    //     }
    // }
    // return mGeolocationPermissions;
    assert(0);
    AutoPtr<IInterface/*IGeolocationPermissions*/> empty;
    return empty;
}

AutoPtr<IInterface/*ICookieManager*/> WebViewChromiumFactoryProvider::GetCookieManager()
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     if (mCookieManager == null) {
    //         if (!mStarted) {
    //             // We can use CookieManager without starting Chromium; the native code
    //             // will bring up just the parts it needs to make this work on a temporary
    //             // basis until Chromium is started for real. The temporary cookie manager
    //             // needs the application context to have been set.
    //             ContentMain.initApplicationContext(ActivityThread.currentApplication());
    //         }
    //         mCookieManager = new CookieManagerAdapter(new AwCookieManager());
    //     }
    // }
    // return mCookieManager;
    assert(0);
    AutoPtr<IInterface/*ICookieManager*/> empty;
    return empty;
}

AutoPtr<IInterface/*IWebIconDatabase*/> WebViewChromiumFactoryProvider::GetWebIconDatabase()
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     if (mWebIconDatabase == null) {
    //         ensureChromiumStartedLocked(true);
    //         mWebIconDatabase = new WebIconDatabaseAdapter();
    //     }
    // }
    // return mWebIconDatabase;
    assert(0);
    AutoPtr<IInterface/*IWebIconDatabase*/> empty;
    return empty;
}

AutoPtr<IInterface/*IWebStorage*/> WebViewChromiumFactoryProvider::GetWebStorage()
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     if (mWebStorage == null) {
    //         ensureChromiumStartedLocked(true);
    //         mWebStorage = new WebStorageAdapter(AwQuotaManagerBridge.getInstance());
    //     }
    // }
    // return mWebStorage;
    assert(0);
    AutoPtr<IInterface/*IWebStorage*/> empty;
    return empty;
}

AutoPtr<IInterface/*IWebIconDatabase*/> WebViewChromiumFactoryProvider::GetWebViewDatabase(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     if (mWebViewDatabase == null) {
    //         ensureChromiumStartedLocked(true);
    //         AwBrowserContext browserContext = getBrowserContextLocked();
    //         mWebViewDatabase = new WebViewDatabaseAdapter(
    //                 browserContext.getFormDatabase(),
    //                 browserContext.getHttpAuthDatabase(context));
    //     }
    // }
    // return mWebViewDatabase;
    assert(0);
    AutoPtr<IInterface/*IWebIconDatabase*/> empty;
    return empty;
}

ECode WebViewChromiumFactoryProvider::InitPlatSupportLibrary()
{
    // ==================before translated======================
    // DrawGLFunctor.setChromiumAwDrawGLFunction(AwContents.getAwDrawGLFunction());
    // AwContents.setAwDrawSWFunctionTable(GraphicsUtils.getDrawSWFunctionTable());
    // AwContents.setAwDrawGLFunctionTable(GraphicsUtils.getDrawGLFunctionTable());
    assert(0);
    return NOERROR;
}

ECode WebViewChromiumFactoryProvider::InitTraceEvent()
{
    // ==================before translated======================
    // syncATraceState();
    // SystemProperties.addChangeCallback(new Runnable() {
    //     @Override
    //     public void run() {
    //         syncATraceState();
    //     }
    // });
    assert(0);
    return NOERROR;
}

ECode WebViewChromiumFactoryProvider::SyncATraceState()
{
    // ==================before translated======================
    // long enabledFlags = SystemProperties.getLong("debug.atrace.tags.enableflags", 0);
    // TraceEvent.setATraceEnabled((enabledFlags & Trace.TRACE_TAG_WEBVIEW) != 0);
    assert(0);
    return NOERROR;
}

ECode WebViewChromiumFactoryProvider::EnsureChromiumStartedLocked(
    /* [in] */ Boolean onMainThread)
{
    // ==================before translated======================
    // assert Thread.holdsLock(mLock);
    //
    // if (mStarted) {  // Early-out for the common case.
    //     return;
    // }
    //
    // Looper looper = !onMainThread ? Looper.myLooper() : Looper.getMainLooper();
    // Log.v(TAG, "Binding Chromium to " +
    //         (Looper.getMainLooper().equals(looper) ? "main":"background") +
    //         " looper " + looper);
    // ThreadUtils.setUiThread(looper);
    //
    // if (ThreadUtils.runningOnUiThread()) {
    //     startChromiumLocked();
    //     return;
    // }
    //
    // // We must post to the UI thread to cover the case that the user has invoked Chromium
    // // startup by using the (thread-safe) CookieManager rather than creating a WebView.
    // ThreadUtils.postOnUiThread(new Runnable() {
    //     @Override
    //     public void run() {
    //         synchronized (mLock) {
    //             startChromiumLocked();
    //         }
    //     }
    // });
    // while (!mStarted) {
    //     try {
    //         // Important: wait() releases |mLock| the UI thread can take it :-)
    //         mLock.wait();
    //     } catch (InterruptedException e) {
    //         // Keep trying... eventually the UI thread will process the task we sent it.
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode WebViewChromiumFactoryProvider::StartChromiumLocked()
{
    // ==================before translated======================
    // assert Thread.holdsLock(mLock) && ThreadUtils.runningOnUiThread();
    //
    // // The post-condition of this method is everything is ready, so notify now to cover all
    // // return paths. (Other threads will not wake-up until we release |mLock|, whatever).
    // mLock.notifyAll();
    //
    // if (mStarted) {
    //     return;
    // }
    //
    // if (Build.IS_DEBUGGABLE) {
    //     // Suppress the StrictMode violation as this codepath is only hit on debugglable builds.
    //     StrictMode.ThreadPolicy oldPolicy = StrictMode.allowThreadDiskReads();
    //     CommandLine.initFromFile(COMMAND_LINE_FILE);
    //     StrictMode.setThreadPolicy(oldPolicy);
    // } else {
    //     CommandLine.init(null);
    // }
    //
    // CommandLine cl = CommandLine.getInstance();
    // // TODO: currently in a relase build the DCHECKs only log. We either need to insall
    // // a report handler with SetLogReportHandler to make them assert, or else compile
    // // them out of the build altogether (b/8284203). Either way, so long they're
    // // compiled in, we may as unconditionally enable them here.
    // cl.appendSwitch("enable-dcheck");
    //
    // // TODO: Remove when GL is supported by default in the upstream code.
    // if (!cl.hasSwitch("disable-webview-gl-mode")) {
    //     cl.appendSwitch("testing-webview-gl-mode");
    // }
    //
    // // We don't need to extract any paks because for WebView, they are
    // // in the system image.
    // ResourceExtractor.setMandatoryPaksToExtract("");
    //
    // try {
    //     LibraryLoader.ensureInitialized();
    // } catch(ProcessInitException e) {
    //     throw new RuntimeException("Error initializing WebView library", e);
    // }
    //
    // PathService.override(PathService.DIR_MODULE, "/system/lib/");
    // // TODO: DIR_RESOURCE_PAKS_ANDROID needs to live somewhere sensible,
    // // inlined here for simplicity setting up the HTMLViewer demo. Unfortunately
    // // it can't go into base.PathService, as the native constant it refers to
    // // lives in the ui/ layer. See ui/base/ui_base_paths.h
    // final int DIR_RESOURCE_PAKS_ANDROID = 3003;
    // PathService.override(DIR_RESOURCE_PAKS_ANDROID,
    //         "/system/framework/webview/paks");
    //
    // // Make sure that ResourceProvider is initialized before starting the browser process.
    // setUpResources(ActivityThread.currentApplication());
    // initPlatSupportLibrary();
    // AwBrowserProcess.start(ActivityThread.currentApplication());
    //
    // if (Build.IS_DEBUGGABLE) {
    //     setWebContentsDebuggingEnabled(true);
    // }
    //
    // initTraceEvent();
    // mStarted = true;
    //
    // for (WeakReference<WebViewChromium> wvc : mWebViewsToStart) {
    //     WebViewChromium w = wvc.get();
    //     if (w != null) {
    //         w.startYourEngine();
    //     }
    // }
    // mWebViewsToStart.clear();
    // mWebViewsToStart = null;
    assert(0);
    return NOERROR;
}

AutoPtr<AwBrowserContext> WebViewChromiumFactoryProvider::GetBrowserContextLocked()
{
    // ==================before translated======================
    // assert Thread.holdsLock(mLock);
    // assert mStarted;
    // if (mBrowserContext == null) {
    //     mBrowserContext = new AwBrowserContext(mWebViewPrefs);
    // }
    // return mBrowserContext;
    assert(0);
    AutoPtr<AwBrowserContext> empty;
    return empty;
}

ECode WebViewChromiumFactoryProvider::SetWebContentsDebuggingEnabled(
    /* [in] */ Boolean enable)
{
    // ==================before translated======================
    // if (Looper.myLooper() != ThreadUtils.getUiThreadLooper()) {
    //     throw new RuntimeException(
    //             "Toggling of Web Contents Debugging must be done on the UI thread");
    // }
    // if (mDevToolsServer == null) {
    //     if (!enable) return;
    //     mDevToolsServer = new AwDevToolsServer();
    // }
    // mDevToolsServer.setRemoteDebuggingEnabled(enable);
    assert(0);
    return NOERROR;
}

ECode WebViewChromiumFactoryProvider::SetUpResources(
    /* [in] */ IContext* ctx)
{
    VALIDATE_NOT_NULL(ctx);
    // ==================before translated======================
    // ResourceRewriter.rewriteRValues(ctx);
    //
    // AwResource.setResources(ctx.getResources());
    // AwResource.setErrorPageResources(android.R.raw.loaderror,
    //         android.R.raw.nodomain);
    // AwResource.setConfigKeySystemUuidMapping(
    //         android.R.array.config_keySystemUuidMapping);
    assert(0);
    return NOERROR;
}

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


