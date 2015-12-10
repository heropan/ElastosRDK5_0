
#include "Elastos.CoreLibrary.External.h"
#include "Elastos.CoreLibrary.IO.h"
#include "_Elastos.Droid.Core.h"
#include "Elastos.Droid.Os.h"
#include "Elastos.Droid.Utility.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/graphics/drawable/ColorDrawable.h"
#include "elastos/droid/graphics/drawable/CColorDrawable.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/content/res/CResources.h"
#include "elastos/droid/content/res/CTypedArray.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::CResources;
using Elastos::Droid::Content::Res::CTypedArray;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ColorDrawable::ColorState::ColorState()
    : mBaseColor(0)
    , mUseColor(0)
    , mChangingConfigurations(0)
    , mTintMode(DEFAULT_TINT_MODE)
{
    // Empty constructor.
}

ColorDrawable::ColorState::ColorState(
    /* [in] */ ColorState* state)
    : mBaseColor(0)
    , mUseColor(0)
    , mChangingConfigurations(0)
    , mTintMode(DEFAULT_TINT_MODE)
{
    if (state != NULL) {
        mThemeAttrs = state->mThemeAttrs;
        mBaseColor = state->mBaseColor;
        mUseColor = state->mUseColor;
        mChangingConfigurations = state->mChangingConfigurations;
        mTint = state->mTint;
        mTintMode = state->mTintMode;
    }
}

ECode ColorDrawable::ColorState::CanApplyTheme(
    /* [out] */ Boolean* can)
{
    VALIDATE_NOT_NULL(can);
    *can = mThemeAttrs != NULL;
    return NOERROR;
}

ECode ColorDrawable::ColorState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CColorDrawable::New(THIS_PROBE(IDrawableConstantState), NULL, NULL, (IColorDrawable**)drawable);
}

ECode ColorDrawable::ColorState::NewDrawable(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CColorDrawable::New(THIS_PROBE(IDrawableConstantState), res, NULL, (IColorDrawable**)drawable);
}

ECode ColorDrawable::ColorState::NewDrawable(
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme,
    /* [out] */ IDrawable** drawable)
{
    return CColorDrawable::New(THIS_PROBE(IDrawableConstantState), res, theme, (IColorDrawable**)drawable);
}

ECode ColorDrawable::ColorState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}

CAR_INTERFACE_IMPL(ColorDrawable, Drawable, IColorDrawable)
ColorDrawable::ColorDrawable()
    : mMutated(FALSE)
{
    constructor();
}

ColorDrawable::ColorDrawable(
    /* [in] */ Int32 color)
    : mMutated(FALSE)
{
    constructor(color);
}

ColorDrawable::ColorDrawable(
    /* [in] */ IDrawableConstantState* state,
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme)
    : mMutated(FALSE)
{
    constructor(state, res, theme);
}

ECode ColorDrawable::GetChangingConfigurations(
    /* [out] */ Int32* configuration)
{
    VALIDATE_NOT_NULL(configuration);
    Drawable::GetChangingConfigurations(configuration);
    *configuration = (*configuration) | mColorState->mChangingConfigurations;
    return NOERROR;
}

ECode ColorDrawable::Mutate(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> tmp;
    if (!mMutated && (Drawable::Mutate((IDrawable**)&tmp), tmp.Get()) == (IDrawable*)this->Probe(EIID_IDrawable)) {
        mColorState = new ColorState(mColorState);
        mMutated = TRUE;
    }
    *drawable = (IDrawable*)this->Probe(EIID_IDrawable);
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode ColorDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IColorFilter> colorFilter;
    mPaint->GetColorFilter((IColorFilter**)&colorFilter);
    if (((mColorState->mUseColor >> 24) & 0xFF) != 0 || colorFilter != NULL || mTintFilter != NULL) {
        if (colorFilter == NULL) {
            mPaint->SetColorFilter(IColorFilter::Probe(mTintFilter));
        }

        mPaint->SetColor(mColorState->mUseColor);
        AutoPtr<IRect> bounds;
        GetBounds((IRect**)&bounds);
        canvas->DrawRect(bounds, mPaint);

        // Restore original color filter.
        mPaint->SetColorFilter(colorFilter);
    }

    return NOERROR;
}

ECode ColorDrawable::GetColor(
    /* [out] */ Int32* color)
{
    assert(color != NULL);
    *color = mColorState->mUseColor;
    return NOERROR;
}

ECode ColorDrawable::SetColor(
    /* [in] */ Int32 color)
{
    if (mColorState->mBaseColor != color || mColorState->mUseColor != color) {
        mColorState->mBaseColor = mColorState->mUseColor = color;
        InvalidateSelf();
    }
    return NOERROR;
}

ECode ColorDrawable::GetAlpha(
    /* [out] */ Int32* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = (mColorState->mUseColor >> 24) & 0xFF;
    return NOERROR;
}

ECode ColorDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    alpha += alpha >> 7;   // make it 0..256
    Int32 baseAlpha = (mColorState->mBaseColor >> 24) & 0xFF;
    Int32 useAlpha = baseAlpha * alpha >> 8;
    Int32 useColor = (mColorState->mBaseColor & 0x00FFFFFF) | (useAlpha << 24);
    if (mColorState->mUseColor != useColor) {
        mColorState->mUseColor = useColor;
        InvalidateSelf();
    }
    return NOERROR;
}

ECode ColorDrawable::SetColorFilter(
    /* [in] */ IColorFilter* colorFilter)
{
    return mPaint->SetColorFilter(colorFilter);
}

ECode ColorDrawable::SetTintList(
    /* [in] */ IColorStateList* tint)
{
    mColorState->mTint = tint;
    mTintFilter = UpdateTintFilter(mTintFilter, tint, mColorState->mTintMode);
    return InvalidateSelf();
}

ECode ColorDrawable::SetTintMode(
    /* [in] */ PorterDuffMode tintMode)
{
    mColorState->mTintMode = tintMode;
    mTintFilter = UpdateTintFilter(mTintFilter, mColorState->mTint, tintMode);
    return InvalidateSelf();
}

Boolean ColorDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* stateSet)
{
    AutoPtr<ColorState> state = mColorState;
    if (state->mTint.Get() != NULL && state->mTintMode != -1) {
        mTintFilter = UpdateTintFilter(mTintFilter, state->mTint, state->mTintMode);
        return TRUE;
    }
    return FALSE;
}

ECode ColorDrawable::IsStateful(
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mColorState->mTint != NULL && (mColorState->mTint->IsStateful(state), *state);
    return NOERROR;
}

ECode ColorDrawable::GetOpacity(
    /* [out] */ Int32* opacity)
{
    VALIDATE_NOT_NULL(opacity);
    AutoPtr<IColorFilter> cf;
    if (mTintFilter != NULL || (mPaint->GetColorFilter((IColorFilter**)&cf), cf.Get()) != NULL) {
        *opacity = IPixelFormat::TRANSLUCENT;
        return NOERROR;
    }

    switch ((mColorState->mUseColor >> 24) & 0xFF) {
        case 255:
            *opacity = IPixelFormat::OPAQUE;
            return NOERROR;
        case 0:
            *opacity = IPixelFormat::TRANSPARENT;
            return NOERROR;
    }

    *opacity = IPixelFormat::TRANSLUCENT;
    return NOERROR;
}

ECode ColorDrawable::GetOutline(
    /* [in] */ /*@NonNull*/ IOutline* outline)
{
    AutoPtr<IRect> bounds;
    GetBounds((IRect**)&bounds);
    outline->SetRect(bounds);
    Int32 alpha = 0;
    GetAlpha(&alpha);
    return outline->SetAlpha(alpha / 255.0f);
}

ECode ColorDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IResourcesTheme* theme)
{
    FAIL_RETURN(Drawable::Inflate(r, parser, attrs, theme));

    Int32 size = ARRAY_SIZE(R::styleable::ColorDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::ColorDrawable, size);

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(ObtainAttributes(r, theme, attrs, layout, (ITypedArray**)&a));

    UpdateStateFromTypedArray(a);
    a->Recycle();
    return NOERROR;
}

void ColorDrawable::UpdateStateFromTypedArray(
    /* [in] */ ITypedArray* a)
{
    AutoPtr<ColorState> state = mColorState;

    // Account for any configuration changes.
    Int32 value = 0;
    state->mChangingConfigurations |= (a->GetChangingConfigurations(&value), value);

    // Extract the theme attributes, if any.
    ((CTypedArray*)a)->ExtractThemeAttrs((ArrayOf<Int32>**)&state->mThemeAttrs);

    state->mBaseColor = (a->GetColor(R::styleable::ColorDrawable_color, state->mBaseColor, &value), value);
    state->mUseColor = state->mBaseColor;
}

ECode ColorDrawable::ApplyTheme(
    /* [in] */ IResourcesTheme* t)
{
    Drawable::ApplyTheme(t);

    AutoPtr<ColorState> state = mColorState;
    if (state == NULL || state->mThemeAttrs == NULL) {
        return NOERROR;
    }

    AutoPtr<ITypedArray> a;
    Int32 size = ARRAY_SIZE(R::styleable::ColorDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::ColorDrawable, size);

    ((CResources::Theme*)t)->ResolveAttribute(state->mThemeAttrs, layout, (ITypedArray**)&a);
    UpdateStateFromTypedArray(a);
    return a->Recycle();
}

ECode ColorDrawable::GetConstantState(
    /* [out] */ IDrawableConstantState** state)
{
    VALIDATE_NOT_NULL(state);
    *state = (IDrawableConstantState*)mColorState->Probe(EIID_IDrawableConstantState);
    REFCOUNT_ADD(*state);
    return NOERROR;
}

ECode ColorDrawable::constructor()
{
    CPaint::New((IPaint**)&mPaint);
    mColorState = new ColorState();
    return NOERROR;
}

ECode ColorDrawable::constructor(
    /* [in] */ Int32 color)
{
    CPaint::New((IPaint**)&mPaint);
    mColorState = new ColorState();
    SetColor(color);
    return NOERROR;
}

ECode ColorDrawable::constructor(
    /* [in] */ IDrawableConstantState* state,
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme)
{
    CPaint::New((IPaint**)&mPaint);
    Boolean can = FALSE;
    if (theme != NULL && (state->CanApplyTheme(&can), can)) {
        mColorState = new ColorState((ColorState*)state);
        ApplyTheme(theme);
    } else {
        mColorState = (ColorState*)state;
    }

    mTintFilter = UpdateTintFilter(mTintFilter, ((ColorState*)state)->mTint, ((ColorState*)state)->mTintMode);
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
