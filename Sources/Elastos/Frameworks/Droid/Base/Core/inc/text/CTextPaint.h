
#ifndef __CTEXTPAINT_H__
#define __CTEXTPAINT_H__

#include "_CTextPaint.h"
#include "graphics/Paint.h"
#include "text/TextPaint.h"

using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::IPath;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::IShader;
using Elastos::Droid::Graphics::IColorFilter;
using Elastos::Droid::Graphics::IMaskFilter;
using Elastos::Droid::Graphics::IRasterizer;
using Elastos::Droid::Graphics::IPathEffect;
using Elastos::Droid::Graphics::ITypeface;
using Elastos::Droid::Graphics::IXfermode;
using Elastos::Droid::Graphics::IPaintFontMetrics;
using Elastos::Droid::Graphics::IPaintFontMetricsInt;
using Elastos::Droid::Graphics::PaintCap;
using Elastos::Droid::Graphics::PaintJoin;
using Elastos::Droid::Graphics::PaintStyle;
using Elastos::Droid::Graphics::PaintAlign;

namespace Elastos {
namespace Droid {
namespace Text {

/**
 * TextPaint is an extension of Paint that leaves room for some extra
 * data used during text measuring and drawing.
 */
CarClass(CTextPaint), public TextPaint
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ IPaint* p);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
public:
    /**
     * Return the distance above (negative) the baseline (ascent) based on the
     * current typeface and text size.
     *
     * @return the distance above (negative) the baseline (ascent) based on the
     *         current typeface and text size.
     */
    CARAPI Ascent(
        /* [out] */ Float* distance);

    /**
     * Measure the text, stopping early if the measured width exceeds maxWidth.
     * Return the number of chars that were measured, and if measuredWidth is
     * not null, return in it the actual width measured.
     *
     * @param text  The text to measure
     * @param index The offset into text to begin measuring at
     * @param count The number of maximum number of entries to measure. If count
     *              is negative, then the characters before index are measured
     *              in reverse order. This allows for measuring the end of
     *              string.
     * @param maxWidth The maximum width to accumulate.
     * @param measuredWidth Optional. If not null, returns the actual width
     *                     measured.
     * @return The number of chars that were measured. Will always be <=
     *         abs(count).
     */
    CARAPI BreakText(
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth,
        /* [out] */ Int32* number);

    /**
     * Measure the text, stopping early if the measured width exceeds maxWidth.
     * Return the number of chars that were measured, and if measuredWidth is
     * not null, return in it the actual width measured.
     *
     * @param text  The text to measure
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
    CARAPI BreakTextEx(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Boolean measureForwards,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth,
        /* [out] */ Int32* number);

    /**
     * Measure the text, stopping early if the measured width exceeds maxWidth.
     * Return the number of chars that were measured, and if measuredWidth is
     * not null, return in it the actual width measured.
     *
     * @param text  The text to measure
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
    CARAPI BreakTextEx2(
        /* [in] */ const String& text,
        /* [in] */ Boolean measureForwards,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth,
        /* [out] */ Int32* number);

    /**
     * Temporary API to clear the shadow layer.
     */
    CARAPI ClearShadowLayer();

    /**
     * Return the distance below (positive) the baseline (descent) based on the
     * current typeface and text size.
     *
     * @return the distance below (positive) the baseline (descent) based on
     *         the current typeface and text size.
     */
    CARAPI Descent(
        /* [out] */ Float* distance);

    /**
     * Helper to getColor() that just returns the color's alpha value. This is
     * the same as calling getColor() >>> 24. It always returns a value between
     * 0 (completely transparent) and 255 (completely opaque).
     *
     * @return the alpha component of the paint's color.
     */
    CARAPI GetAlpha(
        /* [out] */ Int32* alpha);

    /**
     * Return the paint's color. Note that the color is a 32bit value
     * containing alpha as well as r,g,b. This 32bit value is not premultiplied,
     * meaning that its alpha can be any value, regardless of the values of
     * r,g,b. See the Color class for more details.
     *
     * @return the paint's color (and alpha).
     */
    CARAPI GetColor(
        /* [out] */ Int32* color);

    /**
     * Get the paint's colorfilter (maybe be null).
     *
     * @return the paint's colorfilter (maybe be null)
     */
    CARAPI GetColorFilter(
        /* [out] */ IColorFilter** filter);

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
    CARAPI GetFillPath(
        /* [in] */ IPath* src,
        /* [in] */ IPath* dst,
        /* [out] */ Boolean* isShouldFilled);

    /**
     * Return the paint's flags. Use the Flag enum to test flag values.
     *
     * @return the paint's flags (see enums ending in _Flag for bit masks)
     */
    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    /**
     * Return the font's recommended interline spacing, given the Paint's
     * settings for typeface, textSize, etc. If metrics is not null, return the
     * fontmetric values in it.
     *
     * @param metrics If this object is not null, its fields are filled with
     *                the appropriate values given the paint's text attributes.
     * @return the font's recommended interline spacing.
     */
    CARAPI GetFontMetrics(
        /* [in] */ IPaintFontMetrics* metrics,
        /* [out] */ Float* spacing);

    /**
     * Allocates a new FontMetrics object, and then calls getFontMetrics(fm)
     * with it, returning the object.
     */
    CARAPI GetFontMetricsEx(
        /* [out] */ IPaintFontMetrics** metrics);

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
    CARAPI GetFontMetricsInt(
        /* [in] */ IPaintFontMetricsInt* fmi,
        /* [out] */ Int32* spacing);

    CARAPI GetFontMetricsIntEx(
        /* [out] */ IPaintFontMetricsInt** fmi);

    /**
     * Return the recommend line spacing based on the current typeface and
     * text size.
     *
     * @return  recommend line spacing based on the current typeface and
     *          text size.
     */
    CARAPI GetFontSpacing(
        /* [out] */ Float* spacing);

    /**
     * Return the paint's hinting mode.  Returns either
     * {@link #HINTING_OFF} or {@link #HINTING_ON}.
     */
    CARAPI GetHinting(
        /* [out] */ Int32* result);

    /**
     * Get the paint's maskfilter object.
     *
     * @return the paint's maskfilter (or null)
     */
    CARAPI GetMaskFilter(
        /* [out] */ IMaskFilter** maskfilter);

    /**
     * Get the paint's patheffect object.
     *
     * @return the paint's patheffect (or null)
     */
    CARAPI GetPathEffect(
        /* [out] */ IPathEffect** effect);

    /**
     * Get the paint's rasterizer (or null).
     * <p />
     * The raster controls/modifies how paths/text are turned into alpha masks.
     *
     * @return         the paint's rasterizer (or null)
     */
    CARAPI GetRasterizer(
        /* [out] */ IRasterizer** rasterizer);

    /**
     * Get the paint's shader object.
     *
     * @return the paint's shader (or null)
     */
    CARAPI GetShader(
        /* [out] */ IShader** shader);

    /**
     * Return the paint's Cap, controlling how the start and end of stroked
     * lines and paths are treated.
     *
     * @return the line cap style for the paint, used whenever the paint's
     *         style is Stroke or StrokeAndFill.
     */
    CARAPI GetStrokeCap(
        /* [out] */ PaintCap* cap);

    /**
     * Return the paint's stroke join type.
     *
     * @return the paint's Join.
     */
    CARAPI GetStrokeJoin(
        /* [out] */ PaintJoin* join);

    /**
     * Return the paint's stroke miter value. Used to control the behavior
     * of miter joins when the joins angle is sharp.
     *
     * @return the paint's miter limit, used whenever the paint's style is
     *         Stroke or StrokeAndFill.
     */
    CARAPI GetStrokeMiter(
        /* [out] */ Float* strokeMiter);

    /**
     * Return the width for stroking.
     * <p />
     * A value of 0 strokes in hairline mode.
     * Hairlines always draws a single pixel independent of the canva's matrix.
     *
     * @return the paint's stroke width, used whenever the paint's style is
     *         Stroke or StrokeAndFill.
     */
    CARAPI GetStrokeWidth(
        /* [out] */ Float* width);

    /**
     * Return the paint's style, used for controlling how primitives'
     * geometries are interpreted (except for drawBitmap, which always assumes
     * FILL_STYLE).
     *
     * @return the paint's style setting (Fill, Stroke, StrokeAndFill)
     */
    CARAPI GetStyle(
        /* [out] */ PaintStyle* style);

    /**
     * Return the paint's Align value for drawing text. This controls how the
     * text is positioned relative to its origin. LEFT align means that all of
     * the text will be drawn to the right of its origin (i.e. the origin
     * specifieds the LEFT edge of the text) and so on.
     *
     * @return the paint's Align value for drawing text.
     */
    CARAPI GetTextAlign(
        /* [out] */ PaintAlign* align);

    /**
     * Get the text Locale.
     *
     * @return the paint's Locale used for drawing text, never null.
     */
    CARAPI GetTextLocale(
        /* [out] */ ILocale** result);

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
    CARAPI GetTextPath(
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPath* path);

    /**
     * Return the paint's horizontal scale factor for text. The default value
     * is 1.0.
     *
     * @return the paint's scale factor in X for drawing/measuring text
     */
    CARAPI GetTextScaleX(
        /* [out] */ Float* scaleX);

    /**
     * Return the paint's text size.
     *
     * @return the paint's text size.
     */
    CARAPI GetTextSize(
        /* [out] */ Float* size);

    /**
     * Return the paint's horizontal skew factor for text. The default value
     * is 0.
     *
     * @return         the paint's skew factor in X for drawing text.
     */
    CARAPI GetTextSkewX(
        /* [out] */ Float* skewX);

    /**
     * Get the paint's typeface object.
     * <p />
     * The typeface object identifies which font to use when drawing or
     * measuring text.
     *
     * @return the paint's typeface (or null)
     */
    CARAPI GetTypeface(
        /* [out] */ ITypeface** typeface);

    /**
     * Get the paint's xfermode object.
     *
     * @return the paint's xfermode (or null)
     */
    CARAPI GetXfermode(
        /* [out] */ IXfermode** xfermode);

    /**
     * Helper for getFlags(), returning true if ANTI_ALIAS_FLAG bit is set
     * AntiAliasing smooths out the edges of what is being drawn, but is has
     * no impact on the interior of the shape. See setDither() and
     * setFilterBitmap() to affect how colors are treated.
     *
     * @return true if the antialias bit is set in the paint's flags.
     */
    CARAPI IsAntiAlias(
        /* [out] */ Boolean* isAntiAlias);

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
    CARAPI IsDither(
        /* [out] */ Boolean* isDither);

    /**
     * Helper for getFlags(), returning true if FAKE_BOLD_TEXT_FLAG bit is set
     *
     * @return true if the fakeBoldText bit is set in the paint's flags.
     */
    CARAPI IsFakeBoldText(
        /* [out] */ Boolean* isFakeBoldText);

    /**
     * Whether or not the bitmap filter is activated.
     * Filtering affects the sampling of bitmaps when they are transformed.
     * Filtering does not affect how the colors in the bitmap are converted into
     * device pixels. That is dependent on dithering and xfermodes.
     *
     * @see #setFilterBitmap(boolean) setFilterBitmap()
     */
    CARAPI IsFilterBitmap(
        /* [out] */ Boolean* isFilterBitmap);

    /**
     * Helper for getFlags(), returning true if LINEAR_TEXT_FLAG bit is set
     *
     * @return true if the lineartext bit is set in the paint's flags
     */
    CARAPI IsLinearText(
        /* [out] */ Boolean* isLinearText);

    /**
     * Helper for getFlags(), returning true if STRIKE_THRU_TEXT_FLAG bit is set
     *
     * @return true if the strikeThruText bit is set in the paint's flags.
     */
    CARAPI IsStrikeThruText(
        /* [out] */ Boolean* IsStrikeThruText);

    /**
     * Helper for getFlags(), returning true if SUBPIXEL_TEXT_FLAG bit is set
     *
     * @return true if the subpixel bit is set in the paint's flags
     */
    CARAPI IsSubpixelText(
        /* [out] */ Boolean* isSubpixelText);

    /**
     * Helper for getFlags(), returning true if UNDERLINE_TEXT_FLAG bit is set
     *
     * @return true if the underlineText bit is set in the paint's flags.
     */
    CARAPI IsUnderlineText(
        /* [out] */ Boolean* isUnderlineText);

    /** Restores the paint to its default settings. */
    CARAPI Reset();

    /**
     * Copy the fields from src into this paint. This is equivalent to calling
     * get() on all of the src fields, and calling the corresponding set()
     * methods on this.
     */
    CARAPI Set(
        /* [in] */ IPaint* src);

    /**
     * Helper to setColor(), that takes a,r,g,b and constructs the color int
     *
     * @param a The new alpha component (0..255) of the paint's color.
     * @param r The new red component (0..255) of the paint's color.
     * @param g The new green component (0..255) of the paint's color.
     * @param b The new blue component (0..255) of the paint's color.
     */
    CARAPI SetARGB(
        /* [in] */ Int32 a,
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    /**
     * Helper to setColor(), that only assigns the color's alpha value,
     * leaving its r,g,b values unchanged. Results are undefined if the alpha
     * value is outside of the range [0..255]
     *
     * @param a set the alpha component [0..255] of the paint's color.
     */
    CARAPI SetAlpha(
        /* [in] */ Int32 a);

    /**
     * Helper for setFlags(), setting or clearing the ANTI_ALIAS_FLAG bit
     * AntiAliasing smooths out the edges of what is being drawn, but is has
     * no impact on the interior of the shape. See setDither() and
     * setFilterBitmap() to affect how colors are treated.
     *
     * @param aa true to set the antialias bit in the flags, false to clear it
     */
    CARAPI SetAntiAlias(
        /* [in] */ Boolean aa);

    /**
     * Set the paint's color. Note that the color is an int containing alpha
     * as well as r,g,b. This 32bit value is not premultiplied, meaning that
     * its alpha can be any value, regardless of the values of r,g,b.
     * See the Color class for more details.
     *
     * @param color The new color (including alpha) to set in the paint.
     */
    CARAPI SetColor(
        /* [in] */ Int32 color);

    /**
     * Set or clear the paint's colorfilter, returning the parameter.
     *
     * @param filter May be null. The new filter to be installed in the paint
     * @return       filter
     */
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* filter);

    /** @hide */
    CARAPI SetCompatibilityScaling(
        /* [in] */ Float factor);

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
    CARAPI SetDither(
        /* [in] */ Boolean dither);

    /**
     * Helper for setFlags(), setting or clearing the STRIKE_THRU_TEXT_FLAG bit
     *
     * @param fakeBoldText true to set the fakeBoldText bit in the paint's
     *                     flags, false to clear it.
     */
    CARAPI SetFakeBoldText(
        /* [in] */ Boolean fakeBoldText);

    /**
     * Helper for setFlags(), setting or clearing the FILTER_BITMAP_FLAG bit.
     * Filtering affects the sampling of bitmaps when they are transformed.
     * Filtering does not affect how the colors in the bitmap are converted into
     * device pixels. That is dependent on dithering and xfermodes.
     *
     * @param filter true to set the FILTER_BITMAP_FLAG bit in the paint's
     *               flags, false to clear it.
     */
    CARAPI SetFilterBitmap(
        /* [in] */ Boolean filter);

    /**
     * Set the paint's flags. Use the Flag enum to specific flag values.
     *
     * @param flags The new flag bits for the paint
     */
    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    /**
     * Set the paint's hinting mode.  May be either
     * {@link #HINTING_OFF} or {@link #HINTING_ON}.
     */
    CARAPI SetHinting(
        /* [in] */ Int32 mode);

    /**
     * Helper for setFlags(), setting or clearing the LINEAR_TEXT_FLAG bit
     *
     * @param linearText true to set the linearText bit in the paint's flags,
     *                   false to clear it.
     */
    CARAPI SetLinearText(
        /* [in] */ Boolean linearText);

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
    CARAPI SetMaskFilter(
        /* [in] */ IMaskFilter* maskfilter);

    /**
     * Set or clear the patheffect object.
     * <p />
     * Pass null to clear any previous patheffect.
     * As a convenience, the parameter passed is also returned.
     *
     * @param effect May be null. The patheffect to be installed in the paint
     * @return       effect
     */
    CARAPI SetPathEffect(
        /* [in] */ IPathEffect* effect);

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
    CARAPI SetRasterizer(
        /* [in] */ IRasterizer* rasterizer);

    /**
     * Set or clear the shader object.
     * <p />
     * Pass null to clear any previous shader.
     * As a convenience, the parameter passed is also returned.
     *
     * @param shader May be null. the new shader to be installed in the paint
     * @return       shader
     */
    CARAPI SetShader(
        /* [in] */ IShader* shader);

    /**
     * Temporary API to expose layer drawing. This draws a shadow layer below
     * the main layer, with the specified offset and color, and blur radius.
     * If radius is 0, then the shadow layer is removed.
     */
    CARAPI SetShadowLayer(
        /* [in] */ Float radius,
        /* [in] */ Float rdx,
        /* [in] */ Float rdy,
        /* [in] */ Int32 color);

    /**
     * Helper for setFlags(), setting or clearing the STRIKE_THRU_TEXT_FLAG bit
     *
     * @param strikeThruText true to set the strikeThruText bit in the paint's
     *                       flags, false to clear it.
     */
    CARAPI SetStrikeThruText(
        /* [in] */ Boolean strikeThruText);

    /**
     * Set the paint's Cap.
     *
     * @param cap set the paint's line cap style, used whenever the paint's
     *            style is Stroke or StrokeAndFill.
     */
    CARAPI SetStrokeCap(
        /* [in] */ PaintCap cap);

    /**
     * Set the paint's Join.
     *
     * @param join set the paint's Join, used whenever the paint's style is
     *             Stroke or StrokeAndFill.
     */
    CARAPI SetStrokeJoin(
        /* [in] */ PaintJoin join);

    /**
     * Set the paint's stroke miter value. This is used to control the behavior
     * of miter joins when the joins angle is sharp. This value must be >= 0.
     *
     * @param miter set the miter limit on the paint, used whenever the paint's
     *              style is Stroke or StrokeAndFill.
     */
    CARAPI SetStrokeMiter(
        /* [in] */ Float miter);

    /**
     * Set the width for stroking.
     * Pass 0 to stroke in hairline mode.
     * Hairlines always draws a single pixel independent of the canva's matrix.
     *
     * @param width set the paint's stroke width, used whenever the paint's
     *              style is Stroke or StrokeAndFill.
     */
    CARAPI SetStrokeWidth(
        /* [in] */ Float width);

    /**
     * Set the paint's style, used for controlling how primitives'
     * geometries are interpreted (except for drawBitmap, which always assumes
     * Fill).
     *
     * @param style The new style to set in the paint
     */
    CARAPI SetStyle(
        /* [in] */ PaintStyle style);

    /**
     * Helper for setFlags(), setting or clearing the SUBPIXEL_TEXT_FLAG bit
     *
     * @param subpixelText true to set the subpixelText bit in the paint's
     *                     flags, false to clear it.
     */
    CARAPI SetSubpixelText(
        /* [in] */ Boolean subpixelText);

    /**
     * Set the paint's text alignment. This controls how the
     * text is positioned relative to its origin. LEFT align means that all of
     * the text will be drawn to the right of its origin (i.e. the origin
     * specifieds the LEFT edge of the text) and so on.
     *
     * @param align set the paint's Align value for drawing text.
     */
    CARAPI SetTextAlign(
        /* [in] */ PaintAlign align);

    /**
     * Set the text locale.
     *
     * The text locale affects how the text is drawn for some languages.
     *
     * For example, if the locale is {@link Locale#CHINESE} or {@link Locale#CHINA},
     * then the text renderer will prefer to draw text using a Chinese font. Likewise,
     * if the locale is {@link Locale#JAPANESE} or {@link Locale#JAPAN}, then the text
     * renderer will prefer to draw text using a Japanese font.
     *
     * This distinction is important because Chinese and Japanese text both use many
     * of the same Unicode code points but their appearance is subtly different for
     * each language.
     *
     * By default, the text locale is initialized to the system locale (as returned
     * by {@link Locale#getDefault}). This assumes that the text to be rendered will
     * most likely be in the user's preferred language.
     *
     * If the actual language of the text is known, then it can be provided to the
     * text renderer using this method. The text renderer may attempt to guess the
     * language script based on the contents of the text to be drawn independent of
     * the text locale here. Specifying the text locale just helps it do a better
     * job in certain ambiguous cases
     *
     * @param locale the paint's locale value for drawing text, must not be null.
     */
    CARAPI SetTextLocale(
        /* [in] */ ILocale* locale);

    /**
     * Set the paint's horizontal scale factor for text. The default value
     * is 1.0. Values > 1.0 will stretch the text wider. Values < 1.0 will
     * stretch the text narrower.
     *
     * @param scaleX set the paint's scale in X for drawing/measuring text.
     */
    CARAPI SetTextScaleX(
        /* [in] */ Float scaleX);

    /**
     * Set the paint's text size. This value must be > 0
     *
     * @param textSize set the paint's text size.
     */
    CARAPI SetTextSize(
        /* [in] */ Float textSize);

    /**
     * Set the paint's horizontal skew factor for text. The default value
     * is 0. For approximating oblique text, use values around -0.25.
     *
     * @param skewX set the paint's skew factor in X for drawing text.
     */
    CARAPI SetTextSkewX(
        /* [in] */ Float skewX);

    /**
     * Set or clear the typeface object.
     * <p />
     * Pass null to clear any previous typeface.
     * As a convenience, the parameter passed is also returned.
     *
     * @param typeface May be null. The typeface to be installed in the paint
     * @return         typeface
     */
    CARAPI SetTypeface(
        /* [in] */ ITypeface* typeface);

    /**
     * Helper for setFlags(), setting or clearing the UNDERLINE_TEXT_FLAG bit
     *
     * @param underlineText true to set the underlineText bit in the paint's
     *                      flags, false to clear it.
     */
    CARAPI SetUnderlineText(
        /* [in] */ Boolean underlineText);

    /**
     * Set or clear the xfermode object.
     * <p />
     * Pass null to clear any previous xfermode.
     * As a convenience, the parameter passed is also returned.
     *
     * @param xfermode May be null. The xfermode to be installed in the paint
     * @return         xfermode
     */
    CARAPI SetXfermode(
        /* [in] */ IXfermode* xfermode);

/********************************TextPaint*****************************/

    // Special value 0 means no background paint
    CARAPI SetBgColor(
        /* [in] */ Int32 bgColor);

    CARAPI GetBgColor(
        /* [out] */ Int32* bgColor);

    CARAPI SetBaselineShift(
        /* [in] */ Int32 baselineShift);

    CARAPI GetBaselineShift(
        /* [out] */ Int32* baselineShift);

    CARAPI SetLinkColor(
        /* [in] */ Int32 linkColor);

    CARAPI GetLinkColor(
        /* [out] */ Int32* linkColor);

    CARAPI SetDrawableState(
        /* [in] */ ArrayOf<Int32>* drawableState);

    CARAPI GetDrawableState(
        /* [out, callee] */ ArrayOf<Int32>** drawableState);

    CARAPI SetDensity(
        /* [in] */ Float density);

    CARAPI GetDensity(
        /* [out] */ Float* density);

    /**
     * Special value 0 means no custom underline
     * @hide
     */
    CARAPI SetUnderlineColor(
        /* [in] */ Int32 underlineColor);

    CARAPI GetUnderlineColor(
        /* [out] */ Int32* underlineColor);

    /**
     * Defined as a multiplier of the default underline thickness. Use 1.0f for default thickness.
     * @hide
     */
    CARAPI SetUnderlineThickness(
        /* [in] */ Float underlineThickness);

    CARAPI GetUnderlineThickness(
        /* [out] */ Float* underlineThickness);

    /**
     * Copy the fields from tp into this TextPaint, including the
     * fields inherited from Paint.
     */
    CARAPI Set(
        /* [in] */ ITextPaint* tp);

    /**
     * Defines a custom underline for this Paint.
     * @param color underline solid color
     * @param thickness underline thickness
     * @hide
     */
    CARAPI SetUnderlineText(
        /* [in] */ Int32 color,
        /* [in] */ Float thickness);

/******************************IPaint extra*********************************/
    /**
     * Return the bidi flags on the paint.
     *
     * @return the bidi flags on the paint
     * @hide
     */
    CARAPI GetBidiFlags(
        /* [out] */ Int32* flags);

    /**
     * Set the bidi flags on the paint.
     * @hide
     */
    CARAPI SetBidiFlags(
        /* [in] */ Int32 flags);

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
    CARAPI GetTextBounds(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IRect* bounds);

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
    CARAPI GetTextBoundsEx(
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ IRect* bounds);

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
    CARAPI GetTextWidths(
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [out] */ ArrayOf<Float>* widths,
        /* [out] */ Int32* number);

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
    CARAPI GetTextWidthsEx(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ArrayOf<Float>* widths,
        /* [out] */ Int32* number);

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
    CARAPI GetTextWidthsEx2(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32* number);

    /**
     * Return the advance widths for the characters in the string.
     *
     * @param text   The text to measure
     * @param widths array to receive the advance widths of the characters.
     *               Must be at least a large as the text.
     * @return       the number of unichars in the specified text.
     */
    CARAPI GetTextWidthsEx3(
        /* [in] */ const String& text,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32* number);

    /* Return the glyph Ids for the characters in the string.
     *
     * @param text   The text to measure
     * @param start  The index of the first char to to measure
     * @param end    The end of the text slice to measure
     * @param contextStart the index of the first character to use for shaping context,
     * must be <= start
     * @param contextEnd the index past the last character to use for shaping context,
     * must be >= end
     * @param flags the flags to control the advances, either {@link #DIRECTION_LTR}
     * or {@link #DIRECTION_RTL}
     * @param glyphs array to receive the glyph Ids of the characters.
     *               Must be at least a large as the text.
     * @return       the number of glyphs in the returned array
     *
     * @hide
     *
     * Used only for BiDi / RTL Tests
     */
    CARAPI GetTextGlyphs(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [out] */ ArrayOf<Char32>* glyphs,
        /* [out] */ Int32* number);

    /**
     * Convenience overload that takes a char array instead of a
     * String.
     *
     * @see #getTextRunAdvances(String, int, int, int, int, int, float[], int)
     * @hide
     */
    CARAPI GetTextRunAdvances(
        /* [in] */ const ArrayOf<Char32>& chars,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Int32 contextIndex,
        /* [in] */ Int32 contextCount,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Float>* advances,
        /* [in] */ Int32 advancesIndex,
        /* [out] */ Float* advance);

    /**
     * Convenience overload that takes a char array instead of a
     * String.
     *
     * @see #getTextRunAdvances(String, int, int, int, int, int, float[], int, int)
     * @hide
     */
    CARAPI GetTextRunAdvancesEx(
        /* [in] */ const ArrayOf<Char32>& chars,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Int32 contextIndex,
        /* [in] */ Int32 contextCount,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Float>* advances,
        /* [in] */ Int32 advancesIndex,
        /* [in] */ Int32 reserved,
        /* [out] */ Float* advance);

    /**
     * Convenience overload that takes a CharSequence instead of a
     * String.
     *
     * @see #getTextRunAdvances(String, int, int, int, int, int, float[], int)
     * @hide
     */
    CARAPI GetTextRunAdvancesEx2(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Float>* advances,
        /* [in] */ Int32 advancesIndex,
        /* [out] */ Float* advance);

    /**
     * Convenience overload that takes a CharSequence instead of a
     * String.
     *
     * @see #getTextRunAdvances(String, int, int, int, int, int, float[], int)
     * @hide
     */
    CARAPI GetTextRunAdvancesEx3(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Float>* advances,
        /* [in] */ Int32 advancesIndex,
        /* [in] */ Int32 reserved,
        /* [out] */ Float* advance);

    /**
     * Returns the total advance width for the characters in the run
     * between start and end, and if advances is not null, the advance
     * assigned to each of these characters (java chars).
     *
     * <p>The trailing surrogate in a valid surrogate pair is assigned
     * an advance of 0.  Thus the number of returned advances is
     * always equal to count, not to the number of unicode codepoints
     * represented by the run.
     *
     * <p>In the case of conjuncts or combining marks, the total
     * advance is assigned to the first logical character, and the
     * following characters are assigned an advance of 0.
     *
     * <p>This generates the sum of the advances of glyphs for
     * characters in a reordered cluster as the width of the first
     * logical character in the cluster, and 0 for the widths of all
     * other characters in the cluster.  In effect, such clusters are
     * treated like conjuncts.
     *
     * <p>The shaping bounds limit the amount of context available
     * outside start and end that can be used for shaping analysis.
     * These bounds typically reflect changes in bidi level or font
     * metrics across which shaping does not occur.
     *
     * @param text the text to measure. Cannot be null.
     * @param start the index of the first character to measure
     * @param end the index past the last character to measure
     * @param contextStart the index of the first character to use for shaping context,
     * must be <= start
     * @param contextEnd the index past the last character to use for shaping context,
     * must be >= end
     * @param flags the flags to control the advances, either {@link #DIRECTION_LTR}
     * or {@link #DIRECTION_RTL}
     * @param advances array to receive the advances, must have room for all advances,
     * can be null if only total advance is needed
     * @param advancesIndex the position in advances at which to put the
     * advance corresponding to the character at start
     * @return the total advance
     *
     * @hide
     */
    CARAPI GetTextRunAdvancesEx4(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Float>* advances,
        /* [in] */ Int32 advancesIndex,
        /* [out] */ Float* advance);

    /**
     * Returns the total advance width for the characters in the run
     * between start and end, and if advances is not null, the advance
     * assigned to each of these characters (java chars).
     *
     * <p>The trailing surrogate in a valid surrogate pair is assigned
     * an advance of 0.  Thus the number of returned advances is
     * always equal to count, not to the number of unicode codepoints
     * represented by the run.
     *
     * <p>In the case of conjuncts or combining marks, the total
     * advance is assigned to the first logical character, and the
     * following characters are assigned an advance of 0.
     *
     * <p>This generates the sum of the advances of glyphs for
     * characters in a reordered cluster as the width of the first
     * logical character in the cluster, and 0 for the widths of all
     * other characters in the cluster.  In effect, such clusters are
     * treated like conjuncts.
     *
     * <p>The shaping bounds limit the amount of context available
     * outside start and end that can be used for shaping analysis.
     * These bounds typically reflect changes in bidi level or font
     * metrics across which shaping does not occur.
     *
     * @param text the text to measure. Cannot be null.
     * @param start the index of the first character to measure
     * @param end the index past the last character to measure
     * @param contextStart the index of the first character to use for shaping context,
     * must be <= start
     * @param contextEnd the index past the last character to use for shaping context,
     * must be >= end
     * @param flags the flags to control the advances, either {@link #DIRECTION_LTR}
     * or {@link #DIRECTION_RTL}
     * @param advances array to receive the advances, must have room for all advances,
     * can be null if only total advance is needed
     * @param advancesIndex the position in advances at which to put the
     * advance corresponding to the character at start
     * @param reserved int reserved value
     * @return the total advance
     *
     * @hide
     */
    CARAPI GetTextRunAdvancesEx5(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Float>* advances,
        /* [in] */ Int32 advancesIndex,
        /* [in] */ Int32 reserved,
        /* [out] */ Float* advance);

    /**
     * Returns the next cursor position in the run.  This avoids placing the
     * cursor between surrogates, between characters that form conjuncts,
     * between base characters and combining marks, or within a reordering
     * cluster.
     *
     * <p>ContextStart and offset are relative to the start of text.
     * The context is the shaping context for cursor movement, generally
     * the bounds of the metric span enclosing the cursor in the direction of
     * movement.
     *
     * <p>If cursorOpt is {@link #CURSOR_AT} and the offset is not a valid
     * cursor position, this returns -1.  Otherwise this will never return a
     * value before contextStart or after contextStart + contextLength.
     *
     * @param text the text
     * @param contextStart the start of the context
     * @param contextLength the length of the context
     * @param flags either {@link #DIRECTION_RTL} or {@link #DIRECTION_LTR}
     * @param offset the cursor position to move from
     * @param cursorOpt how to move the cursor, one of {@link #CURSOR_AFTER},
     * {@link #CURSOR_AT_OR_AFTER}, {@link #CURSOR_BEFORE},
     * {@link #CURSOR_AT_OR_BEFORE}, or {@link #CURSOR_AT}
     * @return the offset of the next position, or -1
     * @hide
     */
    CARAPI GetTextRunCursor(
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextLength,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 cursorOpt,
        /* [out] */ Int32* position);

    /**
     * Returns the next cursor position in the run.  This avoids placing the
     * cursor between surrogates, between characters that form conjuncts,
     * between base characters and combining marks, or within a reordering
     * cluster.
     *
     * <p>ContextStart, contextEnd, and offset are relative to the start of
     * text.  The context is the shaping context for cursor movement, generally
     * the bounds of the metric span enclosing the cursor in the direction of
     * movement.
     *
     * <p>If cursorOpt is {@link #CURSOR_AT} and the offset is not a valid
     * cursor position, this returns -1.  Otherwise this will never return a
     * value before contextStart or after contextEnd.
     *
     * @param text the text
     * @param contextStart the start of the context
     * @param contextEnd the end of the context
     * @param flags either {@link #DIRECTION_RTL} or {@link #DIRECTION_LTR}
     * @param offset the cursor position to move from
     * @param cursorOpt how to move the cursor, one of {@link #CURSOR_AFTER},
     * {@link #CURSOR_AT_OR_AFTER}, {@link #CURSOR_BEFORE},
     * {@link #CURSOR_AT_OR_BEFORE}, or {@link #CURSOR_AT}
     * @return the offset of the next position, or -1
     * @hide
     */
    CARAPI GetTextRunCursorEx(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 cursorOpt,
        /* [out] */ Int32* position);

    /**
     * Returns the next cursor position in the run.  This avoids placing the
     * cursor between surrogates, between characters that form conjuncts,
     * between base characters and combining marks, or within a reordering
     * cluster.
     *
     * <p>ContextStart, contextEnd, and offset are relative to the start of
     * text.  The context is the shaping context for cursor movement, generally
     * the bounds of the metric span enclosing the cursor in the direction of
     * movement.
     *
     * <p>If cursorOpt is {@link #CURSOR_AT} and the offset is not a valid
     * cursor position, this returns -1.  Otherwise this will never return a
     * value before contextStart or after contextEnd.
     *
     * @param text the text
     * @param contextStart the start of the context
     * @param contextEnd the end of the context
     * @param flags either {@link #DIRECTION_RTL} or {@link #DIRECTION_LTR}
     * @param offset the cursor position to move from
     * @param cursorOpt how to move the cursor, one of {@link #CURSOR_AFTER},
     * {@link #CURSOR_AT_OR_AFTER}, {@link #CURSOR_BEFORE},
     * {@link #CURSOR_AT_OR_BEFORE}, or {@link #CURSOR_AT}
     * @return the offset of the next position, or -1
     * @hide
     */
    CARAPI GetTextRunCursorEx2(
        /* [in] */ const String& text,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 cursorOpt,
        /* [out] */ Int32* position);

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
    CARAPI GetTextPathEx(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPath* path);

    /**
     * Return the width of the text.
     *
     * @param text  The text to measure. Cannot be null.
     * @param index The index of the first character to start measuring
     * @param count THe number of characters to measure, beginning with start
     * @return      The width of the text
     */
    CARAPI MeasureText(
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [out] */ Float* width);

     /**
     * Return the width of the text.
     *
     * @param text  The text to measure. Cannot be null.
     * @param start The index of the first character to start measuring
     * @param end   1 beyond the index of the last character to measure
     * @return      The width of the text
     */
    CARAPI MeasureTextEx(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ Float* width);

    /**
     * Return the width of the text.
     *
     * @param text  The text to measure. Cannot be null.
     * @return      The width of the text
     */
    CARAPI MeasureTextEx2(
        /* [in] */ const String& text,
        /* [out] */ Float* width);

    /**
     * Return the width of the text.
     *
     * @param text  The text to measure
     * @param start The index of the first character to start measuring
     * @param end   1 beyond the index of the last character to measure
     * @return      The width of the text
     */
    CARAPI MeasureTextEx3(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ Float* width);
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CTEXTPAINT_H__
