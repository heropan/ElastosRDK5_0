
#include "webkit/PluginFullScreenHolder.h"
#include "webkit/CWebViewClassic.h"
#include "widget/CFrameLayoutLayoutParams.h"

using Elastos::Droid::Widget::IFrameLayoutLayoutParams;
using Elastos::Droid::Widget::CFrameLayoutLayoutParams;
using Elastos::Droid::Widget::EIID_IFrameLayout;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::ISurfaceView;
using Elastos::Droid::View::ISurfaceHolder;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//       PluginFullScreenHolder::InnerCustomFrameLayout
//===============================================================

PluginFullScreenHolder::InnerCustomFrameLayout::InnerCustomFrameLayout(
    /* [in] */ IContext* context)
    : FrameLayout(context)
{
}

CAR_INTERFACE_IMPL_LIGHT(PluginFullScreenHolder::InnerCustomFrameLayout, IFrameLayout);

IVIEW_METHODS_IMPL(PluginFullScreenHolder::InnerCustomFrameLayout, FrameLayout);
IVIEWGROUP_METHODS_IMPL(PluginFullScreenHolder::InnerCustomFrameLayout, FrameLayout);
IFRAMELAYOUT_METHODS_IMPL(PluginFullScreenHolder::InnerCustomFrameLayout, FrameLayout);

//===============================================================
//          PluginFullScreenHolder::CustomFrameLayout
//===============================================================

PluginFullScreenHolder::CustomFrameLayout::CustomFrameLayout(
    /* [in] */ PluginFullScreenHolder* owner,
    /* [in] */ IContext* context)
    : mOwner(owner)
    , InnerCustomFrameLayout(context)
{
}

ECode PluginFullScreenHolder::CustomFrameLayout::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    Boolean bFlag = FALSE;
    event->IsSystem(&bFlag);
    if (bFlag) {
        return InnerCustomFrameLayout::OnKeyDown(keyCode, event);
    }
    Boolean keyDown = FALSE;
    mOwner->mWebView->OnKeyDown(keyCode, event, &keyDown);
    // always return true as we are the handler
    if (result) *result = TRUE;

    return NOERROR;
}

ECode PluginFullScreenHolder::CustomFrameLayout::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    Boolean bFlag = FALSE;
    event->IsSystem(&bFlag);
    if (bFlag) {
        return InnerCustomFrameLayout::OnKeyUp(keyCode, event);
    }

    Boolean keyUp = FALSE;
    mOwner->mWebView->OnKeyUp(keyCode, event, &keyUp);
    // always return true as we are the handler
    if (result) *result = TRUE;

    return NOERROR;
}

ECode PluginFullScreenHolder::CustomFrameLayout::OnTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    // always return true as we don't want the event to propagate any further
    if (result) *result = TRUE;

    return NOERROR;
}

ECode PluginFullScreenHolder::CustomFrameLayout::OnTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    Boolean bFlag = FALSE;
    mOwner->mWebView->OnTrackballEvent(event, &bFlag);
    // always return true as we are the handler
    if (result) *result = TRUE;

    return NOERROR;
}

//===============================================================
//                   PluginFullScreenHolder
//===============================================================

// The container for the plugin view
AutoPtr<PluginFullScreenHolder::CustomFrameLayout> PluginFullScreenHolder::mLayout;

PluginFullScreenHolder::PluginFullScreenHolder(
    /* [in] */ CWebViewClassic* webView,
    /* [in] */ Int32 orientation,
    /* [in] */ Int32 npp)
       : mWebView(webView)
       , mNpp(npp)
       , mOrientation(orientation)
{
}

void PluginFullScreenHolder::SetContentView(
    /* [in] */ IView* contentView)
{
    assert(0);
    // Create a FrameLayout that will contain the plugin's view
//    mLayout = new CustomFrameLayout(mWebView->GetContext());

    AutoPtr<IFrameLayoutLayoutParams> layoutParams;
    CFrameLayoutLayoutParams::New(
                        IViewGroupLayoutParams::MATCH_PARENT,
                        IViewGroupLayoutParams::MATCH_PARENT,
                        IGravity::CENTER,
                        (IFrameLayoutLayoutParams**)&layoutParams);

//    mLayout->AddView(contentView, layoutParams);
    mLayout->SetVisibility(IView::VISIBLE);

    // fixed size is only used either during pinch zoom or surface is too
    // big. Make sure it is not fixed size before setting it to the full
    // screen content view. The SurfaceView will be set to the correct mode
    // by the ViewManager when it is re-attached to the WebView.
    if (/*contentView instanceof SurfaceView*/FALSE) {
        AutoPtr<ISurfaceView> sView;// = (SurfaceView) contentView;
        Boolean bFlag = FALSE;
        sView->IsFixedSize(&bFlag);
        if (bFlag) {
            AutoPtr<ISurfaceHolder> holder;
            sView->GetHolder((ISurfaceHolder**)&holder);
            holder->SetSizeFromLayout();
        }
    }

    mContentView = contentView;
}

void PluginFullScreenHolder::Show()
{
    assert(0);
    // Other plugins may attempt to draw so hide them while we're active.
    if (mWebView->GetViewManager() != NULL) {
//        mWebView->GetViewManager()->HideAll();
    }

    AutoPtr<IWebChromeClient> client;
    mWebView->GetWebChromeClient((IWebChromeClient**)&client);
//    client->OnShowCustomView(mLayout, mOrientation, mCallback);
}

void PluginFullScreenHolder::Hide()
{
    AutoPtr<IWebChromeClient> client;
    mWebView->GetWebChromeClient((IWebChromeClient**)&client);
    client->OnHideCustomView();
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
