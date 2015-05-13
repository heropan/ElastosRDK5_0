
#include "os/CHandler.h"
#include "view/CViewConfiguration.h"
#include "view/animation/CAlphaAnimation.h"
#include "webkit/ZoomControlExternal.h"
#include "webkit/CWebViewClassic.h"
#include "R.h"

using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::View::CViewConfiguration;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::IViewConfigurationHelper;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::Animation::CAlphaAnimation;
using Elastos::Droid::View::Animation::IAlphaAnimation;
using Elastos::Droid::Widget::EIID_IFrameLayout;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::EIID_IView;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//           ZoomControlExternal::ExtendedZoomControls
//===============================================================

ZoomControlExternal::ExtendedZoomControls::ExtendedZoomControls(
    /* [in] */ IContext* context)
    : FrameLayout(context, NULL)
{
    //because this will be transfer to other function(see below)
    //which will store this in a local AutoPtr variable,
    //without this, after the func, the memory for this object is destroyed
    //also remember to add the Release for this, see ZoomControlExternal::CreateZoomControls.
    this->AddRef();
    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);
    AutoPtr<IView> view;
    inflater->InflateEx2(R::layout::zoom_magnify, this, TRUE, (IView**)&view);
    FindViewById(R::id::zoomControls, (IView**)&mPlusMinusZoomControls);
    AutoPtr<IView> v;
    FindViewById(R::id::zoomMagnify, (IView**)&v);
    v->SetVisibility(IView::GONE);
}

CAR_INTERFACE_IMPL_3(ZoomControlExternal::ExtendedZoomControls, IFrameLayout, IViewGroup, IView);

IVIEW_METHODS_IMPL(ZoomControlExternal::ExtendedZoomControls, FrameLayout);
IVIEWGROUP_METHODS_IMPL(ZoomControlExternal::ExtendedZoomControls, FrameLayout);
IFRAMELAYOUT_METHODS_IMPL(ZoomControlExternal::ExtendedZoomControls, FrameLayout);

void ZoomControlExternal::ExtendedZoomControls::Show(
    /* [in] */ Boolean showZoom)
{
    mPlusMinusZoomControls->SetVisibility(showZoom ? IView::VISIBLE : IView::GONE);
    Fade(IView::VISIBLE, 0.0f, 1.0f);
}

void ZoomControlExternal::ExtendedZoomControls::Hide()
{
    Fade(IView::GONE, 1.0f, 0.0f);
}

void ZoomControlExternal::ExtendedZoomControls::Fade(
    /* [in] */ Int32 visibility,
    /* [in] */ Float startAlpha,
    /* [in] */ Float endAlpha)
{
    AutoPtr<IAlphaAnimation> anim;
    CAlphaAnimation::New(startAlpha, endAlpha, (IAlphaAnimation**)&anim);
    anim->SetDuration(500);
    StartAnimation(anim);
    SetVisibility(visibility);
}

Boolean ZoomControlExternal::ExtendedZoomControls::HasFocus()
{
    Boolean hasFocus = FALSE;
    mPlusMinusZoomControls->HasFocus(&hasFocus);
    return hasFocus;
}

void ZoomControlExternal::ExtendedZoomControls::SetOnZoomInClickListener(
    /* [in] */ IViewOnClickListener* listener)
{
    mPlusMinusZoomControls->SetOnZoomInClickListener(listener);
}

void ZoomControlExternal::ExtendedZoomControls::SetOnZoomOutClickListener(
    /* [in] */ IViewOnClickListener* listener)
{
    mPlusMinusZoomControls->SetOnZoomOutClickListener(listener);
}


//===============================================================
//            ZoomControlExternal::ZoomInClickListener
//===============================================================

ZoomControlExternal::ZoomInClickListener::ZoomInClickListener(
    /* [in] */ ZoomControlExternal* owner)
    : mOwner(owner)
{}

CAR_INTERFACE_IMPL_LIGHT(ZoomControlExternal::ZoomInClickListener, IViewOnClickListener);

ECode ZoomControlExternal::ZoomInClickListener::OnClick(
    /* [in] */ IView* v)
{
    // reset time out
    mOwner->mPrivateHandler->RemoveCallbacks(mOwner->mZoomControlRunnable);
    Boolean result = FALSE;
    mOwner->mPrivateHandler->PostDelayed(mOwner->mZoomControlRunnable, ZOOM_CONTROLS_TIMEOUT, &result);
    mOwner->mWebView->ZoomIn(&result);
    return NOERROR;
}


//===============================================================
//            ZoomControlExternal::ZoomOutClickListener
//===============================================================

ZoomControlExternal::ZoomOutClickListener::ZoomOutClickListener(
    /* [in] */ ZoomControlExternal* owner)
    : mOwner(owner)
{}

CAR_INTERFACE_IMPL_LIGHT(ZoomControlExternal::ZoomOutClickListener, IViewOnClickListener);

ECode ZoomControlExternal::ZoomOutClickListener::OnClick(
    /* [in] */ IView* v)
{
    // reset time out
    mOwner->mPrivateHandler->RemoveCallbacks(mOwner->mZoomControlRunnable);
    Boolean result = FALSE;
    mOwner->mPrivateHandler->PostDelayed(mOwner->mZoomControlRunnable, ZOOM_CONTROLS_TIMEOUT, &result);
    mOwner->mWebView->ZoomOut(&result);
    return NOERROR;
}


//===============================================================
//            ZoomControlExternal::ZoomControlRunnable
//===============================================================
ZoomControlExternal::ZoomControlRunnable::ZoomControlRunnable(
    /* [in] */ ZoomControlExternal* owner)
    : mOwner(owner)
{}

ECode ZoomControlExternal::ZoomControlRunnable::Run()
{
    /* Don't dismiss the controls if the user has
     * focus on them. Wait and check again later.
     */
    if (!mOwner->mZoomControls->HasFocus()) {
        mOwner->mZoomControls->Hide();
    }
    else {
        mOwner->mPrivateHandler->RemoveCallbacks(mOwner->mZoomControlRunnable);
        Boolean result = FALSE;
        mOwner->mPrivateHandler->PostDelayed(mOwner->mZoomControlRunnable, ZOOM_CONTROLS_TIMEOUT, &result);
    }
    return NOERROR;
}


//===============================================================
//                     ZoomControlExternal
//===============================================================
const Int64 ZoomControlExternal::ZOOM_CONTROLS_TIMEOUT = CViewConfiguration::GetZoomControlsTimeout();

ZoomControlExternal::ZoomControlExternal(
    /* [in] */ CWebViewClassic* webView)
    : mWebView(webView)
{
    CHandler::New((IHandler**)&mPrivateHandler);
}

PInterface ZoomControlExternal::Probe(
    /* [in] */ REIID riid)
{
    assert(0);
    return NULL;
}

UInt32 ZoomControlExternal::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ZoomControlExternal::Release()
{
    return ElRefBase::Release();
}

ECode ZoomControlExternal::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    assert(0);
    return NOERROR;
}

ECode ZoomControlExternal::Show()
{
    if (mZoomControlRunnable != NULL) {
        mPrivateHandler->RemoveCallbacks(mZoomControlRunnable);
    }
    GetControls()->Show(TRUE);
    Boolean result = FALSE;
    mPrivateHandler->PostDelayed(mZoomControlRunnable, ZOOM_CONTROLS_TIMEOUT, &result);
    return NOERROR;
}

ECode ZoomControlExternal::Hide()
{
    if (mZoomControlRunnable != NULL) {
        mPrivateHandler->RemoveCallbacks(mZoomControlRunnable);
    }
    if (mZoomControls != NULL) {
        mZoomControls->Hide();
    }
    return NOERROR;
}

ECode ZoomControlExternal::IsVisible(
    /* [out] */ Boolean* isVisible)
{
    VALIDATE_NOT_NULL(isVisible);
    Boolean isShown = FALSE;
    *isVisible = mZoomControls != NULL && (mZoomControls->IsShown(&isShown), isShown);
    return NOERROR;
}

ECode ZoomControlExternal::Update()
{
    return NOERROR;
}

AutoPtr<ZoomControlExternal::ExtendedZoomControls> ZoomControlExternal::GetControls()
{
    if (mZoomControls == NULL) {
        mZoomControls = CreateZoomControls();
        /*
         * need to be set to VISIBLE first so that getMeasuredHeight() in
         * {@link #onSizeChanged()} can return the measured value for proper
         * layout.
         */
        mZoomControls->SetVisibility(IView::VISIBLE);
        mZoomControlRunnable = new ZoomControlRunnable(this);
    }
    return mZoomControls;
}

AutoPtr<ZoomControlExternal::ExtendedZoomControls> ZoomControlExternal::CreateZoomControls()
{
    AutoPtr<ExtendedZoomControls> zoomControls = new ExtendedZoomControls(mWebView->GetContext());
    //conterpart for the AddRef in ZoomControlExternal::ExtendedZoomControls::ExtendedZoomControls
    zoomControls->Release();
    AutoPtr<IViewOnClickListener> inListener = new ZoomInClickListener(this);
    zoomControls->SetOnZoomInClickListener(inListener);
    AutoPtr<IViewOnClickListener> outListener = new ZoomOutClickListener(this);
    zoomControls->SetOnZoomOutClickListener(outListener);
    return zoomControls;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
