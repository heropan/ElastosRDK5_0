
#include "elastos/droid/graphics/CPorterDuffXfermode.h"
#include "elastos/droid/R.h"
#include "elastos/droid/utility/FloatMath.h"
#include "elastos/droid/view/animation/CAnimationUtils.h"
#include "elastos/droid/view/animation/CDecelerateInterpolator.h"
#include "elastos/droid/widget/EdgeEffect.h"
#include "elastos/core/Math.h"

using Elastos::Droid::Animation::ITimeInterpolator;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Graphics::CPorterDuffXfermode;
using Elastos::Droid::Graphics::IPorterDuffXfermode;
using Elastos::Droid::Graphics::IXfermode;
using Elastos::Droid::Graphics::PaintStyle_FILL;
using Elastos::Droid::Graphics::PorterDuffMode_SRC_ATOP;
using Elastos::Droid::Utility::FloatMath;
using Elastos::Droid::View::Animation::CAnimationUtils;
using Elastos::Droid::View::Animation::CDecelerateInterpolator;
using Elastos::Droid::View::Animation::IAnimationUtils;
using Elastos::Droid::View::Animation::IDecelerateInterpolator;

namespace Elastos {
namespace Droid {
namespace Widget {

//=====================================================================
//                              EdgeEffect
//=====================================================================
const String EdgeEffect::TAG("EdgeEffect");
const Int32 EdgeEffect::RECEDE_TIME;
const Int32 EdgeEffect::PULL_TIME;
const Int32 EdgeEffect::PULL_DECAY_TIME;
const Float EdgeEffect::MAX_ALPHA = 0.5f;
const Float EdgeEffect::MAX_GLOW_SCALE = 2.f;
const Float EdgeEffect::PULL_GLOW_BEGIN = 0.f;
const Int32 EdgeEffect::MIN_VELOCITY;
const Int32 EdgeEffect::MAX_VELOCITY;
const Float EdgeEffect::EPSILON = 0.001f;
const Double EdgeEffect::ANGLE = Elastos::Core::Math::PI / 6;
const Float EdgeEffect::SIN = (Float) Elastos::Core::Math::Sin(ANGLE);
const Float EdgeEffect::COS = (Float) Elastos::Core::Math::Cos(ANGLE);
const Int32 EdgeEffect::STATE_IDLE;
const Int32 EdgeEffect::STATE_PULL;
const Int32 EdgeEffect::STATE_ABSORB;
const Int32 EdgeEffect::STATE_RECEDE;
const Int32 EdgeEffect::STATE_PULL_DECAY;
const Float EdgeEffect::PULL_DISTANCE_ALPHA_GLOW_FACTOR = 0.8f;
const Int32 EdgeEffect::VELOCITY_GLOW_FACTOR;

CAR_INTERFACE_IMPL(EdgeEffect, Object, IEdgeEffect)

EdgeEffect::EdgeEffect()
{
}

ECode EdgeEffect::constructor(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mPaint.setAntiAlias(true);
    // final TypedArray a = context.obtainStyledAttributes(
    //         com.android.internal.R.styleable.EdgeEffect);
    // final int themeColor = a.getColor(
    //         com.android.internal.R.styleable.EdgeEffect_colorEdgeEffect, 0xff666666);
    // a.recycle();
    // mPaint.setColor((themeColor & 0xffffff) | 0x33000000);
    // mPaint.setStyle(Paint.Style.FILL);
    // mPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.SRC_ATOP));
    // mInterpolator = new DecelerateInterpolator();

    mPaint->SetAntiAlias(TRUE);
    AutoPtr< ArrayOf<Int32> > redgeEffectArray = ArrayOf<Int32>::Alloc(const_cast<Int32*>(R::styleable::EdgeEffect), ARRAY_SIZE(R::styleable::EdgeEffect));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(redgeEffectArray, (ITypedArray**)&a);
    Int32 themeColor = 0;
    a->GetColor(R::styleable::EdgeEffect_colorEdgeEffect, 0xff666666, &themeColor);
    a->Recycle();
    mPaint->SetColor((themeColor & 0xffffff) | 0x33000000);
    mPaint->SetStyle(PaintStyle_FILL);

    AutoPtr<IPorterDuffXfermode> fermode;
    CPorterDuffXfermode::New(PorterDuffMode_SRC_ATOP, (IPorterDuffXfermode**)&fermode);
    mPaint->SetXfermode(IXfermode::Probe(fermode));
    ASSERT_SUCCEEDED(CDecelerateInterpolator::New((IDecelerateInterpolator**)&mInterpolator));
    return NOERROR;
}

ECode EdgeEffect::SetSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    // ==================before translated======================
    // final float r = width * 0.75f / SIN;
    // final float y = COS * r;
    // final float h = r - y;
    // final float or = height * 0.75f / SIN;
    // final float oy = COS * or;
    // final float oh = or - oy;
    //
    // mRadius = r;
    // mBaseGlowScale = h > 0 ? Math.min(oh / h, 1.f) : 1.f;
    //
    // mBounds.set(mBounds.left, mBounds.top, width, (int) Math.min(height, h));

    Float r = width * 0.75f / SIN;
    Float y = COS * r;
    Float h = r - y;
    Float orf = height * 0.75f / SIN;
    Float oyf = COS * orf;
    Float ohf = orf - oyf;

    mRadius = r;
    mBaseGlowScale = h > 0 ? Elastos::Core::Math::Min(ohf / h, 1.f) : 1.f;
    Int32 left, top;
    mBounds->GetLeft(&left);
    mBounds->GetTop(&top);
    mBounds->Set(left, top, width, (Int32)Elastos::Core::Math::Min(height, (Int32)h));
    return NOERROR;
}

ECode EdgeEffect::IsFinished(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mState == STATE_IDLE;

    *result = (mState == STATE_IDLE);
    return NOERROR;
}

ECode EdgeEffect::Finish()
{
    // ==================before translated======================
    // mState = STATE_IDLE;

    mState = STATE_IDLE;
    return NOERROR;
}

ECode EdgeEffect::OnPull(
    /* [in] */ Float deltaDistance)
{
    // ==================before translated======================
    // onPull(deltaDistance, 0.5f);

    return OnPull(deltaDistance, 0.5f);
}

ECode EdgeEffect::OnPull(
    /* [in] */ Float deltaDistance,
    /* [in] */ Float displacement)
{
    // ==================before translated======================
    // final long now = AnimationUtils.currentAnimationTimeMillis();
    // mTargetDisplacement = displacement;
    // if (mState == STATE_PULL_DECAY && now - mStartTime < mDuration) {
    //     return;
    // }
    // if (mState != STATE_PULL) {
    //     mGlowScaleY = Math.max(PULL_GLOW_BEGIN, mGlowScaleY);
    // }
    // mState = STATE_PULL;
    //
    // mStartTime = now;
    // mDuration = PULL_TIME;
    //
    // mPullDistance += deltaDistance;
    //
    // final float absdd = Math.abs(deltaDistance);
    // mGlowAlpha = mGlowAlphaStart = Math.min(MAX_ALPHA,
    //         mGlowAlpha + (absdd * PULL_DISTANCE_ALPHA_GLOW_FACTOR));
    //
    // if (mPullDistance == 0) {
    //     mGlowScaleY = mGlowScaleYStart = 0;
    // } else {
    //     final float scale = Math.max(0, 1 - 1 /
    //             FloatMath.sqrt(Math.abs(mPullDistance) * mBounds.height()) - 0.3f) / 0.7f;
    //
    //     mGlowScaleY = mGlowScaleYStart = scale;
    // }
    //
    // mGlowAlphaFinish = mGlowAlpha;
    // mGlowScaleYFinish = mGlowScaleY;

    assert(0);
    AutoPtr<IAnimationUtils> utils;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&utils);
    Int64 now = 0;
    utils->CurrentAnimationTimeMillis(&now);
    mTargetDisplacement = displacement;
    if (mState == STATE_PULL_DECAY && now - mStartTime < mDuration) {
        return NOERROR;
    }

    if (mState != STATE_PULL) {
        mGlowScaleY = Elastos::Core::Math::Max(PULL_GLOW_BEGIN, mGlowScaleY);
    }

    mState = STATE_PULL;
    mStartTime = now;
    mDuration = PULL_TIME;
    mPullDistance += deltaDistance;

    Float absdd = Elastos::Core::Math::Abs(deltaDistance);
    mGlowAlpha = mGlowAlphaStart = Elastos::Core::Math::Min(MAX_ALPHA, mGlowAlpha + (absdd * PULL_DISTANCE_ALPHA_GLOW_FACTOR));

    if (mPullDistance == 0) {
        mGlowScaleY = mGlowScaleYStart = 0;
    }
    else {
        Int32 height = 0;
        mBounds->GetHeight(&height);
        Float scale = Elastos::Core::Math::Max(0.0f, 1 - 1 / FloatMath::Sqrt(Elastos::Core::Math::Abs(mPullDistance) * height) - 0.3f) / 0.7f;
        mGlowScaleY = mGlowScaleYStart = scale;
    }

    mGlowAlphaFinish = mGlowAlpha;
    mGlowScaleYFinish = mGlowScaleY;
    return NOERROR;
}

ECode EdgeEffect::OnRelease()
{
    // ==================before translated======================
    // mPullDistance = 0;
    //
    // if (mState != STATE_PULL && mState != STATE_PULL_DECAY) {
    //     return;
    // }
    //
    // mState = STATE_RECEDE;
    // mGlowAlphaStart = mGlowAlpha;
    // mGlowScaleYStart = mGlowScaleY;
    //
    // mGlowAlphaFinish = 0.f;
    // mGlowScaleYFinish = 0.f;
    //
    // mStartTime = AnimationUtils.currentAnimationTimeMillis();
    // mDuration = RECEDE_TIME;

    assert(0);
    mPullDistance = 0;
    if (mState != STATE_PULL && mState != STATE_PULL_DECAY) {
        return NOERROR;
    }

    mState = STATE_RECEDE;
    mGlowAlphaStart = mGlowAlpha;
    mGlowScaleYStart = mGlowScaleY;

    mGlowAlphaFinish = 0.f;
    mGlowScaleYFinish = 0.f;

    AutoPtr<IAnimationUtils> utils;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&utils);
    utils->CurrentAnimationTimeMillis(&mStartTime);
    mDuration = RECEDE_TIME;
    return NOERROR;
}

ECode EdgeEffect::OnAbsorb(
    /* [in] */ Int32 velocity)
{
    // ==================before translated======================
    // mState = STATE_ABSORB;
    // velocity = Math.min(Math.max(MIN_VELOCITY, Math.abs(velocity)), MAX_VELOCITY);
    //
    // mStartTime = AnimationUtils.currentAnimationTimeMillis();
    // mDuration = 0.15f + (velocity * 0.02f);
    //
    // // The glow depends more on the velocity, and therefore starts out
    // // nearly invisible.
    // mGlowAlphaStart = 0.3f;
    // mGlowScaleYStart = Math.max(mGlowScaleY, 0.f);
    //
    //
    // // Growth for the size of the glow should be quadratic to properly
    // // respond
    // // to a user's scrolling speed. The faster the scrolling speed, the more
    // // intense the effect should be for both the size and the saturation.
    // mGlowScaleYFinish = Math.min(0.025f + (velocity * (velocity / 100) * 0.00015f) / 2, 1.f);
    // // Alpha should change for the glow as well as size.
    // mGlowAlphaFinish = Math.max(
    //         mGlowAlphaStart, Math.min(velocity * VELOCITY_GLOW_FACTOR * .00001f, MAX_ALPHA));
    // mTargetDisplacement = 0.5f;

    assert(0);
    mState = STATE_ABSORB;
    velocity = Elastos::Core::Math::Min(Elastos::Core::Math::Max(MIN_VELOCITY, Elastos::Core::Math::Abs(velocity)), MAX_VELOCITY);

    AutoPtr<IAnimationUtils> utils;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&utils);
    utils->CurrentAnimationTimeMillis(&mStartTime);
    mDuration = 0.15f + (velocity * 0.02f);

    // The glow depends more on the velocity, and therefore starts out
    // nearly invisible.
    mGlowAlphaStart = 0.3f;
    mGlowScaleYStart = Elastos::Core::Math::Max(mGlowScaleY, 0.f);

    // Growth for the size of the glow should be quadratic to properly
    // respond
    // to a user's scrolling speed. The faster the scrolling speed, the more
    // intense the effect should be for both the size and the saturation.
    mGlowScaleYFinish = Elastos::Core::Math::Min(0.025f + (velocity * (velocity / 100) * 0.00015f) / 2, 1.f);
    // Alpha should change for the glow as well as size.
    mGlowAlphaFinish = Elastos::Core::Math::Max(mGlowAlphaStart, Elastos::Core::Math::Min(velocity * VELOCITY_GLOW_FACTOR * 0.00001f, MAX_ALPHA));
    mTargetDisplacement = 0.5f;
    return NOERROR;
}

ECode EdgeEffect::SetColor(
    /* [in] */ Int32 color)
{
    // ==================before translated======================
    // mPaint.setColor(color);

    mPaint->SetColor(color);
    return NOERROR;
}

ECode EdgeEffect::GetColor(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mPaint.getColor();

    return mPaint->GetColor(result);
}

ECode EdgeEffect::Draw(
    /* [in] */ ICanvas* canvas,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(canvas);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // update();
    //
    // final int count = canvas.save();
    //
    // final float centerX = mBounds.centerX();
    // final float centerY = mBounds.height() - mRadius;
    //
    // canvas.scale(1.f, Math.min(mGlowScaleY, 1.f) * mBaseGlowScale, centerX, 0);
    //
    // final float displacement = Math.max(0, Math.min(mDisplacement, 1.f)) - 0.5f;
    // float translateX = mBounds.width() * displacement / 2;
    //
    // canvas.clipRect(mBounds);
    // canvas.translate(translateX, 0);
    // mPaint.setAlpha((int) (0xff * mGlowAlpha));
    // canvas.drawCircle(centerX, centerY, mRadius, mPaint);
    // canvas.restoreToCount(count);
    //
    // boolean oneLastFrame = false;
    // if (mState == STATE_RECEDE && mGlowScaleY == 0) {
    //     mState = STATE_IDLE;
    //     oneLastFrame = true;
    // }
    //
    // return mState != STATE_IDLE || oneLastFrame;

    assert(0);
    Update();
    Int32 count = 0;
    canvas->Save(&count);

    Int32 centerX;
    mBounds->GetCenterX(&centerX);
    Int32 height = 0;
    mBounds->GetHeight(&height);
    Float centerY = height - mRadius;

    canvas->Scale(1.0f, Elastos::Core::Math::Min(mGlowScaleY, 1.0f) * mBaseGlowScale, centerX, 0);
    Float displacement = Elastos::Core::Math::Max(0.0f, Elastos::Core::Math::Min(mDisplacement, 1.0f)) - 0.5f;
    Int32 width = 0;
    mBounds->GetWidth(&width);
    float translateX = width * displacement / 2;

    Boolean resTmp;
    canvas->ClipRect(mBounds, &resTmp);
    canvas->Translate(translateX, 0);
    mPaint->SetAlpha((Int32) (0xff * mGlowAlpha));
    canvas->DrawCircle(centerX, centerY, mRadius, mPaint);
    canvas->RestoreToCount(count);

    Boolean oneLastFrame = FALSE;
    if (mState == STATE_RECEDE && mGlowScaleY == 0) {
        mState = STATE_IDLE;
        oneLastFrame = TRUE;
    }

    *result = (mState != STATE_IDLE || oneLastFrame);
    return NOERROR;
}

ECode EdgeEffect::GetMaxHeight(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return (int) (mBounds.height() * MAX_GLOW_SCALE + 0.5f);

    Int32 height = 0;
    mBounds->GetHeight(&height);
    *result = (Int32)(height * MAX_GLOW_SCALE + 0.5f);
    return NOERROR;
}

void EdgeEffect::Update()
{
    // ==================before translated======================
    // final long time = AnimationUtils.currentAnimationTimeMillis();
    // final float t = Math.min((time - mStartTime) / mDuration, 1.f);
    //
    // final float interp = mInterpolator.getInterpolation(t);
    //
    // mGlowAlpha = mGlowAlphaStart + (mGlowAlphaFinish - mGlowAlphaStart) * interp;
    // mGlowScaleY = mGlowScaleYStart + (mGlowScaleYFinish - mGlowScaleYStart) * interp;
    // mDisplacement = (mDisplacement + mTargetDisplacement) / 2;
    //
    // if (t >= 1.f - EPSILON) {
    //     switch (mState) {
    //         case STATE_ABSORB:
    //             mState = STATE_RECEDE;
    //             mStartTime = AnimationUtils.currentAnimationTimeMillis();
    //             mDuration = RECEDE_TIME;
    //
    //             mGlowAlphaStart = mGlowAlpha;
    //             mGlowScaleYStart = mGlowScaleY;
    //
    //             // After absorb, the glow should fade to nothing.
    //             mGlowAlphaFinish = 0.f;
    //             mGlowScaleYFinish = 0.f;
    //             break;
    //         case STATE_PULL:
    //             mState = STATE_PULL_DECAY;
    //             mStartTime = AnimationUtils.currentAnimationTimeMillis();
    //             mDuration = PULL_DECAY_TIME;
    //
    //             mGlowAlphaStart = mGlowAlpha;
    //             mGlowScaleYStart = mGlowScaleY;
    //
    //             // After pull, the glow should fade to nothing.
    //             mGlowAlphaFinish = 0.f;
    //             mGlowScaleYFinish = 0.f;
    //             break;
    //         case STATE_PULL_DECAY:
    //             mState = STATE_RECEDE;
    //             break;
    //         case STATE_RECEDE:
    //             mState = STATE_IDLE;
    //             break;
    //     }
    // }

    assert(0);
    Int64 time;
    AutoPtr<IAnimationUtils> utils;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&utils);
    utils->CurrentAnimationTimeMillis(&time);
    Float t = Elastos::Core::Math::Min((time - mStartTime) / mDuration, 1.f);

    ITimeInterpolator* timeInterpolator = ITimeInterpolator::Probe(mInterpolator);
    Float interp = 0.0f;
    timeInterpolator->GetInterpolation(t, &interp);

    mGlowAlpha = mGlowAlphaStart + (mGlowAlphaFinish - mGlowAlphaStart) * interp;
    mGlowScaleY = mGlowScaleYStart + (mGlowScaleYFinish - mGlowScaleYStart) * interp;
    mDisplacement = (mDisplacement + mTargetDisplacement) / 2;

    if (t >= 1.f - EPSILON) {
        switch (mState) {
            case STATE_ABSORB:
                {
                    mState = STATE_RECEDE;
                    utils->CurrentAnimationTimeMillis(&mStartTime);
                    mDuration = RECEDE_TIME;

                    mGlowAlphaStart = mGlowAlpha;
                    mGlowScaleYStart = mGlowScaleY;

                    mGlowAlphaFinish = 0.0f;
                    mGlowScaleYFinish = 0.0f;
                }
                break;
            case STATE_PULL:
                {
                    mState = STATE_PULL_DECAY;
                    utils->CurrentAnimationTimeMillis(&mStartTime);
                    mDuration = PULL_DECAY_TIME;

                    mGlowAlphaStart = mGlowAlpha;
                    mGlowScaleYStart = mGlowScaleY;

                    mGlowAlphaFinish = 0.0f;
                    mGlowScaleYFinish = 0.0f;
                }
                break;
            case STATE_PULL_DECAY:
                mState = STATE_RECEDE;
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

