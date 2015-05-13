
#include "graphics/drawable/BitmapDrawable.h"
#include "graphics/drawable/CBitmapDrawable.h"
#include "graphics/CPaint.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/CRect.h"
#include "graphics/CBitmapShader.h"
#include "view/CGravity.h"
#include "R.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::CGravity;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

BitmapDrawable::BitmapState::BitmapState(
    /* [in] */ IBitmap* bitmap)
    : mBitmap(bitmap)
    , mChangingConfigurations(0)
    , mGravity(IGravity::FILL)
    , mTileModeX(-1)
    , mTileModeY(-1)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
    , mRebuildShader(FALSE)
{
    ASSERT_SUCCEEDED(CPaint::New(DEFAULT_PAINT_FLAGS, (IPaint**)&mPaint));
}

BitmapDrawable::BitmapState::BitmapState(
    /* [in] */ BitmapState* state)
    : mGravity(IGravity::FILL)
    , mTileModeX(-1)
    , mTileModeY(-1)
    , mTargetDensity(IDisplayMetrics::DENSITY_DEFAULT)
{
    mBitmap = state->mBitmap;
    mChangingConfigurations = state->mChangingConfigurations;
    mGravity = state->mGravity;
    mTileModeX = state->mTileModeX;
    mTileModeY = state->mTileModeY;
    mTargetDensity = state->mTargetDensity;
    mRebuildShader = state->mRebuildShader;
    ASSERT_SUCCEEDED(CPaint::New(state->mPaint, (IPaint**)&mPaint));
    mRebuildShader = state->mRebuildShader;
}

ECode BitmapDrawable::BitmapState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CBitmapDrawable::New((Handle32)this, NULL, (IBitmapDrawable**)drawable);
}

ECode BitmapDrawable::BitmapState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CBitmapDrawable::New((Handle32)this, res, (IBitmapDrawable**)drawable);
}

ECode BitmapDrawable::BitmapState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}

//////////////////////////////////////////////////////////////////////
const Int32 BitmapDrawable::DEFAULT_PAINT_FLAGS =
        IPaint::FILTER_BITMAP_FLAG | IPaint::DITHER_FLAG;

BitmapDrawable::BitmapDrawable()
    : mTargetDensity(0)
    , mApplyGravity(FALSE)
    , mMutated(FALSE)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    CRect::New((IRect**)&mDstRect);
    Init();
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ IResources* res)
    : mTargetDensity(0)
    , mApplyGravity(FALSE)
    , mMutated(FALSE)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    CRect::New((IRect**)&mDstRect);
    Init(res);
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ IBitmap* bitmap)
    : mTargetDensity(0)
    , mApplyGravity(FALSE)
    , mMutated(FALSE)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    CRect::New((IRect**)&mDstRect);
    Init(bitmap);
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap)
    : mTargetDensity(0)
    , mApplyGravity(FALSE)
    , mMutated(FALSE)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    CRect::New((IRect**)&mDstRect);
    Init(res, bitmap);
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ const String& filepath)
    : mTargetDensity(0)
    , mApplyGravity(FALSE)
    , mMutated(FALSE)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    CRect::New((IRect**)&mDstRect);
    Init(filepath);
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ IResources* res,
    /* [in] */ const String& filepath)
    : mTargetDensity(0)
    , mApplyGravity(FALSE)
    , mMutated(FALSE)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    CRect::New((IRect**)&mDstRect);
    Init(res, filepath);
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ IInputStream* is)
    : mTargetDensity(0)
    , mApplyGravity(FALSE)
    , mMutated(FALSE)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    CRect::New((IRect**)&mDstRect);
    Init(is);
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ IResources* res,
    /* [in] */ IInputStream* is)
    : mTargetDensity(0)
    , mApplyGravity(FALSE)
    , mMutated(FALSE)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    CRect::New((IRect**)&mDstRect);
    Init(res, is);
}

AutoPtr<IPaint> BitmapDrawable::GetPaint()
{
    return mBitmapState->mPaint;
}

AutoPtr<IBitmap> BitmapDrawable::GetBitmap()
{
    return mBitmap;
}

void BitmapDrawable::ComputeBitmapSize()
{
    mBitmap->GetScaledWidthEx2(mTargetDensity, &mBitmapWidth);
    mBitmap->GetScaledHeightEx2(mTargetDensity, &mBitmapHeight);
}

void BitmapDrawable::SetBitmap(
    /* [in] */ IBitmap* bitmap)
{
    if (bitmap != mBitmap.Get()) {
        mBitmap = bitmap;
        if (bitmap != NULL) {
            ComputeBitmapSize();
        }
        else {
            mBitmapWidth = mBitmapHeight = -1;
        }
        InvalidateSelf();
    }
}

ECode BitmapDrawable::SetTargetDensity(
    /* [in] */ ICanvas* canvas)
{
    Int32 density;
    canvas->GetDensity(&density);
    return SetTargetDensity(density);
}

ECode BitmapDrawable::SetTargetDensity(
    /* [in] */ IDisplayMetrics* metrics)
{
    Int32 dpi;
    metrics->GetDensityDpi(&dpi);
    return SetTargetDensity(dpi);
}

ECode BitmapDrawable::SetTargetDensity(
    /* [in] */ Int32 density)
{
    if (mTargetDensity != density) {
        mTargetDensity = density == 0 ? IDisplayMetrics::DENSITY_DEFAULT : density;
        if (mBitmap != NULL) {
            ComputeBitmapSize();
        }
        InvalidateSelf();
    }
    return NOERROR;
}

Int32 BitmapDrawable::GetGravity()
{
    return mBitmapState->mGravity;
}

ECode BitmapDrawable::SetGravity(
    /* [in] */ Int32 gravity)
{
    if (mBitmapState->mGravity != gravity) {
        mBitmapState->mGravity = gravity;
        mApplyGravity = TRUE;
        InvalidateSelf();
    }
    return NOERROR;
}

ECode BitmapDrawable::SetAntiAlias(
    /* [in] */ Boolean aa)
{
    mBitmapState->mPaint->SetAntiAlias(aa);
    return InvalidateSelf();
}

ECode BitmapDrawable::SetFilterBitmap(
    /* [in] */ Boolean filter)
{
    mBitmapState->mPaint->SetFilterBitmap(filter);
    return InvalidateSelf();
}

ECode BitmapDrawable::SetDither(
    /* [in] */ Boolean dither)
{
    mBitmapState->mPaint->SetDither(dither);
    return InvalidateSelf();
}

ShaderTileMode BitmapDrawable::GetTileModeX()
{
    return mBitmapState->mTileModeX;
}

ShaderTileMode BitmapDrawable::GetTileModeY()
{
    return mBitmapState->mTileModeY;
}

ECode BitmapDrawable::SetTileModeX(
    /* [in] */ ShaderTileMode mode)
{
    return SetTileModeXY(mode, mBitmapState->mTileModeY);
}

ECode BitmapDrawable::SetTileModeY(
    /* [in] */ ShaderTileMode mode)
{
    return SetTileModeXY(mBitmapState->mTileModeX, mode);
}

ECode BitmapDrawable::SetTileModeXY(
    /* [in] */ ShaderTileMode xmode,
    /* [in] */ ShaderTileMode ymode)
{
    BitmapState* state = mBitmapState;
    if (state->mTileModeX != xmode || state->mTileModeY != ymode) {
        state->mTileModeX = xmode;
        state->mTileModeY = ymode;
        state->mRebuildShader = TRUE;
        InvalidateSelf();
    }
    return NOERROR;
}

Int32 BitmapDrawable::GetChangingConfigurations()
{
    return Drawable::GetChangingConfigurations()
        | mBitmapState->mChangingConfigurations;
}

void BitmapDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    Drawable::OnBoundsChange(bounds);
    mApplyGravity = TRUE;
}

ECode BitmapDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IBitmap> bitmap = mBitmap;
    if (bitmap != NULL) {
        BitmapState* state = mBitmapState;
        assert(state != NULL);
        if (state->mRebuildShader) {
            ShaderTileMode tmx = state->mTileModeX;
            ShaderTileMode tmy = state->mTileModeY;

            if (tmx == -1 && tmy == -1) {
                state->mPaint->SetShader(NULL);
            }
            else {
                AutoPtr<IBitmapShader> s;
                CBitmapShader::New(bitmap,
                        tmx == -1 ? ShaderTileMode_CLAMP : tmx,
                        tmy == -1 ? ShaderTileMode_CLAMP : tmy, (IBitmapShader**)&s);
                state->mPaint->SetShader(s);
            }
            state->mRebuildShader = FALSE;
            CopyBounds(mDstRect);
        }

        AutoPtr<IShader> shader;
        state->mPaint->GetShader((IShader**)&shader);
        if (shader == NULL) {
            if (mApplyGravity) {
                Int32 layoutDirection = GetLayoutDirection();
                AutoPtr<IGravity> gravity;
                CGravity::AcquireSingleton((IGravity**)&gravity);
                gravity->ApplyEx(state->mGravity, mBitmapWidth, mBitmapHeight,
                        GetBounds(), mDstRect, layoutDirection);
                mApplyGravity = FALSE;
            }
            canvas->DrawBitmapEx2(bitmap, NULL, mDstRect, state->mPaint);
        }
        else {
            if (mApplyGravity) {
                CopyBounds(mDstRect);
                mApplyGravity = FALSE;
            }
            canvas->DrawRectEx(mDstRect, state->mPaint);
        }
    }
    return NOERROR;
}

ECode BitmapDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    Int32 oldAlpha = 0;
    mBitmapState->mPaint->GetAlpha(&oldAlpha);
    if (alpha != oldAlpha) {
        mBitmapState->mPaint->SetAlpha(alpha);
        InvalidateSelf();
    }
    return NOERROR;
}

ECode BitmapDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    mBitmapState->mPaint->SetColorFilter(cf);
    return InvalidateSelf();
}

AutoPtr<IDrawable> BitmapDrawable::Mutate()
{
    if (!mMutated && Drawable::Mutate().Get()
        == (IDrawable*)this->Probe(EIID_IDrawable)) {
        mBitmapState = new BitmapState(mBitmapState);
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode BitmapDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Drawable::Inflate(r, parser, attrs));

    Int32 size = ARRAY_SIZE(R::styleable::BitmapDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::BitmapDrawable, size);

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(r->ObtainAttributes(attrs, layout, (ITypedArray**)&a));

    Int32 id;
    a->GetResourceId(R::styleable::BitmapDrawable_src, 0, &id);
    if (id == 0) {
        String str;
        parser->GetPositionDescription(&str);
        Logger::E("BitmapDrawable", "%s : <bitmap> requires a valid src attribute", str.string());
        return E_XML_PULL_PARSER_EXCEPTION;
    }
    AutoPtr<IBitmapFactory> bmFactory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bmFactory);
    AutoPtr<IBitmap> bitmap;
    bmFactory->DecodeResourceEx(r, id, (IBitmap**)&bitmap);
    if (bitmap == NULL) {
        String str;
        parser->GetPositionDescription(&str);
        Logger::E("BitmapDrawable", "%s : <bitmap> requires a valid src attribute", str.string());
        return E_XML_PULL_PARSER_EXCEPTION;
    }
    mBitmapState->mBitmap = bitmap;
    SetBitmap(bitmap);
    AutoPtr<IDisplayMetrics> metrics;
    r->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    SetTargetDensity(metrics);

    AutoPtr<IPaint> paint = mBitmapState->mPaint;
    Boolean newValue, oldValue;
    paint->IsAntiAlias(&oldValue);
    a->GetBoolean(R::styleable::BitmapDrawable_antialias, oldValue, &newValue);
    paint->SetAntiAlias(newValue);

    paint->IsFilterBitmap(&oldValue);
    a->GetBoolean(R::styleable::BitmapDrawable_filter, oldValue, &newValue);
    paint->SetFilterBitmap(newValue);

    paint->IsDither(&oldValue);
    a->GetBoolean(R::styleable::BitmapDrawable_dither, oldValue, &newValue);
    paint->SetDither(newValue);

    Int32 gravity;
    a->GetInt32(R::styleable::BitmapDrawable_gravity, IGravity::FILL, &gravity);
    SetGravity(gravity);

    Int32 tileMode;
    a->GetInt32(R::styleable::BitmapDrawable_tileMode, -1, &tileMode);
    if (tileMode != -1) {
        switch (tileMode) {
        case 0:
            SetTileModeXY(ShaderTileMode_CLAMP, ShaderTileMode_CLAMP);
            break;
        case 1:
            SetTileModeXY(ShaderTileMode_REPEAT, ShaderTileMode_REPEAT);
            break;
        case 2:
            SetTileModeXY(ShaderTileMode_MIRROR, ShaderTileMode_MIRROR);
            break;
        }
    }

    return a->Recycle();
}

Int32 BitmapDrawable::GetIntrinsicWidth()
{
    return mBitmapWidth;
}

Int32 BitmapDrawable::GetIntrinsicHeight()
{
    return mBitmapHeight;
}

Int32 BitmapDrawable::GetOpacity()
{
    if (mBitmapState->mGravity != IGravity::FILL) {
        return IPixelFormat::TRANSLUCENT;
    }

    Boolean bmHasAlpha = FALSE;
    if (mBitmap != NULL) {
        mBitmap->HasAlpha(&bmHasAlpha);
    }

    Int32 alpha;
    mBitmapState->mPaint->GetAlpha(&alpha);

    return (bmHasAlpha || alpha < 255) ?
            IPixelFormat::TRANSLUCENT : IPixelFormat::OPAQUE;
}

//@Override
AutoPtr<IDrawableConstantState> BitmapDrawable::GetConstantState()
{
    mBitmapState->mChangingConfigurations = GetChangingConfigurations();
    return mBitmapState;
}

ECode BitmapDrawable::Init(
    /* [in] */ BitmapState* state,
    /* [in] */ IResources* res)
{
    mBitmapState = state;
    if (res != NULL) {
        AutoPtr<IDisplayMetrics> metrics;
        res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
        metrics->GetDensityDpi(&mTargetDensity);
    }
    else {
        mTargetDensity = state->mTargetDensity;
    }
    SetBitmap(state != NULL ? state->mBitmap : NULL);
    return NOERROR;
}

ECode BitmapDrawable::Init()
{
    mBitmapState = new BitmapState((IBitmap*)NULL);
    return NOERROR;
}

ECode BitmapDrawable::Init(
    /* [in] */ IResources* res)
{
    mBitmapState = new BitmapState((IBitmap*)NULL);
    mBitmapState->mTargetDensity = mTargetDensity;
    return NOERROR;
}

ECode BitmapDrawable::Init(
    /* [in] */ IBitmap* bitmap)
{
    AutoPtr<BitmapState> state = new BitmapState(bitmap);
    return Init(state, NULL);
}

ECode BitmapDrawable::Init(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap)
{
    AutoPtr<BitmapState> state = new BitmapState(bitmap);
    Init(state, res);
    mBitmapState->mTargetDensity = mTargetDensity;
    return NOERROR;
}

ECode BitmapDrawable::Init(
    /* [in] */ const String& filepath)
{
    AutoPtr<IBitmapFactory> factory;
    AutoPtr<IBitmap> bitmap;
    FAIL_RETURN(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));
    FAIL_RETURN(factory->DecodeFileEx(filepath, (IBitmap**)&bitmap));

    AutoPtr<BitmapState> state = new BitmapState(bitmap);
    Init(state, NULL);
    if (mBitmap == NULL) {
        Logger::W("BitmapDrawable", "BitmapDrawable cannot decode %s", filepath.string());
    }
    return NOERROR;
}

ECode BitmapDrawable::Init(
    /* [in] */ IResources* res,
    /* [in] */ const String& filepath)
{
    AutoPtr<IBitmapFactory> factory;
    AutoPtr<IBitmap> bitmap;
    FAIL_RETURN(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));
    FAIL_RETURN(factory->DecodeFileEx(filepath, (IBitmap**)&bitmap));

    AutoPtr<BitmapState> state = new BitmapState(bitmap);
    Init(state, NULL);
    mBitmapState->mTargetDensity = mTargetDensity;
    if (mBitmap == NULL) {
        Logger::W("BitmapDrawable", "BitmapDrawable cannot decode %s", filepath.string());
    }
    return NOERROR;
}

ECode BitmapDrawable::Init(
    /* [in] */ IInputStream* is)
{
    AutoPtr<IBitmapFactory> factory;
    AutoPtr<IBitmap> bitmap;
    FAIL_RETURN(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));
    FAIL_RETURN(factory->DecodeStreamEx(is, (IBitmap**)&bitmap));

    AutoPtr<BitmapState> state = new BitmapState(bitmap);
    Init(state, NULL);
    if (mBitmap == NULL) {
        Logger::W("BitmapDrawable", "BitmapDrawable cannot decode"/*is*/);
    }
    return NOERROR;
}

ECode BitmapDrawable::Init(
    /* [in] */ IResources* res,
    /* [in] */ IInputStream* is)
{
    AutoPtr<IBitmapFactory> factory;
    AutoPtr<IBitmap> bitmap;
    FAIL_RETURN(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));
    FAIL_RETURN(factory->DecodeStreamEx(is, (IBitmap**)&bitmap));

    AutoPtr<BitmapState> state = new BitmapState(bitmap);
    Init(state, NULL);
    mBitmapState->mTargetDensity = mTargetDensity;
    if (mBitmap == NULL) {
        Logger::W("BitmapDrawable", "BitmapDrawable cannot decode"/*is*/);
    }
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
