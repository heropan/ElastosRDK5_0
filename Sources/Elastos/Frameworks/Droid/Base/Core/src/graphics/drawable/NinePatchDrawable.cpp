
#include "ext/frameworkext.h"
#include "graphics/drawable/NinePatchDrawable.h"
#include "graphics/drawable/CNinePatchDrawable.h"
#include "graphics/CBitmap.h"
#include "graphics/CRect.h"
#include "graphics/CPaint.h"
#include "graphics/CNinePatch.h"
#include "graphics/CBitmapFactoryOptions.h"
#include "graphics/CBitmapFactory.h"
#include "util/CTypedValue.h"
#include "R.h"

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
    Init(ninePatch, padding, rect, DEFAULT_DITHER);
}

NinePatchDrawable::NinePatchState::NinePatchState(
    /* [in] */ INinePatch* ninePatch,
    /* [in] */ IRect* padding,
    /* [in] */ IRect* layoutInsets)
    : mDither(FALSE)
    , mChangingConfigurations(0)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
{
    Init(ninePatch, padding, layoutInsets, DEFAULT_DITHER);
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
    Init(ninePatch, rect, layoutInsets, dither);
}

NinePatchDrawable::NinePatchState::NinePatchState(
    /* [in] */ NinePatchState* state)
    : mDither(FALSE)
    , mChangingConfigurations(0)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
{
    ASSERT_SUCCEEDED(CNinePatch::New(state->mNinePatch, (INinePatch**)&mNinePatch));
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

ECode NinePatchDrawable::NinePatchState::NewDrawableEx(
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

void NinePatchDrawable::NinePatchState::Init(
    /* [in] */ INinePatch* ninePatch,
    /* [in] */ IRect* rect,
    /* [in] */ IRect* layoutInsets,
    /* [in] */ Boolean dither)
{
    mNinePatch = ninePatch;
    mPadding = rect;
    mLayoutInsets = Insets::Of(layoutInsets);
    mDither = dither;
}

////////////////////////////////////////////////////////////////////////////////
// NinePatchDrawable

const Boolean NinePatchDrawable::DEFAULT_DITHER;

NinePatchDrawable::NinePatchDrawable()
    : mLayoutInsets(Insets::NONE)
    , mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
    : mLayoutInsets(Insets::NONE)
    , mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    Init(bitmap, chunk, padding, srcName);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
    : mLayoutInsets(Insets::NONE)
    , mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    Init(res, bitmap, chunk, padding, srcName);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ IRect* layoutInsets,
    /* [in] */ const String& srcName)
    : mLayoutInsets(Insets::NONE)
    , mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    Init(res, bitmap, chunk, padding, layoutInsets, srcName);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ INinePatch* patch)
    : mLayoutInsets(Insets::NONE)
    , mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    Init(patch);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ IResources* res,
    /* [in] */ INinePatch* patch)
    : mLayoutInsets(Insets::NONE)
    , mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    Init(res, patch);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ NinePatchState* state,
    /* [in] */ IResources* res)
    : mLayoutInsets(Insets::NONE)
    , mMutated(FALSE)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    Init(state, res);
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
    Int32 left = CBitmap::ScaleFromDensity(insets->mLeft, sdensity, tdensity);
    Int32 top = CBitmap::ScaleFromDensity(insets->mTop, sdensity, tdensity);
    Int32 right = CBitmap::ScaleFromDensity(insets->mRight, sdensity, tdensity);
    Int32 bottom = CBitmap::ScaleFromDensity(insets->mBottom, sdensity, tdensity);
    return Insets::Of(left, top, right, bottom);
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
        mBitmapWidth = CBitmap::ScaleFromDensity(width, sdensity, tdensity);
        mBitmapHeight = CBitmap::ScaleFromDensity(height, sdensity, tdensity);
        if (mNinePatchState->mPadding != NULL && mPadding != NULL) {
            AutoPtr<IRect> dest = mPadding;
            AutoPtr<IRect> src = mNinePatchState->mPadding;
            if (dest == src) {
                mPadding = NULL;
                CRect::New(src, (IRect**)&mPadding);
                dest = mPadding;
            }
            CRect* csrc = (CRect*)src.Get();
            CRect* cdest = (CRect*)dest.Get();
            cdest->mLeft = CBitmap::ScaleFromDensity(csrc->mLeft, sdensity, tdensity);
            cdest->mTop = CBitmap::ScaleFromDensity(csrc->mTop, sdensity, tdensity);
            cdest->mRight = CBitmap::ScaleFromDensity(csrc->mRight, sdensity, tdensity);
            cdest->mBottom = CBitmap::ScaleFromDensity(csrc->mBottom, sdensity, tdensity);
        }
        mLayoutInsets = ScaleFromDensity(mNinePatchState->mLayoutInsets, sdensity, tdensity);
    }
}

ECode NinePatchDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    return mNinePatch->DrawEx2(canvas, GetBounds(), mPaint);
}

Int32 NinePatchDrawable::GetChangingConfigurations()
{
    return Drawable::GetChangingConfigurations()
        | mNinePatchState->mChangingConfigurations;
}

Boolean NinePatchDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    assert(padding != NULL);
    padding->SetEx(mPadding);
    return TRUE;
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
    GetPaint()->SetAlpha(alpha);
    return InvalidateSelf();
}

ECode NinePatchDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    if (mPaint == NULL && cf == NULL) {
        // Fast common case -- leave at no color filter.
        return NOERROR;
    }
    return GetPaint()->SetColorFilter(cf);
}

ECode NinePatchDrawable::SetDither(
    /* [in] */ Boolean dither)
{
    if (mPaint == NULL && dither == DEFAULT_DITHER) {
        // Fast common case -- leave at default dither.
        return NOERROR;
    }
    GetPaint()->SetDither(dither);
    return InvalidateSelf();
}

ECode NinePatchDrawable::SetFilterBitmap(
    /* [in] */ Boolean filter)
{
    GetPaint()->SetFilterBitmap(filter);
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
    AutoPtr<CBitmapFactoryOptions> options;
    CBitmapFactoryOptions::NewByFriend((CBitmapFactoryOptions**)&options);
    if (dither) {
        options->mInDither = FALSE;
    }
    AutoPtr<IDisplayMetrics> dis;
    r->GetDisplayMetrics((IDisplayMetrics**)&dis);
    dis->GetNoncompatDensityDpi(&options->mInScreenDensity);

    AutoPtr<IRect> padding, layoutInsets;
    CRect::New((IRect**)&padding);
    CRect::New((IRect**)&layoutInsets);
    AutoPtr<IBitmap> bitmap;

//    try {
    AutoPtr<ITypedValue> value;
    CTypedValue::New((ITypedValue**)&value);
    AutoPtr<IInputStream> is;
    r->OpenRawResourceEx(id, value, (IInputStream**)&is);

    AutoPtr<IBitmapFactory> bmFactory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bmFactory);
    bmFactory->DecodeResourceStream(r, value, is, padding, options, (IBitmap**)&bitmap);

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
    CNinePatch::New(bitmap, data, String("XML 9-patch"), (INinePatch**)&np);
    AutoPtr<NinePatchState> state = new NinePatchState(np, padding, layoutInsets, dither);
    SetNinePatchState(state, r);
    mNinePatchState->mTargetDensity = mTargetDensity;

    a->Recycle();
    return NOERROR;
}

AutoPtr<IPaint> NinePatchDrawable::GetPaint()
{
    if (mPaint == NULL) {
        ASSERT_SUCCEEDED(CPaint::New((IPaint**)&mPaint));
        mPaint->SetDither(DEFAULT_DITHER);
    }
    return mPaint;
}

/**
 * Retrieves the width of the source .png file (before resizing).
 */
//@Override
Int32 NinePatchDrawable::GetIntrinsicWidth()
{
    return mBitmapWidth;
}

/**
 * Retrieves the height of the source .png file (before resizing).
 */
//@Override
Int32 NinePatchDrawable::GetIntrinsicHeight()
{
    return mBitmapHeight;
}

//@Override
Int32 NinePatchDrawable::GetMinimumWidth()
{
    return mBitmapWidth;
}

//@Override
Int32 NinePatchDrawable::GetMinimumHeight()
{
    return mBitmapHeight;
}

//@Override
Int32 NinePatchDrawable::GetOpacity()
{
    Boolean hasAlpha;
    Int32 alpha;
    mNinePatch->HasAlpha(&hasAlpha);
    return (hasAlpha || (mPaint != NULL && (mPaint->GetAlpha(&alpha), alpha < 255))) ?
           IPixelFormat::TRANSLUCENT : IPixelFormat::OPAQUE;
}

//@Override
AutoPtr<IRegion> NinePatchDrawable::GetTransparentRegion()
{
    AutoPtr<IRegion> region;
    mNinePatch->GetTransparentRegion(GetBounds(), (IRegion**)&region);
    return region;
}

//@Override
AutoPtr<IDrawableConstantState> NinePatchDrawable::GetConstantState()
{
    mNinePatchState->mChangingConfigurations = GetChangingConfigurations();
    return (IDrawableConstantState*)mNinePatchState.Get();
}

//@Override
AutoPtr<IDrawable> NinePatchDrawable::Mutate()
{
    if (!mMutated && Drawable::Mutate().Get()
        == (IDrawable*)this->Probe(EIID_IDrawable)) {
        mNinePatchState = new NinePatchState(mNinePatchState);
        mNinePatch = mNinePatchState->mNinePatch;
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode NinePatchDrawable::Init(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
{
    AutoPtr<INinePatch> patch;
    FAIL_RETURN(CNinePatch::New(bitmap, chunk, srcName, (INinePatch**)&patch));
    AutoPtr<NinePatchState> state = new NinePatchState(patch, padding);
    return Init(state, NULL);
}

ECode NinePatchDrawable::Init(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
{
    AutoPtr<INinePatch> patch;
    FAIL_RETURN(CNinePatch::New(bitmap, chunk, srcName, (INinePatch**)&patch));
    AutoPtr<NinePatchState> state = new NinePatchState(patch, padding);
    FAIL_RETURN(Init(state, res));
    mNinePatchState->mTargetDensity = mTargetDensity;
    return NOERROR;
}

ECode NinePatchDrawable::Init(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ IRect* padding,
    /* [in] */ IRect* layoutInsets,
    /* [in] */ const String& srcName)
{
    AutoPtr<INinePatch> ninePatch;
    FAIL_RETURN(CNinePatch::New(bitmap, chunk, srcName, (INinePatch**)&ninePatch));
    AutoPtr<NinePatchState> state = new NinePatchState(ninePatch, padding, layoutInsets);
    FAIL_RETURN(Init(state, res));
    mNinePatchState->mTargetDensity = mTargetDensity;
    return NOERROR;
}

ECode NinePatchDrawable::Init(
    /* [in] */ INinePatch* patch)
{
    AutoPtr<IRect> rect;
    FAIL_RETURN(CRect::New((IRect**)&rect));
    AutoPtr<NinePatchState> state = new NinePatchState(patch, rect);
    return Init(state, NULL);
}

ECode NinePatchDrawable::Init(
    /* [in] */ IResources* res,
    /* [in] */ INinePatch* patch)
{
    AutoPtr<IRect> rect;
    FAIL_RETURN(CRect::New((IRect**)&rect));
    AutoPtr<NinePatchState> state = new NinePatchState(patch, rect);
    FAIL_RETURN(Init(state, res));
    mNinePatchState->mTargetDensity = mTargetDensity;
    return NOERROR;
}

ECode NinePatchDrawable::Init(
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
