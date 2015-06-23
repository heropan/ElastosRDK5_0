#ifndef __CSCROLLER_H_
#define __CSCROLLER_H_

#include "_CScroller.h"
#include "widget/Scroller.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CScroller), public Scroller
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IInterpolator* interpolator);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IInterpolator* interpolator,
        /* [in] */ Boolean flywheel);

public:
    CARAPI SetFriction(
        /* [in] */ Float friction);

    CARAPI IsFinished(
        /* [out] */ Boolean* finished);

    CARAPI ForceFinished(
        /* [in] */ Boolean finished);

    CARAPI GetDuration(
        /* [out] */ Int32* finished);

    CARAPI GetCurrX(
        /* [out] */ Int32* x);

    CARAPI GetCurrY(
        /* [out] */ Int32* y);

    CARAPI GetCurrVelocity(
        /* [out] */ Int32* velocity);

    CARAPI GetStartX(
        /* [out] */ Int32* x);

    CARAPI GetStartY(
        /* [out] */ Int32* y);

    CARAPI GetFinalX(
        /* [out] */ Int32* x);

    CARAPI GetFinalY(
        /* [out] */ Int32* y);

    CARAPI ComputeScrollOffset(
        /* [out] */ Boolean* offset);

    CARAPI StartScroll(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    CARAPI StartScroll(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy,
        /* [in] */ Int32 duration);

    CARAPI Fling(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 velocityX,
        /* [in] */ Int32 velocityY,
        /* [in] */ Int32 minX,
        /* [in] */ Int32 maxX,
        /* [in] */ Int32 minY,
        /* [in] */ Int32 maxY);

    CARAPI AbortAnimation();

    CARAPI ExtendDuration(
        /* [in] */ Int32 extend);

    CARAPI TimePassed(
        /* [out] */ Int32* passedTime);

    CARAPI SetFinalX(
        /* [in] */ Int32 x);

    CARAPI SetFinalY(
        /* [in] */ Int32 y);

    CARAPI IsScrollingInDirection(
        /* [in] */ Int32 xvel,
        /* [in] */ Int32 yvel,
        /* [out] */ Boolean* result);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CSCROLLER_H_