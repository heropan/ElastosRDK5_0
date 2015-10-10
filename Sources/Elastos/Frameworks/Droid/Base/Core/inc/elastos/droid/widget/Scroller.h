#ifndef __ELASTOS_DROID_WIDGET_SCROLLER_H__
#define __ELASTOS_DROID_WIDGET_SCROLLER_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Widget {

using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::Animation::IInterpolator;

class Scroller
{
public:
    Scroller(
        /* [in] */ IContext* context);

    Scroller(
        /* [in] */ IContext* context,
        /* [in] */ IInterpolator* interpolator);

    Scroller(
        /* [in] */ IContext* context,
        /* [in] */ IInterpolator* interpolator,
        /* [in] */ Boolean flywheel);

    CARAPI_(void) SetFriction(
        /* [in] */ Float friction);

    CARAPI_(Boolean) IsFinished();

    CARAPI_(void) ForceFinished(
        /* [in] */ Boolean finished);

    CARAPI_(Int32) GetDuration();

    CARAPI_(Int32) GetCurrX();

    CARAPI_(Int32) GetCurrY();

    CARAPI_(Float) GetCurrVelocity();

    CARAPI_(Int32) GetStartX();

    CARAPI_(Int32) GetStartY();

    CARAPI_(Int32) GetFinalX();

    CARAPI_(Int32) GetFinalY();

    Boolean ComputeScrollOffset();

    CARAPI_(void) StartScroll(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    CARAPI_(void) StartScroll(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy,
        /* [in] */ Int32 duration);

    CARAPI_(void) Fling(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 velocityX,
        /* [in] */ Int32 velocityY,
        /* [in] */ Int32 minX,
        /* [in] */ Int32 maxX,
        /* [in] */ Int32 minY,
        /* [in] */ Int32 maxY);

    CARAPI_(void) AbortAnimation();

    CARAPI_(void) ExtendDuration(
        /* [in] */ Int32 extend);

    CARAPI_(Int32) TimePassed();

    CARAPI_(void) SetFinalX(
        /* [in] */ Int32 newX);

    CARAPI_(void) SetFinalY(
        /* [in] */ Int32 newY);

    Boolean IsScrollingInDirection(
        /* [in] */ Float xvel,
        /* [in] */ Float yvel);

    static CARAPI_(Float) ViscousFluid(
        /* [in] */ Float x);

protected:
    Scroller();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IInterpolator* interpolator = NULL,
        /* [in] */ Boolean flywheel = FALSE);

private:

    CARAPI_(Float) ComputeDeceleration(
        /* [in] */ Float velocity);

    CARAPI_(Double) GetSplineDeceleration(
        /* [in] */ Float velocity);

    CARAPI_(Int32) GetSplineFlingDuration(
        /* [in] */ Float velocity);

    CARAPI_(Double) GetSplineFlingDistance(
        /* [in] */ Float velocity);

private:
    static const Int32 DEFAULT_DURATION  = 250;
    static const Int32 SCROLL_MODE = 0;
    static const Int32 FLING_MODE = 1;

    static Float DECELERATION_RATE;
    static const Float INFLEXION = 0.35f; // Tension lines cross at (INFLEXION, 1)
    static const Float START_TENSION = 0.5f;
    static const Float END_TENSION = 1.0f;
    static const Float P1;// = START_TENSION * INFLEXION;
    static const Float P2;// = 1.0f - END_TENSION * (1.0f - INFLEXION);

    static const Int32 NB_SAMPLES = 100;
    static AutoPtr<ArrayOf<Float> > SPLINE_POSITION;
    static AutoPtr<ArrayOf<Float> > SPLINE_TIME;

    static Float sViscousFluidScale;
    static Float sViscousFluidNormalize;

private:
    Int32 mMode;

    Int32 mStartX;
    Int32 mStartY;
    Int32 mFinalX;
    Int32 mFinalY;

    Int32 mMinX;
    Int32 mMaxX;
    Int32 mMinY;
    Int32 mMaxY;

    Int32 mCurrX;
    Int32 mCurrY;
    Int64 mStartTime;
    Int32 mDuration;
    Float mDurationReciprocal;
    Float mDeltaX;
    Float mDeltaY;
    Boolean mFinished;
    AutoPtr<IInterpolator> mInterpolator;

    Boolean mFlywheel;
    Float mVelocity;
    Float mCurrVelocity;
    Int32 mDistance;

    Float mFlingFriction;// = ViewConfiguration.getScrollFriction();
    Float mDeceleration;
    Float mPpi;

    Float mPhysicalCoeff;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_SCROLLER_H__
