
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//            AwContentsClient::AwWebContentsObserver
//===============================================================

AwContentsClient::AwWebContentsObserver::AwWebContentsObserver(
    /* [in] */ ContentViewCore* contentViewCore)
    : WebContentsObserverAndroid(contentViewCore)
{
}

//@Override
void AwContentsClient::AwWebContentsObserver::DidFinishLoad(
    /* [in] */ Int64 frameId,
    /* [in] */ const String& validatedUrl,
    /* [in] */ Boolean isMainFrame)
{
    String unreachableWebDataUrl = AwContentsStatics::GetUnreachableWebDataUrl();
    Boolean isErrorUrl =
            unreachableWebDataUrl != NULL && unreachableWebDataUrl.Equals(validatedUrl);
    if (isMainFrame && !isErrorUrl) {
        AwContentsClient::OnPageFinished(validatedUrl);
    }
}

//@Override
void AwContentsClient::AwWebContentsObserver::DidFailLoad(
    /* [in] */ Boolean isProvisionalLoad,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
    if (isMainFrame) {
        if (errorCode != NetError::ERR_ABORTED) {
            // This error code is generated for the following reasons:
            // - WebView.stopLoading is called,
            // - the navigation is intercepted by the embedder via shouldOverrideNavigation.
            //
            // The Android WebView does not notify the embedder of these situations using
            // this error code with the WebViewClient.onReceivedError callback.
            AwContentsClient::OnReceivedError(
                    ErrorCodeConversionHelper::ConvertErrorCode(errorCode), description,
                            failingUrl);
        }
        // Need to call onPageFinished after onReceivedError (if there is an error) for
        // backwards compatibility with the classic webview.
        AwContentsClient::OnPageFinished(failingUrl);
    }
}

//@Override
void AwContentsClient::AwWebContentsObserver::DidNavigateMainFrame(
    /* [in] */ const String& url,
    /* [in] */ const String& baseUrl,
    /* [in] */ Boolean isNavigationToDifferentPage, boolean isFragmentNavigation)
{
    // This is here to emulate the Classic WebView firing onPageFinished for main frame
    // navigations where only the hash fragment changes.
    if (isFragmentNavigation) {
        AwContentsClient::OnPageFinished(url);
    }
}

//@Override
void AwContentsClient::AwWebContentsObserver::DidNavigateAnyFrame(
    /* [in] */ const String& url,
    /* [in] */ const String& baseUrl,
    /* [in] */ Boolean isReload)
{
    AwContentsClient::DoUpdateVisitedHistory(url, isReload);
}

//===============================================================
//                      AwContentsClient
//===============================================================

const Int32 AwContentsClient::INVALID_COLOR;

AwContentsClient::AwContentsClient()
{
    AutoPtr<ILooperHelper> helper;
    CLooperHelper::AcquireSingleton((ILooperHelper**)&helper);
    AutoPtr<ILooper> looper;
    helper->MyLooper((ILooper**)&looper);
    Init(looper);
}

// Alllow injection of the callback thread, for testing.
AwContentsClient::AwContentsClient(
    /* [in] */ ILooper* looper)
{
    Init(looper);
}

void AwContentsClient::Init(
    /* [in] */ ILooper* looper)
{
    mCallbackHelper = new AwContentsClientCallbackHelper(looper, this);
    mCachedRendererBackgroundColor = INVALID_COLOR;
}

void AwContentsClient::InstallWebContentsObserver(
    /* [in] */ ContentViewCore* contentViewCore)
{
    if (mWebContentsObserver != NULL) {
        mWebContentsObserver->DetachFromWebContents();
    }
    mWebContentsObserver = new AwWebContentsObserver(contentViewCore);
}

AutoPtr<AwContentsClientCallbackHelper> AwContentsClient::GetCallbackHelper()
{
    return mCallbackHelper;
}

Int32 AwContentsClient::GetCachedRendererBackgroundColor()
{
    assert(isCachedRendererBackgroundColorValid());
    return mCachedRendererBackgroundColor;
}

Boolean AwContentsClient::IsCachedRendererBackgroundColorValid()
{
    return mCachedRendererBackgroundColor != INVALID_COLOR;
}

void AwContentsClient::OnBackgroundColorChanged(
    /* [in] */ Int32 color)
{
    // Avoid storing the sentinal INVALID_COLOR (note that both 0 and 1 are both
    // fully transparent so this transpose makes no visible difference).
    mCachedRendererBackgroundColor = color == INVALID_COLOR ? 1 : color;
}


// TODO(sgurun): Make abstract once this has rolled in downstream.
void AwContentsClient::OnReceivedClientCertRequest(
    /* [in] */ const AwContentsClientBridge::ClientCertificateRequestCallback* callback,
    /* [in] */ const ArrayOf<String>* keyTypes,
    /* [in] */ const ArrayOf<IPrincipal>* principals,
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
}


// TODO(joth): Make abstract once this has rolled in downstream.
void AwContentsClient::ShowFileChooser(
    /* [in] */ IValueCallback* uploadFilePathsCallback,
    /* [in] */ FileChooserParams fileChooserParams)
{
}

// TODO(michaelbai): Change the abstract once merged
void AwContentsClient::OnPermissionRequest(
    /* [in] */ AwPermissionRequest* awPermissionRequest)
{
}

// TODO(michaelbai): Change the abstract once merged
void AwContentsClient::OnPermissionRequestCanceled(
    /* [in] */ AwPermissionRequest* awPermissionRequest)
{
}


// TODO (michaelbai): Remove this method once the same method remove from
// WebViewContentsClientAdapter.
void AwContentsClient::OnShowCustomView(
    /* [in] */ IView* view,
    /* [in] */ Int32 requestedOrientation,
    /* [in] */ IWebChromeClientCustomViewCallback* callback)
{
}

// TODO (michaelbai): This method should be abstract, having empty body here
// makes the merge to the Android easy.
void AwContentsClient::OnShowCustomView(
    /* [in] */ IView* view,
    /* [in] */ IWebChromeClientCustomViewCallback* callback)
{
    OnShowCustomView(view, IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED, callback);
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
