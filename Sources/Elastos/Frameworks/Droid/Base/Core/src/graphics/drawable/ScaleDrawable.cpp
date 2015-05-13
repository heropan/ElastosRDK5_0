
#include "ext/frameworkext.h"
#include "graphics/drawable/ScaleDrawable.h"
#include "graphics/drawable/CScaleDrawable.h"
#include "view/CGravity.h"
#include "R.h"
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::CGravity;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ScaleDrawable::ScaleState::ScaleState(
    /* [in] */ ScaleState* orig,
    /* [in] */ ScaleDrawable* owner,
    /* [in] */ IResources* res)
    : mChangingConfigurations(0)
    , mScaleWidth(0)
    , mScaleHeight(0)
    , mGravity(0)
    , mUseIntrinsicSizeAsMin(FALSE)
    , mCheckedConstantState(FALSE)
    , mCanConstantState(FALSE)
{
    if (orig != NULL) {
        AutoPtr<IDrawableConstantState> state;
        orig->mDrawable->GetConstantState((IDrawableConstantState**)&state);
        if (res != NULL) {
            state->NewDrawableEx(res, (IDrawable**)&mDrawable);
        }
        else {
            state->NewDrawable((IDrawable**)&mDrawable);
        }
        mDrawable->SetCallback((IDrawableCallback*)owner->Probe(EIID_IDrawableCallback));
        mScaleWidth = orig->mScaleWidth;
        mScaleHeight = orig->mScaleHeight;
        mGravity = orig->mGravity;
        mUseIntrinsicSizeAsMin = orig->mUseIntrinsicSizeAsMin;
        mCheckedConstantState = mCanConstantState = TRUE;
    }
}

ECode ScaleDrawable::ScaleState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CScaleDrawable::New((Handle32)this, NULL, (IScaleDrawable**)drawable);
}

ECode ScaleDrawable::ScaleState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CScaleDrawable::New((Handle32)this, res, (IScaleDrawable**)drawable);
}

ECode ScaleDrawable::ScaleState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}

Boolean ScaleDrawable::ScaleState::CanConstantState()
{
    if (!mCheckedConstantState) {
        AutoPtr<IDrawableConstantState> state;
        mDrawable->GetConstantState((IDrawableConstantState**)&state);
        mCanConstantState = state != NULL;
        mCheckedConstantState = TRUE;
    }

    return mCanConstantState;
}


ScaleDrawable::ScaleDrawable()
    : mMutated(FALSE)
{
    CRect::NewByFriend((CRect**)&mTmpRect);
}

ScaleDrawable::ScaleDrawable(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 gravity,
    /* [in] */ Float scaleWidth,
    /* [in] */ Float scaleHeight)
    : mMutated(FALSE)
{
    CRect::NewByFriend((CRect**)&mTmpRect);
    ASSERT_SUCCEEDED(Init(drawable, gravity, scaleWidth, scaleHeight));
}

ScaleDrawable::ScaleDrawable(
    /* [in] */ ScaleState* state,
    /* [in] */ IResources* res)
    : mMutated(FALSE)
{
    mScaleState = new ScaleState(state, this, res);
}

AutoPtr<IDrawable> ScaleDrawable::GetDrawable()
{
    return mScaleState->mDrawable;
}

Float ScaleDrawable::GetPercent(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 name)
{
    String s;
    a->GetString(name, &s);
    if (!s.IsNull()) {
        if (s.EndWith("%")) {
            String f = s.Substring(0, s.GetLength() - 1);
            return StringUtils::ParseFloat(f) / 100.0f;
        }
    }
    return -1.0f;
}

ECode ScaleDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Drawable::Inflate(r, parser, attrs));

    Int32 type;

    Int32 size = ARRAY_SIZE(R::styleable::ScaleDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::ScaleDrawable, size);

    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);

    Float sw = GetPercent(a, R::styleable::ScaleDrawable_scaleWidth);
    Float sh = GetPercent(a, R::styleable::ScaleDrawable_scaleHeight);
    Int32 g;
    a->GetInt32(
            R::styleable::ScaleDrawable_scaleGravity, IGravity::LEFT, &g);
    Boolean min = FALSE;
    a->GetBoolean(R::styleable::ScaleDrawable_useIntrinsicSizeAsMinimum, FALSE, &min);
    AutoPtr<IDrawable> dr;
    a->GetDrawable(R::styleable::ScaleDrawable_drawable, (IDrawable**)&dr);

    a->Recycle();

    Int32 outerDepth, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser::END_DOCUMENT)
            && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }
        dr = NULL;
        Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&dr);
    }

    if (dr == NULL) {
//        throw new IllegalArgumentException("No drawable specified for <scale>");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mScaleState->mDrawable = dr;
    mScaleState->mScaleWidth = sw;
    mScaleState->mScaleHeight = sh;
    mScaleState->mGravity = g;
    mScaleState->mUseIntrinsicSizeAsMin = min;
    if (dr != NULL) {
        dr->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }
    return NOERROR;
}

ECode ScaleDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->InvalidateDrawable(THIS_PROBE(IDrawable));
    }
    return NOERROR;
}

ECode ScaleDrawable::ScheduleDrawable(
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

ECode ScaleDrawable::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->UnscheduleDrawable(THIS_PROBE(IDrawable), what);
    }
    return NOERROR;
}

ECode ScaleDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    Int32 level;
    mScaleState->mDrawable->GetLevel(&level);
    if (level != 0) {
        mScaleState->mDrawable->Draw(canvas);
    }
    return NOERROR;
}

Int32 ScaleDrawable::GetChangingConfigurations()
{
    Int32 config;
    mScaleState->mDrawable->GetChangingConfigurations(&config);
    return Drawable::GetChangingConfigurations()
            | mScaleState->mChangingConfigurations
            | config;
}

Boolean ScaleDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    // XXX need to adjust padding!
    Boolean pad;
    mScaleState->mDrawable->GetPadding(padding, &pad);
    return pad;
}

Boolean ScaleDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean isDifferent;
    mScaleState->mDrawable->SetVisible(visible, restart, &isDifferent);
    return Drawable::SetVisible(visible, restart);
}

ECode ScaleDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return mScaleState->mDrawable->SetAlpha(alpha);
}

ECode ScaleDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return mScaleState->mDrawable->SetColorFilter(cf);
}

Int32 ScaleDrawable::GetOpacity()
{
    Int32 opacity;
    mScaleState->mDrawable->GetOpacity(&opacity);
    return opacity;
}

Boolean ScaleDrawable::IsStateful()
{
    Boolean isStateful;
    mScaleState->mDrawable->IsStateful(&isStateful);
    return isStateful;
}

Boolean ScaleDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* state)
{
    Boolean changed;
    mScaleState->mDrawable->SetState(
            const_cast<ArrayOf<Int32>*>(state), &changed);
    OnBoundsChange(GetBounds());
    return changed;
}

Boolean ScaleDrawable::OnLevelChange(
    /* [in] */ Int32 level)
{
    Boolean changed;
    mScaleState->mDrawable->SetLevel(level, &changed);
    OnBoundsChange(GetBounds());
    InvalidateSelf();
    return TRUE;
}

void ScaleDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    CRect* r = mTmpRect;
    Boolean min = mScaleState->mUseIntrinsicSizeAsMin;
    Int32 level = GetLevel();
    Int32 w;
    bounds->GetWidth(&w);
    if (mScaleState->mScaleWidth > 0) {
        Int32 iw = 0;
        if (min) {
            mScaleState->mDrawable->GetIntrinsicWidth(&iw);
        }
        w -= (Int32)((w - iw) * (10000 - level) * mScaleState->mScaleWidth / 10000);
    }
    Int32 h;
    bounds->GetHeight(&h);
    if (mScaleState->mScaleHeight > 0) {
        Int32 ih = 0;
        if (min) {
            mScaleState->mDrawable->GetIntrinsicHeight(&ih);
        }
        h -= (Int32)((h - ih) * (10000 - level) * mScaleState->mScaleHeight / 10000);
    }
    Int32 layoutDirection = GetLayoutDirection();
    AutoPtr<IGravity> gravity;
    CGravity::AcquireSingleton((IGravity**)&gravity);
    gravity->ApplyEx(mScaleState->mGravity, w, h, bounds, r, layoutDirection);

    if (w > 0 && h > 0) {
        mScaleState->mDrawable->SetBounds(r->mLeft, r->mTop, r->mRight, r->mBottom);
    }
}

Int32 ScaleDrawable::GetIntrinsicWidth()
{
    Int32 width;
    mScaleState->mDrawable->GetIntrinsicWidth(&width);
    return width;
}

Int32 ScaleDrawable::GetIntrinsicHeight()
{
    Int32 height;
    mScaleState->mDrawable->GetIntrinsicHeight(&height);
    return height;
}

AutoPtr<IDrawableConstantState> ScaleDrawable::GetConstantState()
{
    if (mScaleState->CanConstantState()) {
        mScaleState->mChangingConfigurations = GetChangingConfigurations();
        return mScaleState;
    }
    return NULL;
}

AutoPtr<IDrawable> ScaleDrawable::Mutate()
{
    if (!mMutated &&
            Drawable::Mutate().Get() == THIS_PROBE(IDrawable)) {
        AutoPtr<IDrawable> drawable;
        mScaleState->mDrawable->Mutate((IDrawable**)&drawable);
        mMutated = TRUE;
    }
    return THIS_PROBE(IDrawable);
}

ECode ScaleDrawable::Init()
{
    return Init((ScaleState*)NULL, (IResources*)NULL);
}

ECode ScaleDrawable::Init(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 gravity,
    /* [in] */ Float scaleWidth,
    /* [in] */ Float scaleHeight)
{
    FAIL_RETURN(Init((ScaleState*)NULL, (IResources*)NULL));

    mScaleState->mDrawable = drawable;
    mScaleState->mGravity = gravity;
    mScaleState->mScaleWidth = scaleWidth;
    mScaleState->mScaleHeight = scaleHeight;

    if (drawable != NULL) {
        drawable->SetCallback(
                (IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }
    return NOERROR;
}

ECode ScaleDrawable::Init(
    /* [in] */ ScaleState* state,
    /* [in] */ IResources* res)
{
    mScaleState = new ScaleState(state, this, res);
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
