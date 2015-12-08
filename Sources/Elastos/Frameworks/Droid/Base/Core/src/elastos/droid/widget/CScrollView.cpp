
#include "elastos/droid/widget/CScrollView.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CScrollView, ScrollView)
IVIEWGROUP_METHODS_IMPL(CScrollView, ScrollView)
IVIEWPARENT_METHODS_IMPL(CScrollView, ScrollView)
IVIEWMANAGER_METHODS_IMPL(CScrollView, ScrollView)
IDRAWABLECALLBACK_METHODS_IMPL(CScrollView, ScrollView)
IKEYEVENTCALLBACK_METHODS_IMPL(CScrollView, ScrollView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CScrollView, ScrollView)
IFRAMELAYOUT_METHODS_IMPL(CScrollView, ScrollView)

PInterface CScrollView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CScrollView::Probe(riid);
}

ECode CScrollView::constructor(
    /* [in] */ IContext* context)
{
    return ScrollView::Init(context);
}

ECode CScrollView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ScrollView::Init(context, attrs);
}

ECode CScrollView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ScrollView::Init(context, attrs, defStyle);
}

ECode CScrollView::GetMaxScrollAmount(
    /* [out] */ Int32* maxAmount)
{
    VALIDATE_NOT_NULL(maxAmount);
    *maxAmount  = ScrollView::GetMaxScrollAmount();
    return NOERROR;
}

ECode CScrollView::IsFillViewport(
    /* [out] */ Boolean* isFilled)
{
    VALIDATE_NOT_NULL(isFilled);
    *isFilled  = ScrollView::IsFillViewport();
    return NOERROR;
}

ECode CScrollView::SetFillViewport(
    /* [in] */ Boolean fillViewport)
{
    return ScrollView::SetFillViewport(fillViewport);
}

ECode CScrollView::IsSmoothScrollingEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled  = ScrollView::IsSmoothScrollingEnabled();
    return NOERROR;
}

ECode CScrollView::SetSmoothScrollingEnabled(
    /* [in] */ Boolean smoothScrollingEnabled)
{
    return ScrollView::SetSmoothScrollingEnabled(smoothScrollingEnabled);
}

ECode CScrollView::ExecuteKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* handled)
{
    VALIDATE_NOT_NULL(handled);
    *handled = ScrollView::ExecuteKeyEvent(event);
    return NOERROR;
}

ECode CScrollView::PageScroll(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* consumed)
{
    VALIDATE_NOT_NULL(consumed);
    *consumed = ScrollView::PageScroll(direction);
    return NOERROR;
}

ECode CScrollView::FullScroll(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* consumed)
{
    VALIDATE_NOT_NULL(consumed);
    *consumed = ScrollView::FullScroll(direction);
    return NOERROR;
}

ECode CScrollView::ArrowScroll(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* consumed)
{
    VALIDATE_NOT_NULL(consumed);
    *consumed = ScrollView::ArrowScroll(direction);
    return NOERROR;
}

ECode CScrollView::SmoothScrollBy(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    return ScrollView::SmoothScrollBy(dx, dy);
}

ECode CScrollView::SmoothScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return ScrollView::SmoothScrollTo(x, y);
}

ECode CScrollView::Fling(
    /* [in] */ Int32 velocityY)
{
    return ScrollView::Fling(velocityY);
}


}// namespace Widget
}// namespace Droid
}// namespace Elastos
