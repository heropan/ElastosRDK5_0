
#include "elastos/droid/graphics/drawable/NinePatchDrawable.h"
#include "elastos/droid/graphics/drawable/CNinePatchDrawable.h"
// #include "elastos/droid/graphics/CBitmap.h"
#include "elastos/droid/graphics/CRect.h"
// #include "elastos/droid/graphics/CPaint.h"
// #include "elastos/droid/graphics/CNinePatch.h"
// #include "elastos/droid/graphics/CBitmapFactoryOptions.h"
// #include "elastos/droid/graphics/CBitmapFactory.h"
#include "elastos/droid/utility/CTypedValue.h"
#include "elastos/droid/R.h"

using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

////////////////////////////////////////////////////////////////////////////////
// NinePatchDrawable::NinePatchState
NinePatchDrawable::NinePatchState::NinePatchState(
    /* [in] */ INinePatch* ninePatch,
    /* [in] */ IRect* padding)
    : mDither(FALSE)
    , mChangingConfigurations(0)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
{
    AutoPtr<IRect> rect;
    CRect::New((IRect**)&rect);
    constructor(ninePatch, padding, rect, DEFAULT_DITHER);
}

NinePatchDrawable::NinePatchState::NinePatchState(
    /* [in] */ INinePatch* ninePatch,
    /* [in] */ IRect* padding,
    /* [in] */ IRect* layoutInsets)
    : mDither(FALSE)
    , mChangingConfigurations(0)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
{
    constructor(ninePatch, padding, layoutInsets, DEFAULT_DITHER);
}

NinePatchDrawable::NinePatchState::NinePatchState(
    /* [in] */ INinePatch* ninePatch,
    /* [in] */ IRect* rect,
    /* [in] */ IRect* layoutInsets,
    /* [in] */ Boolean dither)
    : mDither(FALSE)
    , mChangingConfigurations(0)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
{
    constructor(ninePatch, rect, layoutInsets, dither);
}

NinePatchDrawable::NinePatchState::NinePatchState(
    /* [in] */ NinePatchState* state)
    : mDither(FALSE)
    , mChangingConfigurations(0)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
{
    assert(0 && "TODO");
    // ASSERT_SUCCEEDED(CNinePatch::New(state->mNinePatch, (INinePatch**)&mNinePatch));
    // Note we don't copy the padding because it is immutable.
    mPadding = state->mPadding;
    mLayoutInsets = state->mLayoutInsets;
    mDither = state->mDither;
    mChangingConfigurations = state->mChangingConfigurations;
    mTargetDensity = state->mTargetDensity;
}

ECode NinePatchDrawable::NinePatchState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CNinePatchDrawable::New(
        reinterpret_cast<Handle32>(this), NULL, (INinePatchDrawable**)drawable);
}

ECode NinePatchDrawable::NinePatchState::NewDrawable(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CNinePatchDrawable::New(
        reinterpret_cast<Handle32>(this), res, (INinePatchDrawable**)drawable);
}

ECode NinePatchDrawable::NinePatchState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}

void NinePatchDrawable::NinePatchState::constructor(
    /* [in] */ INinePatch* ninePatch,
    /* [in] */ IRect* rect,
    /* [in] */ IRect* layoutInsets,
    /* [in] */ Boolean dither)
{
    mNinePatch = ninePatch;
    mPadding = rect;
    assert(0 && "TODO");
    // mLayoutInsets = Insets::Of(layoutInsets);
    mDither = dither;
}

////////////////////////////////////////////////////////////////////////////////
// NinePatchDrawable
CAR_INTERFACE_IMPL(NinePatchDrawable, Drawable, INinePatchDrawable)

const Boolean NinePatchDrawable::DEFAULT_DITHER;
NinePatchDrawable::NinePatchDrawable()
    : mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    assert(0 && "TODO");
    // mLayoutInsets = Insets::NONE;
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
    : mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    assert(0 && "TODO");
    // mLayoutInsets = Insets::NONE;
    constructor(bitmap, chunk, padding, srcName);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
    : mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    assert(0 && "TODO");
    // mLayoutInsets = Insets::NONE;
    constructor(res, bitmap, chunk, padding, srcName);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ IRect* layoutInsets,
    /* [in] */ const String& srcName)
    : mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    assert(0 && "TODO");
    // mLayoutInsets = Insets::NONE;
    constructor(res, bitmap, chunk, padding, layoutInsets, srcName);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ INinePatch* patch)
    : mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    assert(0 && "TODO");
    // mLayoutInsets = Insets::NONE;
    constructor(patch);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ IResources* res,
    /* [in] */ INinePatch* patch)
    : mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    assert(0 && "TODO");
    // mLayoutInsets = Insets::NONE;
    constructor(res, patch);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ NinePatchState* state,
    /* [in] */ IResources* res)
    : mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    assert(0 && "TODO");
    // mLayoutInsets = Insets::NONE;
    constructor(state, res);
}

void NinePatchDrawable::SetNinePatchState(
    /* [in] */ NinePatchState* state,
    /* [in] */ IResources* res)
{
    mNinePatchState = state;
    mNinePatch = state->mNinePatch;
    mPadding = state->mPadding;
    if (res != NULL) {
        AutoPtr<IDisplayMetrics> dm;
        res->GetDisplayMetrics((IDisplayMetrics**)&dm);
        dm->GetDensityDpi(&mTargetDensity);
    }
    else {
        mTargetDensity = state->mTargetDensity;
    }
    //noinspection PointlessBooleanExpression
    if (DEFAULT_DITHER != state->mDither) {
        // avoid calling the setter unless we need to, since it does a
        // lazy allocation of a paint
        SetDither(state->mDither);
    }
    if (mNinePatch != NULL) {
        ComputeBitmapSize();
    }
}

/**
 * Set the density scale at which this drawable will be rendered. This
 * method assumes the drawable will be rendered at the same density as the
 * specified canvas.
 *
 * @param canvas The Canvas from which the density scale must be obtained.
 *
 * @see android.graphics.Bitmap#setDensity(int)
 * @see android.graphics.Bitmap#getDensity()
 */
ECode NinePatchDrawable::SetTargetDensity(
    /* [in] */ ICanvas* canvas)
{
    Int32 density;
    canvas->GetDensity(&density);
    return SetTargetDensity(density);
}

/**
 * Set the density scale at which this drawable will be rendered.
 *
 * @param metrics The DisplayMetrics indicating the density scale for this drawable.
 *
 * @see android.graphics.Bitmap#setDensity(int)
 * @see android.graphics.Bitmap#getDensity()
 */
ECode NinePatchDrawable::SetTargetDensity(
    /* [in] */ IDisplayMetrics* metrics)
{
    Int32 dpi;
    metrics->GetDensityDpi(&dpi);
    return SetTargetDensity(dpi);
}

ECode NinePatchDrawable::SetTargetDensity(
    /* [in] */ Int32 density)
{
    if (density != mTargetDensity) {
        mTargetDensity = density == 0 ? IDisplayMetrics::DENSITY_DEFAULT : density;
        if (mNinePatch != NULL) {
            ComputeBitmapSize();
        }
        InvalidateSelf();
    }
    return NOERROR;
}

AutoPtr<Insets> NinePatchDrawable::ScaleFromDensity(
    /* [in] */ Insets* insets,
    /* [in] */ Int32 sdensity,
    /* [in] */ Int32 tdensity)
{
    assert(0 && "TODO");
    // Int32 left = CBitmap::ScaleFromDensity(insets->mLeft, sdensity, tdensity);
    // Int32 top = CBitmap::ScaleFromDensity(insets->mTop, sdensity, tdensity);
    // Int32 right = CBitmap::ScaleFromDensity(insets->mRight, sdensity, tdensity);
    // Int32 bottom = CBitmap::ScaleFromDensity(insets->mBottom, sdensity, tdensity);
    // return Insets::Of(left, top, right, bottom);
    return NULL;
}

void NinePatchDrawable::ComputeBitmapSize()
{
    Int32 sdensity = 0;
    mNinePatch->GetDensity(&sdensity);
    const Int32 tdensity = mTargetDensity;
    if (sdensity == tdensity) {
        mNinePatch->GetWidth(&mBitmapWidth);
        mNinePatch->GetHeight(&mBitmapHeight);
        mLayoutInsets = mNinePatchState->mLayoutInsets;
    }
    else {
        Int32 width = 0, height = 0;
        mNinePatch->GetWidth(&width);
        mNinePatch->GetHeight(&height);
        assert(0 && "TODO");
        // mBitmapWidth = CBitmap::ScaleFromDensity(width, sdensity, tdensity);
        // mBitmapHeight = CBitmap::ScaleFromDensity(height, sdensity, tdensity);
        // if (mNinePatchState->mPadding != NULL && mPadding != NULL) {
        //     AutoPtr<IRect> dest = mPadding;
        //     AutoPtr<IRect> src = mNinePatchState->mPadding;
        //     if (dest == src) {
        //         mPadding = NULL;
        //         CRect::New(src, (IRect**)&mPadding);
        //         dest = mPadding;
        //     }
        //     CRect* csrc = (CRect*)src.Get();
        //     CRect* cdest = (CRect*)dest.Get();
        //     cdest->mLeft = CBitmap::ScaleFromDensity(csrc->mLeft, sdensity, tdensity);
        //     cdest->mTop = CBitmap::ScaleFromDensity(csrc->mTop, sdensity, tdensity);
        //     cdest->mRight = CBitmap::ScaleFromDensity(csrc->mRight, sdensity, tdensity);
        //     cdest->mBottom = CBitmap::ScaleFromDensity(csrc->mBottom, sdensity, tdensity);
        // }
        mLayoutInsets = ScaleFromDensity(mNinePatchState->mLayoutInsets, sdensity, tdensity);
    }
}

ECode NinePatchDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IRect> rect;
    GetBounds((IRect**)&rect);
    return mNinePatch->Draw(canvas, rect, mPaint);
}

ECode NinePatchDrawable::GetChangingConfigurations(
    /* [out] */ Int32* configuration)
{
    VALIDATE_NOT_NULL(configuration);
    Drawable::GetChangingConfigurations(configuration);
    *configuration = (*configuration)
        | mNinePatchState->mChangingConfigurations;

    return NOERROR;
}

ECode NinePatchDrawable::GetPadding(
    /* [in] */ IRect* padding,
    /* [out] */ Boolean* isPadding)
{
    VALIDATE_NOT_NULL(isPadding);
    assert(padding != NULL);
    padding->Set(mPadding);
    *isPadding = TRUE;
    return NOERROR;
}

AutoPtr<IInsets> NinePatchDrawable::GetLayoutInsets()
{
    return (IInsets*)mLayoutInsets.Get();
}

ECode NinePatchDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    if (mPaint == NULL && alpha == 0xFF) {
        // Fast common case -- leave at normal alpha.
        return NOERROR;
    }
    AutoPtr<IPaint> paint;
    GetPaint((IPaint**)&paint);
    paint->SetAlpha(alpha);
    return InvalidateSelf();
}

ECode NinePatchDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    if (mPaint == NULL && cf == NULL) {
        // Fast common case -- leave at no color filter.
        return NOERROR;
    }
    AutoPtr<IPaint> paint;
    GetPaint((IPaint**)&paint);
    return paint->SetColorFilter(cf);
}

ECode NinePatchDrawable::SetDither(
    /* [in] */ Boolean dither)
{
    if (mPaint == NULL && dither == DEFAULT_DITHER) {
        // Fast common case -- leave at default dither.
        return NOERROR;
    }
    AutoPtr<IPaint> paint;
    GetPaint((IPaint**)&paint);
    paint->SetDither(dither);
    return InvalidateSelf();
}

ECode NinePatchDrawable::SetFilterBitmap(
    /* [in] */ Boolean filter)
{
    AutoPtr<IPaint> paint;
    GetPaint((IPaint**)&paint);
    paint->SetFilterBitmap(filter);
    return InvalidateSelf();
}

ECode NinePatchDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    Drawable::Inflate(r, parser, attrs);

    Int32 size = ARRAY_SIZE(R::styleable::NinePatchDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::NinePatchDrawable, size);

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(r->ObtainAttributes(attrs, layout, (ITypedArray**)&a));

    Int32 id;
    a->GetResourceId(R::styleable::NinePatchDrawable_src, 0, &id);
    if (id == 0) {
//        throw new XmlPullParserException(parser.getPositionDescription() +
//                ": <nine-patch> requires a valid src attribute");
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    Boolean dither;
    a->GetBoolean(R::styleable::NinePatchDrawable_dither,
            DEFAULT_DITHER, &dither);

    assert(0 && "TODO");
//     AutoPtr<CBitmapFactoryOptions> options;
//     CBitmapFactoryOptions::NewByFriend((CBitmapFactoryOptions**)&options);
//     if (dither) {
//         options->mInDither = FALSE;
//     }
//     AutoPtr<IDisplayMetrics> dis;
//     r->GetDisplayMetrics((IDisplayMetrics**)&dis);
//     dis->GetNoncompatDensityDpi(&options->mInScreenDensity);

    AutoPtr<IRect> padding, layoutInsets;
//     CRect::New((IRect**)&padding);
//     CRect::New((IRect**)&layoutInsets);
    AutoPtr<IBitmap> bitmap;

// //    try {
//     AutoPtr<ITypedValue> value;
//     CTypedValue::New((ITypedValue**)&value);
    AutoPtr<IInputStream> is;
//     r->OpenRawResource(id, value, (IInputStream**)&is);

//     AutoPtr<IBitmapFactory> bmFactory;
//     CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bmFactory);
//     bmFactory->DecodeResourceStream(r, value, is, padding, options, (IBitmap**)&bitmap);

    is->Close();
//    } catch (IOException e) {
//        // Ignore
//    }

    AutoPtr< ArrayOf<Byte> > data;
    if (bitmap == NULL) {
//        throw new XmlPullParserException(parser.getPositionDescription() +
//                ": <nine-patch> requires a valid src attribute");
        return E_XML_PULL_PARSER_EXCEPTION;
    }
    else {
        bitmap->GetNinePatchChunk((ArrayOf<Byte>**)&data);
        if (data == NULL) {
//            throw new XmlPullParserException(parser.getPositionDescription() +
//                    ": <nine-patch> requires a valid 9-patch source image");
            return E_XML_PULL_PARSER_EXCEPTION;
        }

    }

    AutoPtr<INinePatch> np;
    assert(0 && "TODO");
    // CNinePatch::New(bitmap, data, String("XML 9-patch"), (INinePatch**)&np);
    AutoPtr<NinePatchState> state = new NinePatchState(np, padding, layoutInsets, dither);
    SetNinePatchState(state, r);
    mNinePatchState->mTargetDensity = mTargetDensity;

    a->Recycle();
    return NOERROR;
}

ECode NinePatchDrawable::GetPaint(
    /* [out] */ IPaint** paint)
{
    VALIDATE_NOT_NULL(paint);
    VALIDATE_NOT_NULL(paint);
    if (mPaint == NULL) {
        assert(0 && "TODO");
        // ASSERT_SUCCEEDED(CPaint::New((IPaint**)&mPaint));
        mPaint->SetDither(DEFAULT_DITHER);
    }
    *paint = mPaint;
    REFCOUNT_ADD(*paint);
    return NOERROR;
}

Int32 NinePatchDrawable::GetIntrinsicWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = mBitmapWidth;
    return NOERROR;;
}

Int32 NinePatchDrawable::GetIntrinsicHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = mBitmapHeight;
    return NOERROR;;
}

ECode NinePatchDrawable::GetMinimumWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = mBitmapWidth;
    return NOERROR;
}

ECode NinePatchDrawable::GetMinimumHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = mBitmapHeight;
    return NOERROR;
}

ECode NinePatchDrawable::GetOpacity(
    /* [out] */ Int32* opacity)
{
    VALIDATE_NOT_NULL(opacity);
    Boolean hasAlpha;
    Int32 alpha;
    mNinePatch->HasAlpha(&hasAlpha);
    *opacity = (hasAlpha || (mPaint != NULL && (mPaint->GetAlpha(&alpha), alpha < 255))) ?
           IPixelFormat::TRANSLUCENT : IPixelFormat::OPAQUE;
    return NOERROR;
}

ECode NinePatchDrawable::GetTransparentRegion(
    /* [out] */ IRegion** bounds)
{
    VALIDATE_NOT_NULL(bounds);
    AutoPtr<IRect> tmp;
    GetBounds((IRect**)&tmp);
    return mNinePatch->GetTransparentRegion(tmp, bounds);
}

ECode NinePatchDrawable::GetConstantState(
    /* [out] */ IDrawableConstantState** state)
{
    VALIDATE_NOT_NULL(state);
    GetChangingConfigurations(&mNinePatchState->mChangingConfigurations);
    *state = (IDrawableConstantState*)mNinePatchState.Get();
    REFCOUNT_ADD(*state);
    return NOERROR;
}

ECode NinePatchDrawable::Mutate(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> tmp;
    if (!mMutated && (Drawable::Mutate((IDrawable**)&tmp), tmp.Get())
        == (IDrawable*)this->Probe(EIID_IDrawable)) {
        mNinePatchState = new NinePatchState(mNinePatchState);
        mNinePatch = mNinePatchState->mNinePatch;
        mMutated = TRUE;
    }
    *drawable = (IDrawable*)this->Probe(EIID_IDrawable);
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode NinePatchDrawable::constructor(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
{
    AutoPtr<INinePatch> patch;
    assert(0 && "TODO");
    // FAIL_RETURN(CNinePatch::New(bitmap, chunk, srcName, (INinePatch**)&patch));
    AutoPtr<NinePatchState> state = new NinePatchState(patch, padding);
    return constructor(state, NULL);
}

ECode NinePatchDrawable::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
{
    AutoPtr<INinePatch> patch;
    assert(0 && "TODO");
    // FAIL_RETURN(CNinePatch::New(bitmap, chunk, srcName, (INinePatch**)&patch));
    AutoPtr<NinePatchState> state = new NinePatchState(patch, padding);
    FAIL_RETURN(constructor(state, res));
    mNinePatchState->mTargetDensity = mTargetDensity;
    return NOERROR;
}

ECode NinePatchDrawable::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ IRect* layoutInsets,
    /* [in] */ const String& srcName)
{
    AutoPtr<INinePatch> ninePatch;
    assert(0 && "TODO");
    // FAIL_RETURN(CNinePatch::New(bitmap, chunk, srcName, (INinePatch**)&ninePatch));
    AutoPtr<NinePatchState> state = new NinePatchState(ninePatch, padding, layoutInsets);
    FAIL_RETURN(constructor(state, res));
    mNinePatchState->mTargetDensity = mTargetDensity;
    return NOERROR;
}

ECode NinePatchDrawable::constructor(
    /* [in] */ INinePatch* patch)
{
    AutoPtr<IRect> rect;
    FAIL_RETURN(CRect::New((IRect**)&rect));
    AutoPtr<NinePatchState> state = new NinePatchState(patch, rect);
    return constructor(state, NULL);
}

ECode NinePatchDrawable::constructor(
    /* [in] */ IResources* res,
    /* [in] */ INinePatch* patch)
{
    AutoPtr<IRect> rect;
    FAIL_RETURN(CRect::New((IRect**)&rect));
    AutoPtr<NinePatchState> state = new NinePatchState(patch, rect);
    FAIL_RETURN(constructor(state, res));
    mNinePatchState->mTargetDensity = mTargetDensity;
    return NOERROR;
}

ECode NinePatchDrawable::constructor(
    /* [in] */ NinePatchState* state,
    /* [in] */ IResources* res)
{
    SetNinePatchState(state, res);
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
