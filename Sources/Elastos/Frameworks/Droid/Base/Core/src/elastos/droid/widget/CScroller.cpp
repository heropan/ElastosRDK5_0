#include "elastos/droid/widget/CScroller.h"

namespace Elastos {
namespace Droid {
namespace Widget {

ECode CScroller::constructor(
    /* [in] */ IContext* context)
{
    return Scroller::Init(context);
}

ECode CScroller::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IInterpolator* interpolator)
{
    return Scroller::Init(context, interpolator);
}

ECode CScroller::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IInterpolator* interpolator,
    /* [in] */ Boolean flywheel)
{
    return Scroller::Init(context, interpolator, flywheel);
}

ECode CScroller::SetFriction(
    /* [in] */ Float friction)
{
    Scroller::SetFriction(friction);
    return NOERROR;
}

ECode CScroller::IsFinished(
    /* [out] */ Boolean* finished)
{
    VALIDATE_NOT_NULL(finished);
    *finished = Scroller::IsFinished();
    return NOERROR;
}

ECode CScroller::ForceFinished(
    /* [in] */ Boolean finished)
{
    Scroller::ForceFinished(finished);
    return NOERROR;
}

ECode CScroller::GetDuration(
    /* [out] */ Int32* finished)
{
    VALIDATE_NOT_NULL(finished);
    *finished = Scroller::GetDuration();
    return NOERROR;
}

ECode CScroller::GetCurrX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);
    *x = Scroller::GetCurrX();
    return NOERROR;
}

ECode CScroller::GetCurrY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);
    *y = Scroller::GetCurrY();
    return NOERROR;
}

ECode CScroller::GetCurrVelocity(
    /* [out] */ Int32* velocity)
{
    VALIDATE_NOT_NULL(velocity);
    *velocity = Scroller::GetCurrVelocity();
    return NOERROR;
}

ECode CScroller::GetStartX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);
    *x = Scroller::GetStartX();
    return NOERROR;
}

ECode CScroller::GetStartY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);
    *y = Scroller::GetStartY();
    return NOERROR;
}

ECode CScroller::GetFinalX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);
    *x = Scroller::GetFinalX();
    return NOERROR;
}

ECode CScroller::GetFinalY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);
    *y = Scroller::GetFinalY();
    return NOERROR;
}

ECode CScroller::ComputeScrollOffset(
    /* [out] */ Boolean* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = Scroller::ComputeScrollOffset();
    return NOERROR;
}

ECode CScroller::StartScroll(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    Scroller::StartScroll(startX, startY, dx, dy);
    return NOERROR;
}

ECode CScroller::StartScroll(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy,
    /* [in] */ Int32 duration)
{
    Scroller::StartScroll(startX, startY, dx, dy, duration);
    return NOERROR;
}

ECode CScroller::Fling(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 velocityX,
    /* [in] */ Int32 velocityY,
    /* [in] */ Int32 minX,
    /* [in] */ Int32 maxX,
    /* [in] */ Int32 minY,
    /* [in] */ Int32 maxY)
{
    Scroller::Fling(startX, startY, velocityX, velocityY, minX, maxX, minY, maxY);
    return NOERROR;
}

ECode CScroller::AbortAnimation()
{
    Scroller::AbortAnimation();
    return NOERROR;
}

ECode CScroller::ExtendDuration(
    /* [in] */ Int32 extend)
{
    Scroller::ExtendDuration(extend);
    return NOERROR;
}

ECode CScroller::TimePassed(
    /* [out] */ Int32* passedTime)
{
    VALIDATE_NOT_NULL(passedTime);
    *passedTime = Scroller::TimePassed();
    return NOERROR;
}

ECode CScroller::SetFinalX(
    /* [in] */ Int32 x)
{
    Scroller::SetFinalX(x);
    return NOERROR;
}

ECode CScroller::SetFinalY(
    /* [in] */ Int32 y)
{
    Scroller::SetFinalY(y);
    return NOERROR;
}

ECode CScroller::IsScrollingInDirection(
    /* [in] */ Int32 xvel,
    /* [in] */ Int32 yvel,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Scroller::IsScrollingInDirection(xvel, yvel);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos