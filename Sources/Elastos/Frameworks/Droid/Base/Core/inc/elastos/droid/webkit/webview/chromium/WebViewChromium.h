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

#ifndef _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_WEBVIEWCHROMIUM_H_
#define _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_WEBVIEWCHROMIUM_H_

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/utility/concurrent/FutureTask.h"
//#include "elastos/droid/webkit/WebView.h"
//#include "elastos/droid/webkit/WebSettings.h"
#include "elastos/droid/webkit/native/content/browser/SmartClipProvider.h"
#include "elastos/droid/webkit/native/content/browser/LoadUrlParams.h"
#include "elastos/droid/webkit/native/android_webview/AwContents.h"
//#include "elastos/droid/webkit/webview/chromium/WebViewChromiumFactoryProvider.h"
//#include "elastos/droid/webkit/webview/chromium/WebViewContentsClientAdapter.h"
#include "elastos/droid/webkit/webview/chromium/ContentSettingsAdapter.h"
#include "elastos/droid/webkit/webview/chromium/DrawGLFunctor.h"

// package com.android.webview.chromium;
// import android.content.Context;
// import android.content.ContextWrapper;
// import android.content.pm.PackageManager;
// import android.content.res.Configuration;
// import android.content.res.Resources;
// import android.graphics.Bitmap;
// import android.graphics.Canvas;
// import android.graphics.Paint;
// import android.graphics.Picture;
// import android.graphics.Rect;
// import android.graphics.drawable.Drawable;
// import android.net.Uri;
// import android.net.http.SslCertificate;
// import android.os.Build;
// import android.os.Bundle;
// import android.os.Looper;
// import android.os.Handler;
// import android.os.Message;
// import android.print.PrintDocumentAdapter;
// import android.text.TextUtils;
// import android.util.Base64;
// import android.util.Log;
// import android.view.HardwareCanvas;
// import android.view.KeyEvent;
// import android.view.LayoutInflater;
// import android.view.MotionEvent;
// import android.view.View;
// import android.view.View.MeasureSpec;
// import android.view.ViewGroup;
// import android.view.accessibility.AccessibilityEvent;
// import android.view.accessibility.AccessibilityNodeInfo;
// import android.view.accessibility.AccessibilityNodeProvider;
// import android.view.inputmethod.EditorInfo;
// import android.view.inputmethod.InputConnection;
// import android.webkit.DownloadListener;
// import android.webkit.FindActionModeCallback;
// import android.webkit.JavascriptInterface;
// import android.webkit.ValueCallback;
// import android.webkit.WebBackForwardList;
// import android.webkit.WebChromeClient;
// import android.webkit.WebSettings;
// import android.webkit.WebView;
// import android.webkit.WebViewClient;
// import android.webkit.WebViewFactory;
// import android.webkit.WebViewProvider;
// import android.webkit.WebChromeClient.CustomViewCallback;
// import android.widget.TextView;
// import org.chromium.android_webview.AwBrowserContext;
// import org.chromium.android_webview.AwContents;
// import org.chromium.android_webview.AwContentsStatics;
// import org.chromium.android_webview.AwLayoutSizer;
// import org.chromium.android_webview.AwSettings;
// import org.chromium.android_webview.AwPrintDocumentAdapter;
// import org.chromium.base.ThreadUtils;
// import org.chromium.content.browser.LoadUrlParams;
// import org.chromium.content.browser.SmartClipProvider;
// import org.chromium.net.NetworkChangeNotifier;
// import java.io.BufferedWriter;
// import java.io.File;
// import java.lang.annotation.Annotation;
// import java.util.concurrent.Callable;
// import java.util.concurrent.ConcurrentLinkedQueue;
// import java.util.concurrent.FutureTask;
// import java.util.concurrent.TimeUnit;
// import java.util.HashMap;
// import java.util.Map;
// import java.util.Queue;

using Elastos::Core::IRunnable;
using Elastos::Core::IClassLoader;
using Elastos::Core::IInteger32;
using Elastos::IO::IFile;
using Elastos::IO::IBufferedWriter;
using Elastos::Utility::IQueue;
using Elastos::Utility::IMap;
using Elastos::Utility::Concurrent::ICallable;
using Elastos::Utility::Concurrent::FutureTask;
using Elastos::Droid::Content::IContextWrapper;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Graphics::IPicture;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeProvider;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::InputMethod::IInputConnection;
using Elastos::Droid::View::InputMethod::IEditorInfo;
using Elastos::Droid::Net::Http::ISslCertificate;
//using Elastos::Droid::Print::IPrintDocumentAdapter;
//using Elastos::Droid::Webkit::IWebViewProvider;
//using Elastos::Droid::Webkit::IWebChromeClient;
//using Elastos::Droid::Webkit::IWebViewProviderScrollDelegate;
//using Elastos::Droid::Webkit::IWebViewProviderViewDelegate;
//using Elastos::Droid::Webkit::IWebBackForwardList;
//using Elastos::Droid::Webkit::IWebViewClient;
//using Elastos::Droid::Webkit::IDownloadListener;
//using Elastos::Droid::Webkit::WebView;
//using Elastos::Droid::Webkit::WebSettings;
using Elastos::Droid::Webkit::Content::Browser::SmartClipProvider;
using Elastos::Droid::Webkit::Content::Browser::LoadUrlParams;
//using Elastos::Droid::Webkit::Webview::Chromium::WebViewChromiumFactoryProvider;
//using Elastos::Droid::Webkit::Webview::Chromium::WebViewContentsClientAdapter;
using Elastos::Droid::Webkit::Webview::Chromium::ContentSettingsAdapter;
using Elastos::Droid::Webkit::Webview::Chromium::DrawGLFunctor;
using Elastos::Droid::Webkit::AndroidWebview::AwContents;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

/**
  * This class is the delegate to which WebViewProxy forwards all API calls.
  *
  * Most of the actual functionality is implemented by AwContents (or ContentViewCore within
  * it). This class also contains WebView-specific APIs that require the creation of other
  * adapters (otherwise org.chromium.content would depend on the webview.chromium package)
  * and a small set of no-op deprecated APIs.
  */
class WebViewChromium
    : public SmartClipProvider
    //, public IWebViewProvider
    //, public IWebViewProviderScrollDelegate
    //, public IWebViewProviderViewDelegate
{
private:
    class WebViewChromiumRunQueue : public Object
    {
    private:
        class InnerDrainQueueRunnable
            : public Object
            , public IRunnable
        {
        public:
            CAR_INTERFACE_DECL()

            InnerDrainQueueRunnable(
                /* [in] */ WebViewChromiumRunQueue* owner);

            // @Override
            CARAPI Run();

        private:
            WebViewChromiumRunQueue* mOwner;
        };

    public:
        WebViewChromiumRunQueue(
            /* [in] */ WebViewChromium* owner);

        virtual CARAPI AddTask(
            /* [in] */ IRunnable* task);

        virtual CARAPI DrainQueue();

    private:
        WebViewChromium* mOwner;
        AutoPtr<IQueue> mQueue;
    };

    class InnerInitForRealRunnable
        : public Object
        , public IRunnable
    {
    public:
        CAR_INTERFACE_DECL()

        InnerInitForRealRunnable(
            /* [in] */ WebViewChromium* owner,
            /* [in] */ const Boolean privateBrowsing);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
        const Boolean mPrivateBrowsing;
    };

    class InnerContextWrapper
        : public Object
        , public IContextWrapper
    {
    private:
        class InnerClassLoader
            : public Object
            , public IClassLoader
        {
        public:
            InnerClassLoader(
                /* [in] */ InnerContextWrapper* owner);

        protected:
            // @Override
            CARAPI FindClass(
                /* [in] */ const String& name,
                /* [out] */ IClassInfo** klass);

        private:
            InnerContextWrapper* mOwner;
        };

    public:
        InnerContextWrapper(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI_(AutoPtr<IClassLoader>) GetClassLoader();

        // @Override
        CARAPI GetSystemService(
            /* [in] */ const String& name,
            /* [out] */ IInterface** object);

    private:
        WebViewChromium* mOwner;
    };

    class InnerThreadViolationRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerThreadViolationRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerSetHorizontalScrollbarOverlayRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerSetHorizontalScrollbarOverlayRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerSetVerticalScrollbarOverlayRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerSetVerticalScrollbarOverlayRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerOverlayHorizontalScrollbarCallable
        : public Object
        , public ICallable
    {
    public:
        InnerOverlayHorizontalScrollbarCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerOverlayVerticalScrollbarCallable
        : public Object
        , public ICallable
    {
    public:
        InnerOverlayVerticalScrollbarCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerGetCertificateCallable
        : public Object
        , public ICallable
    {
    public:
        InnerGetCertificateCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerSetHttpAuthUsernamePasswordRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerSetHttpAuthUsernamePasswordRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerGetHttpAuthUsernamePasswordCallable
        : public Object
        , public ICallable
    {
    public:
        InnerGetHttpAuthUsernamePasswordCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerDestroyRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerDestroyRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerSetNetworkAvailableRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerSetNetworkAvailableRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerSaveStateCallable
        : public Object
        , public ICallable
    {
    public:
        InnerSaveStateCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerRestoreStateCallable
        : public Object
        , public ICallable
    {
    public:
        InnerRestoreStateCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerEvaluateJavaScriptRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerEvaluateJavaScriptRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerLoadUrlRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerLoadUrlRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerSaveWebArchiveRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerSaveWebArchiveRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerStopLoadingRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerStopLoadingRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerReloadRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerReloadRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerCanGoBackCallable
        : public Object
        , public ICallable
    {
    public:
        InnerCanGoBackCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerGoBackRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerGoBackRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerCanGoForwardCallable
        : public Object
        , public ICallable
    {
    public:
        InnerCanGoForwardCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerGoForwardRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerGoForwardRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerCanGoBackOrForwardCallable
        : public Object
        , public ICallable
    {
    public:
        InnerCanGoBackOrForwardCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerGoBackOrForwardRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerGoBackOrForwardRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerPageUpCallable
        : public Object
        , public ICallable
    {
    public:
        InnerPageUpCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerPageDownCallable
        : public Object
        , public ICallable
    {
    public:
        InnerPageDownCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerClearViewRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerClearViewRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerCapturePictureCallable
        : public Object
        , public ICallable
    {
    public:
        InnerCapturePictureCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerInvokeZoomPickerRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerInvokeZoomPickerRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerGetHitTestResultCallable
        : public Object
        , public ICallable
    {
    public:
        InnerGetHitTestResultCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerRequestFocusNodeHrefRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerRequestFocusNodeHrefRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerRequestImageRefRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerRequestImageRefRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerGetUrlCallable
        : public Object
        , public ICallable
    {
    public:
        InnerGetUrlCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerGetOriginalUrlCallable
        : public Object
        , public ICallable
    {
    public:
        InnerGetOriginalUrlCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerGetTitleCallable
        : public Object
        , public ICallable
    {
    public:
        InnerGetTitleCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerGetFaviconCallable
        : public Object
        , public ICallable
    {
    public:
        InnerGetFaviconCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerPauseTimersRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerPauseTimersRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerResumeTimersRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerResumeTimersRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnPauseRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnPauseRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnResumeRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnResumeRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerIsPausedCallable
        : public Object
        , public ICallable
    {
    public:
        InnerIsPausedCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerClearCacheRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerClearCacheRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerClearFormDataRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerClearFormDataRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerClearHistoryRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerClearHistoryRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerClearSslPreferencesRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerClearSslPreferencesRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerCopyBackForwardListCallable
        : public Object
        , public ICallable
    {
    public:
        InnerCopyBackForwardListCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerFindNextRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerFindNextRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerFindAllAsyncRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerFindAllAsyncRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerNotifyFindDialogDismissedRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerNotifyFindDialogDismissedRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerClearMatchesRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerClearMatchesRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerDocumentHasImagesRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerDocumentHasImagesRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerSetPictureListenerRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerSetPictureListenerRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerAddJavascriptInterfaceRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerAddJavascriptInterfaceRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerRemoveJavascriptInterfaceRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerRemoveJavascriptInterfaceRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerFlingScrollRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerFlingScrollRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerZoomInCallable
        : public Object
        , public ICallable
    {
    public:
        InnerZoomInCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerZoomOutCallable
        : public Object
        , public ICallable
    {
    public:
        InnerZoomOutCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerShouldDelayChildPressedStateCallable
        : public Object
        , public ICallable
    {
    public:
        InnerShouldDelayChildPressedStateCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerGetAccessibilityNodeProviderCallable
        : public Object
        , public ICallable
    {
    public:
        InnerGetAccessibilityNodeProviderCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnInitializeAccessibilityNodeInfoRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnInitializeAccessibilityNodeInfoRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnInitializeAccessibilityEventRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnInitializeAccessibilityEventRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerPerformAccessibilityActionCallable
        : public Object
        , public ICallable
    {
    public:
        InnerPerformAccessibilityActionCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerSetOverScrollModeRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerSetOverScrollModeRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerSetScrollBarStyleRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerSetScrollBarStyleRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnOverScrolledRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnOverScrolledRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnWindowVisibilityChangedRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnWindowVisibilityChangedRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnDrawRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnDrawRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnConfigurationChangedRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnConfigurationChangedRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnKeyMultipleCallable
        : public Object
        , public ICallable
    {
    public:
        InnerOnKeyMultipleCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnKeyDownCallable
        : public Object
        , public ICallable
    {
    public:
        InnerOnKeyDownCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnKeyUpCallable
        : public Object
        , public ICallable
    {
    public:
        InnerOnKeyUpCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnDetachedFromWindowRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnDetachedFromWindowRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnVisibilityChangedRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnVisibilityChangedRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnWindowFocusChangedRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnWindowFocusChangedRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnFocusChangedRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnFocusChangedRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnSizeChangedRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnSizeChangedRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerDispatchKeyEventCallable
        : public Object
        , public ICallable
    {
    public:
        InnerDispatchKeyEventCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnTouchEventCallable
        : public Object
        , public ICallable
    {
    public:
        InnerOnTouchEventCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnHoverEventCallable
        : public Object
        , public ICallable
    {
    public:
        InnerOnHoverEventCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnGenericMotionEventCallable
        : public Object
        , public ICallable
    {
    public:
        InnerOnGenericMotionEventCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerRequestFocusCallable
        : public Object
        , public ICallable
    {
    public:
        InnerRequestFocusCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerOnMeasureRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerOnMeasureRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerRequestChildRectangleOnScreenCallable
        : public Object
        , public ICallable
    {
    public:
        InnerRequestChildRectangleOnScreenCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerSetBackgroundColorRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerSetBackgroundColorRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerSetLayerTypeRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerSetLayerTypeRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    class InnerComputeHorizontalScrollRangeCallable
        : public Object
        , public ICallable
    {
    public:
        InnerComputeHorizontalScrollRangeCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerComputeHorizontalScrollOffsetCallable
        : public Object
        , public ICallable
    {
    public:
        InnerComputeHorizontalScrollOffsetCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerComputeVerticalScrollRangeCallable
        : public Object
        , public ICallable
    {
    public:
        InnerComputeVerticalScrollRangeCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerComputeVerticalScrollOffsetCallable
        : public Object
        , public ICallable
    {
    public:
        InnerComputeVerticalScrollOffsetCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerComputeVerticalScrollExtentCallable
        : public Object
        , public ICallable
    {
    public:
        InnerComputeVerticalScrollExtentCallable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Call(
            /* [out] */ IInterface** result);

    private:
        WebViewChromium* mOwner;
    };

    class InnerComputeScrollRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerComputeScrollRunnable(
            /* [in] */ WebViewChromium* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewChromium* mOwner;
    };

    // AwContents.NativeGLDelegate implementation --------------------------------------
    class WebViewNativeGLDelegate : public AwContents::NativeGLDelegate
    {
    public:
        // @Override
        CARAPI_(Boolean) RequestDrawGL(
            /* [in] */ ICanvas* canvas,
            /* [in] */ Boolean waitForCompletion,
            /* [in] */ IView* containerView);

        // @Override
        CARAPI_(void) DetachGLFunctor();
    };

    // AwContents.InternalAccessDelegate implementation --------------------------------------
    class InternalAccessAdapter : public AwContents::InternalAccessDelegate
    {
    public:
        // @Override
        CARAPI_(Boolean) DrawChild(
            /* [in] */ ICanvas* arg0,
            /* [in] */ IView* arg1,
            /* [in] */ Int64 arg2);

        // @Override
        CARAPI_(Boolean) Super_onKeyUp(
            /* [in] */ Int32 arg0,
            /* [in] */ IKeyEvent* arg1);

        // @Override
        CARAPI_(Boolean) Super_dispatchKeyEventPreIme(
            /* [in] */ IKeyEvent* arg0);

        // @Override
        CARAPI_(Boolean) Super_dispatchKeyEvent(
            /* [in] */ IKeyEvent* event);

        // @Override
        CARAPI_(Boolean) Super_onGenericMotionEvent(
            /* [in] */ IMotionEvent* arg0);

        // @Override
        CARAPI_(void) Super_onConfigurationChanged(
            /* [in] */ IConfiguration* arg0);

        // @Override
        CARAPI_(Int32) Super_getScrollBarStyle();

        // @Override
        CARAPI_(Boolean) AwakenScrollBars();

        // @Override
        CARAPI_(Boolean) Super_awakenScrollBars(
            /* [in] */ Int32 arg0,
            /* [in] */ Boolean arg1);

        // @Override
        CARAPI_(void) OnScrollChanged(
            /* [in] */ Int32 l,
            /* [in] */ Int32 t,
            /* [in] */ Int32 oldl,
            /* [in] */ Int32 oldt);

        // @Override
        CARAPI_(void) OverScrollBy(
            /* [in] */ Int32 deltaX,
            /* [in] */ Int32 deltaY,
            /* [in] */ Int32 scrollX,
            /* [in] */ Int32 scrollY,
            /* [in] */ Int32 scrollRangeX,
            /* [in] */ Int32 scrollRangeY,
            /* [in] */ Int32 maxOverScrollX,
            /* [in] */ Int32 maxOverScrollY,
            /* [in] */ Boolean isTouchEvent);

        // @Override
        CARAPI_(void) Super_scrollTo(
            /* [in] */ Int32 scrollX,
            /* [in] */ Int32 scrollY);

        // @Override
        CARAPI_(void) SetMeasuredDimension(
            /* [in] */ Int32 measuredWidth,
            /* [in] */ Int32 measuredHeight);

        // @Override
        CARAPI_(Boolean) Super_onHoverEvent(
            /* [in] */ IMotionEvent* event);
    };

public:
    // This does not touch any global / non-threadsafe state, but note that
    // init is ofter called right after and is NOT threadsafe.
    WebViewChromium(
        /* [in] */ IInterface/*WebViewChromiumFactoryProvider*/* factory,
        /* [in] */ IInterface/*WebView*/* webView,
        /* [in] */ IInterface/*WebView::PrivateAccess*/* webViewPrivate);

    static CARAPI EnableSlowWholeDocumentDraw();

    static CARAPI CompleteWindowCreation(
        /* [in] */ IInterface/*WebView*/* parent,
        /* [in] */ IInterface/*WebView*/* child);

    // WebViewProvider methods --------------------------------------------------------------------
    // @Override
    // BUG=6790250 |javaScriptInterfaces| was only ever used by the obsolete DumpRenderTree
    // so is ignored. TODO: remove it from WebViewProvider.
    virtual CARAPI Init(
        /* [in] */ const IMap* javaScriptInterfaces,
        /* [in] */ const Boolean& privateBrowsing);

    virtual CARAPI StartYourEngine();

    // @Override
    CARAPI SetHorizontalScrollbarOverlay(
        /* [in] */ const Boolean& overlay);

    // @Override
    CARAPI SetVerticalScrollbarOverlay(
        /* [in] */ const Boolean& overlay);

    // @Override
    CARAPI_(Boolean) OverlayHorizontalScrollbar();

    // @Override
    CARAPI_(Boolean) OverlayVerticalScrollbar();

    // @Override
    CARAPI_(Int32) GetVisibleTitleHeight();

    // @Override
    CARAPI_(AutoPtr<ISslCertificate>) GetCertificate();

    // @Override
    CARAPI SetCertificate(
        /* [in] */ ISslCertificate* certificate);

    // @Override
    CARAPI SavePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    // @Override
    CARAPI SetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    // @Override
    CARAPI_(AutoPtr< ArrayOf<String> >) GetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& realm);

    // @Override
    CARAPI Destroy();

    // @Override
    CARAPI SetNetworkAvailable(
        /* [in] */ const Boolean& networkUp);

    // @Override
    CARAPI_(AutoPtr<IInterface/*IWebBackForwardList*/>) SaveState(
        /* [in] */ const IBundle* outState);

    // @Override
    CARAPI_(Boolean) SavePicture(
        /* [in] */ IBundle* b,
        /* [in] */ IFile* dest);

    // @Override
    CARAPI_(Boolean) RestorePicture(
        /* [in] */ IBundle* b,
        /* [in] */ IFile* src);

    // @Override
    CARAPI_(AutoPtr<IInterface/*IWebBackForwardList*/>) RestoreState(
        /* [in] */ const IBundle* inState);

    // @Override
    CARAPI LoadUrl(
        /* [in] */ const String& url,
        /* [in] */ IMap* additionalHttpHeaders);

    // @Override
    CARAPI LoadUrl(
        /* [in] */ const String& url);

    // @Override
    CARAPI PostUrl(
        /* [in] */ const String& url,
        /* [in] */ ArrayOf<Byte>* postData);

    // @Override
    CARAPI LoadData(
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding);

    // @Override
    CARAPI LoadDataWithBaseURL(
        /* [in] */ const String& baseUrl,
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ const String& historyUrl);

    virtual CARAPI EvaluateJavaScript(
        /* [in] */ const String& script,
        /* [in] */ IInterface/*IValueCallback*/* resultCallback);

    // @Override
    CARAPI SaveWebArchive(
        /* [in] */ const String& filename);

    // @Override
    CARAPI SaveWebArchive(
        /* [in] */ const String& basename,
        /* [in] */ const Boolean& autoname,
        /* [in] */ const IInterface/*IValueCallback*/* callback);

    // @Override
    CARAPI StopLoading();

    // @Override
    CARAPI Reload();

    // @Override
    CARAPI_(Boolean) CanGoBack();

    // @Override
    CARAPI GoBack();

    // @Override
    CARAPI_(Boolean) CanGoForward();

    // @Override
    CARAPI GoForward();

    // @Override
    CARAPI_(Boolean) CanGoBackOrForward(
        /* [in] */ const Int32& steps);

    // @Override
    CARAPI GoBackOrForward(
        /* [in] */ const Int32& steps);

    // @Override
    CARAPI_(Boolean) IsPrivateBrowsingEnabled();

    // @Override
    CARAPI_(Boolean) PageUp(
        /* [in] */ const Boolean& top);

    // @Override
    CARAPI_(Boolean) PageDown(
        /* [in] */ const Boolean& bottom);

    // @Override
    CARAPI ClearView();

    // @Override
    CARAPI_(AutoPtr<IPicture>) CapturePicture();

    // @Override
    CARAPI_(Float) GetScale();

    // @Override
    CARAPI SetInitialScale(
        /* [in] */ const Int32& scaleInPercent);

    // @Override
    CARAPI InvokeZoomPicker();

    // @Override
    CARAPI_(AutoPtr<IInterface/*WebView::HitTestResult*/>) GetHitTestResult();

    // @Override
    CARAPI RequestFocusNodeHref(
        /* [in] */ const IMessage* hrefMsg);

    // @Override
    CARAPI RequestImageRef(
        /* [in] */ const IMessage* msg);

    // @Override
    CARAPI_(String) GetUrl();

    // @Override
    CARAPI_(String) GetOriginalUrl();

    // @Override
    CARAPI_(String) GetTitle();

    // @Override
    CARAPI_(AutoPtr<IBitmap>) GetFavicon();

    // @Override
    CARAPI_(String) GetTouchIconUrl();

    // @Override
    CARAPI_(Int32) GetProgress();

    // @Override
    CARAPI_(Int32) GetContentHeight();

    // @Override
    CARAPI_(Int32) GetContentWidth();

    // @Override
    CARAPI PauseTimers();

    // @Override
    CARAPI ResumeTimers();

    // @Override
    CARAPI OnPause();

    // @Override
    CARAPI OnResume();

    // @Override
    CARAPI_(Boolean) IsPaused();

    // @Override
    CARAPI FreeMemory();

    // @Override
    CARAPI ClearCache(
        /* [in] */ const Boolean& includeDiskFiles);

    /**
      * This is a poorly named method, but we keep it for historical reasons.
      */
    // @Override
    CARAPI ClearFormData();

    // @Override
    CARAPI ClearHistory();

    // @Override
    CARAPI ClearSslPreferences();

    // @Override
    CARAPI_(AutoPtr<IInterface/*IWebBackForwardList*/>) CopyBackForwardList();

    // @Override
    CARAPI SetFindListener(
        /* [in] */ IInterface/*WebView::FindListener*/* listener);

    // @Override
    CARAPI FindNext(
        /* [in] */ const Boolean& forwards);

    // @Override
    CARAPI_(Int32) FindAll(
        /* [in] */ const String& searchString);

    // @Override
    CARAPI FindAllAsync(
        /* [in] */ const String& searchString);

    // @Override
    CARAPI_(Boolean) ShowFindDialog(
        /* [in] */ const String& text,
        /* [in] */ const Boolean& showIme);

    // @Override
    CARAPI NotifyFindDialogDismissed();

    // @Override
    CARAPI ClearMatches();

    // @Override
    CARAPI DocumentHasImages(
        /* [in] */ const IMessage* response);

    // @Override
    CARAPI SetWebViewClient(
        /* [in] */ IInterface/*IWebViewClient*/* client);

    // @Override
    CARAPI SetDownloadListener(
        /* [in] */ IInterface/*IDownloadListener*/* listener);

    // @Override
    CARAPI SetWebChromeClient(
        /* [in] */ IInterface/*WebChromeClient*/* client);

    // @Override
    CARAPI SetPictureListener(
        /* [in] */ const IInterface/*WebView::PictureListener*/* listener);

    // @Override
    CARAPI AddJavascriptInterface(
        /* [in] */ const Object* obj,
        /* [in] */ const String& interfaceName);

    // @Override
    CARAPI RemoveJavascriptInterface(
        /* [in] */ const String& interfaceName);

    // @Override
    CARAPI_(AutoPtr<IInterface/*WebSettings*/>) GetSettings();

    // @Override
    CARAPI SetMapTrackballToArrowKeys(
        /* [in] */ Boolean setMap);

    // @Override
    CARAPI FlingScroll(
        /* [in] */ const Int32& vx,
        /* [in] */ const Int32& vy);

    // @Override
    CARAPI_(AutoPtr<IView>) GetZoomControls();

    // @Override
    CARAPI_(Boolean) CanZoomIn();

    // @Override
    CARAPI_(Boolean) CanZoomOut();

    // @Override
    CARAPI_(Boolean) ZoomIn();

    // @Override
    CARAPI_(Boolean) ZoomOut();

    // @Override
    CARAPI_(Boolean) ZoomBy(
        /* [in] */ Float factor);

    // @Override
    CARAPI DumpViewHierarchyWithProperties(
        /* [in] */ IBufferedWriter* out,
        /* [in] */ Int32 level);

    // @Override
    CARAPI_(AutoPtr<IView>) FindHierarchyView(
        /* [in] */ const String& className,
        /* [in] */ Int32 hashCode);

    // WebViewProvider glue methods ---------------------------------------------------------------
    // @Override
    // This needs to be kept thread safe!
    virtual CARAPI_(AutoPtr<IInterface/*IWebViewProviderViewDelegate*/>) GetViewDelegate();

    // @Override
    // This needs to be kept thread safe!
    virtual CARAPI_(AutoPtr<IInterface/*IWebViewProviderScrollDelegate*/>) GetScrollDelegate();

    // WebViewProvider.ViewDelegate implementation ------------------------------------------------
    // TODO: remove from WebViewProvider and use default implementation from
    // ViewGroup.
    // @Override
    virtual CARAPI_(Boolean) ShouldDelayChildPressedState();

    // @Override
    virtual CARAPI_(AutoPtr<IAccessibilityNodeProvider>) GetAccessibilityNodeProvider();

    // @Override
    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ const IAccessibilityNodeInfo* info);

    // @Override
    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ const IAccessibilityEvent* event);

    // @Override
    CARAPI_(Boolean) PerformAccessibilityAction(
        /* [in] */ const Int32& action,
        /* [in] */ const IBundle* arguments);

    // @Override
    CARAPI SetOverScrollMode(
        /* [in] */ const Int32& mode);

    // @Override
    CARAPI SetScrollBarStyle(
        /* [in] */ const Int32& style);

    // @Override
    CARAPI OnDrawVerticalScrollBar(
        /* [in] */ const ICanvas* canvas,
        /* [in] */ const IDrawable* scrollBar,
        /* [in] */ const Int32& l,
        /* [in] */ const Int32& t,
        /* [in] */ const Int32& r,
        /* [in] */ const Int32& b);

    // @Override
    CARAPI OnOverScrolled(
        /* [in] */ const Int32& scrollX,
        /* [in] */ const Int32& scrollY,
        /* [in] */ const Boolean& clampedX,
        /* [in] */ const Boolean& clampedY);

    // @Override
    CARAPI OnWindowVisibilityChanged(
        /* [in] */ const Int32& visibility);

    // @Override
    CARAPI OnDraw(
        /* [in] */ const ICanvas* canvas);

    // @Override
    CARAPI SetLayoutParams(
        /* [in] */ const IViewGroupLayoutParams* layoutParams);

    // @Override
    CARAPI_(Boolean) PerformLongClick();

    // @Override
    CARAPI OnConfigurationChanged(
        /* [in] */ const IConfiguration* newConfig);

    // @Override
    CARAPI_(AutoPtr<IInputConnection>) OnCreateInputConnection(
        /* [in] */ const IEditorInfo* outAttrs);

    // @Override
    CARAPI_(Boolean) OnKeyMultiple(
        /* [in] */ const Int32& keyCode,
        /* [in] */ const Int32& repeatCount,
        /* [in] */ const IKeyEvent* event);

    // @Override
    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ const Int32& keyCode,
        /* [in] */ const IKeyEvent* event);

    // @Override
    CARAPI_(Boolean) OnKeyUp(
        /* [in] */ const Int32& keyCode,
        /* [in] */ const IKeyEvent* event);

    // @Override
    CARAPI OnAttachedToWindow();

    // @Override
    CARAPI OnDetachedFromWindow();

    // @Override
    CARAPI OnVisibilityChanged(
        /* [in] */ const IView* changedView,
        /* [in] */ const Int32& visibility);

    // @Override
    CARAPI OnWindowFocusChanged(
        /* [in] */ const Boolean& hasWindowFocus);

    // @Override
    CARAPI OnFocusChanged(
        /* [in] */ const Boolean& focused,
        /* [in] */ const Int32& direction,
        /* [in] */ const IRect* previouslyFocusedRect);

    // @Override
    CARAPI_(Boolean) SetFrame(
        /* [in] */ const Int32& left,
        /* [in] */ const Int32& top,
        /* [in] */ const Int32& right,
        /* [in] */ const Int32& bottom);

    // @Override
    CARAPI OnSizeChanged(
        /* [in] */ const Int32& w,
        /* [in] */ const Int32& h,
        /* [in] */ const Int32& ow,
        /* [in] */ const Int32& oh);

    // @Override
    CARAPI OnScrollChanged(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 oldl,
        /* [in] */ Int32 oldt);

    // @Override
    CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ const IKeyEvent* event);

    // @Override
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ const IMotionEvent* ev);

    // @Override
    CARAPI_(Boolean) OnHoverEvent(
        /* [in] */ const IMotionEvent* event);

    // @Override
    CARAPI_(Boolean) OnGenericMotionEvent(
        /* [in] */ const IMotionEvent* event);

    // @Override
    CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* ev);

    // @Override
    CARAPI_(Boolean) RequestFocus(
        /* [in] */ const Int32& direction,
        /* [in] */ const IRect* previouslyFocusedRect);

    // @Override
    CARAPI OnMeasure(
        /* [in] */ const Int32& widthMeasureSpec,
        /* [in] */ const Int32& heightMeasureSpec);

    // @Override
    CARAPI_(Boolean) RequestChildRectangleOnScreen(
        /* [in] */ const IView* child,
        /* [in] */ const IRect* rect,
        /* [in] */ const Boolean& immediate);

    // @Override
    CARAPI SetBackgroundColor(
        /* [in] */ const Int32& color);

    // @Override
    CARAPI SetLayerType(
        /* [in] */ const Int32& layerType,
        /* [in] */ const IPaint* paint);

    // Remove from superclass
    virtual CARAPI PreDispatchDraw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI OnStartTemporaryDetach();

    virtual CARAPI OnFinishTemporaryDetach();

    // WebViewProvider.ScrollDelegate implementation ----------------------------------------------
    // @Override
    CARAPI_(Int32) ComputeHorizontalScrollRange();

    // @Override
    CARAPI_(Int32) ComputeHorizontalScrollOffset();

    // @Override
    CARAPI_(Int32) ComputeVerticalScrollRange();

    // @Override
    CARAPI_(Int32) ComputeVerticalScrollOffset();

    // @Override
    CARAPI_(Int32) ComputeVerticalScrollExtent();

    // @Override
    CARAPI ComputeScroll();

    // TODO(sgurun) this is only to have master-gpl compiling.
    virtual CARAPI_(AutoPtr<IInterface/*IPrintDocumentAdapter*/>) CreatePrintDocumentAdapter();

    //@Override TODO(sgurun) commenting this out to have master-gpl compiling.
    virtual CARAPI_(AutoPtr<IInterface/*IPrintDocumentAdapter*/>) CreatePrintDocumentAdapter(
        /* [in] */ const String& documentName);

    // Implements SmartClipProvider
    // @Override
    CARAPI ExtractSmartClipData(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    // Implements SmartClipProvider
    // @Override
    CARAPI SetSmartClipResultHandler(
        /* [in] */ const IHandler* resultHandler);

public:
    // The WebView that this WebViewChromium is the provider for.
    AutoPtr<IInterface/*IWebView*/> mWebView;
    // Lets us access protected View-derived methods on the WebView instance we're backing.
    AutoPtr<IInterface/*IWebViewPrivateAccess*/> mWebViewPrivate;

private:
    CARAPI_(AutoPtr<IInterface>) RunBlockingFuture(
        /* [in] */ FutureTask* task);

    // We have a 4 second timeout to try to detect deadlocks to detect and aid in debuggin
    // deadlocks.
    // Do not call this method while on the UI thread!
    CARAPI RunVoidTaskOnUiThreadBlocking(
        /* [in] */ IRunnable* r);

    CARAPI_(AutoPtr<IInterface>) RunOnUiThreadBlocking(
        /* [in] */ ICallable* c);

    // Wrap Context so that we can use resources from the webview resource apk.
    static CARAPI_(AutoPtr<IContext>) ResourcesContextWrapper(
        /* [in] */ const IContext* ctx);

    CARAPI InitForReal();

    // exception, note it temporarily
    // CARAPI_(AutoPtr<RuntimeException>) CreateThreadException();

    CARAPI_(Boolean) CheckNeedsPost();

    //  Intentionally not static, as no need to check thread on static methods
    CARAPI CheckThread();

    static CARAPI_(String) FixupMimeType(
        /* [in] */ const String& mimeType);

    static CARAPI_(String) FixupData(
        /* [in] */ const String& data);

    static CARAPI_(String) FixupBase(
        /* [in] */ const String& url);

    static CARAPI_(String) FixupHistory(
        /* [in] */ const String& url);

    static CARAPI_(Boolean) IsBase64Encoded(
        /* [in] */ const String& encoding);

    CARAPI LoadUrlOnUiThread(
        /* [in] */ const IInterface/*LoadUrlParams*/* loadUrlParams);

    /**
     * Returns true if the supplied {@link WebChromeClient} supports fullscreen.
     *
     * <p>For fullscreen support, implementations of {@link WebChromeClient#onShowCustomView}
     * and {@link WebChromeClient#onHideCustomView()} are required.
     */
    CARAPI_(Boolean) DoesSupportFullscreen(
        /* [in] */ IInterface/*IWebChromeClient*/* client);

private:
    AutoPtr<WebViewChromiumRunQueue> mRunQueue;
    static const String TAG;
    // The client adapter class.
    AutoPtr<IInterface/*WebViewContentsClientAdapter*/> mContentsClientAdapter;
    // Variables for functionality provided by this adapter ---------------------------------------
    AutoPtr<ContentSettingsAdapter> mWebSettings;
    // The WebView wrapper for ContentViewCore and required browser compontents.
    AutoPtr<AwContents> mAwContents;
    // Non-null if this webview is using the GL accelerated draw path.
    AutoPtr<DrawGLFunctor> mGLfunctor;
    /*const*/ AutoPtr<IInterface/*WebView::HitTestResult*/> mHitTestResult;
    /*const*/ Int32 mAppTargetSdkVersion;
    AutoPtr<IInterface/*WebViewChromiumFactoryProvider*/> mFactory;
    static Boolean sRecordWholeDocumentEnabledByApi;
};

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_WEBVIEWCHROMIUM_H_

