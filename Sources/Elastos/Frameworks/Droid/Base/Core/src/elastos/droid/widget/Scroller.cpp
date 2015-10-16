
#include "elastos/droid/widget/Scroller.h"
#include "elastos/droid/view/CViewConfigurationHelper.h"
#include "elastos/droid/view/animation/AnimationUtils.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/os/Build.h"
#include <elastos/core/Math.h>

using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::View::IViewConfigurationHelper;
using Elastos::Droid::View::CViewConfigurationHelper;
using Elastos::Droid::View::Animation::AnimationUtils;

namespace Elastos {
namespace Droid {
namespace Widget {

static Float ViscousFluidImpl(Float x, Float scale, Float normalize)
{
    x *= scale;
    if (x < 1.0f) {
        x -= (1.0f - (Float)Elastos::Core::Math::Exp(-x));
    } else {
        Float start = 0.36787944117f;   // 1/e == exp(-1)
        x = 1.0f - (Float)Elastos::Core::Math::Exp(1.0f - x);
        x = start + x * (1.0f - start);
    }
    x *= normalize;
    return x;
}

static Float InitDECELERATION_RATE()
{
    return (Float) (Elastos::Core::Math::Log(0.78) / Elastos::Core::Math::Log(0.9));
}

static AutoPtr<ArrayOf<Float> > InitSPLINE_POSITION()
{
    const Int32 NB_SAMPLES = 100;
    const Float START_TENSION = 0.5f;
    const Float P1 = 0.5f * 0.35f;
    const Float P2 = 1.0f - 1.0f * (1.0f - 0.35f);

    AutoPtr<ArrayOf<Float> > positions = ArrayOf<Float>::Alloc(NB_SAMPLES + 1);

    Float x_min = 0.0f;
    for (Int32 i = 0; i < NB_SAMPLES; i++) {
        Float alpha = (Float) i / NB_SAMPLES;

        Float x_max = 1.0f;
        Float x, tx, coef;
        while (TRUE) {
            x = x_min + (x_max - x_min) / 2.0f;
            coef = 3.0f * x * (1.0f - x);
            tx = coef * ((1.0f - x) * P1 + x * P2) + x * x * x;
            if (Elastos::Core::Math::Abs(tx - alpha) < 1E-5) break;
            if (tx > alpha) x_max = x;
            else x_min = x;
        }
        (*positions)[i] = coef * ((1.0f - x) * START_TENSION + x) + x * x * x;
    }
    (*positions)[NB_SAMPLES] = 1.0f;
    return positions;
}

static AutoPtr<ArrayOf<Float> > InitSPLINE_TIME()
{
    const Int32 NB_SAMPLES = 100;
    const Float START_TENSION = 0.5f;
    const Float P1 = 0.5f * 0.35f;
    const Float P2 = 1.0f - 1.0f * (1.0f - 0.35f);

    AutoPtr<ArrayOf<Float> > times = ArrayOf<Float>::Alloc(NB_SAMPLES + 1);
    Float y_min = 0.0f;
    for (Int32 i = 0; i < NB_SAMPLES; i++) {
        const Float alpha = (Float) i / NB_SAMPLES;

        Float y_max = 1.0f;
        Float y, dy, coef;
        while (TRUE) {
            y = y_min + (y_max - y_min) / 2.0f;
            coef = 3.0f * y * (1.0f - y);
            dy = coef * ((1.0f - y) * START_TENSION + y) + y * y * y;
            if (Elastos::Core::Math::Abs(dy - alpha) < 1E-5) break;
            if (dy > alpha) y_max = y;
            else y_min = y;
        }
        (*times)[i] = coef * ((1.0f - y) * P1 + y * P2) + y * y * y;
    }
    (*times)[NB_SAMPLES] = 1.0f;
    return times;
}

// This controls the viscous fluid effect (how much of it)
Float Scroller::sViscousFluidScale = 8.0f;
Float Scroller::sViscousFluidNormalize = 1.0f / ViscousFluidImpl(1.0f, 8.0f, 1.0f);

const Int32 Scroller::DEFAULT_DURATION;
const Int32 Scroller::SCROLL_MODE;
const Int32 Scroller::FLING_MODE;

Float Scroller::DECELERATION_RATE = InitDECELERATION_RATE();
const Float Scroller::INFLEXION;
const Float Scroller::START_TENSION;
const Float Scroller::END_TENSION;
const Float Scroller::P1 = 0.5f * 0.35f; //START_TENSION * INFLEXION;
const Float Scroller::P2 = 1.0f - 1.0f * (1.0f - 0.35f); //1.0f - END_TENSION * (1.0f - INFLEXION);

const Int32 Scroller::NB_SAMPLES;

AutoPtr<ArrayOf<Float> > Scroller::SPLINE_POSITION = InitSPLINE_POSITION();
AutoPtr<ArrayOf<Float> > Scroller::SPLINE_TIME = InitSPLINE_TIME();

const Float GRAVITY_EARTH = 9.80665f;   // g (m/s^2) SensorManager.GRAVITY_EARTH

Scroller::Scroller()
    : mMode(0)
    , mStartX(0)
    , mStartY(0)
    , mFinalX(0)
    , mFinalY(0)
    , mMinX(0)
    , mMaxX(0)
    , mMinY(0)
    , mMaxY(0)
    , mCurrX(0)
    , mCurrY(0)
    , mStartTime(0)
    , mDuration(0)
    , mDurationReciprocal(0)
    , mDeltaX(0)
    , mDeltaY(0)
    , mFinished(FALSE)
    , mFlywheel(FALSE)
    , mVelocity(0)
    , mCurrVelocity(0)
    , mDistance(0)
    , mPpi(0)
    , mPhysicalCoeff(0)
{

}

Scroller::Scroller(
    /* [in] */ IContext* context)
    : mMode(0)
    , mStartX(0)
    , mStartY(0)
    , mFinalX(0)
    , mFinalY(0)
    , mMinX(0)
    , mMaxX(0)
    , mMinY(0)
    , mMaxY(0)
    , mCurrX(0)
    , mCurrY(0)
    , mStartTime(0)
    , mDuration(0)
    , mDurationReciprocal(0)
    , mDeltaX(0)
    , mDeltaY(0)
    , mFinished(FALSE)
    , mFlywheel(FALSE)
    , mVelocity(0)
    , mCurrVelocity(0)
    , mDistance(0)
    , mPpi(0)
    , mPhysicalCoeff(0)
{
    assert(context != NULL);

    AutoPtr<IApplicationInfo> appInfo;
    context->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 version;
    appInfo->GetTargetSdkVersion(&version);
    Boolean flywheel = version >= Build::VERSION_CODES::HONEYCOMB;

    Init(context, NULL, flywheel);
}

Scroller::Scroller(
    /* [in] */ IContext* context,
    /* [in] */ IInterpolator* interpolator)
    : mMode(0)
    , mStartX(0)
    , mStartY(0)
    , mFinalX(0)
    , mFinalY(0)
    , mMinX(0)
    , mMaxX(0)
    , mMinY(0)
    , mMaxY(0)
    , mCurrX(0)
    , mCurrY(0)
    , mStartTime(0)
    , mDuration(0)
    , mDurationReciprocal(0)
    , mDeltaX(0)
    , mDeltaY(0)
    , mFinished(FALSE)
    , mFlywheel(FALSE)
    , mVelocity(0)
    , mCurrVelocity(0)
    , mDistance(0)
    , mPpi(0)
    , mPhysicalCoeff(0)
{
    AutoPtr<IApplicationInfo> appInfo;
    context->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 version;
    appInfo->GetTargetSdkVersion(&version);
    Boolean flywheel = version >= Build::VERSION_CODES::HONEYCOMB;

    Init(context, interpolator, flywheel);
}

Scroller::Scroller(
    /* [in] */ IContext* context,
    /* [in] */ IInterpolator* interpolator,
    /* [in] */ Boolean flywheel)
    : mMode(0)
    , mStartX(0)
    , mStartY(0)
    , mFinalX(0)
    , mFinalY(0)
    , mMinX(0)
    , mMaxX(0)
    , mMinY(0)
    , mMaxY(0)
    , mCurrX(0)
    , mCurrY(0)
    , mStartTime(0)
    , mDuration(0)
    , mDurationReciprocal(0)
    , mDeltaX(0)
    , mDeltaY(0)
    , mFinished(FALSE)
    , mFlywheel(FALSE)
    , mVelocity(0)
    , mCurrVelocity(0)
    , mDistance(0)
    , mPpi(0)
    , mPhysicalCoeff(0)
{
    Init(context, interpolator, flywheel);
}

ECode Scroller::Init(
    /* [in] */ IContext* context,
    /* [in] */ IInterpolator* interpolator,
    /* [in] */ Boolean flywheel)
{
    //TODO
    AutoPtr<IViewConfigurationHelper> helper;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&helper);
    helper->GetScrollFriction(&mFlingFriction);

    mFinished = TRUE;
    mInterpolator = interpolator;

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> metrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Float density;
    metrics->GetDensity(&density);

    mPpi = density * 160.0f;
    mDeceleration = ComputeDeceleration(mFlingFriction);
    mFlywheel = flywheel;

    mPhysicalCoeff = ComputeDeceleration(0.84f); // look and feel tuning

    return NOERROR;
}

Float Scroller::ViscousFluid(
    /* [in] */ Float x)
{
    return ViscousFluidImpl(x, sViscousFluidScale, sViscousFluidNormalize);
}

void Scroller::SetFriction(
    /* [in] */ Float friction)
{
    mDeceleration = ComputeDeceleration(friction);
    mFlingFriction = friction;
}

Float Scroller::ComputeDeceleration(
    /* [in] */ Float friction)
{
    return GRAVITY_EARTH //SensorManager.GRAVITY_EARTH   // g (m/s^2)
                  * 39.37f               // inch/meter
                  * mPpi                 // pixels per inch
                  * friction;
}

Boolean Scroller::IsFinished()
{
    return mFinished;
}

void Scroller::ForceFinished(
    /* [in] */ Boolean finished)
{
    mFinished = finished;
}

Int32 Scroller::GetDuration()
{
    return mDuration;
}

Int32 Scroller::GetCurrX()
{
    return mCurrX;
}

Int32 Scroller::GetCurrY()
{
    return mCurrY;
}

Float Scroller::GetCurrVelocity()
{
    return mMode == FLING_MODE ?
            mCurrVelocity : mVelocity - mDeceleration * TimePassed() / 2000.0f;
}

Int32 Scroller::GetStartX()
{
    return mStartX;
}

Int32 Scroller::GetStartY()
{
    return mStartY;
}

Int32 Scroller::GetFinalX()
{
    return mFinalX;
}

Int32 Scroller::GetFinalY()
{
    return mFinalY;
}

Boolean Scroller::ComputeScrollOffset()
{
    if (mFinished) {
        return FALSE;
    }

    Int32 timePassed = (Int32)(SystemClock::GetUptimeMillis() - mStartTime);

    if (timePassed < mDuration) {
        switch (mMode) {
        case SCROLL_MODE:
            {
                Float x = (Float)timePassed * mDurationReciprocal;

                if (mInterpolator == NULL) {
                    x = ViscousFluid(x);
                }
                else {
                    Float xTemp;
                    mInterpolator->GetInterpolation(x, &xTemp);
                    x = xTemp;
                }

                mCurrX = mStartX + Elastos::Core::Math::Round(x * mDeltaX );
                mCurrY = mStartY + Elastos::Core::Math::Round(x * mDeltaY);
            }
            break;
        case FLING_MODE:
            {
                Float t = (Float) timePassed / mDuration;
                Int32 index = (Int32) (NB_SAMPLES * t);
                Float distanceCoef = 1.f;
                Float velocityCoef = 0.f;
                if (index < NB_SAMPLES) {
                    Float t_inf = (Float) index / NB_SAMPLES;
                    Float t_sup = (Float) (index + 1) / NB_SAMPLES;
                    Float d_inf = (*SPLINE_POSITION)[index];
                    Float d_sup = (*SPLINE_POSITION)[index + 1];
                    velocityCoef = (d_sup - d_inf) / (t_sup - t_inf);
                    distanceCoef = d_inf + (t - t_inf) * velocityCoef;
                }

                mCurrVelocity = velocityCoef * mDistance / mDuration * 1000.0f;

                mCurrX = mStartX + Elastos::Core::Math::Round(distanceCoef * (mFinalX - mStartX));
                // Pin to mMinX <= mCurrX <= mMaxX
                mCurrX = Elastos::Core::Math::Min(mCurrX, mMaxX);
                mCurrX = Elastos::Core::Math::Max(mCurrX, mMinX);

                mCurrY = mStartY + Elastos::Core::Math::Round(distanceCoef * (mFinalY - mStartY));
                // Pin to mMinY <= mCurrY <= mMaxY
                mCurrY = Elastos::Core::Math::Min(mCurrY, mMaxY);
                mCurrY = Elastos::Core::Math::Max(mCurrY, mMinY);

                if (mCurrX == mFinalX && mCurrY == mFinalY) {
                    mFinished = TRUE;
                }
            }
            break;
        default:
            break;
        }
    }
    else {
        mCurrX = mFinalX;
        mCurrY = mFinalY;
        mFinished = TRUE;
    }

    return TRUE;
}

void Scroller::StartScroll(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    StartScroll(startX, startY, dx, dy, DEFAULT_DURATION);
}

void Scroller::StartScroll(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy,
    /* [in] */ Int32 duration)
{
    mMode = SCROLL_MODE;
    mFinished = FALSE;
    mDuration = duration;
    mStartTime = AnimationUtils::CurrentAnimationTimeMillis();
    mStartX = startX;
    mStartY = startY;
    mFinalX = startX + dx;
    mFinalY = startY + dy;
    mDeltaX = dx;
    mDeltaY = dy;
    mDurationReciprocal = 1.0f / (Float) mDuration;
}

void Scroller::Fling(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 velocityX,
    /* [in] */ Int32 velocityY,
    /* [in] */ Int32 minX,
    /* [in] */ Int32 maxX,
    /* [in] */ Int32 minY,
    /* [in] */ Int32 maxY)
{
    // Continue a scroll or fling in progress
    if (mFlywheel && !mFinished) {
        Float oldVel = GetCurrVelocity();

        Float dx = (Float) (mFinalX - mStartX);
        Float dy = (Float) (mFinalY - mStartY);
        Float hyp = Elastos::Core::Math::Sqrt(dx * dx + dy * dy);//TODO FloatElastos::Core::Math::Sqrt(..);

        Float ndx = dx / hyp;
        Float ndy = dy / hyp;

        Float oldVelocityX = ndx * oldVel;
        Float oldVelocityY = ndy * oldVel;
        if (Elastos::Core::Math::Signum(velocityX) == Elastos::Core::Math::Signum(oldVelocityX) &&
                Elastos::Core::Math::Signum(velocityY) == Elastos::Core::Math::Signum(oldVelocityY)) {
            velocityX += oldVelocityX;
            velocityY += oldVelocityY;
        }
    }

    mMode = FLING_MODE;
    mFinished = FALSE;

    Float velocity = Elastos::Core::Math::Sqrt(velocityX * velocityX + velocityY * velocityY);//TODO FloatMath::Sqrt(..);

    mVelocity = velocity;
    mDuration = GetSplineFlingDuration(velocity);
    mStartTime = AnimationUtils::CurrentAnimationTimeMillis();
    mStartX = startX;
    mStartY = startY;

    Float coeffX = velocity == 0 ? 1.0f : velocityX / velocity;
    Float coeffY = velocity == 0 ? 1.0f : velocityY / velocity;

    Double totalDistance = GetSplineFlingDistance(velocity);
    mDistance = (Int32) (totalDistance * Elastos::Core::Math::Signum(velocity));

    mMinX = minX;
    mMaxX = maxX;
    mMinY = minY;
    mMaxY = maxY;

    mFinalX = startX + (Int32) Elastos::Core::Math::Round(totalDistance * coeffX);
    // Pin to mMinX <= mFinalX <= mMaxX
    mFinalX = Elastos::Core::Math::Min(mFinalX, mMaxX);
    mFinalX = Elastos::Core::Math::Max(mFinalX, mMinX);

    mFinalY = startY + (Int32) Elastos::Core::Math::Round(totalDistance * coeffY);
    // Pin to mMinY <= mFinalY <= mMaxY
    mFinalY = Elastos::Core::Math::Min(mFinalY, mMaxY);
    mFinalY = Elastos::Core::Math::Max(mFinalY, mMinY);
}

Double Scroller::GetSplineDeceleration(
    /* [in] */ Float velocity)
{
    return Elastos::Core::Math::Log(INFLEXION * Elastos::Core::Math::Abs(velocity) / (mFlingFriction * mPhysicalCoeff));
}

Int32 Scroller::GetSplineFlingDuration(
    /* [in] */ Float velocity)
{
    Double l = GetSplineDeceleration(velocity);
    Double decelMinusOne = DECELERATION_RATE - 1.0;
    return (Int32) (1000.0 * Elastos::Core::Math::Exp(l / decelMinusOne));
}

Double Scroller::GetSplineFlingDistance(
    /* [in] */ Float velocity)
{
    Double l = GetSplineDeceleration(velocity);
    Double decelMinusOne = DECELERATION_RATE - 1.0;
    return mFlingFriction * mPhysicalCoeff * Elastos::Core::Math::Exp(DECELERATION_RATE / decelMinusOne * l);
}

void Scroller::AbortAnimation()
{
    mCurrX = mFinalX;
    mCurrY = mFinalY;
    mFinished = TRUE;
}

void Scroller::ExtendDuration(
    /* [in] */ Int32 extend)
{
    Int32 passed = TimePassed();
    mDuration = passed + extend;
    mDurationReciprocal = 1.0f / (Float)mDuration;
    mFinished = FALSE;
}

Int32 Scroller::TimePassed()
{
    return (Int32)(AnimationUtils::CurrentAnimationTimeMillis() - mStartTime);
}

void Scroller::SetFinalX(
    /* [in] */ Int32 newX)
{
    mFinalX = newX;
    mDeltaX = mFinalX - mStartX;
    mFinished = FALSE;
}

void Scroller::SetFinalY(
    /* [in] */ Int32 newY)
{
    mFinalY = newY;
    mDeltaY = mFinalY - mStartY;
    mFinished = FALSE;
}

Boolean Scroller::IsScrollingInDirection(
    /* [in] */ Float xvel,
    /* [in] */ Float yvel)
{
    return !mFinished && Elastos::Core::Math::Signum(xvel) == Elastos::Core::Math::Signum(mFinalX - mStartX) &&
            Elastos::Core::Math::Signum(yvel) == Elastos::Core::Math::Signum(mFinalY - mStartY);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
