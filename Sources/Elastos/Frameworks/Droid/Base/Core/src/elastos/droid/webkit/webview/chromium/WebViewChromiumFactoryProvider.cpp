
#include "elastos/droid/webkit/webview/chromium/WebViewChromiumFactoryProvider.h"
#include "elastos/core/AutoLock.h"
#include "elastos/core/Thread.h"
#include "elastos/droid/os/Build.h"
#include "elastos/droid/os/FileUtils.h"
#include "elastos/droid/os/SystemProperties.h"
#include "elastos/droid/os/CLooperHelper.h"
//#include "elastos/droid/app/CActivityManagerHelper.h"
//#include "elastos/droid/app/CActivityThreadHelper.h"
//#include "elastos/droid/webkit/WebViewFactory.h"
#include "elastos/droid/webkit/native/content/browser/ContentViewStatics.h"
#include "elastos/droid/webkit/native/content/browser/ResourceExtractor.h"
#include "elastos/droid/webkit/native/content/app/ContentMain.h"
#include "elastos/droid/webkit/native/base/MemoryPressureListener.h"
#include "elastos/droid/webkit/native/base/ThreadUtils.h"
#include "elastos/droid/webkit/native/base/PathUtils.h"
#include "elastos/droid/webkit/native/base/TraceEvent.h"
#include "elastos/droid/webkit/native/base/CommandLine.h"
#include "elastos/droid/webkit/native/base/PathService.h"
#include "elastos/droid/webkit/native/android_webview/AwSettings.h"
#include "elastos/droid/webkit/native/android_webview/AwContentsStatics.h"
#include "elastos/droid/webkit/native/android_webview/AwBrowserProcess.h"
#include "elastos/droid/webkit/native/android_webview/AwResource.h"
#include "elastos/droid/webkit/webview/chromium/WebViewChromium.h"
#include "elastos/droid/webkit/webview/chromium/FileChooserParamsAdapter.h"
#include "elastos/droid/webkit/webview/chromium/GraphicsUtils.h"
#include "elastos/droid/webkit/webview/chromium/ResourceRewriter.h"

using Elastos::Core::AutoLock;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::Thread;
using Elastos::Core::EIID_IRunnable;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::FileUtils;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::ILooperHelper;
using Elastos::Droid::Os::CLooperHelper;
using Elastos::Droid::Os::IStrictMode;
using Elastos::Droid::Os::IStrictModeThreadPolicy;
using Elastos::Droid::App::IActivityManagerHelper;
//using Elastos::Droid::App::CActivityManagerHelper;
using Elastos::Droid::App::IActivityThreadHelper;
//using Elastos::Droid::App::CActivityThreadHelper;
using Elastos::Droid::App::IApplication;
using Elastos::Droid::Content::IComponentCallbacks2;
using Elastos::Droid::Content::ISharedPreferencesEditor;
//using Elastos::Droid::Webkit::WebViewFactory;
using Elastos::Droid::Webkit::Base::MemoryPressureListener;
using Elastos::Droid::Webkit::Base::ThreadUtils;
using Elastos::Droid::Webkit::Base::PathUtils;
using Elastos::Droid::Webkit::Base::TraceEvent;
using Elastos::Droid::Webkit::Base::CommandLine;
using Elastos::Droid::Webkit::Base::PathService;
using Elastos::Droid::Webkit::Content::Browser::ContentViewStatics;
using Elastos::Droid::Webkit::Content::Browser::ResourceExtractor;
using Elastos::Droid::Webkit::Content::App::ContentMain;
using Elastos::Droid::Webkit::AndroidWebview::AwSettings;
using Elastos::Droid::Webkit::AndroidWebview::AwContentsStatics;
using Elastos::Droid::Webkit::AndroidWebview::AwBrowserProcess;
using Elastos::Droid::Webkit::AndroidWebview::AwResource;
using Elastos::Droid::Webkit::Webview::Chromium::WebViewChromium;
using Elastos::Droid::Webkit::Webview::Chromium::FileChooserParamsAdapter;
using Elastos::Droid::Webkit::Webview::Chromium::ResourceRewriter;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

//=====================================================================
//    WebViewChromiumFactoryProvider::InnerSyncATraceStateRunnable
//=====================================================================
CAR_INTERFACE_IMPL(WebViewChromiumFactoryProvider::InnerSyncATraceStateRunnable, Object, IRunnable)

WebViewChromiumFactoryProvider::InnerSyncATraceStateRunnable::InnerSyncATraceStateRunnable()
{
}

ECode WebViewChromiumFactoryProvider::InnerSyncATraceStateRunnable::Run()
{
    // ==================before translated======================
    // syncATraceState();

    return WebViewChromiumFactoryProvider::SyncATraceState();
}

//=====================================================================
//  WebViewChromiumFactoryProvider::InnerStartChromiumLockedRunnable
//=====================================================================
CAR_INTERFACE_IMPL(WebViewChromiumFactoryProvider::InnerStartChromiumLockedRunnable, Object, IRunnable)

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

    assert(NULL == mOwner);
    AutoLock lock(mOwner->mLock);
    return mOwner->StartChromiumLocked();
}

//=====================================================================
// WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics
//=====================================================================
//CAR_INTERFACE_IMPL(WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics, Object, IWebViewFactoryProvider::IStatics)

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

    assert(NULL == mOwner);
    return ContentViewStatics::FindAddress(addr);
}

String WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics::GetDefaultUserAgent(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // return AwSettings.getDefaultUserAgent();

    assert(NULL == mOwner);
    return AwSettings::GetDefaultUserAgent();
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

    assert(NULL == mOwner);
    // Web Contents debugging is always enabled on debug builds.
    if (!Build::IS_DEBUGGABLE) {
        mOwner->SetWebContentsDebuggingEnabled(enable);
    }
    return NOERROR;
}

ECode WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics::ClearClientCertPreferences(
    /* [in] */ IRunnable* onCleared)
{
    VALIDATE_NOT_NULL(onCleared);
    // ==================before translated======================
    // AwContentsStatics.clearClientCertPreferences(onCleared);

    assert(NULL == mOwner);
    AwContentsStatics::ClearClientCertPreferences(onCleared);
    return NOERROR;
}

ECode WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics::FreeMemoryForTests()
{
    // ==================before translated======================
    // if (ActivityManager.isRunningInTestHarness()) {
    //     MemoryPressureListener.maybeNotifyMemoryPresure(
    //             ComponentCallbacks2.TRIM_MEMORY_COMPLETE);
    // }

    assert(NULL == mOwner);
    AutoPtr<IActivityManagerHelper> helper;
    //CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&helper);
    Boolean isRunningInTest = FALSE;
    helper->IsRunningInTestHarness(&isRunningInTest);
    if (isRunningInTest) {
        MemoryPressureListener::MaybeNotifyMemoryPresure(IComponentCallbacks2::TRIM_MEMORY_COMPLETE);
    }
    return NOERROR;
}

ECode WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics::EnableSlowWholeDocumentDraw()
{
    // ==================before translated======================
    // WebViewChromium.enableSlowWholeDocumentDraw();

    assert(NULL == mOwner);
    return WebViewChromium::EnableSlowWholeDocumentDraw();
}

AutoPtr< ArrayOf<IUri*> > WebViewChromiumFactoryProvider::InnerWebViewFactoryProviderStatics::ParseFileChooserResult(
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* intent)
{
    // ==================before translated======================
    // return FileChooserParamsAdapter.parseFileChooserResult(resultCode, intent);

    assert(NULL == mOwner);
    return FileChooserParamsAdapter::ParseFileChooserResult(resultCode, intent);
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

    ThreadUtils::SetWillOverrideUiThread();
    // Load chromium library.
    //Trace.traceBegin(Trace.TRACE_TAG_WEBVIEW, "AwBrowserProcess.loadLibrary()");
    AwBrowserProcess::LoadLibrary();
    //Trace.traceEnd(Trace.TRACE_TAG_WEBVIEW);
    // Load glue-layer support library.

    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);
    system->LoadLibrary(String("webviewchromium_plat_support"));

    // TODO: temporary try/catch while framework builds catch up with WebView builds.
    // Remove this.
    // try {
        // Use shared preference to check for package downgrade.
        AutoPtr<IActivityThreadHelper> helper;
        //CActivityThreadHelper::AcquireSingleton((IActivityThreadHelper**)&helper);

        AutoPtr<IApplication> app;
        helper->GetCurrentApplication((IApplication**)&app);

        AutoPtr<IContext> appContext = IContext::Probe(app);
        appContext->GetSharedPreferences(CHROMIUM_PREFS_NAME, IContext::MODE_PRIVATE, (ISharedPreferences**)&mWebViewPrefs);

        Int32 lastVersion = 0;
        mWebViewPrefs->GetInt32(VERSION_CODE_PREF, 0, &lastVersion);
        Int32 currentVersion = 0;// class has no this func: WebViewFactory::GetLoadedPackageInfo().versionCode;
        if (lastVersion > currentVersion) {
            // The WebView package has been downgraded since we last ran in this application.
            // Delete the WebView data directory's contents.
            String dataDir = PathUtils::GetDataDirectory(appContext);
            //Logger::I(TAG, "WebView package downgraded from " + lastVersion + " to " + currentVersion + "; deleting contents of " + dataDir);

            AutoPtr<IFile> file;
            CFile::New(dataDir, (IFile**)&file);
            FileUtils::DeleteContents(file);
        }

        if (lastVersion != currentVersion) {
            AutoPtr<ISharedPreferencesEditor> edit;
            mWebViewPrefs->Edit((ISharedPreferencesEditor**)&edit);
            edit->PutInt32(VERSION_CODE_PREF, currentVersion);
            edit->Apply();
        }
    // } catch (NoSuchMethodError e) {
    //    Logger::W(TAG, "Not doing version downgrade check as framework is too old.");
    // }

    // Now safe to use WebView data directory.
}

Boolean WebViewChromiumFactoryProvider::HasStarted()
{
    // ==================before translated======================
    // return mStarted;

    return mStarted;
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
    AutoLock lock(mLock);
    EnsureChromiumStartedLocked(onMainThread);
    return NOERROR;
}

AutoPtr<AwBrowserContext> WebViewChromiumFactoryProvider::GetBrowserContext()
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     return getBrowserContextLocked();
    // }

    assert(0);
    AutoLock lock(mLock);
    return GetBrowserContextLocked();;
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
    AutoLock lock(mLock);
    if (NULL == mStaticMethods) {
        // TODO: Optimization potential: most these methods only need the native library
        // loaded and initialized, not the entire browser process started.
        // See also http://b/7009882
        EnsureChromiumStartedLocked(TRUE);
        //mStaticMethods = new InnerWebViewFactoryProviderStatics(this);
    }
    return mStaticMethods;
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
    AutoPtr<IInterface> thisInterface = this->Probe(EIID_IInterface);
    AutoPtr<WebViewChromium> wvc;// = new WebViewChromium(thisInterface, webView, privateAccess);
    AutoLock lock(mLock);
    if (mWebViewsToStart != NULL) {
        AutoPtr<IWeakReference> wr;
        wvc->GetWeakReference((IWeakReference**)&wr);
        mWebViewsToStart->Add(wr);
    }

    AutoPtr<IInterface/*IWebViewProvider*/> result;// = (IWebViewProvider*)wvc;
    return result;
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
    AutoLock lock(mLock);
    if (NULL == mGeolocationPermissions) {
        EnsureChromiumStartedLocked(TRUE);
        mGeolocationPermissions = new GeolocationPermissionsAdapter(GetBrowserContextLocked()->GetGeolocationPermissions());
    }

    AutoPtr<IInterface/*IGeolocationPermissions*/> result;// = (IGeolocationPermissions*)mGeolocationPermissions;
    return result;
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
    AutoLock lock(mLock);
    if (NULL == mCookieManager) {
        if (!mStarted) {
            // We can use CookieManager without starting Chromium; the native code
            // will bring up just the parts it needs to make this work on a temporary
            // basis until Chromium is started for real. The temporary cookie manager
            // needs the application context to have been set.
            AutoPtr<IActivityThreadHelper> helper;
            //CActivityThreadHelper::AcquireSingleton((IActivityThreadHelper**)&helper);
            AutoPtr<IApplication> app;
            helper->GetCurrentApplication((IApplication**)&app);
            AutoPtr<IContext> appContext = IContext::Probe(app);
            ContentMain::InitApplicationContext(appContext);
        }

        AutoPtr<AwCookieManager> cookieManager = new AwCookieManager();
        mCookieManager = new CookieManagerAdapter(cookieManager);
    }

    AutoPtr<IInterface/*ICookieManager*/> result;// = (ICookieManager*)mCookieManager;
    return result;
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
    AutoLock lock(mLock);
    if (NULL == mWebIconDatabase) {
        EnsureChromiumStartedLocked(TRUE);
        mWebIconDatabase = new WebIconDatabaseAdapter();
    }

    AutoPtr<IInterface/*IWebIconDatabase*/> result;// = (IWebIconDatabase*)mWebIconDatabase;
    return result;
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
    AutoLock lock(mLock);
    if (NULL == mWebStorage) {
        EnsureChromiumStartedLocked(TRUE);
        mWebStorage = new WebStorageAdapter(AwQuotaManagerBridge::GetInstance());
    }

    AutoPtr<IInterface/*IWebStorage*/> result;// = (IWebStorage*)mWebStorage;
    return result;
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
    AutoLock lock(mLock);
    if (NULL == mWebViewDatabase) {
        EnsureChromiumStartedLocked(TRUE);
        AutoPtr<AwBrowserContext> browserContext = GetBrowserContextLocked();
        mWebViewDatabase = new WebViewDatabaseAdapter(
            browserContext->GetFormDatabase(),
            browserContext->GetHttpAuthDatabase(context));
    }

    AutoPtr<IInterface/*IWebIconDatabase*/> result;// = (IWebIconDatabase*)mWebStorage;
    return result;
}

ECode WebViewChromiumFactoryProvider::InitPlatSupportLibrary()
{
    // ==================before translated======================
    // DrawGLFunctor.setChromiumAwDrawGLFunction(AwContents.getAwDrawGLFunction());
    // AwContents.setAwDrawSWFunctionTable(GraphicsUtils.getDrawSWFunctionTable());
    // AwContents.setAwDrawGLFunctionTable(GraphicsUtils.getDrawGLFunctionTable());

    assert(0);
    DrawGLFunctor::SetChromiumAwDrawGLFunction(AwContents::GetAwDrawGLFunction());
    AwContents::SetAwDrawSWFunctionTable(GraphicsUtils::GetDrawSWFunctionTable());
    AwContents::SetAwDrawGLFunctionTable(GraphicsUtils::GetDrawGLFunctionTable());
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
    SyncATraceState();
    AutoPtr<IRunnable> runnable = new InnerSyncATraceStateRunnable();
    return SystemProperties::AddChangeCallback(runnable);
}

ECode WebViewChromiumFactoryProvider::SyncATraceState()
{
    // ==================before translated======================
    // long enabledFlags = SystemProperties.getLong("debug.atrace.tags.enableflags", 0);
    // TraceEvent.setATraceEnabled((enabledFlags & Trace.TRACE_TAG_WEBVIEW) != 0);

    assert(0);
    Int64 enabledFlags = 0;
    SystemProperties::GetInt64(String("debug.atrace.tags.enableflags"), 0, &enabledFlags);
    TraceEvent::SetATraceEnabled((enabledFlags /*& Trace.TRACE_TAG_WEBVIEW*/) != 0);
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
    assert(-1/*Thread::HoldsLock(mLock)*/);

    if (mStarted) {  // Early-out for the common case.
        return NOERROR;
    }

    AutoPtr<ILooper> looper;
    AutoPtr<ILooperHelper> looperHelper;
    CLooperHelper::AcquireSingleton((ILooperHelper**)&looperHelper);
    if (!onMainThread) {
        looperHelper->GetMyLooper((ILooper**)&looper);
    }
    else {
        looperHelper->GetMainLooper((ILooper**)&looper);
    }

    //Logger::V(TAG, "Binding Chromium to " + (Looper.getMainLooper().equals(looper) ? "main":"background") + " looper " + looper);
    ThreadUtils::SetUiThread(looper);

    if (ThreadUtils::RunningOnUiThread()) {
        StartChromiumLocked();
        return NOERROR;
    }

    // We must post to the UI thread to cover the case that the user has invoked Chromium
    // startup by using the (thread-safe) CookieManager rather than creating a WebView.
    AutoPtr<IRunnable> runnable = new InnerStartChromiumLockedRunnable(this);
    ThreadUtils::PostOnUiThread(runnable);
    while (!mStarted) {
        //try {
            // Important: wait() releases |mLock| the UI thread can take it :-)
            mLock->Wait();
        //} catch (InterruptedException e) {
            // Keep trying... eventually the UI thread will process the task we sent it.
        //}
    }

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
    assert(-1/*Thread::HoldsLock(mLock) && ThreadUtils::RunningOnUiThread()*/);

    // The post-condition of this method is everything is ready, so notify now to cover all
    // return paths. (Other threads will not wake-up until we release |mLock|, whatever).
    //mLock->NotifyAll();

    if (mStarted) {
        return NOERROR;
    }

    if (Build::IS_DEBUGGABLE) {
        // Suppress the StrictMode violation as this codepath is only hit on debugglable builds.
        AutoPtr<IStrictMode> strictMode;
        //CStrictMode::AcquireSingleton((IStrictMode**)&strictMode);
        AutoPtr<IStrictModeThreadPolicy> oldPolicy;
        strictMode->AllowThreadDiskReads((IStrictModeThreadPolicy**)&oldPolicy);
        CommandLine::InitFromFile(COMMAND_LINE_FILE);
        strictMode->SetThreadPolicy(oldPolicy);
    }
    else {
        CommandLine::Init(NULL);
    }

    AutoPtr<CommandLine> cl = CommandLine::GetInstance();
    // TODO: currently in a relase build the DCHECKs only log. We either need to insall
    // a report handler with SetLogReportHandler to make them assert, or else compile
    // them out of the build altogether (b/8284203). Either way, so long they're
    // compiled in, we may as unconditionally enable them here.
    cl->AppendSwitch(String("enable-dcheck"));

    // TODO: Remove when GL is supported by default in the upstream code.
    if (!cl->HasSwitch(String("disable-webview-gl-mode"))) {
        cl->AppendSwitch(String("testing-webview-gl-mode"));
    }

    // We don't need to extract any paks because for WebView, they are
    // in the system image.
    AutoPtr< ArrayOf<String> > pakesToExtract = ArrayOf<String>::Alloc(1);
    pakesToExtract->Set(0, String(""));
    ResourceExtractor::SetMandatoryPaksToExtract(pakesToExtract);

    //try {
        //--LibraryLoader::EnsureInitialized();
    //} catch(ProcessInitException e) {
    //    throw new RuntimeException("Error initializing WebView library", e);
    //}

    PathService::Override(PathService::DIR_MODULE, String("/system/lib/"));
    // TODO: DIR_RESOURCE_PAKS_ANDROID needs to live somewhere sensible,
    // inlined here for simplicity setting up the HTMLViewer demo. Unfortunately
    // it can't go into base.PathService, as the native constant it refers to
    // lives in the ui/ layer. See ui/base/ui_base_paths.h
    const Int32 DIR_RESOURCE_PAKS_ANDROID = 3003;
    PathService::Override(DIR_RESOURCE_PAKS_ANDROID, String("/system/framework/webview/paks"));

    // Make sure that ResourceProvider is initialized before starting the browser process.
    AutoPtr<IActivityThreadHelper> helper;
    //CActivityThreadHelper::AcquireSingleton((IActivityThreadHelper**)&helper);
    AutoPtr<IApplication> app;
    helper->GetCurrentApplication((IApplication**)&app);
    AutoPtr<IContext> appContext = IContext::Probe(app);
    SetUpResources(appContext);
    InitPlatSupportLibrary();
    AwBrowserProcess::Start(appContext);

    if (Build::IS_DEBUGGABLE) {
        SetWebContentsDebuggingEnabled(TRUE);
    }

    InitTraceEvent();
    mStarted = TRUE;

    Int32 webviewsSize = 0;
    mWebViewsToStart->GetSize(&webviewsSize);
    for (Int32 idx=0; idx<webviewsSize; ++idx) {
        AutoPtr<IInterface> item;
        mWebViewsToStart->Get(idx, (IInterface**)&item);
        AutoPtr<IWeakReference> wrItem = IWeakReference::Probe(item);
        AutoPtr<WebViewChromium> w = (WebViewChromium*)wrItem.Get();
        if (w != NULL) {
            w->StartYourEngine();
        }
    }

    mWebViewsToStart->Clear();
    mWebViewsToStart = NULL;
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
    assert(-1/*Thread::HoldsLock(mLock)*/);
    assert(mStarted);
    if (NULL == mBrowserContext) {
        mBrowserContext = new AwBrowserContext(mWebViewPrefs);
    }
    return mBrowserContext;
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
    AutoPtr<ILooper> looper;
    AutoPtr<ILooperHelper> helper;
    CLooperHelper::AcquireSingleton((ILooperHelper**)&helper);
    helper->GetMyLooper((ILooper**)&looper);

    AutoPtr<ILooper> uiLooper = ThreadUtils::GetUiThreadLooper();
    AutoPtr<IInterface> looperInterface = looper->Probe(EIID_IInterface);
    AutoPtr<IInterface> uiLooperInterface = uiLooper->Probe(EIID_IInterface);
    if (looperInterface != uiLooperInterface) {
        //throw new RuntimeException("Toggling of Web Contents Debugging must be done on the UI thread");
    }
    if (NULL == mDevToolsServer) {
        if (!enable)
            return NOERROR;
        mDevToolsServer = new AwDevToolsServer();
    }

    mDevToolsServer->SetRemoteDebuggingEnabled(enable);
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
    ResourceRewriter::RewriteRValues(ctx);

    AutoPtr<IResources> res;
    ctx->GetResources((IResources**)&res);
    AwResource::SetResources(res);
    AwResource::SetErrorPageResources(-1/*android::R::raw::loaderror*/, -1/*android::R::raw::nodomain*/);
    AwResource::SetConfigKeySystemUuidMapping(-1/*android::R::array::config_keySystemUuidMapping*/);
    return NOERROR;
}

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


