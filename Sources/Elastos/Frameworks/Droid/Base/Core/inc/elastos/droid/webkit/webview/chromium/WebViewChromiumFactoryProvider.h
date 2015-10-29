/*
  * Copyright (C) 2012 The Android Open Source Project
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *      http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */

#ifndef _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_WEBVIEWCHROMIUMFACTORYPROVIDER_H_
#define _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_WEBVIEWCHROMIUMFACTORYPROVIDER_H_

#include "elastos/droid/ext/frameworkext.h"
//#include "elastos/droid/webkit/WebViewFactoryProvider.h"
//#include "elastos/droid/webkit/WebView.h"
#include "elastos/droid/webkit/native/android_webview/AwBrowserContext.h"
#include "elastos/droid/webkit/native/android_webview/AwDevToolsServer.h"
#include "elastos/droid/webkit/webview/chromium/CookieManagerAdapter.h"
#include "elastos/droid/webkit/webview/chromium/WebViewDatabaseAdapter.h"
#include "elastos/droid/webkit/webview/chromium/GeolocationPermissionsAdapter.h"
#include "elastos/droid/webkit/webview/chromium/WebIconDatabaseAdapter.h"
#include "elastos/droid/webkit/webview/chromium/WebStorageAdapter.h"

// package com.android.webview.chromium;
// import android.content.pm.PackageManager;
// import android.content.res.Resources;
// import android.app.ActivityManager;
// import android.app.ActivityThread;
// import android.content.ComponentCallbacks2;
// import android.content.IContext;
// import android.content.IIntent;
// import android.content.SharedPreferences;
// import android.net.IUri;
// import android.os.Build;
// import android.os.FileUtils;
// import android.os.Looper;
// import android.os.StrictMode;
// import android.os.SystemProperties;
// import android.os.Trace;
// import android.util.Log;
// import android.webkit.ICookieManager;
// import android.webkit.IGeolocationPermissions;
// import android.webkit.IWebIconDatabase;
// import android.webkit.WebStorage;
// import android.webkit.WebView;
// import android.webkit.WebViewDatabase;
// import android.webkit.WebViewFactory;
// import android.webkit.WebViewFactoryProvider;
// import android.webkit.WebViewProvider;
// import org.chromium.android_webview.AwBrowserContext;
// import org.chromium.android_webview.AwBrowserProcess;
// import org.chromium.android_webview.AwContents;
// import org.chromium.android_webview.AwContentsStatics;
// import org.chromium.android_webview.AwCookieManager;
// import org.chromium.android_webview.AwDevToolsServer;
// import org.chromium.android_webview.AwFormDatabase;
// import org.chromium.android_webview.AwGeolocationPermissions;
// import org.chromium.android_webview.AwQuotaManagerBridge;
// import org.chromium.android_webview.AwResource;
// import org.chromium.android_webview.AwSettings;
// import org.chromium.base.CommandLine;
// import org.chromium.base.MemoryPressureListener;
// import org.chromium.base.PathService;
// import org.chromium.base.PathUtils;
// import org.chromium.base.ThreadUtils;
// import org.chromium.base.TraceEvent;
// import org.chromium.base.library_loader.LibraryLoader;
// import org.chromium.base.library_loader.ProcessInitException;
// import org.chromium.content.app.ContentMain;
// import org.chromium.content.browser.ContentViewStatics;
// import org.chromium.content.browser.ResourceExtractor;
// import java.io.File;
// import java.lang.ref.WeakReference;
// import java.util.ArrayList;

using Elastos::Core::IRunnable;
using Elastos::Utility::IList;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Net::IUri;
//using Elastos::Droid::Webkit::IWebViewFactoryProvider;
//using Elastos::Droid::Webkit::IWebViewProvider;
//using Elastos::Droid::Webkit::IGeolocationPermissions;
//using Elastos::Droid::Webkit::ICookieManager;
//using Elastos::Droid::Webkit::IWebIconDatabase;
//using Elastos::Droid::Webkit::IWebStorage;
//using Elastos::Droid::Webkit::WebView;
using Elastos::Droid::Webkit::AndroidWebview::AwBrowserContext;
using Elastos::Droid::Webkit::AndroidWebview::AwDevToolsServer;
using Elastos::Droid::Webkit::Webview::Chromium::CookieManagerAdapter;
using Elastos::Droid::Webkit::Webview::Chromium::WebViewDatabaseAdapter;
using Elastos::Droid::Webkit::Webview::Chromium::GeolocationPermissionsAdapter;
using Elastos::Droid::Webkit::Webview::Chromium::WebIconDatabaseAdapter;
using Elastos::Droid::Webkit::Webview::Chromium::WebStorageAdapter;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

class WebViewChromiumFactoryProvider
    : public Object
    //, public IWebViewFactoryProvider
{
private:
    class InnerSyncATraceStateRunnable
        : public Object
        , public IRunnable
    {
    public:
        CAR_INTERFACE_DECL()

        InnerSyncATraceStateRunnable();

        // @Override
        CARAPI Run();
    };

    class InnerStartChromiumLockedRunnable
        : public Object
        , public IRunnable
    {
    public:
        CAR_INTERFACE_DECL()

        InnerStartChromiumLockedRunnable(
            /* [in] */ WebViewChromiumFactoryProvider* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromiumFactoryProvider* mOwner;
    };

    class InnerWebViewFactoryProviderStatics
        : public Object
        //, public IWebViewFactoryProvider::IStatics
    {
    public:
        //CAR_INTERFACE_DECL()

        InnerWebViewFactoryProviderStatics(
            /* [in] */ WebViewChromiumFactoryProvider* owner);

        // @Override
        CARAPI_(String) FindAddress(
            /* [in] */ const String& addr);

        // @Override
        CARAPI_(String) GetDefaultUserAgent(
            /* [in] */ IContext* context);

        // @Override
        CARAPI SetWebContentsDebuggingEnabled(
            /* [in] */ Boolean enable);

        // TODO enable after L release to AOSP
        //@Override
        CARAPI ClearClientCertPreferences(
            /* [in] */ IRunnable* onCleared);

        // @Override
        CARAPI FreeMemoryForTests();

        // TODO: Add @Override.
        CARAPI EnableSlowWholeDocumentDraw();

        // @Override
        CARAPI_(AutoPtr< ArrayOf<IUri*> >) ParseFileChooserResult(
            /* [in] */ Int32 resultCode,
            /* [in] */ IIntent* intent);

    private:
        WebViewChromiumFactoryProvider* mOwner;
    };

public:
    CAR_INTERFACE_DECL()

    WebViewChromiumFactoryProvider();

    virtual CARAPI_(Boolean) HasStarted();

    virtual CARAPI StartYourEngines(
        /* [in] */ Boolean onMainThread);

    virtual CARAPI_(AutoPtr<AwBrowserContext>) GetBrowserContext();

    // @Override
    CARAPI_(AutoPtr<IInterface/*IWebViewFactoryProvider::IStatics*/>) GetStatics();

    // @Override
    CARAPI_(AutoPtr<IInterface/*IWebViewProvider*/>) CreateWebView(
        /* [in] */ IInterface/*WebView*/* webView,
        /* [in] */ IInterface/*WebView::PrivateAccess*/* privateAccess);

    // @Override
    CARAPI_(AutoPtr<IInterface/*IGeolocationPermissions*/>) GetGeolocationPermissions();

    // @Override
    CARAPI_(AutoPtr<IInterface/*ICookieManager*/>) GetCookieManager();

    // @Override
    CARAPI_(AutoPtr<IInterface/*IWebIconDatabase*/>) GetWebIconDatabase();

    // @Override
    CARAPI_(AutoPtr<IInterface/*IWebStorage*/>) GetWebStorage();

    // @Override
    CARAPI_(AutoPtr<IInterface/*IWebIconDatabase*/>) GetWebViewDatabase(
        /* [in] */ IContext* context);

private:
    CARAPI InitPlatSupportLibrary();

    static CARAPI InitTraceEvent();

    static CARAPI SyncATraceState();

    CARAPI EnsureChromiumStartedLocked(
        /* [in] */ Boolean onMainThread);

    CARAPI StartChromiumLocked();

    CARAPI_(AutoPtr<AwBrowserContext>) GetBrowserContextLocked();

    CARAPI SetWebContentsDebuggingEnabled(
        /* [in] */ Boolean enable);

    CARAPI SetUpResources(
        /* [in] */ IContext* ctx);

private:
    static const String TAG;
    static const String CHROMIUM_PREFS_NAME;
    static const String VERSION_CODE_PREF;
    static const String COMMAND_LINE_FILE;
    // Guards accees to the other members, and is notifyAll() signalled on the UI thread
    // when the chromium process has been started.
    /*const*/ AutoPtr<Object> mLock;
    // Initialization guarded by mLock.
    AutoPtr<AwBrowserContext> mBrowserContext;
    AutoPtr<IInterface/*IWebViewFactoryProvider::IStatics*/> mStaticMethods;
    AutoPtr<GeolocationPermissionsAdapter> mGeolocationPermissions;
    AutoPtr<CookieManagerAdapter> mCookieManager;
    AutoPtr<WebIconDatabaseAdapter> mWebIconDatabase;
    AutoPtr<WebStorageAdapter> mWebStorage;
    AutoPtr<WebViewDatabaseAdapter> mWebViewDatabase;
    AutoPtr<AwDevToolsServer> mDevToolsServer;
    AutoPtr<IList> mWebViewsToStart;
    // Read/write protected by mLock.
    Boolean mStarted;
    AutoPtr<ISharedPreferences> mWebViewPrefs;
};

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_WEBVIEWCHROMIUMFACTORYPROVIDER_H_

