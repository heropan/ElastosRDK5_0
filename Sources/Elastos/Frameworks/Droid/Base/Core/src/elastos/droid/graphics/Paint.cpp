
#include "elastos/droid/graphics/Paint.h"
#include "elastos/droid/graphics/CPath.h"
#include "elastos/droid/graphics/CPaintFontMetrics.h"
#include "elastos/droid/graphics/CPaintFontMetricsInt.h"
#include "elastos/droid/graphics/Shader.h"
#include "elastos/droid/graphics/ColorFilter.h"
#include "elastos/droid/graphics/Xfermode.h"
#include "elastos/droid/graphics/PathEffect.h"
#include "elastos/droid/graphics/MaskFilter.h"
#include "elastos/droid/graphics/Typeface.h"
#include "elastos/droid/graphics/Rasterizer.h"
// #include "elastos/droid/graphics/TemporaryBuffer.h"
// #include "elastos/droid/graphics/TextLayout.h"
// #include "elastos/droid/graphics/TextLayoutCache.h"
#include "elastos/droid/graphics/GraphicsNative.h"
#include "elastos/droid/text/TextUtils.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>
#include <Elastos.CoreLibrary.h>
#include <skia/effects/SkBlurDrawLooper.h>
#include <skia/core/SkPaint.h>
#include <unicode/uloc.h>
#include <elastos/core/Character.h>

using Elastos::Core::Character;
using Elastos::Core::IString;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;

using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::ISpannedString;
using Elastos::Droid::Text::ISpannableString;
using Elastos::Droid::Text::IGraphicsOperations;

namespace Elastos {
namespace Droid {
namespace Graphics {

static const String TAG("Paint");

// {4407AE9E-A61A-4FE3-8F99-12BFFDD68E56}
extern const InterfaceID EIID_Paint =
    { 0x4407ae9e, 0xa61a, 0x4fe3, { 0x8f, 0x99, 0x12, 0xbf, 0xfd, 0xd6, 0x8e, 0x56 } };

static void DefaultSettingsForElastos(SkPaint* paint)
{
    // GlyphID encoding is required because we are using Harfbuzz shaping
    paint->setTextEncoding(SkPaint::kGlyphID_TextEncoding);
}

const PaintStyle Paint::sStyleArray[] = {
    PaintStyle_FILL, PaintStyle_STROKE, PaintStyle_FILL_AND_STROKE
};
const PaintCap Paint::sCapArray[] = {
    PaintCap_BUTT, PaintCap_ROUND, PaintCap_SQUARE
};
const PaintJoin Paint::sJoinArray[] = {
    PaintJoin_MITER, PaintJoin_ROUND, PaintJoin_BEVEL
};
const PaintAlign Paint::sAlignArray[] = {
    PaintAlign_LEFT, PaintAlign_CENTER, PaintAlign_RIGHT
};

AutoPtr<ILocale> Paint::GetDefaultLocale()
{
    AutoPtr<ILocaleHelper> helper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
    AutoPtr<ILocale> locale;
    helper->GetDefault((ILocale**)&locale);
    return locale;
}

const Int32 Paint::DEFAULT_PAINT_FLAGS = IPaint::DEV_KERN_TEXT_FLAG | IPaint::EMBEDDED_BITMAP_TEXT_FLAG;
const Int32 Paint::BIDI_MAX_FLAG_VALUE = IPaint::BIDI_FORCE_RTL;
const Int32 Paint::BIDI_FLAG_MASK = 0x7;
const Int32 Paint::CURSOR_OPT_MAX_VALUE = IPaint::CURSOR_AT;

CAR_INTERFACE_IMPL(Paint, Object, IPaint);
Paint::Paint()
    : mNativePaint(0)
    , mNativeTypeface(0)
    , mBidiFlags(IPaint::BIDI_DEFAULT_LTR)
    , mHasCompatScaling(FALSE)
    , mCompatScaling(0)
    , mInvCompatScaling(0)
{}

Paint::~Paint()
{
    NativeFinalizer(mNativePaint);
}

/**
 * Create a new paint with default settings.
 */
ECode Paint::constructor()
{
    return constructor(0);
}

/**
 * Create a new paint with the specified flags. Use setFlags() to change
 * these after the paint is created.
 *
 * @param flags initial flag bits, as if they were passed via setFlags().
 */
ECode Paint::constructor(
    /* [in] */ Int32 flags)
{
    mNativePaint = NativeInit();
    SetFlags(flags | DEFAULT_PAINT_FLAGS);
    // TODO: Turning off hinting has undesirable side effects, we need to
    //       revisit hinting once we add support for subpixel positioning
    // setHinting(DisplayMetrics.DENSITY_DEVICE >= DisplayMetrics.DENSITY_TV
    //        ? HINTING_OFF : HINTING_ON);
    mCompatScaling = mInvCompatScaling = 1;
    AutoPtr<ILocale> locale = GetDefaultLocale();
    SetTextLocale(locale);
    return NOERROR;
}

ECode Paint::constructor(
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    Paint* paint_ = (Paint*)(IPaint*)paint->Probe(EIID_Paint);
    mNativePaint = NativeInitWithPaint(paint_->mNativePaint);
    SetClassVariablesFrom(paint_);
    return NOERROR;
}

/** Restores the paint to its default settings. */
ECode Paint::Reset()
{
    NativeReset(mNativePaint);
    SetFlags(DEFAULT_PAINT_FLAGS);

    // TODO: Turning off hinting has undesirable side effects, we need to
    //       revisit hinting once we add support for subpixel positioning
    // setHinting(DisplayMetrics.DENSITY_DEVICE >= DisplayMetrics.DENSITY_TV
    //        ? HINTING_OFF : HINTING_ON);

    mColorFilter = NULL;
    mMaskFilter = NULL;
    mPathEffect = NULL;
    mRasterizer = NULL;
    mShader = NULL;
    mTypeface = NULL;
    mNativeTypeface = 0;
    mXfermode = NULL;

    mHasCompatScaling = FALSE;
    mCompatScaling = 1;

    mInvCompatScaling = 1;

    mBidiFlags = IPaint::BIDI_DEFAULT_LTR;
    AutoPtr<ILocale> locale = GetDefaultLocale();
    SetTextLocale(locale);
    SetElegantTextHeight(FALSE);
    mFontFeatureSettings = NULL;
    return NOERROR;
}

/**
 * Copy the fields from src into this paint. This is equivalent to calling
 * get() on all of the src fields, and calling the corresponding set()
 * methods on this.
 */
ECode Paint::Set(
    /* [in] */ IPaint* src)
{
    assert(src != NULL);
    Paint* src_ = (Paint*)(IPaint*)src->Probe(EIID_Paint);
    if (this != src_) {
        assert(src_ != NULL);
        // copy over the native settings
        NativeSet(mNativePaint, src_->mNativePaint);
        SetClassVariablesFrom(src_);
    }
    return NOERROR;
}

void Paint::SetClassVariablesFrom(
    /* [in] */ Paint* paint)
{
    mColorFilter = paint->mColorFilter;
    mMaskFilter = paint->mMaskFilter;
    mPathEffect = paint->mPathEffect;
    mRasterizer = paint->mRasterizer;
    mShader = paint->mShader;

    if (paint->mShader != NULL) {
        mShader = ((Shader*)paint->mShader.Get())->Copy();
    } else {
        mShader = NULL;
    }

    mTypeface = paint->mTypeface;
    mNativeTypeface = paint->mNativeTypeface;
    mXfermode = paint->mXfermode;

    mHasCompatScaling = paint->mHasCompatScaling;
    mCompatScaling = paint->mCompatScaling;
    mInvCompatScaling = paint->mInvCompatScaling;

    mBidiFlags = paint->mBidiFlags;
    mLocale = paint->mLocale;
    mFontFeatureSettings = paint->mFontFeatureSettings;
}

/** @hide */
ECode Paint::SetCompatibilityScaling(
    /* [in] */ Float factor)
{
    if (factor == 1.0) {
        mHasCompatScaling = FALSE;
        mCompatScaling = mInvCompatScaling = 1.0f;
    }
    else {
        mHasCompatScaling = TRUE;
        mCompatScaling = factor;
        mInvCompatScaling = 1.0f/factor;
    }
    return NOERROR;
}

ECode Paint::GetBidiFlags(
    /* [out] */ Int32* flags)
{
    *flags = mBidiFlags;
    return NOERROR;
}

ECode Paint::SetBidiFlags(
    /* [in] */ Int32 flags)
{
    // only flag value is the 3-bit BIDI control setting
    flags &= BIDI_FLAG_MASK;
    if (flags > BIDI_MAX_FLAG_VALUE) {
        // throw new IllegalArgumentException("unknown bidi flag: " + flags);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mBidiFlags = flags;
    return NOERROR;
}


 /**
 * Return the paint's flags. Use the Flag enum to test flag values.
 *
 * @return the paint's flags (see enums ending in _Flag for bit masks)
 */
ECode Paint::GetFlags(
    /* [out] */ Int32* flags)
{
    *flags = ((SkPaint*)mNativePaint)->getFlags();
    return NOERROR;
}

/**
 * Return the paint's flags. Use the Flag enum to test flag values.
 *
 * @return the paint's flags (see enums ending in _Flag for bit masks)
 */
ECode Paint::SetFlags(
    /* [in] */ Int32 flags)
{
    ((SkPaint*)mNativePaint)->setFlags(flags);
    return NOERROR;
}

ECode Paint::GetHinting(
    /* [out] */ Int32* mode)
{
    *mode = ((SkPaint*)mNativePaint)->getHinting() == SkPaint::kNo_Hinting ? 0 : 1;
    return NOERROR;
}

ECode Paint::SetHinting(
    /* [in] */ Int32 mode)
{
    ((SkPaint*)mNativePaint)->setFlags(mode == 0 ? SkPaint::kNo_Hinting : SkPaint::kSlight_Hinting);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if ANTI_ALIAS_FLAG bit is set
 * AntiAliasing smooths out the edges of what is being drawn, but is has
 * no impact on the interior of the shape. See setDither() and
 * setFilterBitmap() to affect how colors are treated.
 *
 * @return true if the antialias bit is set in the paint's flags.
 */
ECode Paint::IsAntiAlias(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::ANTI_ALIAS_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the ANTI_ALIAS_FLAG bit
 * AntiAliasing smooths out the edges of what is being drawn, but is has
 * no impact on the interior of the shape. See setDither() and
 * setFilterBitmap() to affect how colors are treated.
 *
 * @param aa true to set the antialias bit in the flags, false to clear it
 */
ECode Paint::SetAntiAlias(
    /* [in] */ Boolean aa)
{
    ((SkPaint*)mNativePaint)->setAntiAlias(aa);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if DITHER_FLAG bit is set
 * Dithering affects how colors that are higher precision than the device
 * are down-sampled. No dithering is generally faster, but higher precision
 * colors are just truncated down (e.g. 8888 -> 565). Dithering tries to
 * distribute the error inherent in this process, to reduce the visual
 * artifacts.
 *
 * @return true if the dithering bit is set in the paint's flags.
 */
ECode Paint::IsDither(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::DITHER_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the DITHER_FLAG bit
 * Dithering affects how colors that are higher precision than the device
 * are down-sampled. No dithering is generally faster, but higher precision
 * colors are just truncated down (e.g. 8888 -> 565). Dithering tries to
 * distribute the error inherent in this process, to reduce the visual
 * artifacts.
 *
 * @param dither true to set the dithering bit in flags, false to clear it
 */
ECode Paint::SetDither(
    /* [in] */ Boolean dither)
{
    ((SkPaint*)mNativePaint)->setDither(dither);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if LINEAR_TEXT_FLAG bit is set
 *
 * @return true if the lineartext bit is set in the paint's flags
 */
ECode Paint::IsLinearText(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::LINEAR_TEXT_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the LINEAR_TEXT_FLAG bit
 *
 * @param linearText true to set the linearText bit in the paint's flags,
 *                   false to clear it.
 */
ECode Paint::SetLinearText(
    /* [in] */ Boolean linearText)
{
    ((SkPaint*)mNativePaint)->setLinearText(linearText);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if SUBPIXEL_TEXT_FLAG bit is set
 *
 * @return true if the subpixel bit is set in the paint's flags
 */
ECode Paint::IsSubpixelText(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::SUBPIXEL_TEXT_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the SUBPIXEL_TEXT_FLAG bit
 *
 * @param subpixelText true to set the subpixelText bit in the paint's
 *                     flags, false to clear it.
 */
ECode Paint::SetSubpixelText(
    /* [in] */ Boolean subpixelText)
{
    ((SkPaint*)mNativePaint)->setSubpixelText(subpixelText);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if UNDERLINE_TEXT_FLAG bit is set
 *
 * @return true if the underlineText bit is set in the paint's flags.
 */
ECode Paint::IsUnderlineText(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::UNDERLINE_TEXT_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the UNDERLINE_TEXT_FLAG bit
 *
 * @param underlineText true to set the underlineText bit in the paint's
 *                      flags, false to clear it.
 */
ECode Paint::SetUnderlineText(
    /* [in] */ Boolean underlineText)
{
    ((SkPaint*)mNativePaint)->setUnderlineText(underlineText);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if STRIKE_THRU_TEXT_FLAG bit is set
 *
 * @return true if the strikeThruText bit is set in the paint's flags.
 */
ECode Paint::IsStrikeThruText(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::STRIKE_THRU_TEXT_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the STRIKE_THRU_TEXT_FLAG bit
 *
 * @param strikeThruText true to set the strikeThruText bit in the paint's
 *                       flags, false to clear it.
 */
ECode Paint::SetStrikeThruText(
    /* [in] */ Boolean strikeThruText)
{
    ((SkPaint*)mNativePaint)->setStrikeThruText(strikeThruText);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if FAKE_BOLD_TEXT_FLAG bit is set
 *
 * @return true if the fakeBoldText bit is set in the paint's flags.
 */
ECode Paint::IsFakeBoldText(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::FAKE_BOLD_TEXT_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the STRIKE_THRU_TEXT_FLAG bit
 *
 * @param fakeBoldText true to set the fakeBoldText bit in the paint's
 *                     flags, false to clear it.
 */
ECode Paint::SetFakeBoldText(
    /* [in] */ Boolean fakeBoldText)
{
    ((SkPaint*)mNativePaint)->setFakeBoldText(fakeBoldText);
    return NOERROR;
}

/**
 * Whether or not the bitmap filter is activated.
 * Filtering affects the sampling of bitmaps when they are transformed.
 * Filtering does not affect how the colors in the bitmap are converted into
 * device pixels. That is dependent on dithering and xfermodes.
 *
 * @see #setFilterBitmap(boolean) setFilterBitmap()
 */
ECode Paint::IsFilterBitmap(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::FILTER_BITMAP_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the FILTER_BITMAP_FLAG bit.
 * Filtering affects the sampling of bitmaps when they are transformed.
 * Filtering does not affect how the colors in the bitmap are converted into
 * device pixels. That is dependent on dithering and xfermodes.
 *
 * @param filter true to set the FILTER_BITMAP_FLAG bit in the paint's
 *               flags, false to clear it.
 */
ECode Paint::SetFilterBitmap(
    /* [in] */ Boolean filterBitmap)
{
    ((SkPaint*)mNativePaint)->setFilterBitmap(filterBitmap);
    return NOERROR;
}

/**
 * Return the paint's style, used for controlling how primitives'
 * geometries are interpreted (except for drawBitmap, which always assumes
 * FILL_STYLE).
 *
 * @return the paint's style setting (Fill, Stroke, StrokeAndFill)
 */
ECode Paint::GetStyle(
    /* [out] */ PaintStyle* style)
{
    *style = sStyleArray[NativeGetStyle(mNativePaint)];
    return NOERROR;
}

/**
 * Set the paint's style, used for controlling how primitives'
 * geometries are interpreted (except for drawBitmap, which always assumes
 * Fill).
 *
 * @param style The new style to set in the paint
 */
ECode Paint::SetStyle(
    /* [in] */ PaintStyle style)
{
    NativeSetStyle(mNativePaint, style);
    return NOERROR;
}

/**
 * Return the paint's color. Note that the color is a 32bit value
 * containing alpha as well as r,g,b. This 32bit value is not premultiplied,
 * meaning that its alpha can be any value, regardless of the values of
 * r,g,b. See the Color class for more details.
 *
 * @return the paint's color (and alpha).
 */
ECode Paint::GetColor(
    /* [out] */ Int32* color)
{
    *color = ((SkPaint*)mNativePaint)->getColor();
    return NOERROR;
}

/**
 * Set the paint's color. Note that the color is an int containing alpha
 * as well as r,g,b. This 32bit value is not premultiplied, meaning that
 * its alpha can be any value, regardless of the values of r,g,b.
 * See the Color class for more details.
 *
 * @param color The new color (including alpha) to set in the paint.
 */
ECode Paint::SetColor(
    /* [in] */ Int32 color)
{
    ((SkPaint*)mNativePaint)->setColor(color);
    return NOERROR;
}

/**
 * Helper to getColor() that just returns the color's alpha value. This is
 * the same as calling getColor() >>> 24. It always returns a value between
 * 0 (completely transparent) and 255 (completely opaque).
 *
 * @return the alpha component of the paint's color.
 */
ECode Paint::GetAlpha(
    /* [out] */ Int32* alpha)
{
    *alpha = ((SkPaint*)mNativePaint)->getAlpha();
    return NOERROR;
}

/**
 * Helper to setColor(), that only assigns the color's alpha value,
 * leaving its r,g,b values unchanged. Results are undefined if the alpha
 * value is outside of the range [0..255]
 *
 * @param a set the alpha component [0..255] of the paint's color.
 */
ECode Paint::SetAlpha(
    /* [in] */ Int32 a)
{
    ((SkPaint*)mNativePaint)->setAlpha(a);
    return NOERROR;
}

/**
 * Helper to setColor(), that takes a,r,g,b and constructs the color int
 *
 * @param a The new alpha component (0..255) of the paint's color.
 * @param r The new red component (0..255) of the paint's color.
 * @param g The new green component (0..255) of the paint's color.
 * @param b The new blue component (0..255) of the paint's color.
 */
ECode Paint::SetARGB(
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return SetColor((a << 24) | (r << 16) | (g << 8) | b);
}

/**
 * Return the width for stroking.
 * <p />
 * A value of 0 strokes in hairline mode.
 * Hairlines always draws a single pixel independent of the canva's matrix.
 *
 * @return the paint's stroke width, used whenever the paint's style is
 *         Stroke or StrokeAndFill.
 */
ECode Paint::GetStrokeWidth(
    /* [out] */ Float* width)
{
    *width = SkScalarToFloat(((SkPaint*)mNativePaint)->getStrokeWidth());
    return NOERROR;
}

/**
 * Set the width for stroking.
 * Pass 0 to stroke in hairline mode.
 * Hairlines always draws a single pixel independent of the canva's matrix.
 *
 * @param width set the paint's stroke width, used whenever the paint's
 *              style is Stroke or StrokeAndFill.
 */
ECode Paint::SetStrokeWidth(
    /* [in] */ Float width)
{
    assert(0 && "TODO: need jni codes.");
    return NOERROR;
}

/**
 * Return the paint's stroke miter value. Used to control the behavior
 * of miter joins when the joins angle is sharp.
 *
 * @return the paint's miter limit, used whenever the paint's style is
 *         Stroke or StrokeAndFill.
 */
ECode Paint::GetStrokeMiter(
    /* [out] */ Float* miter)
{
    *miter = SkScalarToFloat(((SkPaint*)mNativePaint)->getStrokeMiter());
    return NOERROR;
}

/**
 * Set the paint's stroke miter value. This is used to control the behavior
 * of miter joins when the joins angle is sharp. This value must be >= 0.
 *
 * @param miter set the miter limit on the paint, used whenever the paint's
 *              style is Stroke or StrokeAndFill.
 */
ECode Paint::SetStrokeMiter(
    /* [in] */ Float miter)
{
    assert(0 && "TODO: need jni codes.");
    return NOERROR;
}

/**
 * Return the paint's Cap, controlling how the start and end of stroked
 * lines and paths are treated.
 *
 * @return the line cap style for the paint, used whenever the paint's
 *         style is Stroke or StrokeAndFill.
 */
ECode Paint::GetStrokeCap(
    /* [out] */ PaintCap* cap)
{
    *cap = sCapArray[NativeGetStrokeCap(mNativePaint)];
    return NOERROR;
}

/**
 * Set the paint's Cap.
 *
 * @param cap set the paint's line cap style, used whenever the paint's
 *            style is Stroke or StrokeAndFill.
 */
ECode Paint::SetStrokeCap(
    /* [in] */ PaintCap cap)
{
    NativeSetStrokeCap(mNativePaint, cap);
    return NOERROR;
}

/**
 * Return the paint's stroke join type.
 *
 * @return the paint's Join.
 */
ECode Paint::GetStrokeJoin(
    /* [out] */ PaintJoin* join)
{
    *join = sJoinArray[NativeGetStrokeJoin(mNativePaint)];
    return NOERROR;
}

/**
 * Set the paint's Join.
 *
 * @param join set the paint's Join, used whenever the paint's style is
 *             Stroke or StrokeAndFill.
 */
ECode Paint::SetStrokeJoin(
    /* [in] */ PaintJoin join)
{
    NativeSetStrokeJoin(mNativePaint, join);
    return NOERROR;
}

/**
 * Applies any/all effects (patheffect, stroking) to src, returning the
 * result in dst. The result is that drawing src with this paint will be
 * the same as drawing dst with a default paint (at least from the
 * geometric perspective).
 *
 * @param src input path
 * @param dst output path (may be the same as src)
 * @return    true if the path should be filled, or false if it should be
 *                 drawn with a hairline (width == 0)
 */
ECode Paint::GetFillPath(
    /* [in] */ IPath* src,
    /* [in] */ IPath* dst,
    /* [out] */ Boolean* result)
{
    *result = NativeGetFillPath(mNativePaint, ((CPath*)src)->Ni(), ((CPath*)dst)->Ni());
    return NOERROR;
}

/**
 * Get the paint's shader object.
 *
 * @return the paint's shader (or null)
 */
ECode Paint::GetShader(
    /* [out] */ IShader** shader)
{
    *shader = mShader;
    REFCOUNT_ADD(*shader);
    return NOERROR;
}

/**
 * Set or clear the shader object.
 * <p />
 * Pass null to clear any previous shader.
 * As a convenience, the parameter passed is also returned.
 *
 * @param shader May be null. the new shader to be installed in the paint
 * @return       shader
 */
ECode Paint::SetShader(
    /* [in] */ IShader* shader)
{
    Int64 shaderNative = 0;
    if (shader != NULL) {
        Shader* s = (Shader*)(IShader*)shader->Probe(EIID_Shader);
        assert(s != NULL);
        shaderNative = s->mNativeInstance;
    }
    NativeSetShader(mNativePaint, shaderNative);
    mShader = shader;
    return NOERROR;
}

 /**
 * Get the paint's colorfilter (maybe be null).
 *
 * @return the paint's colorfilter (maybe be null)
 */
ECode Paint::GetColorFilter(
    /* [out] */ IColorFilter** filter)
{
    *filter = mColorFilter;
    REFCOUNT_ADD(*filter);
    return NOERROR;
}

/**
 * Set or clear the paint's colorfilter, returning the parameter.
 *
 * @param filter May be null. The new filter to be installed in the paint
 * @return       filter
 */
ECode Paint::SetColorFilter(
    /* [in] */ IColorFilter* filter)
{
    Int64 filterNative = 0;
    if (filter != NULL) {
        ColorFilter* cf = (ColorFilter*)(IColorFilter*)filter->Probe(EIID_ColorFilter);
        assert(cf != NULL);
        filterNative = cf->mNativeInstance;
    }
    NativeSetColorFilter(mNativePaint, filterNative);
    mColorFilter = filter;
    return NOERROR;
}

/**
 * Get the paint's xfermode object.
 *
 * @return the paint's xfermode (or null)
 */
ECode Paint::GetXfermode(
    /* [out] */ IXfermode** xfermode)
{
    *xfermode = mXfermode;
    REFCOUNT_ADD(*xfermode);
    return NOERROR;
}

/**
 * Set or clear the xfermode object.
 * <p />
 * Pass null to clear any previous xfermode.
 * As a convenience, the parameter passed is also returned.
 *
 * @param xfermode May be null. The xfermode to be installed in the paint
 * @return         xfermode
 */
ECode Paint::SetXfermode(
    /* [in] */ IXfermode* xfermode)
{
    Int64 xfermodeNative = 0;
    if (xfermode != NULL) {
        Xfermode* x = (Xfermode*)(IXfermode*)xfermode->Probe(EIID_Xfermode);
        assert(x != NULL);
        xfermodeNative = x->mNativeInstance;
    }
    NativeSetXfermode(mNativePaint, xfermodeNative);
    mXfermode = xfermode;
    return NOERROR;
}

/**
 * Get the paint's patheffect object.
 *
 * @return the paint's patheffect (or null)
 */
ECode Paint::GetPathEffect(
    /* [out] */ IPathEffect** effect)
{
    *effect = mPathEffect;
    REFCOUNT_ADD(*effect);
    return NOERROR;
}

/**
 * Set or clear the patheffect object.
 * <p />
 * Pass null to clear any previous patheffect.
 * As a convenience, the parameter passed is also returned.
 *
 * @param effect May be null. The patheffect to be installed in the paint
 * @return       effect
 */
ECode Paint::SetPathEffect(
    /* [in] */ IPathEffect* effect)
{
    Int64 effectNative = 0;
    if (effect != NULL) {
        PathEffect* pe = (PathEffect*)(IPathEffect*)effect->Probe(EIID_PathEffect);
        assert(pe != NULL);
        effectNative = pe->mNativeInstance;
    }
    NativeSetPathEffect(mNativePaint, effectNative);
    mPathEffect = effect;
    return NOERROR;
}

/**
 * Get the paint's maskfilter object.
 *
 * @return the paint's maskfilter (or null)
 */
ECode Paint::GetMaskFilter(
    /* [out] */ IMaskFilter** maskfilter)
{
    *maskfilter = mMaskFilter;
    REFCOUNT_ADD(*maskfilter)
    return NOERROR;
}

/**
 * Set or clear the maskfilter object.
 * <p />
 * Pass null to clear any previous maskfilter.
 * As a convenience, the parameter passed is also returned.
 *
 * @param maskfilter May be null. The maskfilter to be installed in the
 *                   paint
 * @return           maskfilter
 */
ECode Paint::SetMaskFilter(
    /* [in] */ IMaskFilter* maskfilter)
{
    Int64 maskfilterNative = 0;
    if (maskfilter != NULL) {
        MaskFilter* mf = (MaskFilter*)(IMaskFilter*)maskfilter->Probe(EIID_MaskFilter);
        assert(mf != NULL);
        maskfilterNative = mf->mNativeInstance;
    }
    NativeSetMaskFilter(mNativePaint, maskfilterNative);
    mMaskFilter = maskfilter;
    return NOERROR;
}

/**
 * Get the paint's typeface object.
 * <p />
 * The typeface object identifies which font to use when drawing or
 * measuring text.
 *
 * @return the paint's typeface (or null)
 */
ECode Paint::GetTypeface(
    /* [out] */ ITypeface** typeface)
{
    *typeface = mTypeface;
    REFCOUNT_ADD(*typeface);
    return NOERROR;
}

/**
 * Set or clear the typeface object.
 * <p />
 * Pass null to clear any previous typeface.
 * As a convenience, the parameter passed is also returned.
 *
 * @param typeface May be null. The typeface to be installed in the paint
 * @return         typeface
 */
ECode Paint::SetTypeface(
    /* [in] */ ITypeface* typeface)
{
    Int64 typefaceNative = 0;
    if (typeface != NULL) {
        Typeface* t = (Typeface*)(ITypeface*)typeface->Probe(EIID_Typeface);
        typefaceNative = t->mNativeInstance;
    }
    NativeSetTypeface(mNativePaint, typefaceNative);
    mTypeface = typeface;
    mNativeTypeface = typefaceNative;
    return NOERROR;
}

/**
 * Get the paint's rasterizer (or null).
 * <p />
 * The raster controls/modifies how paths/text are turned into alpha masks.
 *
 * @return         the paint's rasterizer (or null)
 */
ECode Paint::GetRasterizer(
    /* [out] */ IRasterizer** rasterizer)
{
    *rasterizer = mRasterizer;
    REFCOUNT_ADD(*rasterizer);
    return NOERROR;
}

/**
 * Set or clear the rasterizer object.
 * <p />
 * Pass null to clear any previous rasterizer.
 * As a convenience, the parameter passed is also returned.
 *
 * @param rasterizer May be null. The new rasterizer to be installed in
 *                   the paint.
 * @return           rasterizer
 */
ECode Paint::SetRasterizer(
    /* [in] */ IRasterizer* rasterizer)
{
    Int64 rasterizerNative = 0;
    if (rasterizer != NULL) {
        Rasterizer* r = (Rasterizer*)(IRasterizer*)rasterizer->Probe(EIID_Rasterizer);
        assert(r != NULL);
        rasterizerNative = r->mNativeInstance;
    }
    NativeSetRasterizer(mNativePaint, rasterizerNative);
    mRasterizer = rasterizer;
    return NOERROR;
}

/**
 * This draws a shadow layer below the main layer, with the specified
 * offset and color, and blur radius. If radius is 0, then the shadow
 * layer is removed.
 */
ECode Paint::SetShadowLayer(
    /* [in] */ Float radius,
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ Int32 shadowColor)
{
    NativeSetShadowLayer(mNativePaint, radius, dx, dy, shadowColor);
    return NOERROR;
}

/**
 * Temporary API to clear the shadow layer.
 */
ECode Paint::ClearShadowLayer()
{
    return SetShadowLayer(0, 0, 0, 0);
}

ECode Paint::HasShadowLayer(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    *has = NativeHasShadowLayer(mNativePaint);
    return NOERROR;
}

/**
 * Return the paint's Align value for drawing text. This controls how the
 * text is positioned relative to its origin. LEFT align means that all of
 * the text will be drawn to the right of its origin (i.e. the origin
 * specifieds the LEFT edge of the text) and so on.
 *
 * @return the paint's Align value for drawing text.
 */
ECode Paint::GetTextAlign(
    /* [out] */ PaintAlign* align)
{
    *align = sAlignArray[NativeGetTextAlign(mNativePaint)];
    return NOERROR;
}

/**
 * Set the paint's text alignment. This controls how the
 * text is positioned relative to its origin. LEFT align means that all of
 * the text will be drawn to the right of its origin (i.e. the origin
 * specifieds the LEFT edge of the text) and so on.
 *
 * @param align set the paint's Align value for drawing text.
 */
ECode Paint::SetTextAlign(
    /* [in] */ PaintAlign align)
{
    NativeSetTextAlign(mNativePaint, align);
    return NOERROR;
}

ECode Paint::GetTextLocale(
    /* [out] */ ILocale** locale)
{
    *locale = mLocale;
    REFCOUNT_ADD(*locale);
    return NOERROR;
}

ECode Paint::SetTextLocale(
    /* [in] */ ILocale* locale)
{
    if (locale == NULL) {
        Logger::E(TAG, "locale cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Boolean isEqual;
    if (locale->Equals(mLocale, &isEqual), isEqual) return NOERROR;
    mLocale = locale;
    String str;
    locale->ToString(&str);
    NativeSetTextLocale(mNativePaint, str);
    return NOERROR;
}

ECode Paint::IsElegantTextHeight(
    /* [out] */ Boolean* isElegantTextHeight)
{
    assert(0 && "TODO: need jni codes.");
    return NOERROR;
}

ECode Paint::SetElegantTextHeight(
    /* [in] */ Boolean elegant)
{
    assert(0 && "TODO: need jni codes.");
    return NOERROR;
}

/**
 * Return the paint's text size.
 *
 * @return the paint's text size.
 */
ECode Paint::GetTextSize(
    /* [out] */ Float* size)
{
    *size = SkScalarToFloat(((SkPaint*)mNativePaint)->getTextSize());
    return NOERROR;
}

/**
 * Set the paint's text size. This value must be > 0
 *
 * @param textSize set the paint's text size.
 */
ECode Paint::SetTextSize(
    /* [in] */ Float textSize)
{
    assert(0 && "TODO: need jni codes.");
    return NOERROR;
}

/**
 * Return the paint's horizontal scale factor for text. The default value
 * is 1.0.
 *
 * @return the paint's scale factor in X for drawing/measuring text
 */
ECode Paint::GetTextScaleX(
    /* [out] */ Float* scaleX)
{
    *scaleX = SkScalarToFloat(((SkPaint*)mNativePaint)->getTextScaleX());
    return NOERROR;
}

/**
 * Set the paint's horizontal scale factor for text. The default value
 * is 1.0. Values > 1.0 will stretch the text wider. Values < 1.0 will
 * stretch the text narrower.
 *
 * @param scaleX set the paint's scale in X for drawing/measuring text.
 */
ECode Paint::SetTextScaleX(
    /* [in] */ Float scaleX)
{
     assert(0 && "TODO: need jni codes.");
     return NOERROR;
}

/**
 * Return the paint's horizonstal skew factor for text. The default value
 * is 0.
 *
 * @return         the paint's skew factor in X for drawing text.
 */
ECode Paint::GetTextSkewX(
    /* [out] */ Float* skewX)
{
    *skewX = SkScalarToFloat(((SkPaint*)mNativePaint)->getTextSkewX());
    return NOERROR;
}

/**
 * Set the paint's horizontal skew factor for text. The default value
 * is 0. For approximating oblique text, use values around -0.25.
 *
 * @param skewX set the paint's skew factor in X for drawing text.
 */
ECode Paint::SetTextSkewX(
    /* [in] */ Float skewX)
{
    assert(0 && "TODO: need jni codes.");
    return NOERROR;
}

ECode Paint::GetLetterSpacing(
    /* [out] */ Float* spacing)
{
    VALIDATE_NOT_NULL(spacing);
    assert(0 && "TODO");
    // *spacing = native_getLetterSpacing(mNativePaint);
    return NOERROR;
}

ECode Paint::SetLetterSpacing(
    /* [in] */ Float letterSpacing)
{
    assert(0 && "TODO");
    // native_setLetterSpacing(mNativePaint, letterSpacing);
    return NOERROR;
}

ECode Paint::GetFontFeatureSettings(
    /* [out] */ String* settings)
{
    VALIDATE_NOT_NULL(settings);
    *settings = mFontFeatureSettings;
    return NOERROR;
}

ECode Paint::SetFontFeatureSettings(
    /* [in] */ const String& settings)
{
    assert(0 && "TODO");
    // if (settings != null && settings.equals("")) {
    //     settings = null;
    // }
    // if ((settings == null && mFontFeatureSettings == null)
    //         || (settings != null && settings.equals(mFontFeatureSettings))) {
    //     return;
    // }
    // mFontFeatureSettings = settings;
    // native_setFontFeatureSettings(mNativePaint, settings);
    return NOERROR;
}

/**
 * Return the distance above (negative) the baseline (ascent) based on the
 * current typeface and text size.
 *
 * @return the distance above (negative) the baseline (ascent) based on the
 *         current typeface and text size.
 */
ECode Paint::Ascent(
    /* [out] */ Float* distance)
{
    SkPaint::FontMetrics metrics;
    ((SkPaint*)mNativePaint)->getFontMetrics(&metrics);
    *distance = SkScalarToFloat(metrics.fAscent);
    return NOERROR;
}

/**
 * Return the distance below (positive) the baseline (descent) based on the
 * current typeface and text size.
 *
 * @return the distance below (positive) the baseline (descent) based on
 *         the current typeface and text size.
 */
ECode Paint::Descent(
    /* [out] */ Float* distance)
{
    SkPaint::FontMetrics metrics;
    ((SkPaint*)mNativePaint)->getFontMetrics(&metrics);
    *distance = SkScalarToFloat(metrics.fDescent);
    return NOERROR;
}

/**
 * Return the font's recommended interline spacing, given the Paint's
 * settings for typeface, textSize, etc. If metrics is not null, return the
 * fontmetric values in it.
 *
 * @param metrics If this object is not null, its fields are filled with
 *                the appropriate values given the paint's text attributes.
 * @return the font's recommended interline spacing.
 */
ECode Paint::GetFontMetrics(
    /* [in] */ IPaintFontMetrics* metrics,
    /* [out] */ Float* spacing)
{
    SkPaint::FontMetrics metrics_;
    SkScalar spacing_ = ((SkPaint*)mNativePaint)->getFontMetrics(&metrics_);

    if (metrics) {
        metrics->SetTop(SkScalarToFloat(metrics_.fTop));
        metrics->SetAscent(SkScalarToFloat(metrics_.fAscent));
        metrics->SetDescent(SkScalarToFloat(metrics_.fDescent));
        metrics->SetBottom(SkScalarToFloat(metrics_.fBottom));
        metrics->SetLeading(SkScalarToFloat(metrics_.fLeading));
    }
    *spacing = SkScalarToFloat(spacing_);
    return NOERROR;
}

/**
 * Allocates a new FontMetrics object, and then calls getFontMetrics(fm)
 * with it, returning the object.
 */
ECode Paint::GetFontMetrics(
    /* [out] */ IPaintFontMetrics** metrics)
{
    CPaintFontMetrics::New(metrics);
    Float spacing;
    return GetFontMetrics(*metrics, &spacing);
}

/**
 * Return the font's interline spacing, given the Paint's settings for
 * typeface, textSize, etc. If metrics is not null, return the fontmetric
 * values in it. Note: all values have been converted to integers from
 * floats, in such a way has to make the answers useful for both spacing
 * and clipping. If you want more control over the rounding, call
 * getFontMetrics().
 *
 * @return the font's interline spacing.
 */
ECode Paint::GetFontMetricsInt(
    /* [in] */ IPaintFontMetricsInt* fmi,
    /* [out] */ Int32* spacing)
{
    assert(0 && "TODO: need jni codes.");
    return NOERROR;
}

ECode Paint::GetFontMetricsInt(
    /* [out] */ IPaintFontMetricsInt** fmi)
{
    CPaintFontMetricsInt::New(fmi);
    Int32 spacing;
    return GetFontMetricsInt(*fmi, &spacing);
}

/**
 * Return the recommend line spacing based on the current typeface and
 * text size.
 *
 * @return  recommend line spacing based on the current typeface and
 *          text size.
 */
ECode Paint::GetFontSpacing(
    /* [out] */ Float* spacing)
{
    return GetFontMetrics(NULL, spacing);
}

/**
 * Return the width of the text.
 *
 * @param text  The text to measure. Cannot be null.
 * @param index The index of the first character to start measuring
 * @param count THe number of characters to measure, beginning with start
 * @return      The width of the text
 */
ECode Paint::MeasureText(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [out] */ Float* width)
{
    if ((index | count) < 0 || index + count > text->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (text->GetLength() == 0 || count == 0) {
        *width = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        assert(0 && "TODO");
        // *width = (float) Math.ceil(NativeMeasureText(text, index, count, mBidiFlags));
        return NOERROR;
    }
    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Float w = NativeMeasureText(text, index, count, mBidiFlags);
    SetTextSize(oldSize);
    assert(0 && "TODO");
    // *width = (float) Math.ceil(w * mInvCompatScaling);
    return NOERROR;
}

 /**
 * Return the width of the text.
 *
 * @param text  The text to measure. Cannot be null.
 * @param start The index of the first character to start measuring
 * @param end   1 beyond the index of the last character to measure
 * @return      The width of the text
 */
ECode Paint::MeasureText(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Float* width)
{
    if (text.IsNull()) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if ((start | end | (end - start) | (text.GetLength() - end)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (text.GetLength() == 0 || start == end) {
        *width = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        assert(0 && "TODO");
        // *width = (float) Math.ceil(NativeMeasureText(text, start, end, mBidiFlags));
        return NOERROR;
    }

    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Float w = NativeMeasureText(text, start, end, mBidiFlags);
    SetTextSize(oldSize);
    assert(0 && "TODO");
    // *width = (float) Math.ceil(w * mInvCompatScaling);
    return NOERROR;
}

/**
 * Return the width of the text. Cannot be null.
 *
 * @param text  The text to measure
 * @return      The width of the text
 */
ECode Paint::MeasureText(
    /* [in] */ const String& text,
    /* [out] */ Float* width)
{
    if (text.IsNull()) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (text.IsEmpty()) {
        *width = 0;
        return NOERROR;
    }

    if (!mHasCompatScaling) {
        assert(0 && "TODO");
        // *width = (float) Math.ceil(NativeMeasureText(text, mBidiFlags));
        return NOERROR;
    }
    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Float w = NativeMeasureText(text, mBidiFlags);
    SetTextSize(oldSize);
    assert(0 && "TODO");
    // *width = (float) Math.ceil(w * mInvCompatScaling);
    return NOERROR;
}

/**
 * Return the width of the text.
 *
 * @param text  The text to measure
 * @param start The index of the first character to start measuring
 * @param end   1 beyond the index of the last character to measure
 * @return      The width of the text
 */
ECode Paint::MeasureText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Float* width)
{
    if (text == NULL) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 length;
    text->GetLength(&length);
    if ((start | end | (end - start) | (length - end)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (length == 0 || start == end) {
        *width = 0;
        return NOERROR;
    }
    // if (text instanceof String) {
    //     return measureText((String)text, start, end);
    // }
    // if (text instanceof SpannedString ||
    //     text instanceof SpannableString) {
    //     return measureText(text.toString(), start, end);
    // }
    // if (text instanceof GraphicsOperations) {
    //     return ((GraphicsOperations)text).measureText(start, end, this);
    // }

    assert(0 && "TODO");
    // AutoPtr< ArrayOf<Char32> > buf = TemporaryBuffer::Obtain(end - start);
    // TextUtils::GetChars(text, start, end, buf, 0);
    // ECode ec = MeasureText(buf, 0, end - start, width);
    // TemporaryBuffer::Recycle(buf);
    // return ec;
    return NOERROR;
}

/**
 * Measure the text, stopping early if the measured width exceeds maxWidth.
 * Return the number of chars that were measured, and if measuredWidth is
 * not null, return in it the actual width measured.
 *
 * @param text  The text to measure. Cannot be null.
 * @param index The offset into text to begin measuring at
 * @param count The number of maximum number of entries to measure. If count
 *              is negative, then the characters are measured in reverse order.
 * @param maxWidth The maximum width to accumulate.
 * @param measuredWidth Optional. If not null, returns the actual width
 *                     measured.
 * @return The number of chars that were measured. Will always be <=
 *         abs(count).
 */
ECode Paint::BreakText(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [out] */ Int32* number)
{
    if (index < 0 || text->GetLength() - index < Elastos::Core::Math::Abs(count)) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (text->GetLength() == 0 || count == 0) {
        *number = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        *number = NativeBreakText(mNativePaint, mNativeTypeface, text, index, count, maxWidth, mBidiFlags, measuredWidth);
        return NOERROR;
    }

    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Int32 res = NativeBreakText(mNativePaint, mNativeTypeface, text, index, count, maxWidth * mCompatScaling, mBidiFlags,
            measuredWidth);
    SetTextSize(oldSize);
    if (measuredWidth != NULL) (*measuredWidth)[0] *= mInvCompatScaling;
    *number = res;
    return NOERROR;
}

/**
 * Measure the text, stopping early if the measured width exceeds maxWidth.
 * Return the number of chars that were measured, and if measuredWidth is
 * not null, return in it the actual width measured.
 *
 * @param text  The text to measure. Cannot be null.
 * @param start The offset into text to begin measuring at
 * @param end   The end of the text slice to measure.
 * @param measureForwards If true, measure forwards, starting at start.
 *                        Otherwise, measure backwards, starting with end.
 * @param maxWidth The maximum width to accumulate.
 * @param measuredWidth Optional. If not null, returns the actual width
 *                     measured.
 * @return The number of chars that were measured. Will always be <=
 *         abs(end - start).
 */
ECode Paint::BreakText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Boolean measureForwards,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [out] */ Int32* number)
{
    if (text == NULL) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 length;
    text->GetLength(&length);
    if ((start | end | (end - start) | (length - end)) < 0) {
        return E_RUNTIME_EXCEPTION;
    }

    if (length == 0 || start == end) {
        *number = 0;
        return NOERROR;
    }
    // if (start == 0 && text instanceof String && end == text.length()) {
    //     return breakText((String) text, measureForwards, maxWidth,
    //                      measuredWidth);
    // }

    assert(0 && "TODO");
    AutoPtr< ArrayOf<Char32> > buf/* = TemporaryBuffer::Obtain(end - start)*/;

    TextUtils::GetChars(text, start, end, buf, 0);

    ECode ec;
    if (measureForwards) {
        ec = BreakText(buf, 0, end - start, maxWidth, measuredWidth, number);
    }
    else {
        ec = BreakText(buf, 0, -(end - start), maxWidth, measuredWidth, number);
    }

    assert(0 && "TODO");
    // TemporaryBuffer::Recycle(buf);
    return ec;
}

/**
 * Measure the text, stopping early if the measured width exceeds maxWidth.
 * Return the number of chars that were measured, and if measuredWidth is
 * not null, return in it the actual width measured.
 *
 * @param text  The text to measure. Cannot be null.
 * @param measureForwards If true, measure forwards, starting with the
 *                        first character in the string. Otherwise,
 *                        measure backwards, starting with the
 *                        last character in the string.
 * @param maxWidth The maximum width to accumulate.
 * @param measuredWidth Optional. If not null, returns the actual width
 *                     measured.
 * @return The number of chars that were measured. Will always be <=
 *         abs(count).
 */
ECode Paint::BreakText(
    /* [in] */ const String& text,
    /* [in] */ Boolean measureForwards,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [out] */ Int32* number)
{
    if (text.IsNull()) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (text.IsEmpty()) {
        *number = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        *number = NativeBreakText(mNativePaint, mNativeTypeface, text, measureForwards, maxWidth, mBidiFlags, measuredWidth);
        return NOERROR;
    }

    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Int32 res = NativeBreakText(mNativePaint, mNativeTypeface, text, measureForwards, maxWidth * mCompatScaling,
            mBidiFlags, measuredWidth);
    SetTextSize(oldSize);
    if (measuredWidth != NULL) (*measuredWidth)[0] *= mInvCompatScaling;
    *number = res;
    return NOERROR;
}

/**
 * Return the advance widths for the characters in the string.
 *
 * @param text     The text to measure. Cannot be null.
 * @param index    The index of the first char to to measure
 * @param count    The number of chars starting with index to measure
 * @param widths   array to receive the advance widths of the characters.
 *                 Must be at least a large as count.
 * @return         the actual number of widths returned.
 */
ECode Paint::GetTextWidths(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* number)
{
    if ((index | count) < 0 || index + count > text->GetLength()
            || count > widths->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (text->GetLength() == 0 || count == 0) {
        *number = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        *number = NativeGetTextWidths(mNativePaint, mNativeTypeface, text, index, count, mBidiFlags, widths);
        return NOERROR;
    }

    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Int32 res = NativeGetTextWidths(mNativePaint, mNativeTypeface, text, index, count, mBidiFlags, widths);
    SetTextSize(oldSize);
    for (Int32 i = 0; i < res; i++) {
        (*widths)[i] *= mInvCompatScaling;
    }
    *number = res;
    return NOERROR;
}

/**
 * Return the advance widths for the characters in the string.
 *
 * @param text     The text to measure. Cannot be null.
 * @param start    The index of the first char to to measure
 * @param end      The end of the text slice to measure
 * @param widths   array to receive the advance widths of the characters.
 *                 Must be at least a large as (end - start).
 * @return         the actual number of widths returned.
 */
ECode Paint::GetTextWidths(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* number)
{
    if (text == NULL) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 length;
    text->GetLength(&length);
    if ((start | end | (end - start) | (length - end)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (end - start > widths->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (length == 0 || start == end) {
        *number = 0;
        return NOERROR;
    }
    // if (text instanceof String) {
    //     return getTextWidths((String) text, start, end, widths);
    // }
    // if (text instanceof SpannedString ||
    //     text instanceof SpannableString) {
    //     return getTextWidths(text.toString(), start, end, widths);
    // }
    // if (text instanceof GraphicsOperations) {
    //     return ((GraphicsOperations) text).getTextWidths(start, end,
    //                                                          widths, this);
    // }

    assert(0 && "TODO");
    // AutoPtr< ArrayOf<Char32> > buf = TemporaryBuffer::Obtain(end - start);
    // TextUtils::GetChars(text, start, end, buf, 0);
    // ECode ec = GetTextWidths(buf, 0, end - start, widths, number);
    // TemporaryBuffer::Recycle(buf);
    // return ec;
    return NOERROR;
}

/**
 * Return the advance widths for the characters in the string.
 *
 * @param text   The text to measure. Cannot be null.
 * @param start  The index of the first char to to measure
 * @param end    The end of the text slice to measure
 * @param widths array to receive the advance widths of the characters.
 *               Must be at least a large as the text.
 * @return       the number of unichars in the specified text.
 */
ECode Paint::GetTextWidths(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* number)
{
    if (text.IsNull()) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if ((start | end | (end - start) | (text.GetLength() - end)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (end - start > widths->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (text.GetLength() == 0 || start == end) {
        *number = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        *number = NativeGetTextWidths(mNativePaint, mNativeTypeface, text, start, end, mBidiFlags, widths);
        return NOERROR;
    }
    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Int32 res = NativeGetTextWidths(mNativePaint, mNativeTypeface, text, start, end, mBidiFlags, widths);
    SetTextSize(oldSize);
    for (Int32 i = 0; i < res; i++) {
        (*widths)[i] *= mInvCompatScaling;
    }
    *number = res;
    return NOERROR;
}

/**
 * Return the advance widths for the characters in the string.
 *
 * @param text   The text to measure
 * @param widths array to receive the advance widths of the characters.
 *               Must be at least a large as the text.
 * @return       the number of unichars in the specified text.
 */
ECode Paint::GetTextWidths(
    /* [in] */ const String& text,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* number)
{
    return GetTextWidths(text, 0, text.GetLength(), widths, number);
}

ECode Paint::GetTextRunAdvances(
    /* [in] */ ArrayOf<Char32>* chars,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextIndex,
    /* [in] */ Int32 contextCount,
    /* [in] */ Boolean isRtl,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [out] */ Float* advance)
{
    if ((index | count | contextIndex | contextCount | advancesIndex
            | (index - contextIndex) | (contextCount - count)
            | ((contextIndex + contextCount) - (index + count))
            | (chars->GetLength() - (contextIndex + contextCount))
            | (advances == NULL ? 0 :
                (advances->GetLength() - (advancesIndex + count)))) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (chars->GetLength() == 0 || count == 0){
        *advance = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        *advance = NativeGetTextRunAdvances(mNativePaint, mNativeTypeface, chars, index, count,
                contextIndex, contextCount, isRtl, advances, advancesIndex);
        return NOERROR;
    }

    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Float res = NativeGetTextRunAdvances(mNativePaint, mNativeTypeface, chars, index, count,
                contextIndex, contextCount, isRtl, advances, advancesIndex);
    SetTextSize(oldSize);

    if (advances != NULL) {
        for (Int32 i = advancesIndex, e = i + count; i < e; i++) {
            (*advances)[i] *= mInvCompatScaling;
        }
    }
    *advance = res * mInvCompatScaling; // assume errors are not significant
    return NOERROR;
}

ECode Paint::GetTextRunAdvances(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Boolean isRtl,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [out] */ Float* advance)
{
    if (text == NULL) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 length;
    text->GetLength(&length);
    if ((start | end | contextStart | contextEnd | advancesIndex | (end - start)
            | (start - contextStart) | (contextEnd - end)
            | (length - contextEnd)
            | (advances == NULL ? 0 :
                (advances->GetLength() - advancesIndex - (end - start)))) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

//  if (text instanceof String) {
//        return getTextRunAdvances((String) text, start, end,
//                contextStart, contextEnd, isRtl, advances, advancesIndex);
//    }
//    if (text instanceof SpannedString ||
//        text instanceof SpannableString) {
//        return getTextRunAdvances(text.toString(), start, end,
//                contextStart, contextEnd, isRtl, advances, advancesIndex);
//  }
//    if (text instanceof GraphicsOperations) {
//        return ((GraphicsOperations) text).getTextRunAdvances(start, end,
//                contextStart, contextEnd, flags, advances, advancesIndex, this);
//    }
    if (length == 0 || end == start) {
        *advance = 0;
        return NOERROR;
    }

    Int32 contextLen = contextEnd - contextStart;
    Int32 len = end - start;
    assert(0 && "TODO");
    // AutoPtr< ArrayOf<Char32> > buf = TemporaryBuffer::Obtain(contextLen);
    // TextUtils::GetChars(text, contextStart, contextEnd, buf, 0);
    // ECode ec = GetTextRunAdvances(buf, start - contextStart, len,
    //         0, contextLen, isRtl, advances, advancesIndex, advance);
    // TemporaryBuffer::Recycle(buf);
    // return ec;
    return NOERROR;
}

ECode Paint::GetTextRunAdvances(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Boolean isRtl,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [out] */ Float* advance)
{
    if (text.IsNull()) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if ((start | end | contextStart | contextEnd | advancesIndex | (end - start)
            | (start - contextStart) | (contextEnd - end)
            | (text.GetLength() - contextEnd)
            | (advances == NULL ? 0 :
                (advances->GetLength() - advancesIndex - (end - start)))) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (text.GetLength() == 0 || start == end) {
        *advance = 0;
        return NOERROR;
    }

    if (!mHasCompatScaling) {
        *advance = NativeGetTextRunAdvances(mNativePaint, mNativeTypeface, text, start, end,
                contextStart, contextEnd, isRtl, advances, advancesIndex);
        return NOERROR;
    }

    float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Float totalAdvance = NativeGetTextRunAdvances(mNativePaint, mNativeTypeface, text, start, end,
                contextStart, contextEnd, isRtl, advances, advancesIndex);
    SetTextSize(oldSize);

   if (advances != NULL) {
        for (Int32 i = advancesIndex, e = i + (end - start); i < e; i++) {
            (*advances)[i] *= mInvCompatScaling;
        }
    }

    *advance = totalAdvance * mInvCompatScaling; // assume errors are insignificant
    return NOERROR;
}

ECode Paint::GetTextRunCursor(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextLength,
    /* [in] */ Int32 dir,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt,
    /* [out] */ Int32* position)
{
    Int32 contextEnd = contextStart + contextLength;
    if (((contextStart | contextEnd | offset | (contextEnd - contextStart)
        | (offset - contextStart) | (contextEnd - offset)
        | (text->GetLength() - contextEnd) | cursorOpt) < 0)
        || cursorOpt > CURSOR_OPT_MAX_VALUE) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    *position = NativeGetTextRunCursor(mNativePaint, text,
        contextStart, contextLength, dir, offset, cursorOpt);
    return NOERROR;
}

ECode Paint::GetTextRunCursor(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 dir,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt,
    /* [out] */ Int32* position)
{
   if (IString::Probe(text) || ISpannedString::Probe(text) || ISpannableString::Probe(text)) {
        String str;
        text->ToString(&str);
       return GetTextRunCursor(str, contextStart, contextEnd,
               dir, offset, cursorOpt, position);
   }
   if (IGraphicsOperations::Probe(text)) {
       return IGraphicsOperations::Probe(text)->GetTextRunCursor(
               contextStart, contextEnd, dir, offset, cursorOpt, (IPaint*)this->Probe(EIID_Paint), position);
   }

    Int32 contextLen = contextEnd - contextStart;
    assert(0 && "TODO");
    // AutoPtr< ArrayOf<Char32> > buf = TemporaryBuffer::Obtain(contextLen);
    // TextUtils::GetChars(text, contextStart, contextEnd, buf, 0);
    // ECode ec = GetTextRunCursor(buf, 0, contextLen, dir, offset - contextStart, cursorOpt, position);
    // TemporaryBuffer::Recycle(buf);
    // return ec;
    return NOERROR;
}

ECode Paint::GetTextRunCursor(
    /* [in] */ const String& text,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 dir,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt,
    /* [out] */ Int32* position)
{
    if (((contextStart | contextEnd | offset | (contextEnd - contextStart)
            | (offset - contextStart) | (contextEnd - offset)
            | (text.GetLength() - contextEnd) | cursorOpt) < 0)
            || cursorOpt > CURSOR_OPT_MAX_VALUE) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    *position = NativeGetTextRunCursor(mNativePaint, text,
            contextStart, contextEnd, dir, offset, cursorOpt);
    return NOERROR;
}

/**
 * Return the path (outline) for the specified text.
 * Note: just like Canvas.drawText, this will respect the Align setting in
 * the paint.
 *
 * @param text     The text to retrieve the path from
 * @param index    The index of the first character in text
 * @param count    The number of characterss starting with index
 * @param x        The x coordinate of the text's origin
 * @param y        The y coordinate of the text's origin
 * @param path     The path to receive the data describing the text. Must
 *                 be allocated by the caller.
 */
ECode Paint::GetTextPath(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPath* path)
{
    if ((index | count) < 0 || index + count > text->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    NativeGetTextPath(mNativePaint, mNativeTypeface, mBidiFlags, text, index, count, x, y, ((CPath*)path)->Ni());
    return NOERROR;
}

/**
 * Return the path (outline) for the specified text.
 * Note: just like Canvas.drawText, this will respect the Align setting
 * in the paint.
 *
 * @param text  The text to retrieve the path from
 * @param start The first character in the text
 * @param end   1 past the last charcter in the text
 * @param x     The x coordinate of the text's origin
 * @param y     The y coordinate of the text's origin
 * @param path  The path to receive the data describing the text. Must
 *              be allocated by the caller.
 */
ECode Paint::GetTextPath(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPath* path)
{
    if ((start | end | (end - start) | (text.GetLength() - end)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    NativeGetTextPath(mNativePaint, mNativeTypeface, mBidiFlags, text, start, end, x, y, ((CPath*)path)->Ni());
    return NOERROR;
}

/**
 * Return in bounds (allocated by the caller) the smallest rectangle that
 * encloses all of the characters, with an implied origin at (0,0).
 *
 * @param text  String to measure and return its bounds
 * @param start Index of the first char in the string to measure
 * @param end   1 past the last char in the string measure
 * @param bounds Returns the unioned bounds of all the text. Must be
 *               allocated by the caller.
 */
ECode Paint::GetTextBounds(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IRect* bounds)
{
    if ((start | end | (end - start) | (text.GetLength() - end)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (bounds == NULL) {
//        throw new NullPointerException("need bounds Rect");
        return E_NULL_POINTER_EXCEPTION;
    }
    NativeGetStringBounds(mNativePaint, mNativeTypeface, text, start, end, mBidiFlags, bounds);
    return NOERROR;
}

/**
 * Return in bounds (allocated by the caller) the smallest rectangle that
 * encloses all of the characters, with an implied origin at (0,0).
 *
 * @param text  Array of chars to measure and return their unioned bounds
 * @param index Index of the first char in the array to measure
 * @param count The number of chars, beginning at index, to measure
 * @param bounds Returns the unioned bounds of all the text. Must be
 *               allocated by the caller.
 */
ECode Paint::GetTextBounds(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ IRect* bounds)
{
    if ((index | count) < 0 || index + count > text->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (bounds == NULL) {
//        throw new NullPointerException("need bounds Rect");
        return E_NULL_POINTER_EXCEPTION;
    }
    NativeGetCharArrayBounds(mNativePaint, mNativeTypeface, text, index, count, mBidiFlags, bounds);
    return NOERROR;
}

Int64 Paint::NativeInit()
{
    SkPaint* obj = new SkPaint();
    DefaultSettingsForElastos(obj);
    return (Int32)obj;
}

Int64 Paint::NativeInitWithPaint(
    /* [in] */ Int64 nObj)
{
    SkPaint* obj = new SkPaint(*(SkPaint*)nObj);
    return (Int64)obj;
}

void Paint::NativeReset(
    /* [in] */ Int64 nObj)
{
    ((SkPaint*)nObj)->reset();
    DefaultSettingsForElastos((SkPaint*)nObj);
}

void Paint::NativeSet(
    /* [in] */ Int64 nDst,
    /* [in] */ Int64 nSrc)
{
    *(SkPaint*)nDst = *(SkPaint*)nSrc;
}

Int32 Paint::NativeGetStyle(
    /* [in] */ Int64 nObj)
{
    return ((SkPaint*)nObj)->getStyle();
}

void Paint::NativeSetStyle(
    /* [in] */ Int64 nObj,
    /* [in] */ PaintStyle style)
{
    ((SkPaint*)nObj)->setStyle((SkPaint::Style)style);
}

Int32 Paint::NativeGetStrokeCap(
    /* [in] */ Int64 nObj)
{
    return ((SkPaint*)nObj)->getStrokeCap();
}

void Paint::NativeSetStrokeCap(
    /* [in] */ Int64 nObj,
    /* [in] */ PaintCap cap)
{
    ((SkPaint*)nObj)->setStrokeCap((SkPaint::Cap)cap);
}

Int32 Paint::NativeGetStrokeJoin(
    /* [in] */ Int64 nObj)
{
    return ((SkPaint*)nObj)->getStrokeJoin();
}

void Paint::NativeSetStrokeJoin(
    /* [in] */ Int64 nObj,
    /* [in] */ PaintJoin join)
{
    ((SkPaint*)nObj)->setStrokeJoin((SkPaint::Join)join);
}

Boolean Paint::NativeGetFillPath(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 src,
    /* [in] */ Int64 dst)
{
    return ((SkPaint*)nObj)->getFillPath(*(SkPath*)src, (SkPath*)dst);
}

Int64 Paint::NativeSetShader(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 shader)
{
    return (Int64)((SkPaint*)nObj)->setShader((SkShader*)shader);
}

Int64 Paint::NativeSetColorFilter(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 filter)
{
    return (Int32)((SkPaint*)nObj)->setColorFilter((SkColorFilter*)filter);
}

Int64 Paint::NativeSetXfermode(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 xfermode)
{
    return (Int64)((SkPaint*)nObj)->setXfermode((SkXfermode*)xfermode);
}

Int64 Paint::NativeSetPathEffect(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 effect)
{
    return (Int64)((SkPaint*)nObj)->setPathEffect((SkPathEffect*)effect);
}

Int64 Paint::NativeSetMaskFilter(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 maskfilter)
{
    return (Int32)((SkPaint*)nObj)->setMaskFilter((SkMaskFilter*)maskfilter);
}

Int64 Paint::NativeSetTypeface(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 typeface)
{
    return (Int64)((SkPaint*)nObj)->setTypeface((SkTypeface*)typeface);
}

Int64 Paint::NativeSetRasterizer(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 rasterizer)
{
    return (Int64)((SkPaint*)nObj)->setRasterizer((SkRasterizer*)rasterizer);
}

Int32 Paint::NativeGetTextAlign(
    /* [in] */ Int64 nObj)
{
    return ((SkPaint*)nObj)->getTextAlign();
}

void Paint::NativeSetTextAlign(
    /* [in] */ Int64 nObj,
    /* [in] */ PaintAlign align)
{
    ((SkPaint*)nObj)->setTextAlign((SkPaint::Align)align);
}

// generate bcp47 identifier for the supplied locale
static void ToLanguageTag(
    /* [in] */ char* output,
    /* [in] */ size_t outSize,
    /* [in] */ const char* locale)
{
    if (output == NULL || outSize <= 0) {
        return;
    }
    if (locale == NULL) {
        output[0] = '\0';
        return;
    }
    char canonicalChars[ULOC_FULLNAME_CAPACITY];
    UErrorCode uErr = U_ZERO_ERROR;
    uloc_canonicalize(locale, canonicalChars, ULOC_FULLNAME_CAPACITY,
            &uErr);
    if (U_SUCCESS(uErr)) {
        char likelyChars[ULOC_FULLNAME_CAPACITY];
        uErr = U_ZERO_ERROR;
        uloc_addLikelySubtags(canonicalChars, likelyChars,
                ULOC_FULLNAME_CAPACITY, &uErr);
        if (U_SUCCESS(uErr)) {
            uErr = U_ZERO_ERROR;
            uloc_toLanguageTag(likelyChars, output, outSize, FALSE, &uErr);
            if (U_SUCCESS(uErr)) {
                return;
            }
            else {
                // ALOGD("uloc_toLanguageTag(\"%s\") failed: %s", likelyChars,
                //         u_errorName(uErr));
            }
        }
        else {
            // ALOGD("uloc_addLikelySubtags(\"%s\") failed: %s",
            //         canonicalChars, u_errorName(uErr));
        }
    }
    else {
        // ALOGD("uloc_canonicalize(\"%s\") failed: %s", locale,
        //         u_errorName(uErr));
    }
    // unable to build a proper language identifier
    output[0] = '\0';
}

//anthony native, can't implement
void Paint::NativeSetTextLocale(
    /* [in] */ Int64 nObj,
    /* [in] */ const String& locale)
{
    assert(0 && "TODO: need jni codes.");
}

Float Paint::NativeMeasureText(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 bidiFlags)
{
    assert(0 && "TODO: need jni codes.");

    return 0.f;
}

Float Paint::NativeMeasureText(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 bidiFlags)
{
    assert(0 && "TODO: need jni codes.");

    return 0.f;
}

Float Paint::NativeMeasureText(
    /* [in] */ const String& text,
    /* [in] */ Int32 bidiFlags)
{
    assert(0 && "TODO: need jni codes.");

    return 0.f;
}

Int32 Paint::NativeBreakText(
    /* [in] */ Int64 native_object,
    /* [in] */ Int64 native_typeface,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float maxWidth,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ ArrayOf<Float>* measuredWidth)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

Int32 Paint::NativeBreakText(
    /* [in] */ Int64 native_object,
    /* [in] */ Int64 native_typeface,
    /* [in] */ const String& text,
    /* [in] */ Boolean measureForwards,
    /* [in] */ Float maxWidth,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ ArrayOf<Float>* measuredWidth)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

Int32 Paint::NativeGetTextWidths(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 native_typeface,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 bidiFlags,
    /* [out] */ ArrayOf<Float>* widths)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

Int32 Paint::NativeGetTextWidths(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 native_typeface,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 bidiFlags,
    /* [out] */ ArrayOf<Float>* widths)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

Int32 Paint::NativeGetTextGlyphs(
    /* [in] */ Int64 nObj,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [out] */ ArrayOf<Char32>* glyphs)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

Float Paint::NativeGetTextRunAdvances(
    /* [in] */ Int64 paint,
    /* [in] */ Int64 native_typeface,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextIndex,
    /* [in] */ Int32 contextCount,
    /* [in] */ Boolean isRtl,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex)
{
    assert(0 && "TODO: need jni codes.");
    return 0.f;
}

Float Paint::NativeGetTextRunAdvances(
    /* [in] */ Int64 paint,
    /* [in] */ Int64 native_typeface,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Boolean isRtl,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex)
{
    assert(0 && "TODO: need jni codes.");
    return 0.f;
}

enum MoveOpt {
    AFTER, AT_OR_AFTER, BEFORE, AT_OR_BEFORE, AT
};

Int32 Paint::NativeGetTextRunCursor(
    /* [in] */ Int64 paint,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextCount,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

Int32 Paint::NativeGetTextRunCursor(
    /* [in] */ Int64 paint,
    /* [in] */ const String& text,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

void Paint::NativeGetTextPath(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 native_typeface,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int64 path)
{
    assert(0 && "TODO: need jni codes.");
}

void Paint::NativeGetTextPath(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 native_typeface,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int64 path)
{
    assert(0 && "TODO: need jni codes.");
}

void Paint::NativeGetStringBounds(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 native_typeface,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ IRect* bounds)
{
    assert(0 && "TODO: need jni codes.");
}

void Paint::NativeGetCharArrayBounds(
    /* [in] */ Int64 nObj,
    /* [in] */ Int64 native_typeface,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ IRect* bounds)
{
    assert(0 && "TODO: need jni codes.");
}

void Paint::NativeFinalizer(
    /* [in] */ Int64 nObj)
{
    delete (SkPaint*)nObj;
}

void Paint::NativeSetShadowLayer(
    /* [in] */ Int64 native_object,
    /* [in] */ Float radius,
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ Int32 color)
{
    assert(0 && "TODO: need jni codes.");
}

Boolean Paint::NativeHasShadowLayer(
    /* [in] */ Int64 native_object)
{
    assert(0 && "TODO: need jni codes.");
    return FALSE;
}

Float Paint::NativeGetLetterSpacing(
    /* [in] */ Int64 native_object)
{
    assert(0 && "TODO: need jni codes.");
    return 0.f;
}

void Paint::NativeSetLetterSpacing(
    /* [in] */ Int64 native_object,
    /* [in] */ Float letterSpacing)
{
    assert(0 && "TODO: need jni codes.");
}

void Paint::NativeSetFontFeatureSettings(
    /* [in] */ Int64 native_object,
    /* [in] */ const String& settings)
{
    assert(0 && "TODO: need jni codes.");
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
