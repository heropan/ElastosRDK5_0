
#include "graphics/drawable/RotateDrawable.h"
#include "graphics/drawable/CRotateDrawable.h"
#include "graphics/CRect.h"
#include "R.h"
#include <elastos/Logger.h>


using Elastos::Utility::Logging::Logger;

using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

RotateDrawable::RotateState::RotateState(
    /* [in] */ RotateState* source,
    /* [in] */ RotateDrawable* owner,
    /* [in] */ IResources* res)
    : mChangingConfigurations(0)
    , mPivotXRel(FALSE)
    , mPivotX(0)
    , mPivotYRel(FALSE)
    , mPivotY(0)
    , mFromDegrees(0)
    , mToDegrees(0)
    , mCurrentDegrees(0)
    , mCanConstantState(FALSE)
    , mCheckedConstantState(FALSE)
{
    if (source != NULL) {
        if (res != NULL) {
            AutoPtr<IDrawableConstantState> state;
            source->mDrawable->GetConstantState((IDrawableConstantState**)&state);
            state->NewDrawableEx(res, (IDrawable**)&mDrawable);
        }
        else {
            AutoPtr<IDrawableConstantState> state;
            source->mDrawable->GetConstantState((IDrawableConstantState**)&state);
            state->NewDrawable((IDrawable**)&mDrawable);
        }
        mDrawable->SetCallback((IDrawableCallback*)owner->Probe(EIID_IDrawableCallback));
        mPivotXRel = source->mPivotXRel;
        mPivotX = source->mPivotX;
        mPivotYRel = source->mPivotYRel;
        mPivotY = source->mPivotY;
        mFromDegrees = mCurrentDegrees = source->mFromDegrees;
        mToDegrees = source->mToDegrees;
        mCanConstantState = mCheckedConstantState = TRUE;
    }
}

ECode RotateDrawable::RotateState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CRotateDrawable::New((Handle32)this, NULL, (IRotateDrawable**)drawable);
}

ECode RotateDrawable::RotateState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CRotateDrawable::New((Handle32)this, res, (IRotateDrawable**)drawable);
}

ECode RotateDrawable::RotateState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}

Boolean RotateDrawable::RotateState::CanConstantState()
{
    if (!mCheckedConstantState) {
        AutoPtr<IDrawableConstantState> state;
        mDrawable->GetConstantState((IDrawableConstantState**)&state);
        mCanConstantState = state != NULL;
        mCheckedConstantState = TRUE;
    }
    return mCanConstantState;
}


const Float RotateDrawable::MAX_LEVEL = 10000.0f;

RotateDrawable::RotateDrawable()
    : mMutated(FALSE)
{
}

RotateDrawable::RotateDrawable(
    /* [in] */ RotateState* rotateState,
    /* [in] */ IResources* res)
    : mMutated(FALSE)
{
    mState = new RotateState(rotateState, this, res);
}

ECode RotateDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    Int32 saveCount;
    canvas->Save(&saveCount);

    AutoPtr<IRect> bounds;
    mState->mDrawable->GetBounds((IRect**)&bounds);

    CRect* _bounds = (CRect*)bounds.Get();
    Int32 w = _bounds->mRight - _bounds->mLeft;
    Int32 h = _bounds->mBottom - _bounds->mTop;

    const RotateState* st = mState;

    Float px = st->mPivotXRel ? (w * st->mPivotX) : st->mPivotX;
    Float py = st->mPivotYRel ? (h * st->mPivotY) : st->mPivotY;

    canvas->RotateEx(st->mCurrentDegrees, px + _bounds->mLeft, py + _bounds->mTop);

    st->mDrawable->Draw(canvas);

    canvas->RestoreToCount(saveCount);
    return NOERROR;
}

AutoPtr<IDrawable> RotateDrawable::GetDrawable()
{
    return mState->mDrawable;
}

Int32 RotateDrawable::GetChangingConfigurations()
{
    Int32 config;
    mState->mDrawable->GetChangingConfigurations(&config);
    return Drawable::GetChangingConfigurations()
            | mState->mChangingConfigurations
            | config;
}

ECode RotateDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return mState->mDrawable->SetAlpha(alpha);
}

ECode RotateDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return mState->mDrawable->SetColorFilter(cf);
}

Int32 RotateDrawable::GetOpacity()
{
    Int32 opacity;
    mState->mDrawable->GetOpacity(&opacity);
    return opacity;
}

ECode RotateDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->InvalidateDrawable(THIS_PROBE(IDrawable));
    }
    return NOERROR;
}

ECode RotateDrawable::ScheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->ScheduleDrawable(THIS_PROBE(IDrawable), what, when);
    }
    return NOERROR;
}

ECode RotateDrawable::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->UnscheduleDrawable(THIS_PROBE(IDrawable), what);
    }
    return NOERROR;
}

Boolean RotateDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    Boolean isPadding;
    mState->mDrawable->GetPadding(padding, &isPadding);
    return isPadding;
}

Boolean RotateDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean isDifferent;
    mState->mDrawable->SetVisible(visible, restart, &isDifferent);
    return Drawable::SetVisible(visible, restart);
}

Boolean RotateDrawable::IsStateful()
{
    Boolean isStateful;
    mState->mDrawable->IsStateful(&isStateful);
    return isStateful;
}

Boolean RotateDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* state)
{
    Boolean changed;
    mState->mDrawable->SetState(const_cast<ArrayOf<Int32>*>(state), &changed);
    OnBoundsChange(GetBounds());
    return changed;
}

Boolean RotateDrawable::OnLevelChange(
    /* [in] */ Int32 level)
{
    Boolean changed;
    mState->mDrawable->SetLevel(level, &changed);
    OnBoundsChange(GetBounds());

    mState->mCurrentDegrees = mState->mFromDegrees +
            (mState->mToDegrees - mState->mFromDegrees) *
                    ((Float)level / MAX_LEVEL);

    InvalidateSelf();
    return TRUE;
}

void RotateDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    CRect* _bounds = (CRect*)bounds;
    mState->mDrawable->SetBounds(
            _bounds->mLeft, _bounds->mTop,
            _bounds->mRight, _bounds->mBottom);
}

Int32 RotateDrawable::GetIntrinsicWidth()
{
    Int32 width;
    mState->mDrawable->GetIntrinsicWidth(&width);
    return width;
}

Int32 RotateDrawable::GetIntrinsicHeight()
{
    Int32 height;
    mState->mDrawable->GetIntrinsicHeight(&height);
    return height;
}

AutoPtr<IDrawableConstantState> RotateDrawable::GetConstantState()
{
    if (mState->CanConstantState()) {
        mState->mChangingConfigurations = GetChangingConfigurations();
        return mState;
    }
    return NULL;
}

ECode RotateDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    Int32 size = ARRAY_SIZE(R::styleable::RotateDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::RotateDrawable, size);

    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);

    Drawable::InflateWithAttributes(r, parser, a,
            R::styleable::RotateDrawable_visible);

    AutoPtr<ITypedValue> tv;
    a->PeekValue(R::styleable::RotateDrawable_pivotX, (ITypedValue**)&tv);
    Boolean pivotXRel;
    Float pivotX;
    if (tv == NULL) {
        pivotXRel = TRUE;
        pivotX = 0.5f;
    }
    else {
        Int32 type;
        tv->GetType(&type);
        pivotXRel = type == ITypedValue::TYPE_FRACTION;
        pivotXRel ? tv->GetFraction(1.0f, 1.0f, &pivotX) : tv->GetFloat(&pivotX);
    }

    tv = NULL;
    a->PeekValue(R::styleable::RotateDrawable_pivotY, (ITypedValue**)&tv);
    Boolean pivotYRel;
    Float pivotY;
    if (tv == NULL) {
        pivotYRel = TRUE;
        pivotY = 0.5f;
    }
    else {
        Int32 type;
        tv->GetType(&type);
        pivotYRel = type == ITypedValue::TYPE_FRACTION;
        pivotYRel ? tv->GetFraction(1.0f, 1.0f, &pivotY) : tv->GetFloat(&pivotY);
    }

    Float fromDegrees, toDegrees;
    a->GetFloat(
            R::styleable::RotateDrawable_fromDegrees, 0.0f, &fromDegrees);
    a->GetFloat(
            R::styleable::RotateDrawable_toDegrees, 360.0f, &toDegrees);

    Int32 res;
    a->GetResourceId(
            R::styleable::RotateDrawable_drawable, 0, &res);
    AutoPtr<IDrawable> drawable;
    if (res > 0) {
        r->GetDrawable(res, (IDrawable**)&drawable);
    }

    a->Recycle();

    Int32 outerDepth, type, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser::END_DOCUMENT) &&
           (type != IXmlPullParser::END_TAG || (parser->GetDepth(&depth), depth > outerDepth))) {

        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        drawable = NULL;
        Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&drawable);
        if (drawable == NULL) {
            String name;
            parser->GetName(&name);
            Logger::W("drawable", "Bad element under <rotate>: %s", name.string());
        }
    }

    if (drawable == NULL) {
        Logger::W("drawable", "No drawable specified for <rotate>");
    }

    mState->mDrawable = drawable;
    mState->mPivotXRel = pivotXRel;
    mState->mPivotX = pivotX;
    mState->mPivotYRel = pivotYRel;
    mState->mPivotY = pivotY;
    mState->mFromDegrees = mState->mCurrentDegrees = fromDegrees;
    mState->mToDegrees = toDegrees;

    if (drawable != NULL) {
        drawable->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }
    return NOERROR;
}

AutoPtr<IDrawable> RotateDrawable::Mutate()
{
    if (!mMutated &&
            Drawable::Mutate().Get() == THIS_PROBE(IDrawable)) {
        AutoPtr<IDrawable> drawable;
        mState->mDrawable->Mutate((IDrawable**)&drawable);
        mMutated = TRUE;
    }
    return THIS_PROBE(IDrawable);
}

ECode RotateDrawable::Init()
{
    return Init(NULL, NULL);
}

ECode RotateDrawable::Init(
    /* [in] */ RotateState* rotateState,
    /* [in] */ IResources* res)
{
    mState = new RotateState(rotateState, this, res);
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
