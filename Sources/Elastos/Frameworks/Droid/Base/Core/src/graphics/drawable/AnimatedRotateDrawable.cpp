
#include "ext/frameworkext.h"
#include "graphics/drawable/AnimatedRotateDrawable.h"
#include "graphics/drawable/CAnimatedRotateDrawable.h"
#include "graphics/CRect.h"
#include "os/SystemClock.h"
// #include "util/CTypedValue.h"
#include "R.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::EIID_IRunnable;
using Elastos::Utility::Logging::Logger;

using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

AnimatedRotateDrawable::AnimatedRotateState::AnimatedRotateState(
    /* [in] */ AnimatedRotateState* source,
    /* [in] */ AnimatedRotateDrawable* owner,
    /* [in] */ IResources* res)
    : mChangingConfigurations(0)
    , mPivotXRel(FALSE)
    , mPivotX(0)
    , mPivotYRel(FALSE)
    , mPivotY(0)
    , mFrameDuration(0)
    , mFramesCount(0)
    , mCanConstantState(FALSE)
    , mCheckedConstantState(FALSE)
{
    if (source != NULL) {
        AutoPtr<IDrawableConstantState> state;
        source->mDrawable->GetConstantState((IDrawableConstantState**)&state);
        if (res != NULL) {
            state->NewDrawable(res, (IDrawable**)&mDrawable);
        }
        else {
            state->NewDrawable((IDrawable**)&mDrawable);
        }
        mDrawable->SetCallback(
            owner != NULL ? (IDrawableCallback*)owner->Probe(EIID_IDrawableCallback) : NULL);
        mPivotXRel = source->mPivotXRel;
        mPivotX = source->mPivotX;
        mPivotYRel = source->mPivotYRel;
        mPivotY = source->mPivotY;
        mFramesCount = source->mFramesCount;
        mFrameDuration = source->mFrameDuration;
        mCanConstantState = mCheckedConstantState = TRUE;
    }
}

ECode AnimatedRotateDrawable::AnimatedRotateState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CAnimatedRotateDrawable::New(
        this, NULL, (IAnimatedRotateDrawable**)drawable);
}

ECode AnimatedRotateDrawable::AnimatedRotateState::NewDrawable(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CAnimatedRotateDrawable::New(
        this, res, (IAnimatedRotateDrawable**)drawable);
}

ECode AnimatedRotateDrawable::AnimatedRotateState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}

Boolean AnimatedRotateDrawable::AnimatedRotateState::CanConstantState()
{
    if (!mCheckedConstantState) {
        AutoPtr<IDrawableConstantState> state;
        mDrawable->GetConstantState((IDrawableConstantState**)&state);
        mCanConstantState = state != NULL;
        mCheckedConstantState = TRUE;
    }

    return mCanConstantState;
}


AnimatedRotateDrawable::AnimatedRotateDrawable(
    /* [in] */ AnimatedRotateState* rotateState,
    /* [in] */ IResources* res)
    : mMutated(FALSE)
    , mCurrentDegrees(0)
    , mIncrement(0)
    , mRunning(FALSE)
{
    Init(rotateState, res);
}

void AnimatedRotateDrawable::InitInternal()
{
    AnimatedRotateState* state = mState;
    mIncrement = 360.0f / state->mFramesCount;
    IDrawable* drawable = state->mDrawable;
    if (drawable != NULL) {
        drawable->SetFilterBitmap(TRUE);
        if (IBitmapDrawable::Probe(drawable)) {
            IBitmapDrawable::Probe(drawable)->SetAntiAlias(TRUE);
        }
    }
}

//@Override
ECode AnimatedRotateDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    Int32 saveCount;
    canvas->Save(&saveCount);

    AnimatedRotateState* st = mState;
    AutoPtr<IDrawable> drawable = st->mDrawable;
    AutoPtr<IRect> temp;
    drawable->GetBounds((IRect**)&temp);
    CRect* bounds = (CRect*)temp.Get();

    Int32 w = bounds->mRight - bounds->mLeft;
    Int32 h = bounds->mBottom - bounds->mTop;

    Float px = st->mPivotXRel ? (w * st->mPivotX) : st->mPivotX;
    Float py = st->mPivotYRel ? (h * st->mPivotY) : st->mPivotY;

    canvas->Rotate(mCurrentDegrees, px + bounds->mLeft, py + bounds->mTop);

    drawable->Draw(canvas);

    canvas->RestoreToCount(saveCount);
    return NOERROR;
}

ECode AnimatedRotateDrawable::Start()
{
    if (!mRunning) {
        mRunning = TRUE;
        NextFrame();
    }
    return NOERROR;
}

ECode AnimatedRotateDrawable::Stop()
{
    mRunning = FALSE;
    UnscheduleSelf((IRunnable*)this->Probe(EIID_IRunnable));
    return NOERROR;
}

Boolean AnimatedRotateDrawable::IsRunning()
{
    return mRunning;
}

void AnimatedRotateDrawable::NextFrame()
{
    UnscheduleSelf((IRunnable*)this->Probe(EIID_IRunnable));
    ScheduleSelf((IRunnable*)this->Probe(EIID_IRunnable),
            SystemClock::GetUptimeMillis() + mState->mFrameDuration);
}

ECode AnimatedRotateDrawable::Run()
{
    // TODO: This should be computed in draw(Canvas), based on the amount
    // of time since the last frame drawn
    mCurrentDegrees += mIncrement;
    if (mCurrentDegrees > (360.0f - mIncrement)) {
        mCurrentDegrees = 0.0f;
    }
    InvalidateSelf();
    NextFrame();
    return NOERROR;
}

//@Override
Boolean AnimatedRotateDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean res;
    mState->mDrawable->SetVisible(visible, restart, &res);
    Boolean changed = Drawable::SetVisible(visible, restart);
    if (visible) {
        if (changed || restart) {
            mCurrentDegrees = 0.0f;
            NextFrame();
        }
    }
    else {
        UnscheduleSelf((IRunnable*)this->Probe(EIID_IRunnable));
    }
    return changed;
}

/**
 * Returns the drawable rotated by this RotateDrawable.
 */
AutoPtr<IDrawable> AnimatedRotateDrawable::GetDrawable()
{
    return mState->mDrawable;
}

//@Override
Int32 AnimatedRotateDrawable::GetChangingConfigurations()
{
    Int32 drawableConfig;
    return Drawable::GetChangingConfigurations()
        | mState->mChangingConfigurations
        | (mState->mDrawable->GetChangingConfigurations(&drawableConfig), drawableConfig);
}

ECode AnimatedRotateDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return mState->mDrawable->SetAlpha(alpha);
}

ECode AnimatedRotateDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return mState->mDrawable->SetColorFilter(cf);
}

Int32 AnimatedRotateDrawable::GetOpacity()
{
    Int32 opacity;
    mState->mDrawable->GetOpacity(&opacity);
    return opacity;
}

ECode AnimatedRotateDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->InvalidateDrawable((IDrawable*)this->Probe(EIID_IDrawable));
    }
    return NOERROR;
}

ECode AnimatedRotateDrawable::ScheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->ScheduleDrawable((IDrawable*)this->Probe(EIID_IDrawable), what, when);
    }
    return NOERROR;
}

ECode AnimatedRotateDrawable::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->UnscheduleDrawable((IDrawable*)this->Probe(EIID_IDrawable), what);
    }
    return NOERROR;
}

//@Override
Boolean AnimatedRotateDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    Boolean res;
    mState->mDrawable->GetPadding(padding, &res);
    return res;
}

//@Override
Boolean AnimatedRotateDrawable::IsStateful()
{
    Boolean res;
    mState->mDrawable->IsStateful(&res);
    return res;
}

//@Override
void AnimatedRotateDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    CRect* temp = (CRect*)bounds;
    mState->mDrawable->SetBounds(
        temp->mLeft, temp->mTop, temp->mRight, temp->mBottom);
}

//@Override
Int32 AnimatedRotateDrawable::GetIntrinsicWidth()
{
    Int32 width;
    mState->mDrawable->GetIntrinsicWidth(&width);
    return width;
}

//@Override
Int32 AnimatedRotateDrawable::GetIntrinsicHeight()
{
    Int32 height;
    mState->mDrawable->GetIntrinsicHeight(&height);
    return height;
}

//@Override
AutoPtr<IDrawableConstantState> AnimatedRotateDrawable::GetConstantState()
{
    if (mState->CanConstantState()) {
        mState->mChangingConfigurations = GetChangingConfigurations();
        return mState;
    }
    return NULL;
}

ECode AnimatedRotateDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    Int32 size = ARRAY_SIZE(R::styleable::AnimatedRotateDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AnimatedRotateDrawable, size);

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(r->ObtainAttributes(attrs, layout, (ITypedArray**)&a));

    FAIL_RETURN(Drawable::InflateWithAttributes(
            r, parser, a, R::styleable::AnimatedRotateDrawable_visible));

    AutoPtr<ITypedValue> tv;
    a->PeekValue(R::styleable::AnimatedRotateDrawable_pivotX, (ITypedValue**)&tv);
    Int32 tp;
    tv->GetType(&tp);
    Boolean pivotXRel = tp == ITypedValue::TYPE_FRACTION;
    Float pivotX;
    pivotXRel ? tv->GetFraction(1.0f, 1.0f, &pivotX) : tv->GetFloat(&pivotX);

    tv = NULL;
    a->PeekValue(R::styleable::AnimatedRotateDrawable_pivotY, (ITypedValue**)&tv);
    tv->GetType(&tp);
    Boolean pivotYRel = tp == ITypedValue::TYPE_FRACTION;
    Float pivotY;
    pivotYRel ? tv->GetFraction(1.0f, 1.0f, &pivotY) : tv->GetFloat(&pivotY);

    Int32 framesCount;
    a->GetInt32(R::styleable::AnimatedRotateDrawable_framesCount, 12, &framesCount);
    SetFramesCount(framesCount);

    Int32 frameDuration;
    a->GetInt32(R::styleable::AnimatedRotateDrawable_frameDuration, 150, &frameDuration);
    SetFramesDuration(frameDuration);

    Int32 res;
    a->GetResourceId(R::styleable::AnimatedRotateDrawable_drawable, 0, &res);
    AutoPtr<IDrawable> drawable;
    if (res > 0) {
        r->GetDrawable(res, (IDrawable**)&drawable);
    }

    a->Recycle();

    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    Int32 type, depth;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
        && (type != IXmlPullParser::END_TAG
        || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        drawable = NULL;
        Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&drawable);
        if (drawable == NULL) {
            String name;
            parser->GetName(&name);
            Logger::W("Drawable", "Bad element under <animated-rotate>: %s", name.string());
        }
    }

    if (drawable == NULL) {
        Logger::W("drawable", "No drawable specified for <animated-rotate>");
    }

    mState->mDrawable = drawable;
    mState->mPivotXRel = pivotXRel;
    mState->mPivotX = pivotX;
    mState->mPivotYRel = pivotYRel;
    mState->mPivotY = pivotY;

    InitInternal();

    if (drawable != NULL) {
        drawable->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }
    return NOERROR;
}

CARAPI AnimatedRotateDrawable::SetFramesCount(
    /* [in] */ Int32 framesCount)
{
    mState->mFramesCount = framesCount;
    mIncrement = 360.0f / mState->mFramesCount;
    return NOERROR;
}

CARAPI AnimatedRotateDrawable::SetFramesDuration(
    /* [in] */ Int32 framesDuration)
{
    mState->mFrameDuration = framesDuration;
    return NOERROR;
}

//@Override
AutoPtr<IDrawable> AnimatedRotateDrawable::Mutate()
{
    if (!mMutated && Drawable::Mutate().Get()
        == (IDrawable*)this->Probe(EIID_IDrawable)) {
        AutoPtr<IDrawable> temp;
        mState->mDrawable->Mutate((IDrawable**)&temp);
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode AnimatedRotateDrawable::Init(
    /* [in] */ AnimatedRotateState* rotateState,
    /* [in] */ IResources* res)
{
    mState = new AnimatedRotateState(rotateState, this, res);
    InitInternal();

    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
