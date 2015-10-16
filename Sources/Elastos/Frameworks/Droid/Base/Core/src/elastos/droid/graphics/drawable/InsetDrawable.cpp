
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/graphics/drawable/InsetDrawable.h"
#include "elastos/droid/graphics/drawable/CInsetDrawable.h"
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Logger.h>


using Elastos::Utility::Logging::Logger;

using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

InsetDrawable::InsetState::InsetState(
    /* [in] */ InsetState* orig,
    /* [in] */ InsetDrawable* owner,
    /* [in] */ IResources* res)
    : mChangingConfigurations(0)
    , mInsetLeft(0)
    , mInsetTop(0)
    , mInsetRight(0)
    , mInsetBottom(0)
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
        mDrawable->SetCallback((IDrawableCallback*)owner->Probe(EIID_IDrawableCallback));
        mInsetLeft = orig->mInsetLeft;
        mInsetTop = orig->mInsetTop;
        mInsetRight = orig->mInsetRight;
        mInsetBottom = orig->mInsetBottom;
        mCheckedConstantState = mCanConstantState = TRUE;
    }
}

ECode InsetDrawable::InsetState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CInsetDrawable::New((Handle32)this, NULL, (IInsetDrawable**)drawable);
}

ECode InsetDrawable::InsetState::NewDrawable(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CInsetDrawable::New((Handle32)this, res, (IInsetDrawable**)drawable);
}

ECode InsetDrawable::InsetState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}

Boolean InsetDrawable::InsetState::CanConstantState()
{
    if (!mCheckedConstantState) {
        AutoPtr<IDrawableConstantState> state;
        mDrawable->GetConstantState((IDrawableConstantState**)&state);
        mCanConstantState = state != NULL;
        mCheckedConstantState = TRUE;
    }

    return mCanConstantState;
}

CAR_INTERFACE_IMPL_2(InsetDrawable, Drawable, IInsetDrawable, IDrawableCallback);
InsetDrawable::InsetDrawable()
    : mMutated(FALSE)
{
    CRect::NewByFriend((CRect**)&mTmpRect);
}

InsetDrawable::InsetDrawable(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 inset)
    : mMutated(FALSE)
{
    CRect::NewByFriend((CRect**)&mTmpRect);
    ASSERT_SUCCEEDED(constructor(drawable, inset, inset, inset, inset));
}

InsetDrawable::InsetDrawable(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 insetLeft,
    /* [in] */ Int32 insetTop,
    /* [in] */ Int32 insetRight,
    /* [in] */ Int32 insetBottom)
    : mMutated(FALSE)
{
    CRect::NewByFriend((CRect**)&mTmpRect);
    ASSERT_SUCCEEDED(constructor(drawable, insetLeft, insetTop, insetRight, insetBottom));
}

InsetDrawable::InsetDrawable(
    /* [in] */ InsetState* state,
    /* [in] */ IResources* res)
    : mMutated(FALSE)
{
    CRect::NewByFriend((CRect**)&mTmpRect);
    mInsetState = new InsetState(state, this, res);
}

ECode InsetDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    Int32 type;

    Int32 size = ARRAY_SIZE(R::styleable::InsetDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::InsetDrawable, size);

    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);

    Drawable::InflateWithAttributes(r, parser, a, R::styleable::InsetDrawable_visible);

    Int32 drawableRes;
    a->GetResourceId(R::styleable::InsetDrawable_drawable, 0, &drawableRes);

    Int32 inLeft, inTop, inRight, inBottom;
    a->GetDimensionPixelOffset(R::styleable::InsetDrawable_insetLeft, 0, &inLeft);
    a->GetDimensionPixelOffset(R::styleable::InsetDrawable_insetTop, 0, &inTop);
    a->GetDimensionPixelOffset(R::styleable::InsetDrawable_insetRight, 0, &inRight);
    a->GetDimensionPixelOffset(R::styleable::InsetDrawable_insetBottom, 0, &inBottom);

    a->Recycle();

    AutoPtr<IDrawable> dr;
    if (drawableRes != 0) {
        r->GetDrawable(drawableRes, (IDrawable**)&dr);
    }
    else {
        while (parser->Next(&type), type == IXmlPullParser::TEXT) {
        }
        if (type != IXmlPullParser::START_TAG) {
//            throw new XmlPullParserException(
//                    parser.getPositionDescription()
//                    + ": <inset> tag requires a 'drawable' attribute or "
//                    + "child tag defining a drawable");
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&dr);
    }

    if (dr == NULL) {
        Logger::W("drawable", "No drawable specified for <inset>");
    }

    mInsetState->mDrawable = dr;
    mInsetState->mInsetLeft = inLeft;
    mInsetState->mInsetRight = inRight;
    mInsetState->mInsetTop = inTop;
    mInsetState->mInsetBottom = inBottom;

    if (dr != NULL) {
        dr->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }
    return NOERROR;
}

ECode InsetDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    AutoPtr<IDrawableCallback> callback;
    GetCallback((IDrawableCallback**)&callback);
    if (callback != NULL) {
        callback->InvalidateDrawable(THIS_PROBE(IDrawable));
    }
    return NOERROR;
}

ECode InsetDrawable::ScheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    AutoPtr<IDrawableCallback> callback;
    GetCallback((IDrawableCallback**)&callback);
    if (callback != NULL) {
        callback->ScheduleDrawable(THIS_PROBE(IDrawable), what, when);
    }
    return NOERROR;
}

ECode InsetDrawable::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    AutoPtr<IDrawableCallback> callback;
    GetCallback((IDrawableCallback**)&callback);
    if (callback != NULL) {
        callback->UnscheduleDrawable(THIS_PROBE(IDrawable), what);
    }
    return NOERROR;
}

ECode InsetDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    return mInsetState->mDrawable->Draw(canvas);
}

ECode InsetDrawable::GetChangingConfigurations(
    /* [out] */ Int32* configuration)
{
    VALIDATE_NOT_NULL(configuration);
    Int32 config;
    mInsetState->mDrawable->GetChangingConfigurations(&config);
    Drawable::GetChangingConfigurations(configuration);
    *configuration = (*configuration)
            | mInsetState->mChangingConfigurations
            | config;
    return NOERROR;
}

ECode InsetDrawable::GetPadding(
    /* [in] */ IRect* padding,
    /* [out] */ Boolean* isPadding)
{
    VALIDATE_NOT_NULL(isPadding);
    Boolean pad;
    mInsetState->mDrawable->GetPadding(padding, &pad);

    ((CRect*)padding)->mLeft += mInsetState->mInsetLeft;
    ((CRect*)padding)->mRight += mInsetState->mInsetRight;
    ((CRect*)padding)->mTop += mInsetState->mInsetTop;
    ((CRect*)padding)->mBottom += mInsetState->mInsetBottom;

    if (pad || (mInsetState->mInsetLeft | mInsetState->mInsetRight |
                mInsetState->mInsetTop | mInsetState->mInsetBottom) != 0) {
        *isPadding = TRUE;
        return NOERROR;
    }
    *isPadding = FALSE;
    return NOERROR;
}

ECode InsetDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart,
    /* [out] */ Boolean* isDifferent)
{
    VALIDATE_NOT_NULL(isDifferent);
    mInsetState->mDrawable->SetVisible(visible, restart, isDifferent);
    return Drawable::SetVisible(visible, restart, isDifferent);
}

ECode InsetDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return mInsetState->mDrawable->SetAlpha(alpha);
}

ECode InsetDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return mInsetState->mDrawable->SetColorFilter(cf);
}

ECode InsetDrawable::GetOpacity(
    /* [out] */ Int32* opacity)
{
    return mInsetState->mDrawable->GetOpacity(opacity);
}

ECode InsetDrawable::IsStateful(
    /* [out] */ Boolean* isStateful)
{
    return mInsetState->mDrawable->IsStateful(isStateful);
}

Boolean InsetDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* state)
{
    Boolean changed;
    mInsetState->mDrawable->SetState(
        const_cast<ArrayOf<Int32>*>(state), &changed);
    AutoPtr<IRect> rect;
    GetBounds((IRect**)&rect);
    OnBoundsChange(rect);
    return changed;
}

void InsetDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    CRect* r = mTmpRect;
    r->Set(bounds);

    r->mLeft += mInsetState->mInsetLeft;
    r->mTop += mInsetState->mInsetTop;
    r->mRight -= mInsetState->mInsetRight;
    r->mBottom -= mInsetState->mInsetBottom;

    mInsetState->mDrawable->SetBounds(r->mLeft, r->mTop, r->mRight, r->mBottom);
}

ECode InsetDrawable::GetIntrinsicWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    return mInsetState->mDrawable->GetIntrinsicWidth(width);
}

ECode InsetDrawable::GetIntrinsicHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    return mInsetState->mDrawable->GetIntrinsicHeight(height);
}

ECode InsetDrawable::GetConstantState(
    /* [out] */ IDrawableConstantState** state)
{
    VALIDATE_NOT_NULL(state);
    if (mInsetState->CanConstantState()) {
        GetChangingConfigurations(&mInsetState->mChangingConfigurations);
        *state = mInsetState;
        REFCOUNT_ADD(*state);
        return NOERROR;
    }
    *state = NULL;
    return NOERROR;
}

ECode InsetDrawable::Mutate(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> tmp;
    if (!mMutated && (Drawable::Mutate((IDrawable**)&tmp), tmp.Get()) == THIS_PROBE(IDrawable)) {
        tmp = NULL;
        mInsetState->mDrawable->Mutate((IDrawable**)&tmp);
        mMutated = TRUE;
    }
    *drawable = THIS_PROBE(IDrawable);
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode InsetDrawable::GetDrawable(
    /* [out] */ IDrawable** drawable)
{
    assert(0 && "TODO");
    //not merge from android5.x
    return NOERROR;
}

ECode InsetDrawable::constructor()
{
    return constructor((InsetState*)NULL, (IResources*)NULL);
}

ECode InsetDrawable::constructor(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 inset)
{
    return constructor(drawable, inset, inset, inset, inset);
}

ECode InsetDrawable::constructor(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 insetLeft,
    /* [in] */ Int32 insetTop,
    /* [in] */ Int32 insetRight,
    /* [in] */ Int32 insetBottom)
{
    FAIL_RETURN(constructor((InsetState*)NULL, (IResources*)NULL));

    mInsetState->mDrawable = drawable;
    mInsetState->mInsetLeft = insetLeft;
    mInsetState->mInsetTop = insetTop;
    mInsetState->mInsetRight = insetRight;
    mInsetState->mInsetBottom = insetBottom;

    if (drawable != NULL) {
        drawable->SetCallback(
                (IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }
    return NOERROR;
}

ECode InsetDrawable::constructor(
    /* [in] */ InsetState* state,
    /* [in] */ IResources* res)
{
    mInsetState = new InsetState(state, this, res);
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
