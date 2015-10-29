
#include "elastos/droid/webkit/webview/chromium/WebViewContentsClientAdapter.h"
#include "elastos/core/AutoLock.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/net/CUriHelper.h"
#include "elastos/droid/graphics/CPicture.h"
#include "elastos/droid/graphics/CCanvas.h"
#include "elastos/droid/graphics/CBitmapFactory.h"
#include "elastos/droid/webkit/DebugFlags.h"
#include "elastos/droid/webkit/native/content/browser/ContentViewClient.h"
#include "elastos/droid/webkit/native/base/TraceEvent.h"
#include "elastos/droid/webkit/native/base/ThreadUtils.h"
#include "elastos/droid/webkit/webview/chromium/WebViewChromium.h"
#include "elastos/droid/webkit/webview/chromium/UnimplementedWebViewApi.h"
#include "elastos/droid/webkit/webview/chromium/FileChooserParamsAdapter.h"

using Elastos::Core::AutoLock;
using Elastos::Core::CString;
using Elastos::Core::EIID_IRunnable;
using Elastos::Utility::IList;
using Elastos::Utility::CArrayList;
using Elastos::Utility::CHashMap;
using Elastos::Utility::CWeakHashMap;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Provider::IBrowser;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Graphics::CPicture;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Graphics::BitmapConfig;
using Elastos::Droid::Graphics::IBitmapHelper;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Webkit::DebugFlags;
//using Elastos::Droid::Webkit::IWebBackForwardList;
//using Elastos::Droid::Webkit::IWebResourceResponse;
using Elastos::Droid::Webkit::Base::TraceEvent;
using Elastos::Droid::Webkit::Base::ThreadUtils;
using Elastos::Droid::Webkit::Content::Browser::ContentViewClient;
using Elastos::Droid::Webkit::Webview::Chromium::WebViewChromium;
using Elastos::Droid::Webkit::Webview::Chromium::UnimplementedWebViewApi;
using Elastos::Droid::Webkit::Webview::Chromium::FileChooserParamsAdapter;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

//=====================================================================
//           WebViewContentsClientAdapter::NullWebViewClient
//=====================================================================
Boolean WebViewContentsClientAdapter::NullWebViewClient::ShouldOverrideKeyEvent(
    /* [in] */ IInterface/*WebView*/* view,
    /* [in] */ IKeyEvent* event)
{
    // ==================before translated======================
    // // TODO: Investigate more and add a test case.
    // // This is reflecting Clank's behavior.
    // int keyCode = event.getKeyCode();
    // return !ContentViewClient.shouldPropagateKey(keyCode);

    assert(0);
    Int32 keyCode = 0;
    event->GetKeyCode(&keyCode);
    return !ContentViewClient::ShouldPropagateKey(keyCode);
}

Boolean WebViewContentsClientAdapter::NullWebViewClient::ShouldOverrideUrlLoading(
    /* [in] */ IInterface/*WebView*/* view,
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // Intent intent;
    // // Perform generic parsing of the URI to turn it into an Intent.
    // try {
    //     intent = Intent.parseUri(url, Intent.URI_INTENT_SCHEME);
    // } catch (URISyntaxException ex) {
    //     Log.w(TAG, "Bad URI " + url + ": " + ex.getMessage());
    //     return false;
    // }
    // // Sanitize the Intent, ensuring web pages can not bypass browser
    // // security (only access to BROWSABLE activities).
    // intent.addCategory(Intent.CATEGORY_BROWSABLE);
    // intent.setComponent(null);
    // Intent selector = intent.getSelector();
    // if (selector != null) {
    //     selector.addCategory(Intent.CATEGORY_BROWSABLE);
    //     selector.setComponent(null);
    // }
    // // Pass the package name as application ID so that the intent from the
    // // same application can be opened in the same tab.
    // intent.putExtra(Browser.EXTRA_APPLICATION_ID,
    //         view.getContext().getPackageName());
    // try {
    //     view.getContext().startActivity(intent);
    // } catch (ActivityNotFoundException ex) {
    //     Log.w(TAG, "No application can handle " + url);
    //     return false;
    // }
    // return true;

    assert(0);
    AutoPtr<IIntent> intent;
    // Perform generic parsing of the URI to turn it into an Intent.
    // try {
        //-CIntent::New(url, IIntent::URI_INTENT_SCHEME, (IIntent**)&intent);
    // } catch (URISyntaxException ex) {
        // Logger::W(TAG, "Bad URI " + url + ": " + ex.getMessage());
        // return FALSE;
    // }
    // Sanitize the Intent, ensuring web pages can not bypass browser
    // security (only access to BROWSABLE activities).
    intent->AddCategory(IIntent::CATEGORY_BROWSABLE);
    intent->SetComponent(NULL);
    AutoPtr<IIntent> selector;
    intent->GetSelector((IIntent**)&selector);
    if (selector != NULL) {
        selector->AddCategory(IIntent::CATEGORY_BROWSABLE);
        selector->SetComponent(NULL);
    }
    // Pass the package name as application ID so that the intent from the
    // same application can be opened in the same tab.
    AutoPtr<IContext> context;
    //view->GetContext((IContext**)&context);
    String packageName;
    context->GetPackageName(&packageName);
    intent->PutExtra(IBrowser::EXTRA_APPLICATION_ID, packageName);
    // try {
        context->StartActivity(intent);
    // } catch (ActivityNotFoundException ex) {
        // Logger::W(TAG, "No application can handle " + url);
        // return false;
    // }

    return TRUE;
}

//=====================================================================
//        WebViewContentsClientAdapter::PermissionRequestAdapter
//=====================================================================
const Int64 WebViewContentsClientAdapter::PermissionRequestAdapter::BITMASK_RESOURCE_VIDEO_CAPTURE;
const Int64 WebViewContentsClientAdapter::PermissionRequestAdapter::BITMASK_RESOURCE_AUDIO_CAPTURE;
const Int64 WebViewContentsClientAdapter::PermissionRequestAdapter::BITMASK_RESOURCE_PROTECTED_MEDIA_ID;

WebViewContentsClientAdapter::PermissionRequestAdapter::PermissionRequestAdapter(
    /* [in] */ AwPermissionRequest* awPermissionRequest)
{
    // ==================before translated======================
    // assert awPermissionRequest != null;
    // mAwPermissionRequest = awPermissionRequest;

    assert(awPermissionRequest != NULL);
    mAwPermissionRequest = awPermissionRequest;
}

Int64 WebViewContentsClientAdapter::PermissionRequestAdapter::ToAwPermissionResources(
    /* [in] */ ArrayOf<String>* resources)
{
    // ==================before translated======================
    // long result = 0;
    // for (const String& resource : resources) {
    //     if (resource.equals(PermissionRequest.RESOURCE_VIDEO_CAPTURE))
    //         result |= BITMASK_RESOURCE_VIDEO_CAPTURE;
    //     else if (resource.equals(PermissionRequest.RESOURCE_AUDIO_CAPTURE))
    //         result |= BITMASK_RESOURCE_AUDIO_CAPTURE;
    //     else if (resource.equals(PermissionRequest.RESOURCE_PROTECTED_MEDIA_ID))
    //         result |= BITMASK_RESOURCE_PROTECTED_MEDIA_ID;
    // }
    // return result;

    assert(0);
    Int64 result = 0;
    String resource;
    for (Int32 idx=0; idx<resources->GetLength(); ++idx) {
        resource = (*resources)[idx];
        if (resource.Equals(String("")/*PermissionRequest::RESOURCE_VIDEO_CAPTURE*/))
            result |= BITMASK_RESOURCE_VIDEO_CAPTURE;
        else if (resource.Equals(String("")/*PermissionRequest.RESOURCE_AUDIO_CAPTURE*/))
            result |= BITMASK_RESOURCE_AUDIO_CAPTURE;
        else if (resource.Equals(String("")/*PermissionRequest.RESOURCE_PROTECTED_MEDIA_ID*/))
            result |= BITMASK_RESOURCE_PROTECTED_MEDIA_ID;
    }

    return result;
}

AutoPtr<IUri> WebViewContentsClientAdapter::PermissionRequestAdapter::GetOrigin()
{
    // ==================before translated======================
    // return mAwPermissionRequest.getOrigin();

    assert(0);
    return mAwPermissionRequest->GetOrigin();
}

AutoPtr< ArrayOf<String> > WebViewContentsClientAdapter::PermissionRequestAdapter::GetResources()
{
    // ==================before translated======================
    // synchronized (this) {
    //     if (mResources == null) {
    //         mResources = toPermissionResources(mAwPermissionRequest.getResources());
    //     }
    //     return mResources;
    // }

    assert(0);
    AutoLock lock(this);
    if (NULL == mResources) {
        mResources = ToPermissionResources(mAwPermissionRequest->GetResources());
    }
    return mResources;
}

ECode WebViewContentsClientAdapter::PermissionRequestAdapter::Grant(
    /* [in] */ ArrayOf<String>* resources)
{
    VALIDATE_NOT_NULL(resources);
    // ==================before translated======================
    // long requestedResource = mAwPermissionRequest.getResources();
    // if ((requestedResource & toAwPermissionResources(resources)) == requestedResource)
    //     mAwPermissionRequest.grant();
    // else
    //     mAwPermissionRequest.deny();

    assert(0);
    Int64 requestedResource = mAwPermissionRequest->GetResources();
    if ((requestedResource & ToAwPermissionResources(resources)) == requestedResource)
        return mAwPermissionRequest->Grant();
    else
        return mAwPermissionRequest->Deny();
}

ECode WebViewContentsClientAdapter::PermissionRequestAdapter::Deny()
{
    // ==================before translated======================
    // mAwPermissionRequest.deny();

    assert(0);
    return mAwPermissionRequest->Deny();
}

AutoPtr< ArrayOf<String> > WebViewContentsClientAdapter::PermissionRequestAdapter::ToPermissionResources(
    /* [in] */ Int64 resources)
{
    // ==================before translated======================
    // ArrayList<String> result = new ArrayList<String>();
    // if ((resources & BITMASK_RESOURCE_VIDEO_CAPTURE) != 0)
    //     result.add(PermissionRequest.RESOURCE_VIDEO_CAPTURE);
    // if ((resources & BITMASK_RESOURCE_AUDIO_CAPTURE) != 0)
    //     result.add(PermissionRequest.RESOURCE_AUDIO_CAPTURE);
    // if ((resources & BITMASK_RESOURCE_PROTECTED_MEDIA_ID) != 0)
    //     result.add(PermissionRequest.RESOURCE_PROTECTED_MEDIA_ID);
    // const String&[] resource_array = new const String&[result.size()];
    // return result.toArray(resource_array);

    assert(0);
    Int32 size = 0;
    String tmps[4];
    if ((resources & BITMASK_RESOURCE_VIDEO_CAPTURE) != 0)
        tmps[size++] = String("");//PermissionRequest::RESOURCE_VIDEO_CAPTURE
    if ((resources & BITMASK_RESOURCE_AUDIO_CAPTURE) != 0)
        tmps[size++] = String("");//PermissionRequest::RESOURCE_AUDIO_CAPTURE
    if ((resources & BITMASK_RESOURCE_PROTECTED_MEDIA_ID) != 0)
        tmps[size++] = String("");//PermissionRequest::RESOURCE_PROTECTED_MEDIA_ID

    AutoPtr< ArrayOf<String> > result = ArrayOf<String>::Alloc(size);
    for (Int32 idx=0; idx<size; ++idx) {
        result->Set(idx, tmps[idx]);
    }

    return result;
}

//=====================================================================
//             WebViewContentsClientAdapter::InnerHandler
//=====================================================================
WebViewContentsClientAdapter::InnerHandler::InnerHandler(
    /* [in] */ WebViewContentsClientAdapter* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewContentsClientAdapter::InnerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);
    // ==================before translated======================
    // switch(msg.what) {
    //     case NEW_WEBVIEW_CREATED:
    //         WebView.WebViewTransport t = (WebView.WebViewTransport) msg.obj;
    //         WebView newWebView = t.getWebView();
    //         if (newWebView == mWebView) {
    //             throw new IllegalArgumentException(
    //                     "Parent WebView cannot host it's own popup window. Please " +
    //                     "use WebSettings.setSupportMultipleWindows(false)");
    //         }
    //
    //         if (newWebView != null && newWebView.copyBackForwardList().getSize() != 0) {
    //             throw new IllegalArgumentException(
    //                     "New WebView for popup window must not have been previously " +
    //                     "navigated.");
    //         }
    //
    //         WebViewChromium.completeWindowCreation(mWebView, newWebView);
    //         break;
    //     default:
    //         throw new IllegalStateException();
    // }

    assert(NULL == mOwner);
    Int32 what = 0;
    msg->GetWhat(&what);
    switch (what) {
        case NEW_WEBVIEW_CREATED:
            {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                AutoPtr<IObject> objTmp = IObject::Probe(obj);
                AutoPtr<IInterface/*WebView::WebViewTransport*/> t = NULL;//(WebView::WebViewTransport*)objTmp.Get();

                AutoPtr<IInterface/*WebView*/> newWebView = NULL;//t->GetWebView();
                if (newWebView == mOwner->mWebView) {
                    //throw new IllegalArgumentException(String("Parent WebView cannot host it's own popup window. Please ") +
                    //    String("use WebSettings.setSupportMultipleWindows(false)"));
                }

                AutoPtr<IInterface/*IWebBackForwardList*/> forwardList = NULL;//newWebView->CopyBackForwardList();
                Int32 size = 0;
                //forwardList->GetSize(&size);
                if (newWebView != NULL && size != 0) {
                    //throw new IllegalArgumentException(String("New WebView for popup window must not have been previously ") +
                    //    String("navigated."));
                }

                WebViewChromium::CompleteWindowCreation(mOwner->mWebView, newWebView);
            }
            break;
        default:
            //throw new IllegalStateException();
            break;
    }

    return NOERROR;
}

//=====================================================================
//         WebViewContentsClientAdapter::WebResourceRequestImpl
//=====================================================================
WebViewContentsClientAdapter::WebResourceRequestImpl::WebResourceRequestImpl(
    /* [in] */ ShouldInterceptRequestParams* params)
    : mParams(params)
{
    // ==================before translated======================
    // mParams = params;
}

AutoPtr<IUri> WebViewContentsClientAdapter::WebResourceRequestImpl::GetUrl()
{
    // ==================before translated======================
    // return Uri.parse(mParams.url);

    assert(0);
    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);

    AutoPtr<IUri> result;
    helper->Parse(mParams->url, (IUri**)&result);
    return result;
}

Boolean WebViewContentsClientAdapter::WebResourceRequestImpl::IsForMainFrame()
{
    // ==================before translated======================
    // return mParams.isMainFrame;

    assert(0);
    return mParams->isMainFrame;
}

Boolean WebViewContentsClientAdapter::WebResourceRequestImpl::HasGesture()
{
    // ==================before translated======================
    // return mParams.hasUserGesture;

    assert(0);
    return mParams->hasUserGesture;
}

String WebViewContentsClientAdapter::WebResourceRequestImpl::GetMethod()
{
    // ==================before translated======================
    // return mParams.method;

    assert(0);
    return mParams->method;
}

AutoPtr<IMap> WebViewContentsClientAdapter::WebResourceRequestImpl::GetRequestHeaders()
{
    // ==================before translated======================
    // return mParams.requestHeaders;

    assert(0);
    AutoPtr<IMap> result;//base class is Map; = (IMap*)mParams->requestHeaders;
    return result;
}

//=====================================================================
//             WebViewContentsClientAdapter::InnerRunnable
//=====================================================================
CAR_INTERFACE_IMPL(WebViewContentsClientAdapter::InnerRunnable, Object, IRunnable)

WebViewContentsClientAdapter::InnerRunnable::InnerRunnable(
    /* [in] */ WebViewContentsClientAdapter* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewContentsClientAdapter::InnerRunnable::Run()
{
    // ==================before translated======================
    // UnimplementedWebViewApi.invoke();
    // if (mPictureListener != null) {
    //     if (TRACE) Log.d(TAG, "onPageFinished-fake");
    //     mPictureListener.onNewPicture(mWebView, new Picture());
    // }

    assert(NULL == mOwner);
    UnimplementedWebViewApi::Invoke();
    if (mOwner->mPictureListener != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "onPageFinished-fake");
        }

        AutoPtr<IPicture> picture;
        CPicture::New((IPicture**)&picture);
        //mPictureListener->OnNewPicture(mWebView, picture);
    }
    return NOERROR;
}

//=====================================================================
//     WebViewContentsClientAdapter::JsPromptResultReceiverAdapter
//=====================================================================
WebViewContentsClientAdapter::JsPromptResultReceiverAdapter::JsPromptResultReceiverAdapter(
    /* [in] */ JsPromptResultReceiver* receiver)
    : mChromePromptResultReceiver(receiver)
{
    // ==================before translated======================
    // mChromePromptResultReceiver = receiver;
}

WebViewContentsClientAdapter::JsPromptResultReceiverAdapter::JsPromptResultReceiverAdapter(
    /* [in] */ JsResultReceiver* receiver)
    : mChromeResultReceiver(receiver)
{
    // ==================before translated======================
    // mChromeResultReceiver = receiver;
}

AutoPtr<IInterface/*JsPromptResult*/> WebViewContentsClientAdapter::JsPromptResultReceiverAdapter::GetPromptResult()
{
    // ==================before translated======================
    // return mPromptResult;

    assert(0);
    return mPromptResult;
}

ECode WebViewContentsClientAdapter::JsPromptResultReceiverAdapter::OnJsResultComplete(
    /* [in] */ IInterface/*JsResult*/* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mChromePromptResultReceiver != null) {
    //     if (mPromptResult.getResult()) {
    //         mChromePromptResultReceiver.confirm(mPromptResult.getStringResult());
    //     } else {
    //         mChromePromptResultReceiver.cancel();
    //     }
    // } else {
    //     if (mPromptResult.getResult()) {
    //         mChromeResultReceiver.confirm();
    //     } else {
    //         mChromeResultReceiver.cancel();
    //     }
    // }

    assert(0);
    if (mChromePromptResultReceiver != NULL) {
        if (-1/*mPromptResult->GetResult()*/) {
            mChromePromptResultReceiver->Confirm(String("")/*mPromptResult->GetStringResult()*/);
        }
        else {
            mChromePromptResultReceiver->Cancel();
        }
    }
    else {
        if (-1/*mPromptResult->GetResult()*/) {
            mChromeResultReceiver->Confirm();
        }
        else {
            mChromeResultReceiver->Cancel();
        }
    }

    return NOERROR;
}

//=====================================================================
//         WebViewContentsClientAdapter::InnerSslErrorHandler
//=====================================================================
WebViewContentsClientAdapter::InnerSslErrorHandler::InnerSslErrorHandler(
    /* [in] */ IInterface/*IValueCallback*/* callback)
    : mCallback(callback)
{
    // ==================before translated======================
}

ECode WebViewContentsClientAdapter::InnerSslErrorHandler::Proceed()
{
    // ==================before translated======================
    // callback.onReceiveValue(true);

    assert(0);
    //mCallback->OnReceiveValue(TRUE);
    return NOERROR;
}

ECode WebViewContentsClientAdapter::InnerSslErrorHandler::Cancel()
{
    // ==================before translated======================
    // callback.onReceiveValue(false);

    assert(0);
    //mCallback->OnReceiveValue(FALSE);
    return NOERROR;
}

//=====================================================================
//         WebViewContentsClientAdapter::ClientCertRequestImpl
//=====================================================================
WebViewContentsClientAdapter::ClientCertRequestImpl::ClientCertRequestImpl(
    /* [in] */ AwContentsClientBridge::ClientCertificateRequestCallback* callback,
    /* [in] */ ArrayOf<String>* keyTypes,
    /* [in] */ ArrayOf<IPrincipal*>* principals,
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
    : mCallback(callback)
    , mKeyTypes(keyTypes)
    , mPrincipals(principals)
    , mHost(host)
    , mPort(port)
{
    // ==================before translated======================
    // mCallback = callback;
    // mKeyTypes = keyTypes;
    // mPrincipals = principals;
    // mHost = host;
    // mPort = port;
}

AutoPtr< ArrayOf<String> > WebViewContentsClientAdapter::ClientCertRequestImpl::GetKeyTypes()
{
    // ==================before translated======================
    // // This is already a copy of native argument, so return directly.
    // return mKeyTypes;

    assert(0);
    return mKeyTypes;
}

AutoPtr< ArrayOf<IPrincipal*> > WebViewContentsClientAdapter::ClientCertRequestImpl::GetPrincipals()
{
    // ==================before translated======================
    // // This is already a copy of native argument, so return directly.
    // return mPrincipals;

    assert(0);
    return mPrincipals;
}

String WebViewContentsClientAdapter::ClientCertRequestImpl::GetHost()
{
    // ==================before translated======================
    // return mHost;

    assert(0);
    return mHost;
}

Int32 WebViewContentsClientAdapter::ClientCertRequestImpl::GetPort()
{
    // ==================before translated======================
    // return mPort;

    assert(0);
    return mPort;
}

ECode WebViewContentsClientAdapter::ClientCertRequestImpl::Proceed(
    /* [in] */ IPrivateKey* privateKey,
    /* [in] */ ArrayOf<IX509Certificate*>* chain)
{
    VALIDATE_NOT_NULL(privateKey);
    VALIDATE_NOT_NULL(chain);
    // ==================before translated======================
    // mCallback.proceed(privateKey, chain);

    assert(0);
    mCallback->Proceed(privateKey, chain);
    return NOERROR;
}

ECode WebViewContentsClientAdapter::ClientCertRequestImpl::Ignore()
{
    // ==================before translated======================
    // mCallback.ignore();

    assert(0);
    mCallback->Ignore();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::ClientCertRequestImpl::Cancel()
{
    // ==================before translated======================
    // mCallback.cancel();

    assert(0);
    mCallback->Cancel();
    return NOERROR;
}

//=====================================================================
//           WebViewContentsClientAdapter::InnerValueCallback1
//=====================================================================
WebViewContentsClientAdapter::InnerValueCallback1::InnerValueCallback1(
    /* [in] */ IInterface/*IValueCallback*/* uploadFileCallback)
    : mUploadFileCallback(uploadFileCallback)
{
    // ==================before translated======================
}

ECode WebViewContentsClientAdapter::InnerValueCallback1::OnReceiveValue(
    /* [in] */ ArrayOf<IUri*>* uriList)
{
    VALIDATE_NOT_NULL(uriList);
    // ==================before translated======================
    // if (mCompleted) {
    //     throw new IllegalStateException("showFileChooser result was already called");
    // }
    // mCompleted = true;
    // String s[] = null;
    // if (uriList != null) {
    //     s = new const String&[uriList.length];
    //     for(int i = 0; i < uriList.length; i++) {
    //         s[i] = uriList[i].toString();
    //     }
    // }
    // uploadFileCallback.onReceiveValue(s);

    assert(0);
    if (mCompleted) {
        //throw new IllegalStateException("showFileChooser result was already called");
    }
    mCompleted = TRUE;
    AutoPtr<IList> s = NULL;
    if (uriList != NULL) {
        CArrayList::New(uriList->GetLength(), (IList**)&s);
        String tmp;
        for(Int32 i = 0; i < uriList->GetLength(); ++i) {
            (*uriList)[i]->ToSafeString(&tmp);
            AutoPtr<ICharSequence> item;
            CString::New(tmp, (ICharSequence**)&item);
            s->Set(i, item);
        }
    }

    //AutoPtr<IInterface> sTmp = s->Probe(EIID_IInterface);
    //mUploadFileCallback->OnReceiveValue(sTmp);
    return NOERROR;
}

//=====================================================================
//           WebViewContentsClientAdapter::InnerValueCallback2
//=====================================================================
WebViewContentsClientAdapter::InnerValueCallback2::InnerValueCallback2(
    /* [in] */ IInterface/*IValueCallback*/* uploadFileCallback)
    : mUploadFileCallback(uploadFileCallback)
{
    // ==================before translated======================
}

ECode WebViewContentsClientAdapter::InnerValueCallback2::OnReceiveValue(
    /* [in] */ IUri* uri)
{
    VALIDATE_NOT_NULL(uri);
    // ==================before translated======================
    // if (mCompleted) {
    //     throw new IllegalStateException("showFileChooser result was already called");
    // }
    // mCompleted = true;
    // uploadFileCallback.onReceiveValue(
    //         uri == null ? null : new String[] { uri.toString() });

    assert(0);
    if (mCompleted) {
        //throw new IllegalStateException("showFileChooser result was already called");
    }
    mCompleted = TRUE;
    AutoPtr<ICharSequence> s = NULL;
    if (uri != NULL) {
        String tmp;
        uri->ToSafeString(&tmp);
        CString::New(tmp, (ICharSequence**)&s);
        //AutoPtr<IInterface> sTmp = s->Probe(EIID_IInterface);
        //mUploadFileCallback->OnReceiveValue(sTmp);
    }
    else {
        //mUploadFileCallback->OnReceiveValue(NULL);
    }

    return NOERROR;
}

//=====================================================================
//        WebViewContentsClientAdapter::AwHttpAuthHandlerAdapter
//=====================================================================
WebViewContentsClientAdapter::AwHttpAuthHandlerAdapter::AwHttpAuthHandlerAdapter(
    /* [in] */ AwHttpAuthHandler* awHandler)
    : mAwHandler(awHandler)
{
    // ==================before translated======================
    // mAwHandler = awHandler;
}

ECode WebViewContentsClientAdapter::AwHttpAuthHandlerAdapter::Proceed(
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    // ==================before translated======================
    // if (username == null) {
    //     username = "";
    // }
    //
    // if (password == null) {
    //     password = "";
    // }
    // mAwHandler.proceed(username, password);

    assert(0);
    String userNameTmp = username;
    if (username.IsEmpty()) {
        userNameTmp = String("");
    }

    String passwordTmp = password;
    if (password.IsEmpty()) {
        passwordTmp = String("");
    }

    mAwHandler->Proceed(username, password);
    return NOERROR;
}

ECode WebViewContentsClientAdapter::AwHttpAuthHandlerAdapter::Cancel()
{
    // ==================before translated======================
    // mAwHandler.cancel();

    assert(0);
    mAwHandler->Cancel();
    return NOERROR;
}

Boolean WebViewContentsClientAdapter::AwHttpAuthHandlerAdapter::UseHttpAuthUsernamePassword()
{
    // ==================before translated======================
    // return mAwHandler.isFirstAttempt();

    assert(0);
    return mAwHandler->IsFirstAttempt();
}

//=====================================================================
//                     WebViewContentsClientAdapter
//=====================================================================
const String WebViewContentsClientAdapter::TAG("WebViewCallback");
const Boolean WebViewContentsClientAdapter::TRACE = DebugFlags::TRACE_CALLBACK;
const Int32 WebViewContentsClientAdapter::NEW_WEBVIEW_CREATED;

WebViewContentsClientAdapter::WebViewContentsClientAdapter(
    /* [in] */ IInterface/*WebView*/* webView)
{
    // ==================before translated======================
    // if (webView == null) {
    //     throw new IllegalArgumentException("webView can't be null");
    // }
    //
    // mWebView = webView;
    // setWebViewClient(null);
    //
    // mUiThreadHandler = new IHandler() {
    //
    //     @Override
    //     public void handleMessage(Message msg) {
    //         switch(msg.what) {
    //             case NEW_WEBVIEW_CREATED:
    //                 WebView.WebViewTransport t = (WebView.WebViewTransport) msg.obj;
    //                 WebView newWebView = t.getWebView();
    //                 if (newWebView == mWebView) {
    //                     throw new IllegalArgumentException(
    //                             "Parent WebView cannot host it's own popup window. Please " +
    //                             "use WebSettings.setSupportMultipleWindows(false)");
    //                 }
    //
    //                 if (newWebView != null && newWebView.copyBackForwardList().getSize() != 0) {
    //                     throw new IllegalArgumentException(
    //                             "New WebView for popup window must not have been previously " +
    //                             "navigated.");
    //                 }
    //
    //                 WebViewChromium.completeWindowCreation(mWebView, newWebView);
    //                 break;
    //             default:
    //                 throw new IllegalStateException();
    //         }
    //     }
    // };

    if (NULL == webView) {
        //throw new IllegalArgumentException("webView can't be null");
    }
    mWebView = webView;
    SetWebViewClient(NULL);
    mUiThreadHandler = new WebViewContentsClientAdapter::InnerHandler(this);
}

ECode WebViewContentsClientAdapter::SetWebViewClient(
    /* [in] */ IInterface/*WebViewClient*/* client)
{
    VALIDATE_NOT_NULL(client);
    // ==================before translated======================
    // if (client != null) {
    //     mWebViewClient = client;
    // } else {
    //     mWebViewClient = new NullWebViewClient();
    // }

    assert(0);
    if (client != NULL) {
        mWebViewClient = client;
    }
    else {
        //mWebViewClient = new NullWebViewClient();
    }

    return NOERROR;
}

ECode WebViewContentsClientAdapter::SetWebChromeClient(
    /* [in] */ IInterface/*WebChromeClient*/* client)
{
    VALIDATE_NOT_NULL(client);
    // ==================before translated======================
    // mWebChromeClient = client;

    assert(0);
    mWebChromeClient = client;
    return NOERROR;
}

ECode WebViewContentsClientAdapter::SetDownloadListener(
    /* [in] */ IInterface/*IDownloadListener*/* listener)
{
    VALIDATE_NOT_NULL(listener);
    // ==================before translated======================
    // mDownloadListener = listener;

    assert(0);
    mDownloadListener = listener;
    return NOERROR;
}

ECode WebViewContentsClientAdapter::SetFindListener(
    /* [in] */ IInterface/*IWebViewFindListener*/* listener)
{
    // ==================before translated======================
    // mFindListener = listener;

    assert(0);
    mFindListener = listener;
    return NOERROR;
}

ECode WebViewContentsClientAdapter::SetPictureListener(
    /* [in] */ IInterface/*IWebViewPictureListener*/* listener)
{
    // ==================before translated======================
    // mPictureListener = listener;

    assert(0);
    mPictureListener = listener;
    return NOERROR;
}

ECode WebViewContentsClientAdapter::GetVisitedHistory(
    /* [in] */ IInterface/*IValueCallback*/* callback)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "getVisitedHistory");
    //     mWebChromeClient.getVisitedHistory(callback);
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Loggor::D(TAG, "getVisitedHistory");
        }
        //mWebChromeClient->GetVisitedHistory(callback);
    }
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::DoUpdateVisitedHistory(
    /* [in] */ const String& url,
    /* [in] */ Boolean isReload)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "doUpdateVisitedHistory=" + url + " reload=" + isReload);
    // mWebViewClient.doUpdateVisitedHistory(mWebView, url, isReload);
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (TRACE) {
        //Loggor::D(TAG, "doUpdateVisitedHistory=" + url + " reload=" + isReload);
    }

    //mWebViewClient->DoUpdateVisitedHistory(mWebView, url, isReload);
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnProgressChanged(
    /* [in] */ Int32 progress)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "onProgressChanged=" + progress);
    //     mWebChromeClient.onProgressChanged(mWebView, progress);
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Loggor::D(TAG, "onProgressChanged=" + progress);
        }
        //mWebChromeClient->OnProgressChanged(mWebView, progress);
    }
    TraceEvent::End();
    return NOERROR;
}

AutoPtr<AwWebResourceResponse> WebViewContentsClientAdapter::ShouldInterceptRequest(
    /* [in] */ ShouldInterceptRequestParams* params)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "shouldInterceptRequest=" + params.url);
    // WebResourceResponse response = mWebViewClient.shouldInterceptRequest(mWebView,
    //         new WebResourceRequestImpl(params));
    // TraceEvent.end();
    // if (response == null) return null;
    //
    // // AwWebResourceResponse should support null headers. b/16332774.
    // Map<String, String> responseHeaders = response.getResponseHeaders();
    // if (responseHeaders == null)
    //     responseHeaders = new HashMap<String, String>();
    //
    // return new AwWebResourceResponse(
    //         response.getMimeType(),
    //         response.getEncoding(),
    //         response.getData(),
    //         response.getStatusCode(),
    //         response.getReasonPhrase(),
    //         responseHeaders);

    assert(0);
    TraceEvent::Begin();
    if (TRACE) {
        //Loggor::D(TAG, "shouldInterceptRequest=" + params.url);
    }

    AutoPtr<IInterface/*IWebResourceResponse*/> response;// = mWebViewClient->ShouldInterceptRequest(mWebView, new WebResourceRequestImpl(params));
    TraceEvent::End();
    if (NULL == response)
        return NULL;

    // AwWebResourceResponse should support null headers. b/16332774.
    AutoPtr<IMap> responseHeaders;// has no this func:  = response->GetResponseHeaders();
    if (NULL == responseHeaders)
        CHashMap::New((IMap**)&responseHeaders);

    AutoPtr<AwWebResourceResponse> result;// = new AwWebResourceResponse(response->GetMimeType(),
        //response->GetEncoding(),
        //response->GetData(),
        //response->GetStatusCode(),
        //response->GetReasonPhrase(),
        //responseHeaders);
    return result;
}

Boolean WebViewContentsClientAdapter::ShouldOverrideUrlLoading(
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "shouldOverrideUrlLoading=" + url);
    // boolean result = mWebViewClient.shouldOverrideUrlLoading(mWebView, url);
    // TraceEvent.end();
    // return result;

    assert(0);
    TraceEvent::Begin();
    if (TRACE) {
        //Loggor::D(TAG, "shouldOverrideUrlLoading=" + url);
    }

    Boolean result = FALSE;//mWebViewClient->ShouldOverrideUrlLoading(mWebView, url);
    TraceEvent::End();
    return result;
}

ECode WebViewContentsClientAdapter::OnUnhandledKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    VALIDATE_NOT_NULL(event);
    // ==================before translated======================
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "onUnhandledKeyEvent");
    // mWebViewClient.onUnhandledKeyEvent(mWebView, event);
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (TRACE) {
        //Loggor::D(TAG, "onUnhandledKeyEvent");
    }

    //mWebViewClient->OnUnhandledKeyEvent(mWebView, event);
    TraceEvent::End();
    return NOERROR;
}

Boolean WebViewContentsClientAdapter::OnConsoleMessage(
    /* [in] */ IInterface/*IConsoleMessage*/* consoleMessage)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // boolean result;
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "onConsoleMessage: " + consoleMessage.message());
    //     result = mWebChromeClient.onConsoleMessage(consoleMessage);
    //     String message = consoleMessage.message();
    //     if (result && message != null && message.startsWith("[blocked]")) {
    //         Log.e(TAG, "Blocked URL: " + message);
    //     }
    // } else {
    //     result = false;
    // }
    // TraceEvent.end();
    // return result;

    assert(0);
    TraceEvent::Begin();
    Boolean result = FALSE;
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "onConsoleMessage: " + consoleMessage.message());
        }

        //result = mWebChromeClient->OnConsoleMessage(consoleMessage);
        String message;
        //consoleMessage->Message(&message);
        if (result && !message.IsEmpty() && message.StartWith(String("[blocked]"))) {
            //Logger::E(TAG, "Blocked URL: " + message);
        }
    }

    TraceEvent::End();
    return result;
}

ECode WebViewContentsClientAdapter::OnFindResultReceived(
    /* [in] */ Int32 activeMatchOrdinal,
    /* [in] */ Int32 numberOfMatches,
    /* [in] */ Boolean isDoneCounting)
{
    // ==================before translated======================
    // if (mFindListener == null) return;
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "onFindResultReceived");
    // mFindListener.onFindResultReceived(activeMatchOrdinal, numberOfMatches, isDoneCounting);
    // TraceEvent.end();

    assert(0);
    if (NULL == mFindListener)
        return NOERROR;

    TraceEvent::Begin();
    if (TRACE) {
        //Loggor::D(TAG, "onFindResultReceived");
    }

    //mFindListener->OnFindResultReceived(activeMatchOrdinal, numberOfMatches, isDoneCounting);
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnNewPicture(
    /* [in] */ IPicture* picture)
{
    VALIDATE_NOT_NULL(picture);
    // ==================before translated======================
    // if (mPictureListener == null) return;
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "onNewPicture");
    // mPictureListener.onNewPicture(mWebView, picture);
    // TraceEvent.end();

    assert(0);
    if (NULL == mPictureListener)
        return NOERROR;

    TraceEvent::Begin();
    if (TRACE) {
        //Loggor::D(TAG, "onNewPicture");
    }

    //mPictureListener.onNewPicture(mWebView, picture);
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnLoadResource(
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "onLoadResource=" + url);
    // mWebViewClient.onLoadResource(mWebView, url);
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (TRACE) {
        //Loggor::D(TAG, "onLoadResource=" + url);
    }

    //mWebViewClient->OnLoadResource(mWebView, url);
    TraceEvent::End();
    return NOERROR;
}

Boolean WebViewContentsClientAdapter::OnCreateWindow(
    /* [in] */ Boolean isDialog,
    /* [in] */ Boolean isUserGesture)
{
    // ==================before translated======================
    // Message m = mUiThreadHandler.obtainMessage(
    //         NEW_WEBVIEW_CREATED, mWebView.new WebViewTransport());
    // TraceEvent.begin();
    // boolean result;
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "onCreateWindow");
    //     result = mWebChromeClient.onCreateWindow(mWebView, isDialog, isUserGesture, m);
    // } else {
    //     result = false;
    // }
    // TraceEvent.end();
    // return result;

    assert(0);
    AutoPtr<IMessage> m;
    mUiThreadHandler->ObtainMessage(NEW_WEBVIEW_CREATED, NULL/*mWebView.new WebViewTransport()*/, (IMessage**)&m);
    TraceEvent::Begin();
    Boolean result = FALSE;
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "onCreateWindow");
        }
        //result = mWebChromeClient->OnCreateWindow(mWebView, isDialog, isUserGesture, m);
    }

    TraceEvent::End();
    return result;
}

ECode WebViewContentsClientAdapter::OnCloseWindow()
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "onCloseWindow");
    //     mWebChromeClient.onCloseWindow(mWebView);
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "onCloseWindow");
        }
        //mWebChromeClient->OnCloseWindow(mWebView);
    }

    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnRequestFocus()
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "onRequestFocus");
    //     mWebChromeClient.onRequestFocus(mWebView);
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "onRequestFocus");
        }
        //mWebChromeClient->OnRequestFocus(mWebView);
    }

    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnReceivedTouchIconUrl(
    /* [in] */ const String& url,
    /* [in] */ Boolean precomposed)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "onReceivedTouchIconUrl=" + url);
    //     mWebChromeClient.onReceivedTouchIconUrl(mWebView, url, precomposed);
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "onReceivedTouchIconUrl=" + url);
        }
        //mWebChromeClient->OnReceivedTouchIconUrl(mWebView, url, precomposed);
    }

    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnReceivedIcon(
    /* [in] */ IBitmap* bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "onReceivedIcon");
    //     mWebChromeClient.onReceivedIcon(mWebView, bitmap);
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "onReceivedIcon");
        }
        //mWebChromeClient->OnReceivedIcon(mWebView, bitmap);
    }

    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnPageStarted(
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "onPageStarted=" + url);
    // mWebViewClient.onPageStarted(mWebView, url, mWebView.getFavicon());
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (TRACE) {
        //Logger::D(TAG, "onPageStarted=" + url);
    }

    //mWebViewClient->OnPageStarted(mWebView, url, mWebView.getFavicon());
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnPageFinished(
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "onPageFinished=" + url);
    // mWebViewClient.onPageFinished(mWebView, url);
    // TraceEvent.end();
    //
    // // See b/8208948
    // // This fakes an onNewPicture callback after onPageFinished to allow
    // // CTS tests to run in an un-flaky manner. This is required as the
    // // path for sending Picture updates in Chromium are decoupled from the
    // // page loading callbacks, i.e. the Chrome compositor may draw our
    // // content and send the Picture before onPageStarted or onPageFinished
    // // are invoked. The CTS harness discards any pictures it receives before
    // // onPageStarted is invoked, so in the case we get the Picture before that and
    // // no further updates after onPageStarted, we'll fail the test by timing
    // // out waiting for a Picture.
    // // To ensure backwards compatibility, we need to defer sending Picture updates
    // // until onPageFinished has been invoked. This work is being done
    // // upstream, and we can revert this hack when it lands.
    // if (mPictureListener != null) {
    //     ThreadUtils.postOnUiThreadDelayed(new Runnable() {
    //         @Override
    //         public void run() {
    //             UnimplementedWebViewApi.invoke();
    //             if (mPictureListener != null) {
    //                 if (TRACE) Log.d(TAG, "onPageFinished-fake");
    //                 mPictureListener.onNewPicture(mWebView, new Picture());
    //             }
    //         }
    //     }, 100);
    // }

    assert(0);
    TraceEvent::Begin();
    if (TRACE) {
        //Logger::D(TAG, "onPageFinished=" + url);
    }

    //mWebViewClient->OnPageFinished(mWebView, url);
    TraceEvent::End();

    // See b/8208948
    // This fakes an onNewPicture callback after onPageFinished to allow
    // CTS tests to run in an un-flaky manner. This is required as the
    // path for sending Picture updates in Chromium are decoupled from the
    // page loading callbacks, i.e. the Chrome compositor may draw our
    // content and send the Picture before onPageStarted or onPageFinished
    // are invoked. The CTS harness discards any pictures it receives before
    // onPageStarted is invoked, so in the case we get the Picture before that and
    // no further updates after onPageStarted, we'll fail the test by timing
    // out waiting for a Picture.
    // To ensure backwards compatibility, we need to defer sending Picture updates
    // until onPageFinished has been invoked. This work is being done
    // upstream, and we can revert this hack when it lands.
    if (mPictureListener != NULL) {
        AutoPtr<IRunnable> runnable = new WebViewContentsClientAdapter::InnerRunnable(this);
        ThreadUtils::PostOnUiThreadDelayed(runnable, 100);
    }

    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnReceivedError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
    // ==================before translated======================
    // if (description == null || description.isEmpty()) {
    //     // ErrorStrings is @hidden, so we can't do this in AwContents.
    //     // Normally the net/ layer will set a valid description, but for synthesized callbacks
    //     // (like in the case for intercepted requests) AwContents will pass in null.
    //     description = ErrorStrings.getString(errorCode, mWebView.getContext());
    // }
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "onReceivedError=" + failingUrl);
    // mWebViewClient.onReceivedError(mWebView, errorCode, description, failingUrl);
    // TraceEvent.end();

    assert(0);
    String descriptionTmp = description;
    if (descriptionTmp.IsEmpty()) {
        // ErrorStrings is @hidden, so we can't do this in AwContents.
        // Normally the net/ layer will set a valid description, but for synthesized callbacks
        // (like in the case for intercepted requests) AwContents will pass in null.
        //descriptionTmp = ErrorStrings.getString(errorCode, mWebView.getContext());
    }

    TraceEvent::Begin();
    if (TRACE) {
        //Logger::D(TAG, "onReceivedError=" + failingUrl);
    }

    //mWebViewClient->OnReceivedError(mWebView, errorCode, description, failingUrl);
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnReceivedTitle(
    /* [in] */ const String& title)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "onReceivedTitle");
    //     mWebChromeClient.onReceivedTitle(mWebView, title);
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "onReceivedTitle");
        }
        //mWebChromeClient->OnReceivedTitle(mWebView, title);
    }

    TraceEvent::End();
    return NOERROR;
}

Boolean WebViewContentsClientAdapter::ShouldOverrideKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    // ==================before translated======================
    // // The check below is reflecting Clank's behavior and is a workaround for http://b/7697782.
    // // 1. The check for system key should be made in AwContents or ContentViewCore, before
    // //    shouldOverrideKeyEvent() is called at all.
    // // 2. shouldOverrideKeyEvent() should be called in onKeyDown/onKeyUp, not from
    // //    dispatchKeyEvent().
    // if (!ContentViewClient.shouldPropagateKey(event.getKeyCode())) return true;
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "shouldOverrideKeyEvent");
    // boolean result = mWebViewClient.shouldOverrideKeyEvent(mWebView, event);
    // TraceEvent.end();
    // return result;

    assert(0);
    Int32 keyCode = 0;
    event->GetKeyCode(&keyCode);
    if (!ContentViewClient::ShouldPropagateKey(keyCode))
        return TRUE;

    TraceEvent::Begin();
    if (TRACE) {
        //Logger::D(TAG, "shouldOverrideKeyEvent");
    }

    Boolean result = FALSE;//mWebViewClient->ShouldOverrideKeyEvent(mWebView, event);
    TraceEvent::End();
    return result;
}

ECode WebViewContentsClientAdapter::OnStartContentIntent(
    /* [in] */ IContext* context,
    /* [in] */ const String& contentUrl)
{
    VALIDATE_NOT_NULL(context);
    // ==================before translated======================
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "shouldOverrideUrlLoading=" + contentUrl);
    // mWebViewClient.shouldOverrideUrlLoading(mWebView, contentUrl);
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (TRACE) {
        //Logger::D(TAG, "shouldOverrideUrlLoading=" + contentUrl);
    }

    //mWebViewClient->ShouldOverrideUrlLoading(mWebView, contentUrl);
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnGeolocationPermissionsShowPrompt(
    /* [in] */ const String& origin,
    /* [in] */ IInterface/*GeolocationPermissions::Callback*/* callback)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "onGeolocationPermissionsShowPrompt");
    //     mWebChromeClient.onGeolocationPermissionsShowPrompt(origin, callback);
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "onGeolocationPermissionsShowPrompt");
        }
        //mWebChromeClient->OnGeolocationPermissionsShowPrompt(origin, callback);
    }

    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnGeolocationPermissionsHidePrompt()
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "onGeolocationPermissionsHidePrompt");
    //     mWebChromeClient.onGeolocationPermissionsHidePrompt();
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "onGeolocationPermissionsHidePrompt");
        }
        //mWebChromeClient->OnGeolocationPermissionsHidePrompt();
    }

    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnPermissionRequest(
    /* [in] */ AwPermissionRequest* permissionRequest)
{
    VALIDATE_NOT_NULL(permissionRequest);
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "onPermissionRequest");
    //     if (mOngoingPermissionRequests == null) {
    //         mOngoingPermissionRequests =
    //             new WeakHashMap<AwPermissionRequest, WeakReference<PermissionRequestAdapter>>();
    //     }
    //     PermissionRequestAdapter adapter = new PermissionRequestAdapter(permissionRequest);
    //     mOngoingPermissionRequests.put(
    //             permissionRequest, new WeakReference<PermissionRequestAdapter>(adapter));
    //     mWebChromeClient.onPermissionRequest(adapter);
    // } else {
    //     // By default, we deny the permission.
    //     permissionRequest.deny();
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        if (TRACE) {
                //Logger::D(TAG, "onPermissionRequest");
        }

        if (NULL == mOngoingPermissionRequests) {
            CWeakHashMap::New((IWeakHashMap**)&mOngoingPermissionRequests);
        }

        AutoPtr<PermissionRequestAdapter> adapter = new PermissionRequestAdapter(permissionRequest);

        AutoPtr<IInterface> key = TO_IINTERFACE(permissionRequest);

        IObject* adapterTmp = (IObject*)adapter;
        IWeakReferenceSource* source = IWeakReferenceSource::Probe(adapterTmp);
        AutoPtr<IWeakReference> wr;
        source->GetWeakReference((IWeakReference**)&wr);
        AutoPtr<IInterface> value = TO_IINTERFACE(wr);

        mOngoingPermissionRequests->Put(key, value);
        //mWebChromeClient->OnPermissionRequest(adapter);
    }
    else {
        // By default, we deny the permission.
        permissionRequest->Deny();
    }

    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnPermissionRequestCanceled(
    /* [in] */ AwPermissionRequest* permissionRequest)
{
    VALIDATE_NOT_NULL(permissionRequest);
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null && mOngoingPermissionRequests != null) {
    //     if (TRACE) Log.d(TAG, "onPermissionRequestCanceled");
    //     WeakReference<PermissionRequestAdapter> weakRef =
    //             mOngoingPermissionRequests.get(permissionRequest);
    //     // We don't hold strong reference to PermissionRequestAdpater and don't expect the
    //     // user only holds weak reference to it either, if so, user has no way to call
    //     // grant()/deny(), and no need to be notified the cancellation of request.
    //     if (weakRef != null) {
    //         PermissionRequestAdapter adapter = weakRef.get();
    //         if (adapter != null) mWebChromeClient.onPermissionRequestCanceled(adapter);
    //     }
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL && mOngoingPermissionRequests != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "onPermissionRequestCanceled");
        }

        AutoPtr<IInterface> key = TO_IINTERFACE(permissionRequest);
        AutoPtr<IInterface> value;
        mOngoingPermissionRequests->Get(TO_IINTERFACE(permissionRequest), (IInterface**)&value);
        AutoPtr<IWeakReference> weakRef = IWeakReference::Probe(value);

        // We don't hold strong reference to PermissionRequestAdpater and don't expect the
        // user only holds weak reference to it either, if so, user has no way to call
        // grant()/deny(), and no need to be notified the cancellation of request.
        if (weakRef != NULL) {
            AutoPtr<IInterface> strongObj;
            weakRef->Resolve(EIID_IInterface, (IInterface**)&strongObj);
            IObject* strongObjTmp = (IObject*)strongObj.Get();
            PermissionRequestAdapter* adapter = (PermissionRequestAdapter*)strongObjTmp;

            if (adapter != NULL) {
                //mWebChromeClient->OnPermissionRequestCanceled(adapter);
            }
        }
    }

    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::HandleJsAlert(
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [in] */ JsResultReceiver* receiver)
{
    VALIDATE_NOT_NULL(receiver);
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     final JsPromptResult res =
    //             new JsPromptResultReceiverAdapter(receiver).getPromptResult();
    //     if (TRACE) Log.d(TAG, "onJsAlert");
    //     if (!mWebChromeClient.onJsAlert(mWebView, url, message, res)) {
    //         new JsDialogHelper(res, JsDialogHelper.ALERT, null, message, url)
    //                 .showDialog(mWebView.getContext());
    //     }
    // } else {
    //     receiver.cancel();
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        AutoPtr<IInterface/*JsPromptResult*/> res;// = new JsPromptResultReceiverAdapter(receiver).getPromptResult();
        if (TRACE) {
            //Logger::D(TAG, "onJsAlert");
        }

        if (FALSE/*!mWebChromeClient->OnJsAlert(mWebView, url, message, res)*/) {
            //new JsDialogHelper(res, JsDialogHelper.ALERT, null, message, url)
            //        .showDialog(mWebView.getContext());
        }
    }
    else {
        receiver->Cancel();
    }

    TraceEvent::End();
    return NOERROR;
}

void WebViewContentsClientAdapter::HandleJsBeforeUnload(
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [in] */ JsResultReceiver* receiver)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     final JsPromptResult res =
    //             new JsPromptResultReceiverAdapter(receiver).getPromptResult();
    //     if (TRACE) Log.d(TAG, "onJsBeforeUnload");
    //     if (!mWebChromeClient.onJsBeforeUnload(mWebView, url, message, res)) {
    //         new JsDialogHelper(res, JsDialogHelper.UNLOAD, null, message, url)
    //                 .showDialog(mWebView.getContext());
    //     }
    // } else {
    //     receiver.cancel();
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        AutoPtr<IInterface/*JsPromptResult*/> res;// = new JsPromptResultReceiverAdapter(receiver).getPromptResult();
        if (TRACE) {
            //Logger::D(TAG, "onJsBeforeUnload");
        }

        if (FALSE/*!mWebChromeClient->OnJsBeforeUnload(mWebView, url, message, res)*/) {
            //new JsDialogHelper(res, JsDialogHelper.UNLOAD, null, message, url)
            //        .showDialog(mWebView.getContext());
        }
    }
    else {
        receiver->Cancel();
    }

    TraceEvent::End();
}

void WebViewContentsClientAdapter::HandleJsConfirm(
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [in] */ JsResultReceiver* receiver)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     final JsPromptResult res =
    //             new JsPromptResultReceiverAdapter(receiver).getPromptResult();
    //     if (TRACE) Log.d(TAG, "onJsConfirm");
    //     if (!mWebChromeClient.onJsConfirm(mWebView, url, message, res)) {
    //         new JsDialogHelper(res, JsDialogHelper.CONFIRM, null, message, url)
    //                 .showDialog(mWebView.getContext());
    //     }
    // } else {
    //     receiver.cancel();
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        AutoPtr<IInterface/*JsPromptResult*/> res;// = new JsPromptResultReceiverAdapter(receiver).getPromptResult();
        if (TRACE) {
            //Logger::D(TAG, "onJsConfirm");
        }

        if (FALSE/*!mWebChromeClient->OnJsConfirm(mWebView, url, message, res)*/) {
            //new JsDialogHelper(res, JsDialogHelper.CONFIRM, null, message, url)
            //        .showDialog(mWebView.getContext());
        }
    }
    else {
        receiver->Cancel();
    }

    TraceEvent::End();
}

void WebViewContentsClientAdapter::HandleJsPrompt(
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [in] */ const String& defaultValue,
    /* [in] */ JsPromptResultReceiver* receiver)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     final JsPromptResult res =
    //             new JsPromptResultReceiverAdapter(receiver).getPromptResult();
    //     if (TRACE) Log.d(TAG, "onJsPrompt");
    //     if (!mWebChromeClient.onJsPrompt(mWebView, url, message, defaultValue, res)) {
    //         new JsDialogHelper(res, JsDialogHelper.PROMPT, defaultValue, message, url)
    //                 .showDialog(mWebView.getContext());
    //     }
    // } else {
    //     receiver.cancel();
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        AutoPtr<IInterface/*JsPromptResult*/> res;// = new JsPromptResultReceiverAdapter(receiver).getPromptResult();
        if (TRACE) {
            //Logger::D(TAG, "onJsConfirm");
        }

        if (FALSE/*!mWebChromeClient.onJsPrompt(mWebView, url, message, defaultValue, res)*/) {
            //new JsDialogHelper(res, JsDialogHelper.PROMPT, defaultValue, message, url)
            //        .showDialog(mWebView.getContext());
        }
    }
    else {
        receiver->Cancel();
    }

    TraceEvent::End();
}

ECode WebViewContentsClientAdapter::OnReceivedHttpAuthRequest(
    /* [in] */ AwHttpAuthHandler* handler,
    /* [in] */ const String& host,
    /* [in] */ const String& realm)
{
    VALIDATE_NOT_NULL(handler);
    // ==================before translated======================
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "onReceivedHttpAuthRequest=" + host);
    // mWebViewClient.onReceivedHttpAuthRequest(mWebView,
    //         new AwHttpAuthHandlerAdapter(handler), host, realm);
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (TRACE) {
        //Logger::D(TAG, "onReceivedHttpAuthRequest=" + host);
    }

    AutoPtr<AwHttpAuthHandlerAdapter> adapter = new AwHttpAuthHandlerAdapter(handler);
    //mWebViewClient->OnReceivedHttpAuthRequest(mWebView, adapter, host, realm);
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnReceivedSslError(
    /* [in] */ IInterface/*IValueCallback*/* callback,
    /* [in] */ ISslError* error)
{
    VALIDATE_NOT_NULL(callback);
    VALIDATE_NOT_NULL(error);
    // ==================before translated======================
    // SslErrorHandler handler = new SslErrorHandler() {
    //     @Override
    //     public void proceed() {
    //         callback.onReceiveValue(true);
    //     }
    //     @Override
    //     public void cancel() {
    //         callback.onReceiveValue(false);
    //     }
    // };
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "onReceivedSslError");
    // mWebViewClient.onReceivedSslError(mWebView, handler, error);
    // TraceEvent.end();

    assert(0);
    AutoPtr<InnerSslErrorHandler> handler = new InnerSslErrorHandler(callback);
    TraceEvent::Begin();
    if (TRACE) {
        //Logger::D(TAG, "onReceivedSslError");
    }

    //mWebViewClient->OnReceivedSslError(mWebView, handler, error);
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnReceivedClientCertRequest(
    /* [in] */ AwContentsClientBridge::ClientCertificateRequestCallback* callback,
    /* [in] */ ArrayOf<String>* keyTypes,
    /* [in] */ ArrayOf<IPrincipal*>* principals,
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    VALIDATE_NOT_NULL(callback);
    VALIDATE_NOT_NULL(keyTypes);
    VALIDATE_NOT_NULL(principals);
    // ==================before translated======================
    // if (TRACE) Log.d(TAG, "onReceivedClientCertRequest");
    // TraceEvent.begin();
    // final ClientCertRequestImpl request = new ClientCertRequestImpl(callback,
    //     keyTypes, principals, host, port);
    // mWebViewClient.onReceivedClientCertRequest(mWebView, request);
    // TraceEvent.end();

    assert(0);
    if (TRACE) {
        //Logger::D(TAG, "onReceivedClientCertRequest");
    }

    TraceEvent::Begin();
    AutoPtr<ClientCertRequestImpl> request = new ClientCertRequestImpl(callback, keyTypes, principals, host, port);
    //mWebViewClient->OnReceivedClientCertRequest(mWebView, request);
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnReceivedLoginRequest(
    /* [in] */ const String& realm,
    /* [in] */ const String& account,
    /* [in] */ const String& args)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "onReceivedLoginRequest=" + realm);
    // mWebViewClient.onReceivedLoginRequest(mWebView, realm, account, args);
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (TRACE) {
        //Logger::D(TAG, "onReceivedLoginRequest=" + realm);
    }
    //mWebViewClient->OnReceivedLoginRequest(mWebView, realm, account, args);
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnFormResubmission(
    /* [in] */ IMessage* dontResend,
    /* [in] */ IMessage* resend)
{
    VALIDATE_NOT_NULL(dontResend);
    VALIDATE_NOT_NULL(resend);
    // ==================before translated======================
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, "onFormResubmission");
    // mWebViewClient.onFormResubmission(mWebView, dontResend, resend);
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (TRACE) {
        //Logger::D(TAG, "onFormResubmission");
    }
    //mWebViewClient->OnFormResubmission(mWebView, dontResend, resend);
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnDownloadStart(
    /* [in] */ const String& url,
    /* [in] */ const String& userAgent,
    /* [in] */ const String& contentDisposition,
    /* [in] */ const String& mimeType,
    /* [in] */ Int64 contentLength)
{
    // ==================before translated======================
    // if (mDownloadListener != null) {
    //     TraceEvent.begin();
    //     if (TRACE) Log.d(TAG, "onDownloadStart");
    //     mDownloadListener.onDownloadStart(url,
    //                                       userAgent,
    //                                       contentDisposition,
    //                                       mimeType,
    //                                       contentLength);
    //     TraceEvent.end();
    // }

    assert(0);
    if (mDownloadListener != NULL) {
        TraceEvent::Begin();
        if (TRACE) {
            //Logger::D(TAG, "onDownloadStart");
        }
        //mDownloadListener->OnDownloadStart(url, userAgent, contentDisposition, mimeType, contentLength);
        TraceEvent::End();
    }
    return NOERROR;
}

ECode WebViewContentsClientAdapter::ShowFileChooser(
    /* [in] */ IInterface/*IValueCallback*/* uploadFileCallback,
    /* [in] */ AwContentsClient::FileChooserParams* fileChooserParams)
{
    VALIDATE_NOT_NULL(uploadFileCallback);
    VALIDATE_NOT_NULL(fileChooserParams);
    // ==================before translated======================
    // if (mWebChromeClient == null) {
    //     uploadFileCallback.onReceiveValue(null);
    //     return;
    // }
    // TraceEvent.begin();
    // FileChooserParamsAdapter adapter = new FileChooserParamsAdapter(
    //         fileChooserParams, mWebView.getContext());
    // if (TRACE) Log.d(TAG, "showFileChooser");
    // ValueCallback<IUri[]> callbackAdapter = new ValueCallback<IUri[]>() {
    //     private boolean mCompleted;
    //     @Override
    //     public void onReceiveValue(IUri[] uriList) {
    //         if (mCompleted) {
    //             throw new IllegalStateException("showFileChooser result was already called");
    //         }
    //         mCompleted = true;
    //         const String& s[] = null;
    //         if (uriList != null) {
    //             s = new const String&[uriList.length];
    //             for(int i = 0; i < uriList.length; i++) {
    //                 s[i] = uriList[i].toString();
    //             }
    //         }
    //         uploadFileCallback.onReceiveValue(s);
    //     }
    // };
    // if (mWebChromeClient.onShowFileChooser(mWebView, callbackAdapter, adapter)) {
    //     return;
    // }
    // if (mWebView.getContext().getApplicationInfo().targetSdkVersion >
    //         Build.VERSION_CODES.KITKAT) {
    //     uploadFileCallback.onReceiveValue(null);
    //     return;
    // }
    // ValueCallback<IUri> innerCallback = new ValueCallback<IUri>() {
    //     private boolean mCompleted;
    //     @Override
    //     public void onReceiveValue(IUri uri) {
    //         if (mCompleted) {
    //             throw new IllegalStateException("showFileChooser result was already called");
    //         }
    //         mCompleted = true;
    //         uploadFileCallback.onReceiveValue(
    //                 uri == null ? null : new const String&[] { uri.toString() });
    //     }
    // };
    // if (TRACE) Log.d(TAG, "openFileChooser");
    // mWebChromeClient.openFileChooser(innerCallback, fileChooserParams.acceptTypes,
    //         fileChooserParams.capture ? "*" : "");
    // TraceEvent.end();

    assert(0);
    if (NULL == mWebChromeClient) {
        //uploadFileCallback->OnReceiveValue(NULL);
        return NOERROR;
    }

    TraceEvent::Begin();
    AutoPtr<IContext> context;
    //mWebView->GetContext((IContext**)&context);
    AutoPtr<FileChooserParamsAdapter> adapter = new FileChooserParamsAdapter(fileChooserParams, context);
    if (TRACE) {
        //Logger::D(TAG, "showFileChooser");
    }

    AutoPtr<InnerValueCallback1> callbackAdapter = new InnerValueCallback1(uploadFileCallback);
    if (FALSE/*mWebChromeClient->OnShowFileChooser(mWebView, callbackAdapter, adapter)*/) {
        return NOERROR;
    }

    AutoPtr<IApplicationInfo> applicationInfo;
    context->GetApplicationInfo((IApplicationInfo**)&applicationInfo);
    Int32 targetSdkVersion = 0;
    applicationInfo->GetTargetSdkVersion(&targetSdkVersion);

    if (targetSdkVersion > Build::VERSION_CODES::KITKAT) {
        //uploadFileCallback->OnReceiveValue(NULL);
        return NOERROR;
    }

    AutoPtr<InnerValueCallback2> innerCallback = new InnerValueCallback2(uploadFileCallback);
    if (TRACE) {
        //Logger::D(TAG, "openFileChooser");
    }

    String capture = fileChooserParams->capture ? String("*") : String("");
    //mWebChromeClient->OpenFileChooser(innerCallback, fileChooserParams.acceptTypes, capture);
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnScaleChangedScaled(
    /* [in] */ Float oldScale,
    /* [in] */ Float newScale)
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (TRACE) Log.d(TAG, " onScaleChangedScaled");
    // mWebViewClient.onScaleChanged(mWebView, oldScale, newScale);
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (TRACE) {
        //Logger::D(TAG, " onScaleChangedScaled");
    }

    //mWebViewClient->OnScaleChanged(mWebView, oldScale, newScale);
    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnShowCustomView(
    /* [in] */ IView* view,
    /* [in] */ IInterface/*WebChromeClient::CustomViewCallback*/* cb)
{
    VALIDATE_NOT_NULL(view);
    VALIDATE_NOT_NULL(cb);
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "onShowCustomView");
    //     mWebChromeClient.onShowCustomView(view, cb);
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "onShowCustomView");
        }
        //mWebChromeClient->OnShowCustomView(view, cb);
    }

    TraceEvent::End();
    return NOERROR;
}

ECode WebViewContentsClientAdapter::OnHideCustomView()
{
    // ==================before translated======================
    // TraceEvent.begin();
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "onHideCustomView");
    //     mWebChromeClient.onHideCustomView();
    // }
    // TraceEvent.end();

    assert(0);
    TraceEvent::Begin();
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "onHideCustomView");
        }
        //mWebChromeClient->OnHideCustomView();
    }

    TraceEvent::End();
    return NOERROR;
}

AutoPtr<IBitmap> WebViewContentsClientAdapter::GetDefaultVideoPoster()
{
    // ==================before translated======================
    // TraceEvent.begin();
    // Bitmap result = null;
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "getDefaultVideoPoster");
    //     result = mWebChromeClient.getDefaultVideoPoster();
    // }
    // if (result == null) {
    //     // The ic_media_video_poster icon is transparent so we need to draw it on a gray
    //     // background.
    //     Bitmap poster = BitmapFactory.decodeResource(
    //             mWebView.getContext().getResources(),
    //             R.drawable.ic_media_video_poster);
    //     result = Bitmap.createBitmap(poster.getWidth(), poster.getHeight(), poster.getConfig());
    //     result.eraseColor(Color.GRAY);
    //     Canvas canvas = new Canvas(result);
    //     canvas.drawBitmap(poster, 0f, 0f, null);
    // }
    // TraceEvent.end();
    // return result;

    assert(0);
    TraceEvent::Begin();
    AutoPtr<IBitmap> result = NULL;
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "getDefaultVideoPoster");
        }
        //result = mWebChromeClient->GetDefaultVideoPoster();
    }
    if (NULL == result) {
        // The ic_media_video_poster icon is transparent so we need to draw it on a gray
        // background.
        AutoPtr<IContext> context;
        //mWebView->GetContext((IContext**)&context);
        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);

        AutoPtr<IBitmapFactory> factory;
        CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);

        AutoPtr<IBitmap> poster;
        factory->DecodeResource(resources, -1/*R.drawable.ic_media_video_poster*/, (IBitmap**)&poster);

        Int32 width = 0;
        Int32 height = 0;
        BitmapConfig bitmapConfig = 0;
        poster->GetWidth(&width);
        poster->GetHeight(&height);
        poster->GetConfig(&bitmapConfig);

        AutoPtr<IBitmapHelper> helper;
        //CBitmapHelper::AcquireSingleton((IBitmapHelper**)&helper);
        AutoPtr<IBitmap> bitmap;
        helper->CreateBitmap(width, height, bitmapConfig, (IBitmap**)&result);

        result->EraseColor(IColor::GRAY);
        AutoPtr<ICanvas> canvas;
        CCanvas::New(result, (ICanvas**)&canvas);
        canvas->DrawBitmap(poster, 0.0f, 0.0f, NULL);
    }

    TraceEvent::End();
    return result;
}

AutoPtr<IView> WebViewContentsClientAdapter::GetVideoLoadingProgressView()
{
    // ==================before translated======================
    // TraceEvent.begin();
    // View result;
    // if (mWebChromeClient != null) {
    //     if (TRACE) Log.d(TAG, "getVideoLoadingProgressView");
    //     result = mWebChromeClient.getVideoLoadingProgressView();
    // } else {
    //     result = null;
    // }
    // TraceEvent.end();
    // return result;

    assert(0);
    TraceEvent::Begin();
    AutoPtr<IView> result = NULL;
    if (mWebChromeClient != NULL) {
        if (TRACE) {
            //Logger::D(TAG, "getVideoLoadingProgressView");
        }
        //result = mWebChromeClient->GetVideoLoadingProgressView();
    }

    TraceEvent::End();
    return result;
}

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

