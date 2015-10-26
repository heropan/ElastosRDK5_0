
#include "elastos/droid/webkit/native/content/browser/WebContentsObserverAndroid.h"
#include "elastos/droid/webkit/native/content/browser/ContentViewCore.h"
#include "elastos/droid/webkit/native/base/ThreadUtils.h"

using Elastos::Droid::Webkit::Base::ThreadUtils;

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
    ThreadUtils::AssertOnUiThread();
    mNativeWebContentsObserverAndroid = NativeInit(contentViewCore->GetWebContents());
}

WebContentsObserverAndroid::WebContentsObserverAndroid(
    /* [in] */ WebContents* webContents)
{
    ThreadUtils::AssertOnUiThread();
    mNativeWebContentsObserverAndroid = NativeInit(webContents);
}

ECode WebContentsObserverAndroid::RenderProcessGone(
    /* [in] */ Boolean wasOomProtected)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidStartLoading(
    /* [in] */ const String& url)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidStopLoading(
    /* [in] */ const String& url)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidFailLoad(
    /* [in] */ Boolean isProvisionalLoad,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidNavigateMainFrame(
    /* [in] */ const String& url,
    /* [in] */ const String& baseUrl,
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
    /* [in] */ const String& url,
    /* [in] */ const String& baseUrl,
    /* [in] */ Boolean isReload)
{
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
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidCommitProvisionalLoadForFrame(
    /* [in] */ Int64 frameId,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ const String& url,
    /* [in] */ Int32 transitionType)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidFinishLoad(
    /* [in] */ Int64 frameId,
    /* [in] */ const String& validatedUrl,
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
    if (mNativeWebContentsObserverAndroid != 0) {
        NativeDestroy(mNativeWebContentsObserverAndroid);
        mNativeWebContentsObserverAndroid = 0;
    }

    return NOERROR;
}

Int64 WebContentsObserverAndroid::NativeInit(
    /* [in] */ WebContents* webContents)
{
    return 0;
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
