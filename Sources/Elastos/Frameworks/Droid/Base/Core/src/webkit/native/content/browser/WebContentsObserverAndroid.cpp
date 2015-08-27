// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                      WebContentsObserverAndroid
//=====================================================================
WebContentsObserverAndroid::WebContentsObserverAndroid(
    /* [in] */ ContentViewCore* contentViewCore)
{
    /*
    this(contentViewCore.getWebContents());
    */
}

WebContentsObserverAndroid::WebContentsObserverAndroid(
    /* [in] */ WebContents* webContents)
{
    /*
    ThreadUtils.assertOnUiThread();
    mNativeWebContentsObserverAndroid = nativeInit(webContents);
    */
}

ECode WebContentsObserverAndroid::RenderProcessGone(
    /* [in] */ Boolean wasOomProtected)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidStartLoading(
    /* [in] */ String url)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidStopLoading(
    /* [in] */ String url)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidFailLoad(
    /* [in] */ Boolean isProvisionalLoad,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ Int32 errorCode,
    /* [in] */ String description,
    /* [in] */ String failingUrl)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidNavigateMainFrame(
    /* [in] */ String url,
    /* [in] */ String baseUrl,
    /* [in] */ Boolean isNavigationToDifferentPage,
    /* [in] */ Boolean isFragmentNavigation)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidFirstVisuallyNonEmptyPaint()
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidNavigateAnyFrame(
    /* [in] */ String url,
    /* [in] */ String baseUrl,
    /* [in] */ Boolean isReload)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidStartProvisionalLoadForFrame(
    /* [in] */ Int64 frameId,
    /* [in] */ Int64 parentFrameId,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ String validatedUrl,
    /* [in] */ Boolean isErrorPage,
    /* [in] */ Boolean isIframeSrcdoc)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidCommitProvisionalLoadForFrame(
    /* [in] */ Int64 frameId,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ String url,
    /* [in] */ Int32 transitionType)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidFinishLoad(
    /* [in] */ Int64 frameId,
    /* [in] */ String validatedUrl,
    /* [in] */ Boolean isMainFrame)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DocumentLoadedInFrame(
    /* [in] */ Int64 frameId)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::NavigationEntryCommitted()
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidAttachInterstitialPage()
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidDetachInterstitialPage()
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidChangeThemeColor(
    /* [in] */ Int32 color)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DetachFromWebContents()
{
    /*
    if (mNativeWebContentsObserverAndroid != 0) {
        nativeDestroy(mNativeWebContentsObserverAndroid);
        mNativeWebContentsObserverAndroid = 0;
    }
    */
    return NOERROR;
}

Int64 WebContentsObserverAndroid::NativeInit(
    /* [in] */ WebContents* webContents)
{
}

ECode WebContentsObserverAndroid::NativeDestroy(
    /* [in] */ Int64 nativeWebContentsObserverAndroid)
{
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


