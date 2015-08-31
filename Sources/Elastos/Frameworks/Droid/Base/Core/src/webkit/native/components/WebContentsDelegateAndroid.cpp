// wuweizuo automatic build .cpp file from .java file.

#include "WebContentsDelegateAndroid.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Components {

//=====================================================================
//                      WebContentsDelegateAndroid
//=====================================================================
const Int32 WebContentsDelegateAndroid::LOG_LEVEL_TIP;
const Int32 WebContentsDelegateAndroid::LOG_LEVEL_LOG;
const Int32 WebContentsDelegateAndroid::LOG_LEVEL_WARNING;
const Int32 WebContentsDelegateAndroid::LOG_LEVEL_ERROR;
const Int32 WebContentsDelegateAndroid::INVALIDATE_TYPE_URL;
const Int32 WebContentsDelegateAndroid::INVALIDATE_TYPE_TAB;
const Int32 WebContentsDelegateAndroid::INVALIDATE_TYPE_LOAD;
const Int32 WebContentsDelegateAndroid::INVALIDATE_TYPE_PAGE_ACTIONS;
const Int32 WebContentsDelegateAndroid::INVALIDATE_TYPE_TITLE;

Int32 WebContentsDelegateAndroid::GetMostRecentProgress()
{
    // ==================before translated======================
    // return mMostRecentProgress;
    return 0;
}

ECode WebContentsDelegateAndroid::OpenNewTab(
    /* [in] */ String url,
    /* [in] */ String extraHeaders,
    /* [in] */ ArrayOf<Byte>* postData,
    /* [in] */ Int32 disposition,
    /* [in] */ Boolean isRendererInitiated)
{
    VALIDATE_NOT_NULL(postData);
    return NOERROR;
}

ECode WebContentsDelegateAndroid::ActivateContents()
{
    return NOERROR;
}

ECode WebContentsDelegateAndroid::CloseContents()
{
    return NOERROR;
}

ECode WebContentsDelegateAndroid::OnLoadStarted()
{
    return NOERROR;
}

ECode WebContentsDelegateAndroid::OnLoadStopped()
{
    return NOERROR;
}

ECode WebContentsDelegateAndroid::NavigationStateChanged(
    /* [in] */ Int32 flags)
{
    return NOERROR;
}

ECode WebContentsDelegateAndroid::VisibleSSLStateChanged()
{
    return NOERROR;
}

ECode WebContentsDelegateAndroid::OnLoadProgressChanged(
    /* [in] */ Int32 progress)
{
    return NOERROR;
}

ECode WebContentsDelegateAndroid::RendererUnresponsive()
{
    return NOERROR;
}

ECode WebContentsDelegateAndroid::RendererResponsive()
{
    return NOERROR;
}

ECode WebContentsDelegateAndroid::OnUpdateUrl(
    /* [in] */ String url)
{
    return NOERROR;
}

Boolean WebContentsDelegateAndroid::TakeFocus(
    /* [in] */ Boolean reverse)
{
    // ==================before translated======================
    // return false;
    return FALSE;
}

ECode WebContentsDelegateAndroid::HandleKeyboardEvent(
    /* [in] */ IKeyEvent* event)
{
    VALIDATE_NOT_NULL(event);
    // ==================before translated======================
    // // TODO(bulach): we probably want to re-inject the KeyEvent back into
    // // the system. Investigate if this is at all possible.
    return NOERROR;
}

Boolean WebContentsDelegateAndroid::AddMessageToConsole(
    /* [in] */ Int32 level,
    /* [in] */ String message,
    /* [in] */ Int32 lineNumber,
    /* [in] */ String sourceId)
{
    // ==================before translated======================
    // return false;
    return FALSE;
}

ECode WebContentsDelegateAndroid::ShowRepostFormWarningDialog(
    /* [in] */ ContentViewCore* contentViewCore)
{
    VALIDATE_NOT_NULL(contentViewCore);
    return NOERROR;
}

ECode WebContentsDelegateAndroid::ToggleFullscreenModeForTab(
    /* [in] */ Boolean enterFullscreen)
{
    return NOERROR;
}

Boolean WebContentsDelegateAndroid::IsFullscreenForTabOrPending()
{
    // ==================before translated======================
    // return false;
    return FALSE;
}

const ECode WebContentsDelegateAndroid::NotifyLoadProgressChanged(
    /* [in] */ Double progress)
{
    // ==================before translated======================
    // mMostRecentProgress = (int) (100.0 * progress);
    // onLoadProgressChanged(mMostRecentProgress);
    return NOERROR;
}

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


