
#include "ext/frameworkext.h"
#include "graphics/drawable/ClipDrawable.h"
#include "graphics/drawable/CClipDrawable.h"
#include "view/CGravity.h"
#include "R.h"

using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::CGravity;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ClipDrawable::ClipState::ClipState(
    /* [in] */ ClipState* orig,
    /* [in] */ ClipDrawable* owner,
    /* [in] */ IResources* res)
    : mChangingConfigurations(0)
    , mOrientation(0)
    , mGravity(0)
    , mCheckedConstantState(FALSE)
    , mCanConstantState(FALSE)
{
    if (orig != NULL) {
        AutoPtr<IDrawableConstantState> state;
        orig->mDrawable->GetConstantState((IDrawableConstantState**)&state);
        if (res != NULL) {
            state->NewDrawable(res, (IDrawable**)&mDrawable);
        }
        else {
            state->NewDrawable((IDrawable**)&mDrawable);
        }
        mDrawable->SetCallback(
                (IDrawableCallback*)owner->Probe(EIID_IDrawableCallback));
        mOrientation = orig->mOrientation;
        mGravity = orig->mGravity;
        mCheckedConstantState = mCanConstantState = TRUE;
    }
}

ECode ClipDrawable::ClipState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    return CClipDrawable::New(this, NULL, (IClipDrawable**)drawable);
}

ECode ClipDrawable::ClipState::NewDrawable(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    return CClipDrawable::New(this, res, (IClipDrawable**)drawable);
}

ECode ClipDrawable::ClipState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);

    *config = mChangingConfigurations;
    return NOERROR;
}

Boolean ClipDrawable::ClipState::CanConstantState()
{
    if (!mCheckedConstantState) {
        AutoPtr<IDrawableConstantState> state;
        mDrawable->GetConstantState((IDrawableConstantState**)&state);
        mCanConstantState = state != NULL;
        mCheckedConstantState = TRUE;
    }

    return mCanConstantState;
}


ClipDrawable::ClipDrawable()
{
    CRect::NewByFriend((CRect**)&mTmpRect);
}

/**
 * @param orientation Bitwise-or of {@link #HORIZONTAL} and/or {@link #VERTICAL}
 */
ClipDrawable::ClipDrawable(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 orientation)
{
    CRect::NewByFriend((CRect**)&mTmpRect);
    ASSERT_SUCCEEDED(Init(drawable, gravity, orientation));
}

ECode ClipDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Drawable::Inflate(r, parser, attrs));

    Int32 type;

    Int32 size = ARRAY_SIZE(R::styleable::ClipDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::ClipDrawable, size);

    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);

    Int32 orientation;
    a->GetInt32(
            R::styleable::ClipDrawable_clipOrientation,
            IClipDrawable::HORIZONTAL, &orientation);
    Int32 g;
    a->GetInt32(R::styleable::ClipDrawable_gravity, IGravity::LEFT, &g);
    AutoPtr<IDrawable> dr;
    a->GetDrawable(R::styleable::ClipDrawable_drawable, (IDrawable**)&dr);

    a->Recycle();

    Int32 outerDepth, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser::END_DOCUMENT)
            && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }
        dr = NULL;
        FAIL_RETURN(Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&dr));
    }

    if (dr == NULL) {
//        throw new IllegalArgumentException("No drawable specified for <clip>");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mClipState->mDrawable = dr;
    mClipState->mOrientation = orientation;
    mClipState->mGravity = g;

    dr->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    return NOERROR;
}

ECode ClipDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->InvalidateDrawable(
                (IDrawable*)this->Probe(EIID_IDrawable));
    }
    return NOERROR;
}

ECode ClipDrawable::ScheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->ScheduleDrawable(
                (IDrawable*)this->Probe(EIID_IDrawable), what, when);
    }
    return NOERROR;
}

ECode ClipDrawable::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->UnscheduleDrawable(
                (IDrawable*)this->Probe(EIID_IDrawable), what);
    }
    return NOERROR;
}

Int32 ClipDrawable::GetChangingConfigurations()
{
    Int32 configs;
    mClipState->mDrawable->GetChangingConfigurations(&configs);
    return Drawable::GetChangingConfigurations()
            | mClipState->mChangingConfigurations
            | configs;
}

Boolean ClipDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    // XXX need to adjust padding!
    Boolean isPadding;
    mClipState->mDrawable->GetPadding(padding, &isPadding);
    return isPadding;
}

Boolean ClipDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean different;
    mClipState->mDrawable->SetVisible(visible, restart, &different);
    return Drawable::SetVisible(visible, restart);
}

ECode ClipDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return mClipState->mDrawable->SetAlpha(alpha);
}

ECode ClipDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return mClipState->mDrawable->SetColorFilter(cf);
}

Int32 ClipDrawable::GetOpacity()
{
    Int32 opacity;
    mClipState->mDrawable->GetOpacity(&opacity);
    return opacity;
}

Boolean ClipDrawable::IsStateful()
{
    Boolean stateful;
    mClipState->mDrawable->IsStateful(&stateful);
    return stateful;
}

Boolean ClipDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* state)
{
    Boolean changed;
    mClipState->mDrawable->SetState(const_cast<ArrayOf<Int32>*>(state), &changed);
    return changed;
}

Boolean ClipDrawable::OnLevelChange(
    /* [in] */ Int32 level)
{
    Boolean changed;
    mClipState->mDrawable->SetLevel(level, &changed);
    InvalidateSelf();
    return TRUE;
}

void ClipDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    mClipState->mDrawable->SetBounds(bounds);
}

ECode ClipDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    Int32 l;
    mClipState->mDrawable->GetLevel(&l);
    if (l == 0) {
        return NOERROR;
    }

    IRect* r = (IRect*)mTmpRect.Get();
    AutoPtr<IRect> bounds = GetBounds();
    Int32 level = GetLevel();
    Int32 w;
    bounds->GetWidth(&w);
    Int32 iw = 0; //mClipState.mDrawable.getIntrinsicWidth();
    if ((mClipState->mOrientation & IClipDrawable::HORIZONTAL) != 0) {
        w -= (w - iw) * (10000 - level) / 10000;
    }
    Int32 h;
    bounds->GetHeight(&h);
    Int32 ih = 0; //mClipState.mDrawable.getIntrinsicHeight();
    if ((mClipState->mOrientation & IClipDrawable::VERTICAL) != 0) {
        h -= (h - ih) * (10000 - level) / 10000;
    }
    Int32 layoutDirection = GetLayoutDirection();
    AutoPtr<IGravity> gravity;
    CGravity::AcquireSingleton((IGravity**)&gravity);
    gravity->Apply(mClipState->mGravity, w, h, bounds, r, layoutDirection);

    if (w > 0 && h > 0) {
        Int32 count;
        canvas->Save(&count);
        Boolean nonEmpty;
        canvas->ClipRect(r, &nonEmpty);
        mClipState->mDrawable->Draw(canvas);
        canvas->Restore();
    }
    return NOERROR;
}

//@Override
Int32 ClipDrawable::GetIntrinsicWidth()
{
    Int32 width;
    mClipState->mDrawable->GetIntrinsicWidth(&width);
    return width;
}

//@Override
Int32 ClipDrawable::GetIntrinsicHeight()
{
    Int32 height;
    mClipState->mDrawable->GetIntrinsicHeight(&height);
    return height;
}

//@Override
AutoPtr<IDrawableConstantState> ClipDrawable::GetConstantState()
{
    if (mClipState->CanConstantState()) {
        mClipState->mChangingConfigurations = Drawable::GetChangingConfigurations();
        return mClipState;
    }
    return NULL;
}

ECode ClipDrawable::SetLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    mClipState->mDrawable->SetLayoutDirection(layoutDirection);
    Drawable::SetLayoutDirection(layoutDirection);
    return NOERROR;
}

ClipDrawable::ClipDrawable(
    /* [in] */ ClipState* state,
    /* [in] */ IResources* res)
{
    CRect::NewByFriend((CRect**)&mTmpRect);
    ASSERT_SUCCEEDED(Init(state, res));
}

ECode ClipDrawable::Init(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 orientation)
{
    FAIL_RETURN(Init(NULL, NULL));

    mClipState->mDrawable = drawable;
    mClipState->mGravity = gravity;
    mClipState->mOrientation = orientation;

    if (drawable != NULL) {
        drawable->SetCallback(
                (IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }
    return NOERROR;
}

ECode ClipDrawable::Init(
    /* [in] */ ClipState* state,
    /* [in] */ IResources* res)
{
    mClipState = new ClipState(state, this, res);
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
