
#include "webkit/ZoomControlEmbedded.h"
#include "webkit/CWebViewClassic.h"
#include "webkit/WebSettingsClassic.h"
#include "widget/CZoomButtonsController.h"
#include "widget/CToast.h"
#include "R.h"

using Elastos::Droid::View::IView;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::Widget::CZoomButtonsController;
using Elastos::Droid::Widget::EIID_IOnZoomListener;
using Elastos::Droid::Widget::IFrameLayoutLayoutParams;
using Elastos::Droid::Widget::CToast;
using Elastos::Droid::Widget::IToast;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//             ZoomControlEmbedded::ZoomListener
//===============================================================
ZoomControlEmbedded::ZoomListener::ZoomListener(
    /* [in] */ ZoomControlEmbedded* owner)
       : mOwner(owner)
{}

CAR_INTERFACE_IMPL_LIGHT(ZoomControlEmbedded::ZoomListener, IOnZoomListener);

ECode ZoomControlEmbedded::ZoomListener::OnVisibilityChanged(
    /* [in] */ Boolean visible)
{
    if (visible) {
        mOwner->mWebView->SwitchOutDrawHistory();
        // Bring back the hidden zoom controls.
        AutoPtr<IView> v;
        mOwner->mZoomButtonsController->GetZoomControls((IView**)&v);
        v->SetVisibility(IView::VISIBLE);
        mOwner->Update();
    }
    return NOERROR;
}

ECode ZoomControlEmbedded::ZoomListener::OnZoom(
    /* [in] */ Boolean zoomIn)
{
    if (zoomIn) {
        Boolean result;
        mOwner->mWebView->ZoomIn(&result);
    }
    else {
        Boolean result;
        mOwner->mWebView->ZoomOut(&result);
    }
    return mOwner->Update();
}


//===============================================================
//                    ZoomControlEmbedded
//===============================================================
ZoomControlEmbedded::ZoomControlEmbedded(
    /* [in] */ ZoomManager* zoomManager,
    /* [in] */ CWebViewClassic* webView)
      : mZoomManager(zoomManager)
      , mWebView(webView)
{}

PInterface ZoomControlEmbedded::Probe(
    /* [in] */ REIID riid)
{
    assert(0);
    return NULL;
}

UInt32 ZoomControlEmbedded::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ZoomControlEmbedded::Release()
{
    return ElRefBase::Release();
}

ECode ZoomControlEmbedded::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    assert(0);
    return NOERROR;
}

ECode ZoomControlEmbedded::Show()
{
    Boolean isVisible;
    if ((GetControls()->IsVisible(&isVisible), !isVisible) && !mZoomManager->IsZoomScaleFixed()) {
        mZoomButtonsController->SetVisible(TRUE);

        if (mZoomManager->IsDoubleTapEnabled()) {
            AutoPtr<IWebSettings> _settings;
            mWebView->GetSettings((IWebSettings**)&_settings);
            WebSettingsClassic* settings = (WebSettingsClassic*)IWebSettingsClassic::Probe(_settings);
            Int32 count;
            settings->GetDoubleTapToastCount(&count);
            if (mZoomManager->IsInZoomOverview() && count > 0) {
                settings->SetDoubleTapToastCount(--count);
                AutoPtr<IToast> toast;
                CToast::MakeText(mWebView->GetContext(), R::string::double_tap_toast,
                        IToast::LENGTH_LONG, (IToast**)&toast);
                toast->Show();
            }
        }
    }
    return NOERROR;
}

ECode ZoomControlEmbedded::Hide()
{
    if (mZoomButtonsController != NULL) {
        mZoomButtonsController->SetVisible(FALSE);
    }
    return NOERROR;
}

ECode ZoomControlEmbedded::IsVisible(
    /* [out] */ Boolean* isVisible)
{
    VALIDATE_NOT_NULL(isVisible);
    if (mZoomButtonsController == NULL) {
        *isVisible = FALSE;
        return NOERROR;
    }
    return mZoomButtonsController->IsVisible(isVisible);
}

ECode ZoomControlEmbedded::Update()
{
    if (mZoomButtonsController == NULL) {
        return NOERROR;
    }

    Boolean canZoomIn = mZoomManager->CanZoomIn();
    Boolean canZoomOut = mZoomManager->CanZoomOut() && !mZoomManager->IsInZoomOverview();
    if (!canZoomIn && !canZoomOut) {
        // Hide the zoom in and out buttons if the page cannot zoom
        AutoPtr<IView> v;
        mZoomButtonsController->GetZoomControls((IView**)&v);
        v->SetVisibility(IView::GONE);
    }
    else {
        // Set each one individually, as a page may be able to zoom in or out
        mZoomButtonsController->SetZoomInEnabled(canZoomIn);
        mZoomButtonsController->SetZoomOutEnabled(canZoomOut);
    }
    return NOERROR;
}

AutoPtr<IZoomButtonsController> ZoomControlEmbedded::GetControls()
{
    if (mZoomButtonsController == NULL) {
        AutoPtr<IWebView> webView;
        mWebView->GetWebView((IWebView**)&webView);
        CZoomButtonsController::New(webView, (IZoomButtonsController**)&mZoomButtonsController);
        AutoPtr<IOnZoomListener> ls = new ZoomListener(this);
        mZoomButtonsController->SetOnZoomListener(ls);
        // ZoomButtonsController positions the buttons at the bottom, but in
        // the middle. Change their layout parameters so they appear on the
        // right.
        AutoPtr<IView> controls;
        mZoomButtonsController->GetZoomControls((IView**)&controls);
        AutoPtr<IViewGroupLayoutParams> params;
        controls->GetLayoutParams((IViewGroupLayoutParams**)&params);
       if (IFrameLayoutLayoutParams::Probe(params)) {
           IFrameLayoutLayoutParams::Probe(params)->SetGravity(IGravity::END);
       }
    }
    return mZoomButtonsController;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
