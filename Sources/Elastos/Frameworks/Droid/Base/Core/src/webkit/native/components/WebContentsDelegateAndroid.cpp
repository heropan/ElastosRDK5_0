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
    assert(0);
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
    assert(0);
    return NOERROR;
}

ECode WebContentsDelegateAndroid::ActivateContents()
{
    assert(0);
    return NOERROR;
}

ECode WebContentsDelegateAndroid::CloseContents()
{
    assert(0);
    return NOERROR;
}

ECode WebContentsDelegateAndroid::OnLoadStarted()
{
    assert(0);
    return NOERROR;
}

ECode WebContentsDelegateAndroid::OnLoadStopped()
{
    assert(0);
    return NOERROR;
}

ECode WebContentsDelegateAndroid::NavigationStateChanged(
    /* [in] */ Int32 flags)
{
    assert(0);
    return NOERROR;
}

ECode WebContentsDelegateAndroid::VisibleSSLStateChanged()
{
    assert(0);
    return NOERROR;
}

ECode WebContentsDelegateAndroid::OnLoadProgressChanged(
    /* [in] */ Int32 progress)
{
    assert(0);
    return NOERROR;
}

ECode WebContentsDelegateAndroid::RendererUnresponsive()
{
    assert(0);
    return NOERROR;
}

ECode WebContentsDelegateAndroid::RendererResponsive()
{
    assert(0);
    return NOERROR;
}

ECode WebContentsDelegateAndroid::OnUpdateUrl(
    /* [in] */ String url)
{
    assert(0);
    return NOERROR;
}

Boolean WebContentsDelegateAndroid::TakeFocus(
    /* [in] */ Boolean reverse)
{
    // ==================before translated======================
    // return false;
    assert(0);
    return FALSE;
}

ECode WebContentsDelegateAndroid::HandleKeyboardEvent(
    /* [in] */ IKeyEvent* event)
{
    VALIDATE_NOT_NULL(event);
    // ==================before translated======================
    // // TODO(bulach): we probably want to re-inject the KeyEvent back into
    // // the system. Investigate if this is at all possible.
    assert(0);
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
    assert(0);
    return FALSE;
}

ECode WebContentsDelegateAndroid::ShowRepostFormWarningDialog(
    /* [in] */ ContentViewCore* contentViewCore)
{
    VALIDATE_NOT_NULL(contentViewCore);
    assert(0);
    return NOERROR;
}

ECode WebContentsDelegateAndroid::ToggleFullscreenModeForTab(
    /* [in] */ Boolean enterFullscreen)
{
    assert(0);
    return NOERROR;
}

Boolean WebContentsDelegateAndroid::IsFullscreenForTabOrPending()
{
    // ==================before translated======================
    // return false;
    assert(0);
    return FALSE;
}

const ECode WebContentsDelegateAndroid::NotifyLoadProgressChanged(
    /* [in] */ Double progress)
{
    // ==================before translated======================
    // mMostRecentProgress = (int) (100.0 * progress);
    // onLoadProgressChanged(mMostRecentProgress);
    assert(0);
    return NOERROR;
}

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


