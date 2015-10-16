
#include "elastos/droid/widget/ZoomControls.h"
#include "elastos/droid/view/animation/CAlphaAnimation.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::Animation::CAlphaAnimation;
using Elastos::Droid::View::Animation::IAlphaAnimation;

namespace Elastos {
namespace Droid {
namespace Widget {

ZoomControls::ZoomControls()
{}

ZoomControls::ZoomControls(
    /* [in] */ IContext* context)
    : LinearLayout(context, NULL)
{
    ASSERT_SUCCEEDED(InitInternal(context));
}

ZoomControls::ZoomControls(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : LinearLayout(context, attrs)
{
    ASSERT_SUCCEEDED(InitInternal(context, attrs));
}

ECode ZoomControls::Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(LinearLayout::Init(context, attrs));
    ASSERT_SUCCEEDED(InitInternal(context, attrs));
    return NOERROR;
}

ECode ZoomControls::InitInternal(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs)
{
    SetFocusable(FALSE);

    AutoPtr<ILayoutInflater> inflater;
    AutoPtr<IInterface> svTemp;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&svTemp);
    inflater = ILayoutInflater::Probe(svTemp);
    AutoPtr<IView> v;
    inflater->Inflate(R::layout::zoom_controls, THIS_PROBE(IViewGroup), // we are the parent
            TRUE, (IView**)&v);

    mZoomIn = IZoomButton::Probe(FindViewById(R::id::zoomIn));
    mZoomOut = IZoomButton::Probe(FindViewById(R::id::zoomOut));
    return NOERROR;
}

ECode ZoomControls::SetOnZoomInClickListener(
    /* [in] */ IViewOnClickListener* listener)
{
    return mZoomIn->SetOnClickListener(listener);
}

ECode ZoomControls::SetOnZoomOutClickListener(
    /* [in] */ IViewOnClickListener* listener)
{
    return mZoomOut->SetOnClickListener(listener);
}

/*
 * Sets how fast you get zoom events when the user holds down the
 * zoom in/out buttons.
 */
ECode ZoomControls::SetZoomSpeed(
    /* [in] */ Int64 speed)
{
    mZoomIn->SetZoomSpeed(speed);
    mZoomOut->SetZoomSpeed(speed);
    return NOERROR;
}

// @Override
Boolean ZoomControls::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    return TRUE;
}

ECode ZoomControls::Show()
{
    return Fade(IView::VISIBLE, 0.0f, 1.0f);
}

ECode ZoomControls::Hide()
{
    return Fade(IView::GONE, 1.0f, 0.0f);
}


ECode ZoomControls::SetIsZoomInEnabled(
    /* [in] */ Boolean isEnabled)
{
    return mZoomIn->SetEnabled(isEnabled);
}

ECode ZoomControls::SetIsZoomOutEnabled(
    /* [in] */ Boolean isEnabled)
{
    return mZoomOut->SetEnabled(isEnabled);
}

// @Override
Boolean ZoomControls::HasFocus()
{
    Boolean in, out;
    mZoomIn->HasFocus(&in);
    mZoomOut->HasFocus(&out);
    return in || out;
}

// @Override
ECode ZoomControls::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    LinearLayout::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String("CZoomControls"), (ICharSequence**)&cs);
    event->SetClassName(cs);
    return NOERROR;
}

// @Override
ECode ZoomControls::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    LinearLayout::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String("CZoomControls"), (ICharSequence**)&cs);
    info->SetClassName(cs);
    return NOERROR;
}

ECode ZoomControls::Fade(
    /* [in] */ Int32 visibility,
    /* [in] */ Float startAlpha,
    /* [in] */ Float endAlpha)
{
    AutoPtr<IAlphaAnimation> anim;
    CAlphaAnimation::New(startAlpha, endAlpha, (IAlphaAnimation**)&anim);
    anim->SetDuration(500);
    StartAnimation(anim);
    SetVisibility(visibility);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
