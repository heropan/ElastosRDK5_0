
#include "widget/OverScroller.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>
#include "os/SystemClock.h"
#include "view/animation/AnimationUtils.h"
#include "view/CViewConfigurationHelper.h"
#include "widget/Scroller.h"

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::View::Animation::AnimationUtils;
using Elastos::Droid::View::IViewConfigurationHelper;
using Elastos::Droid::View::CViewConfigurationHelper;

namespace Elastos{
namespace Droid{
namespace Widget{

const Int32 OverScroller::DEFAULT_DURATION;
const Int32 OverScroller::SCROLL_MODE;
const Int32 OverScroller::FLING_MODE;

const Float GRAVITY_EARTH = 9.80665f;

OverScroller::OverScroller(
    /* [in] */ IContext* context)
{
    Init(context, NULL, TRUE);
}

OverScroller::OverScroller(
    /* [in] */ IContext* context,
    /* [in] */ IInterpolator* interpolator)
{
    Init(context, interpolator, TRUE);
}

OverScroller::OverScroller(
    /* [in] */ IContext* context,
    /* [in] */ IInterpolator* interpolator,
    /* [in] */ Boolean flywheel)
{
    Init(context, interpolator, TRUE);
}

OverScroller::OverScroller(
    /* [in] */ IContext* context,
    /* [in] */ IInterpolator* interpolator,
    /* [in] */ Float bounceCoefficientX,
    /* [in] */ Float bounceCoefficientY)
{
    Init(context, interpolator, TRUE);
}

OverScroller::OverScroller(
    /* [in] */ IContext* context,
    /* [in] */ IInterpolator* interpolator,
    /* [in] */ Float bounceCoefficientX,
    /* [in] */ Float bounceCoefficientY,
    /* [in] */ Boolean flywheel)
{
    Init(context, interpolator, flywheel);
}

void OverScroller::Init(
    /* [in] */ IContext* context,
    /* [in] */ IInterpolator* interpolator,
    /* [in] */ Boolean flywheel)
{
    mMode = 0;
    mFlywheel = 0;
    mInterpolator = interpolator;
    mFlywheel = flywheel;

    mScrollerX = new SplineOverScroller(context);
    mScrollerY = new SplineOverScroller(context);
}

void OverScroller::SetInterpolator(
    /* [in] */ IInterpolator* interpolator)
{
    mInterpolator = interpolator;
}

void OverScroller::SetFriction(
    /* [in] */ Float friction)
{
    mScrollerX->SetFriction(friction);
    mScrollerY->SetFriction(friction);
}

Boolean OverScroller::IsFinished()
{
    return mScrollerX->mFinished && mScrollerY->mFinished;
}

void OverScroller::ForceFinished(
    /* [in] */ Boolean finished)
{
    mScrollerX->mFinished = mScrollerY->mFinished = finished;
}

Int32 OverScroller::GetCurrX()
{
    return mScrollerX->mCurrentPosition;
}

Int32 OverScroller::GetCurrY()
{
    return mScrollerY->mCurrentPosition;
}

Float OverScroller::GetCurrVelocity()
{
    Float squaredNorm = mScrollerX->mCurrVelocity * mScrollerX->mCurrVelocity;
    squaredNorm += mScrollerY->mCurrVelocity * mScrollerY->mCurrVelocity;
    return (Float)Elastos::Core::Math::Sqrt(squaredNorm);
}

Int32 OverScroller::GetStartX()
{
    return mScrollerX->mStart;
}

Int32 OverScroller::GetStartY()
{
    return mScrollerY->mStart;
}

Int32 OverScroller::GetFinalX()
{
    return mScrollerX->mFinal;
}

Int32 OverScroller::GetFinalY()
{
    return mScrollerY->mFinal;
}

Int32 OverScroller::GetDuration()
{
    return Elastos::Core::Math::Max(mScrollerX->mDuration, mScrollerY->mDuration);
}

void OverScroller::ExtendDuration(
    /* [in] */ Int32 extend)
{
    mScrollerX->ExtendDuration(extend);
    mScrollerY->ExtendDuration(extend);
}

void OverScroller::SetFinalX(
    /* [in] */ Int32 newX)
{
    mScrollerX->SetFinalPosition(newX);
}

void OverScroller::SetFinalY(
    /* [in] */ Int32 newY)
{
    mScrollerY->SetFinalPosition(newY);
}

Boolean OverScroller::ComputeScrollOffset()
{
    if (IsFinished()) {
        return FALSE;
    }

    switch (mMode) {
    case SCROLL_MODE:
        {
            Int64 time = AnimationUtils::CurrentAnimationTimeMillis();
            // Any scroller can be used for time, since they were started
            // together in scroll mode. We use X here.
            Int64 elapsedTime = time - mScrollerX->mStartTime;

            Int32 duration = mScrollerX->mDuration;
            if (elapsedTime < duration) {
                Float q = (Float)(elapsedTime) / duration;

                if (mInterpolator == NULL) {
                    q = Scroller::ViscousFluid(q);
                }
                else {
                    mInterpolator->GetInterpolation(q, &q);
                }

                mScrollerX->UpdateScroll(q);
                mScrollerY->UpdateScroll(q);
            }
            else {
                AbortAnimation();
            }
        }
        break;
    case FLING_MODE:
        {
            if (!mScrollerX->mFinished) {
                if (!mScrollerX->Update()) {
                    if (!mScrollerX->ContinueWhenFinished()) {
                        mScrollerX->Finish();
                    }
                }
            }

            if (!mScrollerY->mFinished) {
                if (!mScrollerY->Update()) {
                    if (!mScrollerY->ContinueWhenFinished()) {
                        mScrollerY->Finish();
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    return TRUE;
}

void OverScroller::StartScroll(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    StartScroll(startX, startY, dx, dy, DEFAULT_DURATION);
}

void OverScroller::StartScroll(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy,
    /* [in] */ Int32 duration)
{
    mMode = SCROLL_MODE;
    mScrollerX->StartScroll(startX, dx, duration);
    mScrollerY->StartScroll(startY, dy, duration);
}

Boolean OverScroller::SpringBack(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 minX,
    /* [in] */ Int32 maxX,
    /* [in] */ Int32 minY,
    /* [in] */ Int32 maxY)
{
    mMode = FLING_MODE;

    // Make sure both methods are called.
    Boolean spingbackX = mScrollerX->Springback(startX, minX, maxX);
    Boolean spingbackY = mScrollerY->Springback(startY, minY, maxY);

    return spingbackX || spingbackY;
}

void OverScroller::Fling(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 velocityX,
    /* [in] */ Int32 velocityY,
    /* [in] */ Int32 minX,
    /* [in] */ Int32 maxX,
    /* [in] */ Int32 minY,
    /* [in] */ Int32 maxY,
    /* [in] */ Int32 overX/* = 0*/,
    /* [in] */ Int32 overY/* = 0*/)
{
    // Continue a scroll or fling in progress
    if (mFlywheel && !IsFinished()) {
        Float oldVelocityX = mScrollerX->mCurrVelocity;
        Float oldVelocityY = mScrollerY->mCurrVelocity;
        if (Elastos::Core::Math::Signum(velocityX) == Elastos::Core::Math::Signum(oldVelocityX) &&
            Elastos::Core::Math::Signum(velocityY) == Elastos::Core::Math::Signum(oldVelocityY)) {
                velocityX += oldVelocityX;
                velocityY += oldVelocityY;
        }
    }

    mMode = FLING_MODE;
    mScrollerX->Fling(startX, velocityX, minX, maxX, overX);
    mScrollerY->Fling(startY, velocityY, minY, maxY, overY);
}

void OverScroller::NotifyHorizontalEdgeReached(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 finalX,
    /* [in] */ Int32 overX)
{
    mScrollerX->NotifyEdgeReached(startX, finalX, overX);
}

void OverScroller::NotifyVerticalEdgeReached(
    /* [in] */ Int32 startY,
    /* [in] */ Int32 finalY,
    /* [in] */ Int32 overY)
{
    mScrollerY->NotifyEdgeReached(startY, finalY, overY);
}

Boolean OverScroller::IsOverScrolled()
{
    return ((!mScrollerX->mFinished &&
        mScrollerX->mState != SplineOverScroller::SPLINE) ||
        (!mScrollerY->mFinished &&
        mScrollerY->mState != SplineOverScroller::SPLINE));
}

void OverScroller::AbortAnimation()
{
    mScrollerX->Finish();
    mScrollerY->Finish();
}

Int32 OverScroller::TimePassed()
{
    Int64 time = AnimationUtils::CurrentAnimationTimeMillis();
    Int64 startTime = Elastos::Core::Math::Min(mScrollerX->mStartTime, mScrollerY->mStartTime);

    return (Int32)(time - startTime);
}

Boolean OverScroller::IsScrollingInDirection(
    /* [in] */ Float xvel,
    /* [in] */ Float yvel)
{
    Int32 dx = mScrollerX->mFinal - mScrollerX->mStart;
    Int32 dy = mScrollerY->mFinal - mScrollerY->mStart;
    return !IsFinished() && Elastos::Core::Math::Signum(xvel) == Elastos::Core::Math::Signum(dx) &&
            Elastos::Core::Math::Signum(yvel) == Elastos::Core::Math::Signum(dy);
}


//==============================================================================
//          SplineOverScroller
//===============================================================================

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

const Float SplineOverScroller::GRAVITY;
Float SplineOverScroller::DECELERATION_RATE = InitDECELERATION_RATE();
const Float SplineOverScroller::INFLEXION;
const Float SplineOverScroller::START_TENSION;
const Float SplineOverScroller::END_TENSION;
const Float SplineOverScroller::P1 = 0.5f * 0.35f; //START_TENSION * INFLEXION;
const Float SplineOverScroller::P2 = 1.0f - 1.0f * (1.0f - 0.35f); //1.0f - END_TENSION * (1.0f - INFLEXION);
const Int32 SplineOverScroller::NB_SAMPLES;

AutoPtr<ArrayOf<Float> > SplineOverScroller::SPLINE_POSITION = InitSPLINE_POSITION();
AutoPtr<ArrayOf<Float> > SplineOverScroller::SPLINE_TIME = InitSPLINE_TIME();

const Int32 SplineOverScroller::SPLINE;
const Int32 SplineOverScroller::CUBIC;
const Int32 SplineOverScroller::BALLISTIC;

SplineOverScroller::SplineOverScroller(
    /* [in] */ IContext* context)
    : mStart(0)
    , mCurrentPosition(0)
    , mFinal(0)
    , mVelocity(0)
    , mCurrVelocity(0)
    , mDeceleration(0)
    , mStartTime(0)
    , mDuration(0)
    , mSplineDuration(0)
    , mSplineDistance(0)
    , mFinished(TRUE)
    , mOver(0)
    , mState(SplineOverScroller::SPLINE)
{
    AutoPtr<IViewConfigurationHelper> helper;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&helper);
    helper->GetScrollFriction(&mFlingFriction);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> metrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Float density;
    metrics->GetDensity(&density);
    Float ppi = density * 160.0f;
    mPhysicalCoeff = GRAVITY_EARTH //ISensorManager::GRAVITY_EARTH // g (m/s^2)
            * 39.37f // inch/meter
            * ppi
            * 0.84f; // look and feel tuning
}

void SplineOverScroller::SetFriction(
        /* [in] */ Float friction)
{
    mFlingFriction = friction;
}

void SplineOverScroller::UpdateScroll(
        /* [in] */ Float q)
{
    mCurrentPosition = mStart + Elastos::Core::Math::Round(q * (mFinal - mStart));
}

Float SplineOverScroller::GetDeceleration(
    /* [in] */ Int32 velocity)
{
    return velocity > 0 ? -GRAVITY : GRAVITY;
}

void SplineOverScroller::AdjustDuration(
    /* [in] */ Int32 start,
    /* [in] */ Int32 oldFinal,
    /* [in] */ Int32 newFinal)
{
    Int32 oldDistance = oldFinal - start;
    Int32 newDistance = newFinal - start;
    Float x = Elastos::Core::Math::Abs((Float) newDistance / oldDistance);
    Int32 index = (Int32) (NB_SAMPLES * x);
    if (index < NB_SAMPLES) {
        Float x_inf = (Float) index / NB_SAMPLES;
        Float x_sup = (Float) (index + 1) / NB_SAMPLES;
        Float t_inf = (*SPLINE_TIME)[index];
        Float t_sup = (*SPLINE_TIME)[index + 1];
        Float timeCoef = t_inf + (x - x_inf) / (x_sup - x_inf) * (t_sup - t_inf);
        mDuration *= timeCoef;
    }
}

void SplineOverScroller::StartScroll(
    /* [in] */ Int32 start,
    /* [in] */ Int32 distance,
    /* [in] */ Int32 duration)
{
    mFinished = FALSE;

    mStart = start;
    mFinal = start + distance;

    mStartTime = 0;// TODO AnimationUtils::CurrentAnimationTimeMillis();
    mDuration = duration;

    // Unused
    mDeceleration = 0.0f;
    mVelocity = 0;
}

void SplineOverScroller::Finish()
{
    mCurrentPosition = mFinal;
    // Not reset since WebView relies on this value for fast Fling.
    // TODO: restore when WebView uses the fast Fling implemented in this class.
    // mCurrVelocity = 0.0f;
    mFinished = TRUE;
}

void SplineOverScroller::SetFinalPosition(
    /* [in] */ Int32 position)
{
    mFinal = position;
    mFinished = FALSE;
}

void SplineOverScroller::ExtendDuration(
        /* [in] */ Int32 extend)
{
    Int64 time = AnimationUtils::CurrentAnimationTimeMillis();
    Int32 elapsedTime = (Int32) (time - mStartTime);
    mDuration = elapsedTime + extend;
    mFinished = FALSE;
}

Boolean SplineOverScroller::Springback(
    /* [in] */ Int32 start,
    /* [in] */ Int32 min,
    /* [in] */ Int32 max)
{
    mFinished = TRUE;

    mStart = mFinal = start;
    mVelocity = 0;

    mStartTime = AnimationUtils::CurrentAnimationTimeMillis();
    mDuration = 0;

    if (start < min) {
        StartSpringback(start, min, 0);
    } else if (start > max) {
        StartSpringback(start, max, 0);
    }

    return !mFinished;
}

void SplineOverScroller::StartSpringback(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 velocity)
{
    // mStartTime has been set
    mFinished = FALSE;
    mState = CUBIC;
    mStart = start;
    mFinal = end;
    Int32 delta = start - end;
    mDeceleration = GetDeceleration(delta);
    // TODO take velocity into account
    mVelocity = -delta; // only sign is used
    mOver = Elastos::Core::Math::Abs(delta);
    mDuration = (Int32) (1000.0 * Elastos::Core::Math::Sqrt(-2.0 * delta / mDeceleration));
}

void SplineOverScroller::Fling(
    /* [in] */ Int32 start,
    /* [in] */ Int32 velocity,
    /* [in] */ Int32 min,
    /* [in] */ Int32 max,
    /* [in] */ Int32 over)
{
    mOver = over;
    mFinished = FALSE;
    mCurrVelocity = mVelocity = velocity;
    mDuration = mSplineDuration = 0;
    mStartTime = AnimationUtils::CurrentAnimationTimeMillis();
    mCurrentPosition = mStart = start;

    if (start > max || start < min) {
        StartAfterEdge(start, min, max, velocity);
        return;
    }

    mState = SPLINE;
    Double totalDistance = 0.0;

    if (velocity != 0) {
        mDuration = mSplineDuration = GetSplineFlingDuration(velocity);
        totalDistance = GetSplineFlingDistance(velocity);
    }

    mSplineDistance = (Int32) (totalDistance * Elastos::Core::Math::Signum(velocity));
    mFinal = start + mSplineDistance;

    // Clamp to a valid position
    if (mFinal < min) {
        AdjustDuration(mStart, mFinal, min);
        mFinal = min;
    }

    if (mFinal > max) {
        AdjustDuration(mStart, mFinal, max);
        mFinal = max;
    }
}

Double SplineOverScroller::GetSplineDeceleration(
        /* [in] */ Int32 velocity)
{
    return Elastos::Core::Math::Log(INFLEXION * Elastos::Core::Math::Abs(velocity) / (mFlingFriction * mPhysicalCoeff));
}

Double SplineOverScroller::GetSplineFlingDistance(
        /* [in] */ Int32 velocity)
{
    Double l = GetSplineDeceleration(velocity);
    Double decelMinusOne = DECELERATION_RATE - 1.0;
    return mFlingFriction * mPhysicalCoeff * Elastos::Core::Math::Exp(DECELERATION_RATE / decelMinusOne * l);
}

Int32 SplineOverScroller::GetSplineFlingDuration(
        /* [in] */ Int32 velocity)
{
    Double l = GetSplineDeceleration(velocity);
    Double decelMinusOne = DECELERATION_RATE - 1.0;
    return (Int32) (1000.0 * Elastos::Core::Math::Exp(l / decelMinusOne));
}

void SplineOverScroller::FitOnBounceCurve(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 velocity)
{
    // Simulate a bounce that started from edge
    Float durationToApex = - velocity / mDeceleration;
    Float distanceToApex = velocity * velocity / 2.0f / Elastos::Core::Math::Abs(mDeceleration);
    Float distanceToEdge = Elastos::Core::Math::Abs(end - start);
    Float totalDuration = (Float) Elastos::Core::Math::Sqrt(
            2.0 * (distanceToApex + distanceToEdge) / Elastos::Core::Math::Abs(mDeceleration));
    mStartTime -= (Int32) (1000.0f * (totalDuration - durationToApex));
    mStart = end;
    mVelocity = (Int32) (- mDeceleration * totalDuration);
}

void SplineOverScroller::StartBounceAfterEdge(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 velocity)
{
    mDeceleration = GetDeceleration(velocity == 0 ? start - end : velocity);
    FitOnBounceCurve(start, end, velocity);
    OnEdgeReached();
}

void SplineOverScroller::StartAfterEdge(
    /* [in] */ Int32 start,
    /* [in] */ Int32 min,
    /* [in] */ Int32 max,
    /* [in] */ Int32 velocity)
{
    if (start > min && start < max) {
        Logger::E("OverScroller", "StartAfterEdge called from a valid position");
        mFinished = TRUE;
        return;
    }
    Boolean positive = start > max;
    Int32 edge = positive ? max : min;
    Int32 overDistance = start - edge;
    Boolean keepIncreasing = overDistance * velocity >= 0;
    if (keepIncreasing) {
        // Will result in a bounce or a to_boundary depending on velocity.
        StartBounceAfterEdge(start, edge, velocity);
    } else {
        Double totalDistance = GetSplineFlingDistance(velocity);
        if (totalDistance > Elastos::Core::Math::Abs(overDistance)) {
            Fling(start, velocity, positive ? min : start, positive ? start : max, mOver);
        } else {
            StartSpringback(start, edge, velocity);
        }
    }
}

void SplineOverScroller::NotifyEdgeReached(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 over)
{
    // mState is used to detect successive notifications
    if (mState == SPLINE) {
        mOver = over;
        mStartTime = AnimationUtils::CurrentAnimationTimeMillis();
        // We were in Fling/scroll mode before: current velocity is such that distance to
        // edge is increasing. This ensures that StartAfterEdge will not start a new Fling.
        StartAfterEdge(start, end, end, (Int32) mCurrVelocity);
    }
}

void SplineOverScroller::OnEdgeReached()
{
    // mStart, mVelocity and mStartTime were adjusted to their values when edge was reached.
    Float distance = mVelocity * mVelocity / (2.0f * Elastos::Core::Math::Abs(mDeceleration));
    Float sign = Elastos::Core::Math::Signum(mVelocity);

    if (distance > mOver) {
        // Default deceleration is not sufficient to slow us down before boundary
         mDeceleration = - sign * mVelocity * mVelocity / (2.0f * mOver);
         distance = mOver;
    }

    mOver = (Int32) distance;
    mState = BALLISTIC;
    mFinal = mStart + (Int32) (mVelocity > 0 ? distance : -distance);
    mDuration = - (Int32) (1000.0f * mVelocity / mDeceleration);
}

Boolean SplineOverScroller::ContinueWhenFinished()
{
    switch (mState) {
        case SPLINE:
            // Duration from start to null velocity
            if (mDuration < mSplineDuration) {
                // If the animation was clamped, we reached the edge
                mStart = mFinal;
                // TODO Better compute speed when edge was reached
                mVelocity = (Int32) mCurrVelocity;
                mDeceleration = GetDeceleration(mVelocity);
                mStartTime += mDuration;
                OnEdgeReached();
            } else {
                // Normal stop, no need to continue
                return FALSE;
            }
            break;
        case BALLISTIC:
            mStartTime += mDuration;
            StartSpringback(mFinal, mStart, 0);
            break;
        case CUBIC:
            return FALSE;
    }

    Update();
    return TRUE;
}

Boolean SplineOverScroller::Update()
{
    Int64 time = AnimationUtils::CurrentAnimationTimeMillis();
    Int64 currentTime = time - mStartTime;

    if (currentTime > mDuration) {
        return FALSE;
    }

    Double distance = 0.0;
    switch (mState) {
        case SPLINE: {
            Float t = (Float) currentTime / mSplineDuration;
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

            distance = distanceCoef * mSplineDistance;
            mCurrVelocity = velocityCoef * mSplineDistance / mSplineDuration * 1000.0f;
            break;
        }

        case BALLISTIC: {
            Float t = currentTime / 1000.0f;
            mCurrVelocity = mVelocity + mDeceleration * t;
            distance = mVelocity * t + mDeceleration * t * t / 2.0f;
            break;
        }

        case CUBIC: {
            Float t = (Float) (currentTime) / mDuration;
            Float t2 = t * t;
            Float sign = Elastos::Core::Math::Signum(mVelocity);
            distance = sign * mOver * (3.0f * t2 - 2.0f * t * t2);
            mCurrVelocity = sign * mOver * 6.0f * (- t + t2);
            break;
        }
    }

    mCurrentPosition = mStart + (Int32) Elastos::Core::Math::Round(distance);

    return TRUE;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
