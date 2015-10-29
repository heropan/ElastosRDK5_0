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

#ifndef _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_WEBVIEWCONTENTSCLIENTADAPTER_H_
#define _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_WEBVIEWCONTENTSCLIENTADAPTER_H_

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/webkit/native/android_webview/AwContentsClient.h"
#include "elastos/droid/webkit/native/android_webview/JsPromptResultReceiver.h"
#include "elastos/droid/webkit/native/android_webview/JsResultReceiver.h"
#include "elastos/droid/webkit/native/android_webview/AwContentsClientBridge.h"
#include "elastos/droid/webkit/native/android_webview/AwHttpAuthHandler.h"
#include "elastos/droid/webkit/native/android_webview/AwWebResourceResponse.h"
#include "elastos/droid/webkit/native/android_webview/permission/AwPermissionRequest.h"
//#include "elastos/droid/webkit/WebViewClient.h"
//#include "elastos/droid/webkit/WebResourceRequest.h"
//#include "elastos/droid/webkit/JsResult.h"
//#include "elastos/droid/webkit/JsPromptResult.h"
//#include "elastos/droid/webkit/ClientCertRequest.h"
//#include "elastos/droid/webkit/SslErrorHandler.h"
//#include "elastos/droid/webkit/PermissionRequest.h"

// package com.android.webview.chromium;
// import android.content.ActivityNotFoundException;
// import android.content.Context;
// import android.content.Intent;
// import android.graphics.Bitmap;
// import android.graphics.BitmapFactory;
// import android.graphics.Canvas;
// import android.graphics.Color;
// import android.graphics.Picture;
// import android.net.http.ErrorStrings;
// import android.net.http.SslError;
// import android.net.IUri;
// import android.os.Build;
// import android.os.IHandler;
// import android.os.Looper;
// import android.os.Message;
// import android.provider.Browser;
// import android.util.Log;
// import android.view.IKeyEvent;
// import android.view.View;
// import android.webkit.ClientCertRequest;
// import android.webkit.ConsoleMessage;
// import android.webkit.DownloadListener;
// import android.webkit.GeolocationPermissions;
// import android.webkit.JsDialogHelper;
// import android.webkit.JsPromptResult;
// import android.webkit.JsResult;
// import android.webkit.PermissionRequest;
// import android.webkit.SslErrorHandler;
// import android.webkit.ValueCallback;
// import android.webkit.WebChromeClient;
// import android.webkit.WebChromeClient.CustomViewCallback;
// import android.webkit.WebResourceResponse;
// import android.webkit.WebResourceRequest;
// import android.webkit.WebView;
// import android.webkit.WebViewClient;
// import org.chromium.android_webview.AwContentsClient;
// import org.chromium.android_webview.AwContentsClientBridge;
// import org.chromium.android_webview.AwHttpAuthHandler;
// import org.chromium.android_webview.AwWebResourceResponse;
// import org.chromium.android_webview.JsPromptResultReceiver;
// import org.chromium.android_webview.JsResultReceiver;
// import org.chromium.android_webview.permission.AwPermissionRequest;
// import org.chromium.base.ThreadUtils;
// import org.chromium.base.TraceEvent;
// import org.chromium.content.browser.ContentView;
// import org.chromium.content.browser.ContentViewClient;
// import java.lang.ref.WeakReference;
// import java.net.URISyntaxException;
// import java.security.Principal;
// import java.security.PrivateKey;
// import java.security.cert.X509Certificate;
// import java.util.ArrayList;
// import java.util.HashMap;
// import java.util.Map;
// import java.util.WeakHashMap;

using Elastos::Core::IRunnable;
using Elastos::Utility::IMap;
using Elastos::Utility::IWeakHashMap;
using Elastos::Security::IPrincipal;
using Elastos::Security::IPrivateKey;
using Elastos::Security::Cert::IX509Certificate;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Net::Http::ISslError;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Graphics::IPicture;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IView;
//using Elastos::Droid::Webkit::WebViewClient;
//using Elastos::Droid::Webkit::WebResourceRequest;
//using Elastos::Droid::Webkit::PermissionRequest;
//using Elastos::Droid::Webkit::JsResult;
//using Elastos::Droid::Webkit::JsPromptResult;
//using Elastos::Droid::Webkit::ClientCertRequest;
//using Elastos::Droid::Webkit::IHttpAuthHandler;
//using Elastos::Droid::Webkit::IDownloadListener;
//using Elastos::Droid::Webkit::IConsoleMessage;
//using Elastos::Droid::Webkit::SslErrorHandler;
//using Elastos::Droid::Webkit::IWebViewPictureListener;
//using Elastos::Droid::Webkit::IWebViewFindListener;
using Elastos::Droid::Webkit::AndroidWebview::AwContentsClient;
using Elastos::Droid::Webkit::AndroidWebview::JsPromptResultReceiver;
using Elastos::Droid::Webkit::AndroidWebview::JsResultReceiver;
using Elastos::Droid::Webkit::AndroidWebview::AwContentsClientBridge;
using Elastos::Droid::Webkit::AndroidWebview::AwHttpAuthHandler;
using Elastos::Droid::Webkit::AndroidWebview::AwWebResourceResponse;
using Elastos::Droid::Webkit::AndroidWebview::Permission::AwPermissionRequest;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

/**
  * An adapter class that forwards the callbacks from {@link ContentViewClient}
  * to the appropriate {@link WebViewClient} or {@link WebChromeClient}.
  *
  * An instance of this class is associated with one {@link WebViewChromium}
  * instance. A WebViewChromium is a WebView implementation provider (that is
  * android.webkit.WebView delegates all functionality to it) and has exactly
  * one corresponding {@link ContentView} instance.
  *
  * A {@link ContentViewClient} may be shared between multiple {@link ContentView}s,
  * and hence multiple WebViews. Many WebViewClient methods pass the source
  * WebView as an argument. This means that we either need to pass the
  * corresponding ContentView to the corresponding ContentViewClient methods,
  * or use an instance of ContentViewClientAdapter per WebViewChromium, to
  * allow the source WebView to be injected by ContentViewClientAdapter. We
  * choose the latter, because it makes for a cleaner design.
  */
class WebViewContentsClientAdapter
    : public AwContentsClient
{
public:
    // WebViewClassic is coded in such a way that even if a null WebViewClient is set,
    // certain actions take place.
    // We choose to replicate this behavior by using a NullWebViewClient implementation (also known
    // as the Null Object pattern) rather than duplicating the WebViewClassic approach in
    // ContentView.
    class NullWebViewClient
        : public Object// WebViewClient
    {
    public:
        // @Override
        CARAPI_(Boolean) ShouldOverrideKeyEvent(
            /* [in] */ IInterface/*WebView*/* view,
            /* [in] */ IKeyEvent* event);

        // @Override
        CARAPI_(Boolean) ShouldOverrideUrlLoading(
            /* [in] */ IInterface/*WebView*/* view,
            /* [in] */ const String& url);
    };

    // TODO: Move to the upstream once the PermissionRequest is part of SDK.
    class PermissionRequestAdapter
        : public Object// PermissionRequest
    {
    public:
        PermissionRequestAdapter(
            /* [in] */ AwPermissionRequest* awPermissionRequest);

        static CARAPI_(Int64) ToAwPermissionResources(
            /* [in] */ ArrayOf<String>* resources);

        // @Override
        CARAPI_(AutoPtr<IUri>) GetOrigin();

        // @Override
        CARAPI_(AutoPtr< ArrayOf<String> >) GetResources();

        // @Override
        CARAPI Grant(
            /* [in] */ ArrayOf<String>* resources);

        // @Override
        CARAPI Deny();

    private:
        static CARAPI_(AutoPtr< ArrayOf<String> >) ToPermissionResources(
            /* [in] */ Int64 resources);

    private:
        // TODO: Move the below definitions to AwPermissionRequest.
        static const Int64 BITMASK_RESOURCE_VIDEO_CAPTURE = 1 << 1;
        static const Int64 BITMASK_RESOURCE_AUDIO_CAPTURE = 1 << 2;
        static const Int64 BITMASK_RESOURCE_PROTECTED_MEDIA_ID = 1 << 3;
        AutoPtr<AwPermissionRequest> mAwPermissionRequest;
        AutoPtr< ArrayOf<String> > mResources;
    };

private:
    class InnerHandler
        : public Handler
    {
    public:
        InnerHandler(
            /* [in] */ WebViewContentsClientAdapter* owner);

        // @Override
        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        WebViewContentsClientAdapter* mOwner;
    };

    class WebResourceRequestImpl
        : public Object
        //, public WebResourceRequest
    {
    public:
        WebResourceRequestImpl(
            /* [in] */ ShouldInterceptRequestParams* params);

        // @Override
        CARAPI_(AutoPtr<IUri>) GetUrl();

        // @Override
        CARAPI_(Boolean) IsForMainFrame();

        // @Override
        CARAPI_(Boolean) HasGesture();

        // @Override
        CARAPI_(String) GetMethod();

        // @Override
        CARAPI_(AutoPtr<IMap>) GetRequestHeaders();

    private:
        /*const*/ AutoPtr<ShouldInterceptRequestParams> mParams;
    };

    class InnerRunnable
        : public Object
        , public IRunnable
    {
    public:
        CAR_INTERFACE_DECL()

        InnerRunnable(
            /* [in] */ WebViewContentsClientAdapter* owner);

        // @Override
        CARAPI Run();

    private:
        WebViewContentsClientAdapter* mOwner;
    };

    class JsPromptResultReceiverAdapter
        : public Object // JsResult::ResultReceiver
    {
    public:
        JsPromptResultReceiverAdapter(
            /* [in] */ JsPromptResultReceiver* receiver);

        JsPromptResultReceiverAdapter(
            /* [in] */ JsResultReceiver* receiver);

        virtual CARAPI_(AutoPtr<IInterface/*JsPromptResult*/>) GetPromptResult();

        // @Override
        CARAPI OnJsResultComplete(
            /* [in] */ IInterface/*JsResult*/* result);

    private:
        AutoPtr<JsPromptResultReceiver> mChromePromptResultReceiver;
        AutoPtr<JsResultReceiver> mChromeResultReceiver;
        // We hold onto the JsPromptResult here, just to avoid the need to downcast
        // in onJsResultComplete.
        /*const*/ AutoPtr<IInterface/*JsPromptResult*/> mPromptResult;
    };

    class InnerSslErrorHandler
        : public Object// SslErrorHandler
    {
    public:
        InnerSslErrorHandler(
            /* [in] */ IInterface/*IValueCallback*/* callback);

        // @Override
        CARAPI Proceed();

        // @Override
        CARAPI Cancel();

    private:
        IInterface/*IValueCallback*/* mCallback;
    };

    class ClientCertRequestImpl
        : public Object
        //, public ClientCertRequest
    {
    public:
        ClientCertRequestImpl(
            /* [in] */ AwContentsClientBridge::ClientCertificateRequestCallback* callback,
            /* [in] */ ArrayOf<String>* keyTypes,
            /* [in] */ ArrayOf<IPrincipal*>* principals,
            /* [in] */ const String& host,
            /* [in] */ Int32 port);

        // @Override
        CARAPI_(AutoPtr< ArrayOf<String> >) GetKeyTypes();

        // @Override
        CARAPI_(AutoPtr< ArrayOf<IPrincipal*> >) GetPrincipals();

        // @Override
        CARAPI_(String) GetHost();

        // @Override
        CARAPI_(Int32) GetPort();

        // @Override
        CARAPI Proceed(
            /* [in] */ IPrivateKey* privateKey,
            /* [in] */ ArrayOf<IX509Certificate*>* chain);

        // @Override
        CARAPI Ignore();

        // @Override
        CARAPI Cancel();

    private:
        /*const*/ AutoPtr<AwContentsClientBridge::ClientCertificateRequestCallback> mCallback;
        /*const*/ AutoPtr< ArrayOf<String> > mKeyTypes;
        /*const*/ AutoPtr< ArrayOf<IPrincipal*> > mPrincipals;
        /*const*/ String mHost;
        /*const*/ Int32 mPort;
    };

    class InnerValueCallback1
        : public Object
        //, public IValueCallback
    {
    public:
        InnerValueCallback1(
            /* [in] */ IInterface/*IValueCallback*/* uploadFileCallback);

        // @Override
        CARAPI OnReceiveValue(
            /* [in] */ ArrayOf<IUri*>* uriList);

    private:
        Boolean mCompleted;
        IInterface/*IValueCallback*/* mUploadFileCallback;
    };

    class InnerValueCallback2
        : public Object
        //, public IValueCallback
    {
    public:
        InnerValueCallback2(
            /* [in] */ IInterface/*IValueCallback*/* uploadFileCallback);

        // @Override
        CARAPI OnReceiveValue(
            /* [in] */ IUri* uri);

    private:
        Boolean mCompleted;
        IInterface/*IValueCallback*/* mUploadFileCallback;
    };

    // TODO: Move to upstream.
    class AwHttpAuthHandlerAdapter
        : public Object
        //, public IHttpAuthHandler
    {
    public:
        AwHttpAuthHandlerAdapter(
            /* [in] */ AwHttpAuthHandler* awHandler);

        // @Override
        CARAPI Proceed(
            /* [in] */ const String& username,
            /* [in] */ const String& password);

        // @Override
        CARAPI Cancel();

        // @Override
        CARAPI_(Boolean) UseHttpAuthUsernamePassword();

    private:
        AutoPtr<AwHttpAuthHandler> mAwHandler;
    };

public:
    /**
      * Adapter constructor.
      *
      * @param webView the {@link WebView} instance that this adapter is serving.
      */
    WebViewContentsClientAdapter(
        /* [in] */ IInterface/*WebView*/* webView);

    virtual CARAPI SetWebViewClient(
        /* [in] */ IInterface/*WebViewClient*/* client);

    virtual CARAPI SetWebChromeClient(
        /* [in] */ IInterface/*WebChromeClient*/* client);

    virtual CARAPI SetDownloadListener(
        /* [in] */ IInterface/*IDownloadListener*/* listener);

    virtual CARAPI SetFindListener(
        /* [in] */ IInterface/*IWebViewFindListener*/* listener);

    virtual CARAPI SetPictureListener(
        /* [in] */ IInterface/*IWebViewPictureListener*/* listener);

    //--------------------------------------------------------------------------------------------
    //                        Adapter for all the methods.
    //--------------------------------------------------------------------------------------------
    /**
      * @see AwContentsClient#getVisitedHistory
      */
    // @Override
    CARAPI GetVisitedHistory(
        /* [in] */ IInterface/*IValueCallback*/* callback);

    /**
      * @see AwContentsClient#doUpdateVisiteHistory(const String&, boolean)
      */
    // @Override
    CARAPI DoUpdateVisitedHistory(
        /* [in] */ const String& url,
        /* [in] */ Boolean isReload);

    /**
      * @see AwContentsClient#onProgressChanged(int)
      */
    // @Override
    CARAPI OnProgressChanged(
        /* [in] */ Int32 progress);

    /**
      * @see AwContentsClient#shouldInterceptRequest(java.lang.const String&)
      */
    // @Override
    CARAPI_(AutoPtr<AwWebResourceResponse>) ShouldInterceptRequest(
        /* [in] */ ShouldInterceptRequestParams* params);

    /**
      * @see AwContentsClient#shouldOverrideUrlLoading(java.lang.const String&)
      */
    // @Override
    CARAPI_(Boolean) ShouldOverrideUrlLoading(
        /* [in] */ const String& url);

    /**
      * @see AwContentsClient#onUnhandledKeyEvent(android.view.IKeyEvent)
      */
    // @Override
    CARAPI OnUnhandledKeyEvent(
        /* [in] */ IKeyEvent* event);

    /**
      * @see AwContentsClient#onConsoleMessage(android.webkit.ConsoleMessage)
      */
    // @Override
    CARAPI_(Boolean) OnConsoleMessage(
        /* [in] */ IInterface/*IConsoleMessage*/* consoleMessage);

    /**
      * @see AwContentsClient#onFindResultReceived(int,int,boolean)
      */
    // @Override
    CARAPI OnFindResultReceived(
        /* [in] */ Int32 activeMatchOrdinal,
        /* [in] */ Int32 numberOfMatches,
        /* [in] */ Boolean isDoneCounting);

    /**
      * @See AwContentsClient#onNewPicture(Picture)
      */
    // @Override
    CARAPI OnNewPicture(
        /* [in] */ IPicture* picture);

    // @Override
    CARAPI OnLoadResource(
        /* [in] */ const String& url);

    // @Override
    CARAPI_(Boolean) OnCreateWindow(
        /* [in] */ Boolean isDialog,
        /* [in] */ Boolean isUserGesture);

    /**
      * @see AwContentsClient#onCloseWindow()
      */
    // @Override
    CARAPI OnCloseWindow();

    /**
      * @see AwContentsClient#onRequestFocus()
      */
    // @Override
    CARAPI OnRequestFocus();

    /**
      * @see AwContentsClient#onReceivedTouchIconUrl(const String& url, boolean precomposed)
      */
    // @Override
    CARAPI OnReceivedTouchIconUrl(
        /* [in] */ const String& url,
        /* [in] */ Boolean precomposed);

    /**
      * @see AwContentsClient#onReceivedIcon(Bitmap bitmap)
      */
    // @Override
    CARAPI OnReceivedIcon(
        /* [in] */ IBitmap* bitmap);

    /**
      * @see ContentViewClient#onPageStarted(const String&)
      */
    // @Override
    CARAPI OnPageStarted(
        /* [in] */ const String& url);

    /**
      * @see ContentViewClient#onPageFinished(const String&)
      */
    // @Override
    CARAPI OnPageFinished(
        /* [in] */ const String& url);

    /**
      * @see ContentViewClient#onReceivedError(int,const String&,const String&)
      */
    // @Override
    CARAPI OnReceivedError(
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& description,
        /* [in] */ const String& failingUrl);

    /**
      * @see ContentViewClient#onReceivedTitle(const String&)
      */
    // @Override
    CARAPI OnReceivedTitle(
        /* [in] */ const String& title);

    /**
      * @see ContentViewClient#shouldOverrideKeyEvent(IKeyEvent)
      */
    // @Override
    CARAPI_(Boolean) ShouldOverrideKeyEvent(
        /* [in] */ IKeyEvent* event);

    /**
      * @see ContentViewClient#onStartContentIntent(Context, const String&)
      * Callback when detecting a click on a content link.
      */
    // TODO: Delete this method when removed from base class.
    virtual CARAPI OnStartContentIntent(
        /* [in] */ IContext* context,
        /* [in] */ const String& contentUrl);

    // @Override
    CARAPI OnGeolocationPermissionsShowPrompt(
        /* [in] */ const String& origin,
        /* [in] */ IInterface/*GeolocationPermissions::Callback*/* callback);

    // @Override
    CARAPI OnGeolocationPermissionsHidePrompt();

    // @Override
    CARAPI OnPermissionRequest(
        /* [in] */ AwPermissionRequest* permissionRequest);

    // @Override
    CARAPI OnPermissionRequestCanceled(
        /* [in] */ AwPermissionRequest* permissionRequest);

    // @Override
    CARAPI HandleJsAlert(
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [in] */ JsResultReceiver* receiver);

    // @Override
    CARAPI_(void) HandleJsBeforeUnload(
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [in] */ JsResultReceiver* receiver);

    // @Override
    CARAPI_(void) HandleJsConfirm(
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [in] */ JsResultReceiver* receiver);

    // @Override
    CARAPI_(void) HandleJsPrompt(
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [in] */ const String& defaultValue,
        /* [in] */ JsPromptResultReceiver* receiver);

    // @Override
    CARAPI OnReceivedHttpAuthRequest(
        /* [in] */ AwHttpAuthHandler* handler,
        /* [in] */ const String& host,
        /* [in] */ const String& realm);

    // @Override
    CARAPI OnReceivedSslError(
        /* [in] */ IInterface/*IValueCallback*/* callback,
        /* [in] */ ISslError* error);

    // @Override
    CARAPI OnReceivedClientCertRequest(
        /* [in] */ AwContentsClientBridge::ClientCertificateRequestCallback* callback,
        /* [in] */ ArrayOf<String>* keyTypes,
        /* [in] */ ArrayOf<IPrincipal*>* principals,
        /* [in] */ const String& host,
        /* [in] */ Int32 port);

    // @Override
    CARAPI OnReceivedLoginRequest(
        /* [in] */ const String& realm,
        /* [in] */ const String& account,
        /* [in] */ const String& args);

    // @Override
    CARAPI OnFormResubmission(
        /* [in] */ IMessage* dontResend,
        /* [in] */ IMessage* resend);

    // @Override
    CARAPI OnDownloadStart(
        /* [in] */ const String& url,
        /* [in] */ const String& userAgent,
        /* [in] */ const String& contentDisposition,
        /* [in] */ const String& mimeType,
        /* [in] */ Int64 contentLength);

    // @Override
    CARAPI ShowFileChooser(
        /* [in] */ IInterface/*IValueCallback*/* uploadFileCallback,
        /* [in] */ AwContentsClient::FileChooserParams* fileChooserParams);

    // @Override
    CARAPI OnScaleChangedScaled(
        /* [in] */ Float oldScale,
        /* [in] */ Float newScale);

    // @Override
    CARAPI OnShowCustomView(
        /* [in] */ IView* view,
        /* [in] */ IInterface/*WebChromeClient::CustomViewCallback*/* cb);

    // @Override
    CARAPI OnHideCustomView();

    // @Override
    CARAPI_(AutoPtr<IBitmap>) GetDefaultVideoPoster();

protected:
    // @Override
    CARAPI_(AutoPtr<IView>) GetVideoLoadingProgressView();

private:
    // TAG is chosen for consistency with classic webview tracing.
    static const String TAG;
    // Enables API callback tracing
    static const Boolean TRACE;
    // The WebView instance that this adapter is serving.
    /*const*/ AutoPtr<IInterface/*WebView*/> mWebView;
    // The WebViewClient instance that was passed to WebView.setWebViewClient().
    AutoPtr<IInterface/*WebViewClient*/> mWebViewClient;
    // The WebChromeClient instance that was passed to WebView.setContentViewClient().
    AutoPtr<IInterface/*WebChromeClient*/> mWebChromeClient;
    // The listener receiving find-in-page API results.
    AutoPtr<IInterface/*IWebViewFindListener*/> mFindListener;
    // The listener receiving notifications of screen updates.
    AutoPtr<IInterface/*IWebViewPictureListener*/> mPictureListener;
    AutoPtr<IInterface/*IDownloadListener*/> mDownloadListener;
    AutoPtr<IHandler> mUiThreadHandler;
    static const Int32 NEW_WEBVIEW_CREATED = 100;
    AutoPtr<IWeakHashMap> mOngoingPermissionRequests;
};

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_WEBVIEWCONTENTSCLIENTADAPTER_H_

