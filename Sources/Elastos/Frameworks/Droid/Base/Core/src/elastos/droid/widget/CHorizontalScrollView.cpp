
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CHorizontalScrollView.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CHorizontalScrollView, HorizontalScrollView)
IVIEWGROUP_METHODS_IMPL(CHorizontalScrollView, HorizontalScrollView)
IVIEWPARENT_METHODS_IMPL(CHorizontalScrollView, HorizontalScrollView)
IVIEWMANAGER_METHODS_IMPL(CHorizontalScrollView, HorizontalScrollView)
IDRAWABLECALLBACK_METHODS_IMPL(CHorizontalScrollView, HorizontalScrollView)
IKEYEVENTCALLBACK_METHODS_IMPL(CHorizontalScrollView, HorizontalScrollView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CHorizontalScrollView, HorizontalScrollView)
IFRAMELAYOUT_METHODS_IMPL(CHorizontalScrollView, HorizontalScrollView)

PInterface CHorizontalScrollView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CHorizontalScrollView::Probe(riid);
}

ECode CHorizontalScrollView::constructor(
    /* [in] */ IContext* context)
{
    return HorizontalScrollView::Init(context);
}

ECode CHorizontalScrollView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return HorizontalScrollView::Init(context, attrs);
}

ECode CHorizontalScrollView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return HorizontalScrollView::Init(context, attrs, defStyle);
}

ECode CHorizontalScrollView::ArrowScroll(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* consumed)
{
    VALIDATE_NOT_NULL(consumed);
    *consumed = HorizontalScrollView::ArrowScroll(direction);
    return NOERROR;
}

ECode CHorizontalScrollView::ExecuteKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* handled)
{
    VALIDATE_NOT_NULL(handled);
    *handled = HorizontalScrollView::ExecuteKeyEvent(event);
    return NOERROR;
}

ECode CHorizontalScrollView::Fling(
    /* [in] */ Int32 velocityY)
{
    return HorizontalScrollView::Fling(velocityY);
}

ECode CHorizontalScrollView::FullScroll(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* consumed)
{
    VALIDATE_NOT_NULL(consumed);
    *consumed = HorizontalScrollView::FullScroll(direction);
    return NOERROR;
}

ECode CHorizontalScrollView::GetMaxScrollAmount(
    /* [out] */ Int32* maxScrollAmount)
{
    VALIDATE_NOT_NULL(maxScrollAmount);
    *maxScrollAmount = HorizontalScrollView::GetMaxScrollAmount();
    return NOERROR;
}

ECode CHorizontalScrollView::IsFillViewport(
    /* [out] */ Boolean* isFilled)
{
    VALIDATE_NOT_NULL(isFilled);
    *isFilled = HorizontalScrollView::IsFillViewport();
    return NOERROR;
}

ECode CHorizontalScrollView::IsSmoothScrollingEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = HorizontalScrollView::IsSmoothScrollingEnabled();
    return NOERROR;
}

ECode CHorizontalScrollView::PageScroll(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* consumed)
{
    VALIDATE_NOT_NULL(consumed);
    *consumed = HorizontalScrollView::PageScroll(direction);
    return NOERROR;
}

ECode CHorizontalScrollView::SetFillViewport(
    /* [in] */ Boolean fillViewport)
{
    return HorizontalScrollView::SetFillViewport(fillViewport);
}

ECode CHorizontalScrollView::SetSmoothScrollingEnabled(
    /* [in] */ Boolean smoothScrollingEnabled)
{
    return HorizontalScrollView::SetSmoothScrollingEnabled(smoothScrollingEnabled);
}

ECode CHorizontalScrollView::SmoothScrollBy(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    return HorizontalScrollView::SmoothScrollBy(dx, dy);
}

ECode CHorizontalScrollView::SmoothScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return HorizontalScrollView::SmoothScrollTo(x, y);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
