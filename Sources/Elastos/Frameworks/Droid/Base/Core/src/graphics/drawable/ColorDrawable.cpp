
#include "ext/frameworkext.h"
#include "graphics/drawable/ColorDrawable.h"
#include "graphics/drawable/CColorDrawable.h"
#include "graphics/CPaint.h"
#include "R.h"

using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ColorDrawable::ColorState::ColorState(
    /* [in] */ ColorState* state)
    : mBaseColor(0)
    , mUseColor(0)
    , mChangingConfigurations(0)
{
    if (state != NULL) {
        mBaseColor = state->mBaseColor;
        mUseColor = state->mUseColor;
        mChangingConfigurations = state->mChangingConfigurations;
    }
}

ECode ColorDrawable::ColorState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CColorDrawable::New(this, (IColorDrawable**)drawable);
}

ECode ColorDrawable::ColorState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CColorDrawable::New(this, (IColorDrawable**)drawable);
}

ECode ColorDrawable::ColorState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}


ColorDrawable::ColorDrawable()
    : mMutated(FALSE)
{
    CPaint::New((IPaint**)&mPaint);
    Init((ColorState*)NULL);
}

ColorDrawable::ColorDrawable(
    /* [in] */ Int32 color)
    : mMutated(FALSE)
{
    CPaint::New((IPaint**)&mPaint);
    Init((ColorState*)NULL);
    SetColor(color);
}

ColorDrawable::ColorDrawable(
    /* [in] */ ColorState* state)
    : mMutated(FALSE)
{
    CPaint::New((IPaint**)&mPaint);
    Init(state);
}

Int32 ColorDrawable::GetChangingConfigurations()
{
    return Drawable::GetChangingConfigurations() | mState->mChangingConfigurations;
}

AutoPtr<IDrawable> ColorDrawable::Mutate()
{
    if (!mMutated && Drawable::Mutate().Get() == (IDrawable*)this->Probe(EIID_IDrawable)) {
        mState = new ColorState(mState);
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode ColorDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (((mState->mUseColor >> 24) & 0xFF) != 0) {
        mPaint->SetColor(mState->mUseColor);
        canvas->DrawRectEx(GetBounds(), mPaint);
    }
    return NOERROR;
}

ECode ColorDrawable::GetColor(
    /* [out] */ Int32* color)
{
    assert(color != NULL);
    *color = mState->mUseColor;
    return NOERROR;
}

ECode ColorDrawable::SetColor(
    /* [in] */ Int32 color)
{
    if (mState->mBaseColor != color || mState->mUseColor != color) {
        InvalidateSelf();
        mState->mBaseColor = mState->mUseColor = color;
    }
    return NOERROR;
}

Int32 ColorDrawable::GetAlpha()
{
    return (mState->mUseColor >> 24) & 0xFF;
}

ECode ColorDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    alpha += alpha >> 7;   // make it 0..256
    Int32 baseAlpha = (mState->mBaseColor >> 24) & 0xFF;
    Int32 useAlpha = baseAlpha * alpha >> 8;
    Int32 oldUseColor = mState->mUseColor;
    mState->mUseColor = (mState->mBaseColor & 0x00FFFFFF) | (useAlpha << 24);
    if (oldUseColor != mState->mUseColor) {
        InvalidateSelf();
    }
    return NOERROR;
}

ECode ColorDrawable::SetColorFilter(
    /* [in] */ IColorFilter* colorFilter)
{
    return NOERROR;
}

Int32 ColorDrawable::GetOpacity()
{
    switch ((mState->mUseColor >> 24) & 0xFF) {
        case 255:
            return IPixelFormat::OPAQUE;
        case 0:
            return IPixelFormat::TRANSPARENT;
    }

    return IPixelFormat::TRANSLUCENT;
}

ECode ColorDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Drawable::Inflate(r, parser, attrs));

    Int32 size = ARRAY_SIZE(R::styleable::ColorDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::ColorDrawable, size);

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(r->ObtainAttributes(attrs, layout, (ITypedArray**)&a));

    Int32 color = mState->mBaseColor;
    FAIL_RETURN(a->GetColor(R::styleable::ColorDrawable_color, color, &color));
    mState->mBaseColor = mState->mUseColor = color;

    a->Recycle();
    return NOERROR;
}

AutoPtr<IDrawableConstantState> ColorDrawable::GetConstantState()
{
    mState->mChangingConfigurations = GetChangingConfigurations();
    return (IDrawableConstantState*)mState->Probe(EIID_IDrawableConstantState);
}

ECode ColorDrawable::Init(
    /* [in] */ Int32 color)
{
    Init((ColorState*)NULL);
    SetColor(color);
    return NOERROR;
}

ECode ColorDrawable::Init(
    /* [in] */ ColorState* state)
{
    mState = new ColorState(state);
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
