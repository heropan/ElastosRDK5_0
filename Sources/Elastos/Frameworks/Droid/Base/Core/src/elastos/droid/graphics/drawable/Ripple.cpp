
#include "elastos/droid/graphics/drawable/Ripple.h"
#include "elastos/droid/graphics/drawable/CVectorDrawable.h"
#include "elastos/droid/graphics/drawable/RippleDrawable.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/animation/ObjectAnimator.h"
// #include "elastos/droid/view/animation/CLinearInterpolator.h"
#include "elastos/droid/utility/MathUtils.h"

using Elastos::Droid::Animation::ObjectAnimator;
using Elastos::Droid::Animation::EIID_ITimeInterpolator;
using Elastos::Droid::View::IRenderNodeAnimator;
using Elastos::Droid::Utility::MathUtils;
using Elastos::Utility::CArrayList;
using Elastos::Utility::ICollection;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

Boolean Ripple::sInit = InitStatic();
AutoPtr<ITimeInterpolator> Ripple::LINEAR_INTERPOLATOR;
AutoPtr<ITimeInterpolator> Ripple::DECEL_INTERPOLATOR;
const Float Ripple::GLOBAL_SPEED = 1.0f;
const Float Ripple::WAVE_TOUCH_DOWN_ACCELERATION = 1024.0f * GLOBAL_SPEED;
const Float Ripple::WAVE_TOUCH_UP_ACCELERATION = 3400.0f * GLOBAL_SPEED;
const Float Ripple::WAVE_OPACITY_DECAY_VELOCITY = 3.0f / GLOBAL_SPEED;
const Int64 Ripple::RIPPLE_ENTER_DELAY = 80;

CAR_INTERFACE_IMPL(Ripple::LogInterpolator, Object, ITimeInterpolator);
ECode Ripple::LogInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 1 - (Float) Elastos::Core::Math::Pow(400, -input * 1.4);
    return NOERROR;
}


ECode Ripple::LogInterpolator::HasNativeInterpolator(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

Ripple::RippleAnimatorListenerAdapter::RippleAnimatorListenerAdapter(
    /* [in] */ Ripple* host)
    : mHost(host)
{
}

ECode Ripple::RippleAnimatorListenerAdapter::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    mHost->RemoveSelf();
    return NOERROR;
}

///////////////////  Ripple  ///////////////////////
Boolean Ripple::InitStatic()
{
    assert(0 && "TODO");
    // AutoPtr<CLinearInterpolator> li;
    // CLinearInterpolator::New((CTimeInterpolator**)&li);
    // LINEAR_INTERPOLATOR = (ITimeInterpolator*)li.Get();
    DECEL_INTERPOLATOR = new LogInterpolator();
    return TRUE;
}

Ripple::Ripple(
    /* [in] */ IRippleDrawable* owner,
    /* [in] */ IRect* bounds,
    /* [in] */ Float startingX,
    /* [in] */ Float startingY)
    : mColorOpaque(0)
    , mOuterRadius(0)
    , mDensity(0)
    , mStartingX(0)
    , mStartingY(0)
    , mClampedStartingX(0)
    , mClampedStartingY(0)
    , mOpacity(1)
    , mOuterX(0)
    , mOuterY(0)
    , mTweenRadius(0)
    , mTweenX(0)
    , mTweenY(0)
    , mHardwareAnimating(FALSE)
    , mCanUseHardware(FALSE)
    , mHasMaxRadius(FALSE)
    , mCanceled(FALSE)
{
    mOwner = owner;
    mBounds = bounds;

    mStartingX = startingX;
    mStartingY = startingY;

    CArrayList::New((IArrayList**)&mRunningAnimations);
    CArrayList::New((IArrayList**)&mPendingAnimations);
    mAnimationListener = new RippleAnimatorListenerAdapter(this);
}

void Ripple::Setup(
    /* [in] */ Int32 maxRadius,
    /* [in] */ Int32 color,
    /* [in] */ Float density)
{
    mColorOpaque = color | 0xFF000000;

    if (maxRadius != IRippleDrawable::RADIUS_AUTO) {
        mHasMaxRadius = TRUE;
        mOuterRadius = maxRadius;
    } else {
        Int32 width = 0, height = 0;
        mBounds->GetWidth(&width);
        mBounds->GetHeight(&height);
        Float halfWidth = width / 2.0f;
        Float halfHeight = height / 2.0f;
        mOuterRadius = (Float) Elastos::Core::Math::Sqrt(halfWidth * halfWidth + halfHeight * halfHeight);
    }

    mOuterX = 0;
    mOuterY = 0;
    mDensity = density;

    ClampStartingPosition();
}

Boolean Ripple::IsHardwareAnimating()
{
    return mHardwareAnimating;
}

void Ripple::ClampStartingPosition()
{
    Float cX = 0;
    mBounds->GetExactCenterX(&cX);
    Float cY = 0;
    mBounds->GetExactCenterY(&cY);
    Float dX = mStartingX - cX;
    Float dY = mStartingY - cY;
    Float r = mOuterRadius;
    if (dX * dX + dY * dY > r * r) {
        // Point is outside the circle, clamp to the circumference.
        Double angle = Elastos::Core::Math::Atan2(dY, dX);
        mClampedStartingX = cX + (Float) (Elastos::Core::Math::Cos(angle) * r);
        mClampedStartingY = cY + (Float) (Elastos::Core::Math::Sin(angle) * r);
    } else {
        mClampedStartingX = mStartingX;
        mClampedStartingY = mStartingY;
    }
}

void Ripple::OnHotspotBoundsChanged()
{
    if (!mHasMaxRadius) {
        Int32 width = 0, height = 0;
        mBounds->GetWidth(&width);
        mBounds->GetHeight(&height);
        Float halfWidth = width / 2.0f;
        Float halfHeight = height / 2.0f;
        mOuterRadius = (Float) Elastos::Core::Math::Sqrt(halfWidth * halfWidth + halfHeight * halfHeight);

        ClampStartingPosition();
    }
}

void Ripple::SetOpacity(
    /* [in] */ Float a)
{
    mOpacity = a;
    InvalidateSelf();
}

Float Ripple::GetOpacity()
{
    return mOpacity;
}

void Ripple::SetRadiusGravity(
    /* [in] */ Float r)
{
    mTweenRadius = r;
    InvalidateSelf();
}

Float Ripple::GetRadiusGravity()
{
    return mTweenRadius;
}

void Ripple::SetXGravity(
    /* [in] */ Float x)
{
    mTweenX = x;
    InvalidateSelf();
}

Float Ripple::GetXGravity()
{
    return mTweenX;
}

void Ripple::SetYGravity(
    /* [in] */ Float y)
{
    mTweenY = y;
    InvalidateSelf();
}

Float Ripple::GetYGravity()
{
    return mTweenY;
}

Boolean Ripple::Draw(
    /* [in] */ ICanvas* c,
    /* [in] */ IPaint* p)
{
    Boolean canUseHardware = FALSE;
    c->IsHardwareAccelerated(&canUseHardware);
    if (mCanUseHardware != canUseHardware && mCanUseHardware) {
        // We've switched from hardware to non-hardware mode. Panic.
        CancelHardwareAnimations(TRUE);
    }
    mCanUseHardware = canUseHardware;

    Boolean hasContent = FALSE;
    if (canUseHardware && mHardwareAnimating) {
        hasContent = DrawHardware(IHardwareCanvas::Probe(c));
    } else {
        hasContent = DrawSoftware(c, p);
    }

    return hasContent;
}

Boolean Ripple::DrawHardware(
    /* [in] */ IHardwareCanvas* c)
{
    // If we have any pending hardware animations, cancel any running
    // animations and start those now.
    AutoPtr<IArrayList> pendingAnimations = mPendingAnimations;
    Int32 N = 0;
    pendingAnimations->GetSize(&N);
    if (N > 0) {
        CancelHardwareAnimations(FALSE);

        // We canceled old animations, but we're about to run new ones.
        mHardwareAnimating = TRUE;

        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IRenderNodeAnimator> node;
            pendingAnimations->Get(i, (IInterface**)&node);
            node->SetTarget(ICanvas::Probe(c));
            IAnimator::Probe(node)->Start();
        }

        mRunningAnimations->AddAll(ICollection::Probe(pendingAnimations));
        pendingAnimations->Clear();
    }

    c->DrawCircle(mPropX, mPropY, mPropRadius, mPropPaint);

    return TRUE;
}

Boolean Ripple::DrawSoftware(
    /* [in] */ ICanvas* c,
    /* [in] */ IPaint* p)
{
    Boolean hasContent = FALSE;

    p->SetColor(mColorOpaque);
    Int32 alpha = (Int32) (255 * mOpacity + 0.5f);
    Float radius = MathUtils::Lerp(0, mOuterRadius, mTweenRadius);
    if (alpha > 0 && radius > 0) {
        Float value = 0;
        mBounds->GetExactCenterX(&value);
        Float x = MathUtils::Lerp(mClampedStartingX - value, mOuterX, mTweenX);
        mBounds->GetExactCenterY(&value);
        Float y = MathUtils::Lerp(mClampedStartingY - value, mOuterY, mTweenY);
        p->SetAlpha(alpha);
        p->SetStyle(PaintStyle_FILL);
        c->DrawCircle(x, y, radius, p);
        hasContent = TRUE;
    }

    return hasContent;
}

void Ripple::GetBounds(
    /* [in] */ IRect* bounds)
{
    Int32 outerX = (Int32) mOuterX;
    Int32 outerY = (Int32) mOuterY;
    Int32 r = (Int32) mOuterRadius + 1;
    bounds->Set(outerX - r, outerY - r, outerX + r, outerY + r);
}

void Ripple::Move(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    mStartingX = x;
    mStartingY = y;

    ClampStartingPosition();
}

void Ripple::Enter()
{
    Cancel();

    Int32 radiusDuration = (Int32)
            (1000 * Elastos::Core::Math::Sqrt(mOuterRadius / WAVE_TOUCH_DOWN_ACCELERATION * mDensity) + 0.5);

    AutoPtr<ArrayOf<Float> > fa = ArrayOf<Float>::Alloc(1);
    (*fa)[0] = 1;
    AutoPtr<IObjectAnimator> radius = ObjectAnimator::OfFloat((IObject*)this, String("radiusGravity"), fa);
    radius->SetAutoCancel(TRUE);
    IAnimator::Probe(radius)->SetDuration(radiusDuration);
    IAnimator::Probe(radius)->SetInterpolator(LINEAR_INTERPOLATOR);
    IAnimator::Probe(radius)->SetStartDelay(RIPPLE_ENTER_DELAY);

    AutoPtr<IObjectAnimator> cX = ObjectAnimator::OfFloat((IObject*)this, String("xGravity"), fa);
    cX->SetAutoCancel(TRUE);
    IAnimator::Probe(cX)->SetDuration(radiusDuration);
    IAnimator::Probe(cX)->SetInterpolator(LINEAR_INTERPOLATOR);
    IAnimator::Probe(cX)->SetStartDelay(RIPPLE_ENTER_DELAY);

    AutoPtr<IObjectAnimator> cY = ObjectAnimator::OfFloat((IObject*)this, String("yGravity"), fa);
    cY->SetAutoCancel(TRUE);
    IAnimator::Probe(cY)->SetDuration(radiusDuration);
    IAnimator::Probe(cY)->SetInterpolator(LINEAR_INTERPOLATOR);
    IAnimator::Probe(cY)->SetStartDelay(RIPPLE_ENTER_DELAY);

    mAnimRadius = radius;
    mAnimX = cX;
    mAnimY = cY;

    // Enter animations always run on the UI thread, since it's unlikely
    // that anything interesting is happening until the user lifts their
    // finger.
    IAnimator::Probe(radius)->Start();
    IAnimator::Probe(cX)->Start();
    IAnimator::Probe(cY)->Start();
}

void Ripple::Exit()
{
    Cancel();

    Float radius = MathUtils::Lerp(0, mOuterRadius, mTweenRadius);
    Float remaining = 0;
    Boolean running = FALSE;
    if (mAnimRadius != NULL && (IAnimator::Probe(mAnimRadius)->IsRunning(&running), running)) {
        remaining = mOuterRadius - radius;
    } else {
        remaining = mOuterRadius;
    }

    Int32 radiusDuration = (Int32) (1000 * Elastos::Core::Math::Sqrt(remaining / (WAVE_TOUCH_UP_ACCELERATION
            + WAVE_TOUCH_DOWN_ACCELERATION) * mDensity) + 0.5);
    Int32 opacityDuration = (Int32) (1000 * mOpacity / WAVE_OPACITY_DECAY_VELOCITY + 0.5f);

    if (mCanUseHardware) {
        ExitHardware(radiusDuration, opacityDuration);
    } else {
        ExitSoftware(radiusDuration, opacityDuration);
    }
}

void Ripple::ExitHardware(
    /* [in] */ Int32 radiusDuration,
    /* [in] */ Int32 opacityDuration)
{
    mPendingAnimations->Clear();

    Float value = 0;
    mBounds->GetExactCenterX(&value);
    Float startX = MathUtils::Lerp(mClampedStartingX - value, mOuterX, mTweenX);
    mBounds->GetExactCenterY(&value);
    Float startY = MathUtils::Lerp(mClampedStartingY - value, mOuterY, mTweenY);

    Float startRadius = MathUtils::Lerp(0, mOuterRadius, mTweenRadius);
    AutoPtr<IPaint> paint = GetTempPaint();
    paint->SetAntiAlias(TRUE);
    paint->SetColor(mColorOpaque);
    paint->SetAlpha((Int32) (255 * mOpacity + 0.5f));
    paint->SetStyle(PaintStyle_FILL);
    mPropPaint = CanvasProperty::CreatePaint(paint);
    mPropRadius = CanvasProperty::CreateFloat(startRadius);
    mPropX = CanvasProperty::CreateFloat(startX);
    mPropY = CanvasProperty::CreateFloat(startY);

    assert(0 && "TODO");
    AutoPtr<IRenderNodeAnimator> radiusAnim/* = new RenderNodeAnimator(mPropRadius, mOuterRadius)*/;
    IAnimator::Probe(radiusAnim)->SetDuration(radiusDuration);
    IAnimator::Probe(radiusAnim)->SetInterpolator(DECEL_INTERPOLATOR);

    AutoPtr<IRenderNodeAnimator> xAnim/* = new RenderNodeAnimator(mPropX, mOuterX)*/;
    IAnimator::Probe(xAnim)->SetDuration(radiusDuration);
    IAnimator::Probe(xAnim)->SetInterpolator(DECEL_INTERPOLATOR);

    AutoPtr<IRenderNodeAnimator> yAnim/* = new RenderNodeAnimator(mPropY, mOuterY)*/;
    IAnimator::Probe(yAnim)->SetDuration(radiusDuration);
    IAnimator::Probe(yAnim)->SetInterpolator(DECEL_INTERPOLATOR);

    AutoPtr<IRenderNodeAnimator> opacityAnim/* = new RenderNodeAnimator(mPropPaint, RenderNodeAnimator.PAINT_ALPHA, 0)*/;
    IAnimator::Probe(opacityAnim)->SetDuration(opacityDuration);
    IAnimator::Probe(opacityAnim)->SetInterpolator(LINEAR_INTERPOLATOR);
    IAnimator::Probe(opacityAnim)->AddListener(mAnimationListener);

    mPendingAnimations->Add(radiusAnim);
    mPendingAnimations->Add(opacityAnim);
    mPendingAnimations->Add(xAnim);
    mPendingAnimations->Add(yAnim);

    mHardwareAnimating = TRUE;

    // Set up the software values to match the hardware end values.
    mOpacity = 0;
    mTweenX = 1;
    mTweenY = 1;
    mTweenRadius = 1;

    InvalidateSelf();
}

void Ripple::Jump()
{
    mCanceled = TRUE;
    EndSoftwareAnimations();
    CancelHardwareAnimations(TRUE);
    mCanceled = FALSE;
}

void Ripple::EndSoftwareAnimations()
{
    if (mAnimRadius != NULL) {
        IAnimator::Probe(mAnimRadius)->End();
        mAnimRadius = NULL;
    }

    if (mAnimOpacity != NULL) {
        IAnimator::Probe(mAnimOpacity)->End();
        mAnimOpacity = NULL;
    }

    if (mAnimX != NULL) {
        IAnimator::Probe(mAnimX)->End();
        mAnimX = NULL;
    }

    if (mAnimY != NULL) {
        IAnimator::Probe(mAnimY)->End();
        mAnimY = NULL;
    }
}

AutoPtr<IPaint> Ripple::GetTempPaint()
{
    if (mTempPaint == NULL) {
        CPaint::New((IPaint**)&mTempPaint);
    }
    return mTempPaint;
}

void Ripple::ExitSoftware(
    /* [in] */ Int32 radiusDuration,
    /* [in] */ Int32 opacityDuration)
{
    AutoPtr<ArrayOf<Float> > fa = ArrayOf<Float>::Alloc(1);
    (*fa)[0] = 1;
    AutoPtr<IObjectAnimator> radiusAnim = ObjectAnimator::OfFloat((IObject*)this, String("radiusGravity"), fa);
    radiusAnim->SetAutoCancel(TRUE);
    IAnimator::Probe(radiusAnim)->SetDuration(radiusDuration);
    IAnimator::Probe(radiusAnim)->SetInterpolator(DECEL_INTERPOLATOR);

    AutoPtr<IObjectAnimator> xAnim = ObjectAnimator::OfFloat((IObject*)this, String("xGravity"), fa);
    xAnim->SetAutoCancel(TRUE);
    IAnimator::Probe(xAnim)->SetDuration(radiusDuration);
    IAnimator::Probe(xAnim)->SetInterpolator(DECEL_INTERPOLATOR);

    AutoPtr<IObjectAnimator> yAnim = ObjectAnimator::OfFloat((IObject*)this, String("yGravity"), fa);
    yAnim->SetAutoCancel(TRUE);
    IAnimator::Probe(yAnim)->SetDuration(radiusDuration);
    IAnimator::Probe(yAnim)->SetInterpolator(DECEL_INTERPOLATOR);

    (*fa)[0] = 0;
    AutoPtr<IObjectAnimator> opacityAnim = ObjectAnimator::OfFloat((IObject*)this, String("opacity"), fa);
    opacityAnim->SetAutoCancel(TRUE);
    IAnimator::Probe(opacityAnim)->SetDuration(opacityDuration);
    IAnimator::Probe(opacityAnim)->SetInterpolator(LINEAR_INTERPOLATOR);
    IAnimator::Probe(opacityAnim)->AddListener(mAnimationListener);

    mAnimRadius = radiusAnim;
    mAnimOpacity = opacityAnim;
    mAnimX = xAnim;
    mAnimY = yAnim;

    IAnimator::Probe(radiusAnim)->Start();
    IAnimator::Probe(opacityAnim)->Start();
    IAnimator::Probe(xAnim)->Start();
    IAnimator::Probe(yAnim)->Start();
}

void Ripple::Cancel()
{
    mCanceled = TRUE;
    CancelSoftwareAnimations();
    CancelHardwareAnimations(TRUE);
    mCanceled = FALSE;
}

void Ripple::CancelSoftwareAnimations()
{
    if (mAnimRadius != NULL) {
        IAnimator::Probe(mAnimRadius)->Cancel();
        mAnimRadius = NULL;
    }

    if (mAnimOpacity != NULL) {
        IAnimator::Probe(mAnimOpacity)->Cancel();
        mAnimOpacity = NULL;
    }

    if (mAnimX != NULL) {
        IAnimator::Probe(mAnimX)->Cancel();
        mAnimX = NULL;
    }

    if (mAnimY != NULL) {
        IAnimator::Probe(mAnimY)->Cancel();
        mAnimY = NULL;
    }
}

void Ripple::CancelHardwareAnimations(
    /* [in] */ Boolean cancelPending)
{
    AutoPtr<IArrayList> runningAnimations = mRunningAnimations;
    Int32 N = 0;
    runningAnimations->GetSize(&N);
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<IRenderNodeAnimator> node;
        runningAnimations->Get(i, (IInterface**)&node);
        IAnimator::Probe(node)->Cancel();
    }
    runningAnimations->Clear();

    Boolean empty = FALSE;
    if (cancelPending && !(mPendingAnimations->IsEmpty(&empty), empty)) {
        mPendingAnimations->Clear();
    }

    mHardwareAnimating = FALSE;
}

void Ripple::RemoveSelf()
{
    // The owner will invalidate itself.
    if (!mCanceled) {
        ((RippleDrawable*)mOwner.Get())->RemoveRipple(this);
    }
}

void Ripple::InvalidateSelf()
{
    IDrawable::Probe(mOwner)->InvalidateSelf();
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
