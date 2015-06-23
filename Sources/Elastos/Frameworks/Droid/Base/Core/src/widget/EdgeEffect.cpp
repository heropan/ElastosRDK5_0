#include <R.h>
#include "ext/frameworkext.h"
#include "widget/EdgeEffect.h"
#include "view/animation/CAnimationUtils.h"
#include "os/SystemClock.h"
#include "view/animation/CDecelerateInterpolator.h"
#include <elastos/core/Math.h>
#include "graphics/CRect.h"

using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::View::Animation::IDecelerateInterpolator;
using Elastos::Droid::View::Animation::CDecelerateInterpolator;
using Elastos::Droid::View::Animation::IAnimationUtils;
using Elastos::Droid::View::Animation::CAnimationUtils;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Content::Res::IResources;


namespace Elastos {
namespace Droid {
namespace Widget {

EdgeEffect::EdgeEffect()
        : mWidth(0)
        , mHeight(0)
        , mX(0)
        , mY(0)
        , mMinWidth(0)
        , mEdgeAlpha(0.0f)
        , mEdgeScaleY(0.0f)
        , mGlowAlpha(0.0f)
        , mGlowScaleY(0.0f)
        , mEdgeAlphaStart(0.0f)
        , mEdgeAlphaFinish(0.0f)
        , mEdgeScaleYStart(0.0f)
        , mEdgeScaleYFinish(0.0f)
        , mGlowAlphaStart(0.0f)
        , mGlowAlphaFinish(0.0f)
        , mGlowScaleYStart(0.0f)
        , mGlowScaleYFinish(0.0f)
        , mStartTime(0)
        , mDuration(0.0f)
        , mState(0)
        , mPullDistance(0.0f)
        , mEdgeHeight(0)
        , mGlowHeight(0)
        , mGlowWidth(0)
        , mMaxEffectHeight(0)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mBounds));
}

EdgeEffect::EdgeEffect(
    /* [in] */ IContext* context)
        : mWidth(0)
        , mHeight(0)
        , mX(0)
        , mY(0)
        , mMinWidth(0)
        , mEdgeAlpha(0.0f)
        , mEdgeScaleY(0.0f)
        , mGlowAlpha(0.0f)
        , mGlowScaleY(0.0f)
        , mEdgeAlphaStart(0.0f)
        , mEdgeAlphaFinish(0.0f)
        , mEdgeScaleYStart(0.0f)
        , mEdgeScaleYFinish(0.0f)
        , mGlowAlphaStart(0.0f)
        , mGlowAlphaFinish(0.0f)
        , mGlowScaleYStart(0.0f)
        , mGlowScaleYFinish(0.0f)
        , mStartTime(0)
        , mDuration(0.0f)
        , mState(0)
        , mPullDistance(0.0f)
        , mEdgeHeight(0)
        , mGlowHeight(0)
        , mGlowWidth(0)
        , mMaxEffectHeight(0)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mBounds));
    Init(context);
}

ECode EdgeEffect::Init(
    /* [in] */ IContext* contex)
{
    AutoPtr<IResources> res;
    contex->GetResources((IResources**)&res);
    res->GetDrawable(R::drawable::overscroll_edge, (IDrawable**)&mEdge);
    res->GetDrawable(R::drawable::overscroll_glow, (IDrawable**)&mGlow);

    mEdge->GetIntrinsicHeight(&mEdgeHeight);
    mGlow->GetIntrinsicHeight(&mGlowHeight);
    mGlow->GetIntrinsicWidth(&mGlowWidth);

    mMaxEffectHeight = (Int32)(Elastos::Core::Math::Min(
            mGlowWidth * MAX_GLOW_HEIGHT * mGlowHeight / mGlowWidth * 0.6f,
            mGlowHeight* MAX_GLOW_HEIGHT) + 0.5f);

    AutoPtr<IDisplayMetrics> metrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Float density;
    metrics->GetDensity(&density);
    mMinWidth = (Int32) (density * MIN_WIDTH + 0.5f);
    ASSERT_SUCCEEDED(CDecelerateInterpolator::New((IDecelerateInterpolator**)&mInterpolator));

    return NOERROR;
}

void EdgeEffect::SetSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mWidth = width;
    mHeight = height;
}

/**
 * Set the position of this edge effect in pixels. This position is
 * only used by {@link #getBounds(boolean)}.
 *
 * @param x The position of the edge effect on the X axis
 * @param y The position of the edge effect on the Y axis
 */
void EdgeEffect::SetPosition(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    mX = x;
    mY = y;
}

/**
 * Reports if this EdgeEffect's animation is finished. If this method returns false
 * after a call to {@link #draw(Canvas)} the host widget should schedule another
 * drawing pass to continue the animation.
 *
 * @return true if animation is finished, false if drawing should continue on the next frame.
 */
Boolean EdgeEffect::IsFinished()
{
    return mState == STATE_IDLE;
}

void EdgeEffect::Finish()
{
    mState = STATE_IDLE;
}

/**
 * Call when the object is pulled by the user.
 *
 * @param deltaDistance Change in distance since the last call
 */
void EdgeEffect::OnPull(
    /* [in] */ Float deltaDistance)
{
    AutoPtr<IAnimationUtils> utils;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&utils);
    Int64 now;
    utils->CurrentAnimationTimeMillis(&now);
    if (mState == STATE_PULL_DECAY && now - mStartTime < mDuration) {
        return;
    }

    if (mState != STATE_PULL) {
        mGlowScaleY = PULL_GLOW_BEGIN;
    }

    mState = STATE_PULL;

    mStartTime = now;
    mDuration = PULL_TIME;

    mPullDistance += deltaDistance;
    Float distance = Elastos::Core::Math::Abs(mPullDistance);

    mEdgeAlpha = mEdgeAlphaStart = Elastos::Core::Math::Max(PULL_EDGE_BEGIN, Elastos::Core::Math::Min(distance, MAX_ALPHA));
    mEdgeScaleY = mEdgeScaleYStart = Elastos::Core::Math::Max(
        HELD_EDGE_SCALE_Y, Elastos::Core::Math::Min(distance * PULL_DISTANCE_EDGE_FACTOR, 1.f));

    mGlowAlpha = mGlowAlphaStart = Elastos::Core::Math::Min(MAX_ALPHA,
        mGlowAlpha + (Elastos::Core::Math::Abs(deltaDistance) * PULL_DISTANCE_ALPHA_GLOW_FACTOR));

    Float glowChange = Elastos::Core::Math::Abs(deltaDistance);
    if (deltaDistance > 0 && mPullDistance < 0) {
        glowChange = -glowChange;
    }

    if (mPullDistance == 0) {
        mGlowScaleY = 0;
    }

    // Do not allow glow to get larger than MAX_GLOW_HEIGHT.
    mGlowScaleY = mGlowScaleYStart = Elastos::Core::Math::Min(MAX_GLOW_HEIGHT, Elastos::Core::Math::Max(
        0.f, mGlowScaleY + glowChange * PULL_DISTANCE_GLOW_FACTOR));

    mEdgeAlphaFinish = mEdgeAlpha;
    mEdgeScaleYFinish = mEdgeScaleY;
    mGlowAlphaFinish = mGlowAlpha;
    mGlowScaleYFinish = mGlowScaleY;
}

/**
 * Call when the object is released after being pulled.
 */
void EdgeEffect::OnRelease()
{
    mPullDistance = 0;

    if (mState != STATE_PULL && mState != STATE_PULL_DECAY) {
        return;
    }

    mState = STATE_RECEDE;
    mEdgeAlphaStart = mEdgeAlpha;
    mEdgeScaleYStart = mEdgeScaleY;
    mGlowAlphaStart = mGlowAlpha;
    mGlowScaleYStart = mGlowScaleY;

    mEdgeAlphaFinish = 0.f;
    mEdgeScaleYFinish = 0.f;
    mGlowAlphaFinish = 0.f;
    mGlowScaleYFinish = 0.f;

    AutoPtr<IAnimationUtils> utils;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&utils);
    utils->CurrentAnimationTimeMillis(&mStartTime);
    mDuration = RECEDE_TIME;
}

/**
 * Call when the effect absorbs an impact at the given velocity.
 *
 * @param velocity Velocity at impact in pixels per second.
 */
void EdgeEffect::OnAbsorb(
    /* [in] */ Int32 velocity)
{
    mState = STATE_ABSORB;
    velocity = Elastos::Core::Math::Max(MIN_VELOCITY, Elastos::Core::Math::Abs(velocity));

    AutoPtr<IAnimationUtils> utils;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&utils);
    utils->CurrentAnimationTimeMillis(&mStartTime);
    mDuration = 0.1f + (velocity * 0.03f);

    // The edge should always be at least partially visible, regardless
    // of velocity.
    mEdgeAlphaStart = 0.f;
    mEdgeScaleY = mEdgeScaleYStart = 0.f;
    // The glow depends more on the velocity, and therefore starts out
    // nearly invisible.
    mGlowAlphaStart = 0.5f;
    mGlowScaleYStart = 0.f;

    // Factor the velocity by 8. Testing on device shows this works best to
    // reflect the strength of the user's scrolling.
    mEdgeAlphaFinish = Elastos::Core::Math::Max(0, Elastos::Core::Math::Min(velocity * VELOCITY_EDGE_FACTOR, 1));
    // Edge should never get larger than the size of its asset.
    mEdgeScaleYFinish = Elastos::Core::Math::Max(
            HELD_EDGE_SCALE_Y, Elastos::Core::Math::Min(Float(velocity * VELOCITY_EDGE_FACTOR), 1.f));

    // Growth for the size of the glow should be quadratic to properly
    // respond
    // to a user's scrolling speed. The faster the scrolling speed, the more
    // intense the effect should be for both the size and the saturation.
    mGlowScaleYFinish = Elastos::Core::Math::Min(0.025f + (velocity * (velocity / 100) * 0.00015f), 1.75f);
    // Alpha should change for the glow as well as size.
    mGlowAlphaFinish = Elastos::Core::Math::Max(
            mGlowAlphaStart, Elastos::Core::Math::Min(velocity * VELOCITY_GLOW_FACTOR * .00001f, MAX_ALPHA));
}


/**
 * Draw into the provided canvas. Assumes that the canvas has been rotated
 * accordingly and the size has been set. The effect will be drawn the full
 * width of X=0 to X=width, emitting from Y=0 and extending to some factor <
 * 1.f of height.
 *
 * @param canvas Canvas to draw into
 * @return true if drawing should continue beyond this frame to continue the
 *         animation
 */
Boolean EdgeEffect::Draw(
    /* [in] */ ICanvas* canvas)
{
    Update();

    mGlow->SetAlpha((Int32) (Elastos::Core::Math::Max((Float)0, Elastos::Core::Math::Min(mGlowAlpha, (Float)1)) * 255));

    Int32 glowBottom = (Int32) Elastos::Core::Math::Min(
        mGlowHeight * mGlowScaleY * mGlowHeight / mGlowWidth * 0.6f,
        mGlowHeight * MAX_GLOW_HEIGHT);
    if (mWidth < mMinWidth) {
        // Center the glow and clip it.
        Int32 glowLeft = (mWidth - mMinWidth)/2;
        mGlow->SetBounds(glowLeft, 0, mWidth - glowLeft, glowBottom);
    } else {
        // Stretch the glow to fit.
        mGlow->SetBounds(0, 0, mWidth, glowBottom);
    }

    mGlow->Draw(canvas);

    mEdge->SetAlpha((Int32) (Elastos::Core::Math::Max((Float)0, Elastos::Core::Math::Min(mEdgeAlpha, (Float)1)) * 255));

    Int32 edgeBottom = (Int32) (mEdgeHeight * mEdgeScaleY);
    if (mWidth < mMinWidth) {
        // Center the edge and clip it.
        Int32 edgeLeft = (mWidth - mMinWidth)/2;
        mEdge->SetBounds(edgeLeft, 0, mWidth - edgeLeft, edgeBottom);
    } else {
        // Stretch the edge to fit.
        mEdge->SetBounds(0, 0, mWidth, edgeBottom);
    }
    mEdge->Draw(canvas);

    if (mState == STATE_RECEDE && glowBottom == 0 && edgeBottom == 0) {
        mState = STATE_IDLE;
    }

    return mState != STATE_IDLE;
}

 /**
 * Returns the bounds of the edge effect.
 *
 * @hide
 */
AutoPtr<IRect> EdgeEffect::GetBounds(
    /* [in] */ Boolean reverse)
{
    mBounds->Set(0, 0, mWidth, mMaxEffectHeight);
    mBounds->Offset(mX, mY - (reverse ? mMaxEffectHeight : 0));

    return mBounds;
}

void EdgeEffect::Update()
{
    Int64 time;
    AutoPtr<IAnimationUtils> utils;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&utils);
    utils->CurrentAnimationTimeMillis(&time);
    Float t = Elastos::Core::Math::Min((time - mStartTime) / mDuration, 1.f);

    Float interp;
    mInterpolator->GetInterpolation(t, &interp);

    mEdgeAlpha = mEdgeAlphaStart + (mEdgeAlphaFinish - mEdgeAlphaStart) * interp;
    mEdgeScaleY = mEdgeScaleYStart + (mEdgeScaleYFinish - mEdgeScaleYStart) * interp;
    mGlowAlpha = mGlowAlphaStart + (mGlowAlphaFinish - mGlowAlphaStart) * interp;
    mGlowScaleY = mGlowScaleYStart + (mGlowScaleYFinish - mGlowScaleYStart) * interp;

    if (t >= 1.f - EPSILON) {
        switch (mState) {
        case STATE_ABSORB:
            {
                mState = STATE_RECEDE;
                utils->CurrentAnimationTimeMillis(&mStartTime);
                mDuration = RECEDE_TIME;

                mEdgeAlphaStart = mEdgeAlpha;
                mEdgeScaleYStart = mEdgeScaleY;
                mGlowAlphaStart = mGlowAlpha;
                mGlowScaleYStart = mGlowScaleY;

                // After absorb, the glow and edge should fade to nothing.
                mEdgeAlphaFinish = 0.f;
                mEdgeScaleYFinish = 0.f;
                mGlowAlphaFinish = 0.f;
                mGlowScaleYFinish = 0.f;
            }
            break;
        case STATE_PULL:
            {
                mState = STATE_PULL_DECAY;
                utils->CurrentAnimationTimeMillis(&mStartTime);
                mDuration = PULL_DECAY_TIME;

                mEdgeAlphaStart = mEdgeAlpha;
                mEdgeScaleYStart = mEdgeScaleY;
                mGlowAlphaStart = mGlowAlpha;
                mGlowScaleYStart = mGlowScaleY;

                // After pull, the glow and edge should fade to nothing.
                mEdgeAlphaFinish = 0.f;
                mEdgeScaleYFinish = 0.f;
                mGlowAlphaFinish = 0.f;
                mGlowScaleYFinish = 0.f;
            }
            break;
        case STATE_PULL_DECAY:
            {
                // When receding, we want edge to decrease more slowly
                // than the glow.
                Float factor = mGlowScaleYFinish != 0 ? 1
                    / (mGlowScaleYFinish * mGlowScaleYFinish)
                    : Elastos::Core::Math::FLOAT_MIN_VALUE;
                mEdgeScaleY = mEdgeScaleYStart +
                    (mEdgeScaleYFinish - mEdgeScaleYStart) *
                    interp * factor;
                mState = STATE_RECEDE;
            }
            break;
        case STATE_RECEDE:
            mState = STATE_IDLE;
            break;
        default:
            break;
        }
    }
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos

