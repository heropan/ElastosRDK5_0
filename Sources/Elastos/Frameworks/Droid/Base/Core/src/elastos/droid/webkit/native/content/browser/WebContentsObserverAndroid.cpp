// wuweizuo automatic build .cpp file from .java file.

#include "WebContentsObserverAndroid.h"

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
    // ==================before translated======================
    // this(contentViewCore.getWebContents());
}

WebContentsObserverAndroid::WebContentsObserverAndroid(
    /* [in] */ WebContents* webContents)
{
    // ==================before translated======================
    // ThreadUtils.assertOnUiThread();
    // mNativeWebContentsObserverAndroid = nativeInit(webContents);
}

ECode WebContentsObserverAndroid::RenderProcessGone(
    /* [in] */ Boolean wasOomProtected)
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidStartLoading(
    /* [in] */ const String& url)
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidStopLoading(
    /* [in] */ const String& url)
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidFailLoad(
    /* [in] */ Boolean isProvisionalLoad,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidNavigateMainFrame(
    /* [in] */ const String& url,
    /* [in] */ const String& baseUrl,
    /* [in] */ Boolean isNavigationToDifferentPage,
    /* [in] */ Boolean isFragmentNavigation)
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidFirstVisuallyNonEmptyPaint()
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidNavigateAnyFrame(
    /* [in] */ const String& url,
    /* [in] */ const String& baseUrl,
    /* [in] */ Boolean isReload)
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidStartProvisionalLoadForFrame(
    /* [in] */ Int64 frameId,
    /* [in] */ Int64 parentFrameId,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ const String& validatedUrl,
    /* [in] */ Boolean isErrorPage,
    /* [in] */ Boolean isIframeSrcdoc)
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidCommitProvisionalLoadForFrame(
    /* [in] */ Int64 frameId,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ const String& url,
    /* [in] */ Int32 transitionType)
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidFinishLoad(
    /* [in] */ Int64 frameId,
    /* [in] */ const String& validatedUrl,
    /* [in] */ Boolean isMainFrame)
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DocumentLoadedInFrame(
    /* [in] */ Int64 frameId)
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::NavigationEntryCommitted()
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidAttachInterstitialPage()
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidDetachInterstitialPage()
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidChangeThemeColor(
    /* [in] */ Int32 color)
{
    assert(0);
    return NOERROR;
}

ECode WebContentsObserverAndroid::DetachFromWebContents()
{
    // ==================before translated======================
    // if (mNativeWebContentsObserverAndroid != 0) {
    //     nativeDestroy(mNativeWebContentsObserverAndroid);
    //     mNativeWebContentsObserverAndroid = 0;
    // }
    assert(0);
    return NOERROR;
}

Int64 WebContentsObserverAndroid::NativeInit(
    /* [in] */ WebContents* webContents)
{
    assert(0);
    return 0;
}

ECode WebContentsObserverAndroid::NativeDestroy(
    /* [in] */ Int64 nativeWebContentsObserverAndroid)
{
    assert(0);
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


