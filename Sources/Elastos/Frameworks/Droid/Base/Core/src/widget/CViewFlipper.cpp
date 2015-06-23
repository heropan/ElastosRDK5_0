
#include "widget/CViewFlipper.h"
#include "R.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

CViewFlipper::CViewFlipper()
{}

ECode CViewFlipper::constructor(
    /* [in] */ IContext* ctx)
{
    return ViewFlipper::Init(ctx);
}

ECode CViewFlipper::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewFlipper::Init(ctx, attrs);
}

PInterface CViewFlipper::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CViewFlipper::Probe(riid);
}

IVIEW_METHODS_IMPL(CViewFlipper, ViewFlipper);
IVIEWGROUP_METHODS_IMPL(CViewFlipper, ViewFlipper);
IVIEWPARENT_METHODS_IMPL(CViewFlipper, ViewFlipper);
IVIEWMANAGER_METHODS_IMPL(CViewFlipper, ViewFlipper);
IDRAWABLECALLBACK_METHODS_IMPL(CViewFlipper, ViewFlipper);
IKEYEVENTCALLBACK_METHODS_IMPL(CViewFlipper, ViewFlipper);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CViewFlipper, ViewFlipper);

ECode CViewFlipper::SetFlipInterval(
    /* [in] */ Int32 milliseconds)
{
    return ViewFlipper::SetFlipInterval(milliseconds);
}

ECode CViewFlipper::StartFlipping()
{
    return ViewFlipper::StartFlipping();
}

ECode CViewFlipper::StopFlipping()
{
    return ViewFlipper::StopFlipping();
}

ECode CViewFlipper::IsFlipping(
    /* [out] */ Boolean* flipping)
{
    VALIDATE_NOT_NULL(flipping);
    return ViewFlipper::IsFlipping(flipping);
}

ECode CViewFlipper::SetAutoStart(
    /* [in] */ Boolean autoStart)
{
    return ViewFlipper::SetAutoStart(autoStart);
}

ECode CViewFlipper::IsAutoStart(
    /* [out] */ Boolean* start)
{
    VALIDATE_NOT_NULL(start);
    return ViewFlipper::IsAutoStart(start);
}

ECode CViewFlipper::GetForegroundGravity(
    /* [out] */ Int32* foregroundGravity)
{
    VALIDATE_NOT_NULL(foregroundGravity);
    *foregroundGravity = ViewFlipper::GetForegroundGravity();
    return NOERROR;
}

ECode CViewFlipper::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    return ViewFlipper::SetForegroundGravity(foregroundGravity);
}

ECode CViewFlipper::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    return ViewFlipper::SetForeground(drawable);
}

ECode CViewFlipper::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> d = ViewFlipper::GetForeground();
    *foreground = d;
    INTERFACE_ADDREF(*foreground);
    return NOERROR;
}

ECode CViewFlipper::GetMeasureAllChildren(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = ViewFlipper::GetMeasureAllChildren();
    return NOERROR;
}

ECode CViewFlipper::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    return ViewFlipper::SetMeasureAllChildren(measureAll);
}

ECode CViewFlipper::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = ViewFlipper::GetConsiderGoneChildrenWhenMeasuring();
    return NOERROR;
}

ECode CViewFlipper::SetDisplayedChild(
    /* [in] */ Int32 whichChild)
{
    return ViewAnimator::SetDisplayedChild(whichChild);
}

ECode CViewFlipper::GetDisplayedChild(
    /* [out] */ Int32* child)
{
    VALIDATE_NOT_NULL(child);
    return ViewAnimator::GetDisplayedChild(child);
}

ECode CViewFlipper::ShowNext()
{
    return ViewAnimator::ShowNext();
}

ECode CViewFlipper::ShowPrevious()
{
    return ViewAnimator::ShowPrevious();
}

ECode CViewFlipper::GetCurrentView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    return ViewAnimator::GetCurrentView(view);
}

ECode CViewFlipper::GetInAnimation(
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    return ViewAnimator::GetInAnimation(animation);
}

ECode CViewFlipper::SetInAnimation(
    /* [in] */ IAnimation* inAnimation)
{
    return ViewAnimator::SetInAnimation(inAnimation);
}

ECode CViewFlipper::GetOutAnimation(
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    return ViewAnimator::GetOutAnimation(animation);
}

ECode CViewFlipper::SetOutAnimation(
    /* [in] */ IAnimation* outAnimation)
{
    return ViewAnimator::SetOutAnimation(outAnimation);
}

ECode CViewFlipper::SetInAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    return ViewAnimator::SetInAnimation(context, resourceID);
}

ECode CViewFlipper::SetOutAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    return ViewAnimator::SetOutAnimation(context, resourceID);
}

ECode CViewFlipper::GetAnimateFirstView(
    /* [out] */ Boolean* animate)
{
    VALIDATE_NOT_NULL(animate);
    return ViewAnimator::GetAnimateFirstView(animate);
}

ECode CViewFlipper::SetAnimateFirstView(
    /* [in] */ Boolean animate)
{
    return ViewAnimator::SetAnimateFirstView(animate);
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
