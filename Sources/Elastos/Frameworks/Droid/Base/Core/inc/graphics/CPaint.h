
#ifndef __ELASTOS_DROID_GRAPHICS_CPAINT_H__
#define __ELASTOS_DROID_GRAPHICS_CPAINT_H__

#include "_CPaint.h"
#include "graphics/Paint.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CPaint), public Paint
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ IPaint* paint);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Reset();

    CARAPI Set(
        /* [in] */ IPaint* src);

    CARAPI SetCompatibilityScaling(
        /* [in] */ Float factor);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI IsAntiAlias(
        /* [out] */ Boolean* isAntiAlias);

    CARAPI SetAntiAlias(
        /* [in] */ Boolean aa);

    CARAPI IsDither(
        /* [out] */ Boolean* isDither);

    CARAPI SetDither(
        /* [in] */ Boolean dither);

    CARAPI IsLinearText(
        /* [out] */ Boolean* isLinearText);

    CARAPI SetLinearText(
        /* [in] */ Boolean linearText);

    CARAPI IsSubpixelText(
        /* [out] */ Boolean* isSubpixelText);

    CARAPI SetSubpixelText(
        /* [in] */ Boolean subpixelText);

    CARAPI IsUnderlineText(
        /* [out] */ Boolean* isUnderlineText);

    CARAPI SetUnderlineText(
        /* [in] */ Boolean underlineText);

    CARAPI IsStrikeThruText(
        /* [out] */ Boolean* isStrikeThruText);

    CARAPI SetStrikeThruText(
        /* [in] */ Boolean strikeThruText);

    CARAPI IsFakeBoldText(
        /* [out] */ Boolean* isFakeBoldText);

    CARAPI SetFakeBoldText(
        /* [in] */ Boolean fakeBoldText);

    CARAPI IsFilterBitmap(
        /* [out] */ Boolean* isFilterBitmap);

    CARAPI SetFilterBitmap(
        /* [in] */ Boolean filter);

    CARAPI GetStyle(
        /* [out] */ PaintStyle* style);

    CARAPI SetStyle(
        /* [in] */ PaintStyle style);

    CARAPI GetColor(
        /* [out] */ Int32* color);

    CARAPI SetColor(
        /* [in] */ Int32 color);

    CARAPI GetAlpha(
        /* [out] */ Int32* alpha);

    CARAPI SetAlpha(
        /* [in] */ Int32 a);

    CARAPI SetARGB(
        /* [in] */ Int32 a,
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    CARAPI GetStrokeWidth(
        /* [out] */ Float* width);

    CARAPI SetStrokeWidth(
        /* [in] */ Float width);

    CARAPI GetStrokeMiter(
        /* [out] */ Float* strokeMiter);

    CARAPI SetStrokeMiter(
        /* [in] */ Float miter);

    CARAPI GetStrokeCap(
        /* [out] */ PaintCap* cap);

    CARAPI SetStrokeCap(
        /* [in] */ PaintCap cap);

    CARAPI GetStrokeJoin(
        /* [out] */ PaintJoin* join);

    CARAPI SetStrokeJoin(
        /* [in] */ PaintJoin join);

    CARAPI GetFillPath(
        /* [in] */ IPath* src,
        /* [in] */ IPath* dst,
        /* [out] */ Boolean* isShouldFilled);

    CARAPI GetShader(
        /* [out] */ IShader** shader);

    CARAPI SetShader(
        /* [in] */ IShader* shader);

    CARAPI GetColorFilter(
        /* [out] */ IColorFilter** filter);

    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* filter);

    CARAPI GetXfermode(
        /* [out] */ IXfermode** xfermode);

    CARAPI SetXfermode(
        /* [in] */ IXfermode* xfermode);

    CARAPI GetPathEffect(
        /* [out] */ IPathEffect** effect);

    CARAPI SetPathEffect(
        /* [in] */ IPathEffect* effect);

    CARAPI GetMaskFilter(
        /* [out] */ IMaskFilter** maskfilter);

    CARAPI SetMaskFilter(
        /* [in] */ IMaskFilter* maskfilter);

    CARAPI GetTypeface(
        /* [out] */ ITypeface** typeface);

    CARAPI SetTypeface(
        /* [in] */ ITypeface* typeface);

    CARAPI GetRasterizer(
        /* [out] */ IRasterizer** rasterizer);

    CARAPI SetRasterizer(
        /* [in] */ IRasterizer* rasterizer);

    CARAPI SetShadowLayer(
        /* [in] */ Float radius,
        /* [in] */ Float rdx,
        /* [in] */ Float rdy,
        /* [in] */ Int32 color);

    CARAPI ClearShadowLayer();

    CARAPI GetTextAlign(
        /* [out] */ PaintAlign* align);

    CARAPI SetTextAlign(
        /* [in] */ PaintAlign align);

    CARAPI GetTextSize(
        /* [out] */ Float* size);

    CARAPI SetTextSize(
        /* [in] */ Float textSize);

    CARAPI GetTextScaleX(
        /* [out] */ Float* scaleX);

    CARAPI SetTextScaleX(
        /* [in] */ Float scaleX);

    CARAPI GetTextSkewX(
        /* [out] */ Float* skewX);

    CARAPI SetTextSkewX(
        /* [in] */ Float skewX);

    CARAPI Ascent(
        /* [out] */ Float* distance);

    CARAPI Descent(
        /* [out] */ Float* distance);

    CARAPI GetFontMetrics(
        /* [in] */ IPaintFontMetrics* metrics,
        /* [out] */ Float* spacing);

    CARAPI GetFontMetrics(
        /* [out] */ IPaintFontMetrics** metrics);

    CARAPI GetFontMetricsInt(
        /* [in] */ IPaintFontMetricsInt* fmi,
        /* [out] */ Int32* spacing);

    CARAPI GetFontMetricsInt(
        /* [out] */ IPaintFontMetricsInt** fmi);

    CARAPI GetFontSpacing(
        /* [out] */ Float* spacing);

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
    CARAPI MeasureText(
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
    CARAPI MeasureText(
        /* [in] */ const String& text,
        /* [out] */ Float* width);

    CARAPI MeasureText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ Float* width);

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
    CARAPI BreakText(
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
    CARAPI BreakText(
        /* [in] */ const String& text,
        /* [in] */ Boolean measureForwards,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth,
        /* [out] */ Int32* number);

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
        /* [in] */ const ArrayOf<Char32> & text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32* width);

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
    CARAPI GetTextWidths(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32* width);

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
    CARAPI GetTextWidths(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32* width);

    /**
     * Return the advance widths for the characters in the string.
     *
     * @param text   The text to measure
     * @param widths array to receive the advance widths of the characters.
     *               Must be at least a large as the text.
     * @return       the number of unichars in the specified text.
     */
    CARAPI GetTextWidths(
        /* [in] */ const String& text,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32* width);

    CARAPI GetTextPath(
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPath* path);

    CARAPI GetTextPath(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPath* path);

    CARAPI GetTextBounds(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IRect* bounds);

    CARAPI GetTextBounds(
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ IRect* bounds);

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
     * Get the text Locale.
     *
     * @return the paint's Locale used for drawing text, never null.
     */
    GetTextLocale(
        /* [out] */ ILocale** result);


    /**
     * Return the bidi flags on the paint.
     *
     * @return the bidi flags on the paint
     * @hide
     */
    CARAPI GetBidiFlags(
         /* [out] */ Int32* retvalue);

    /**
     * Set the bidi flags on the paint.
     * @hide
     */
    CARAPI SetBidiFlags(
        /* [in] */ Int32 flags);

    /**
     * Return the paint's hinting mode.  Returns either
     * {@link #HINTING_OFF} or {@link #HINTING_ON}.
     */
    CARAPI GetHinting(
        /* [out] */ Int32* result);

    /**
     * Set the paint's hinting mode.  May be either
     * {@link #HINTING_OFF} or {@link #HINTING_ON}.
     */
    CARAPI SetHinting(
        /* [in] */ Int32 flags);

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
    CARAPI GetTextRunAdvances(
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
    CARAPI GetTextRunAdvances(
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
    CARAPI GetTextRunAdvances(
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
    CARAPI GetTextRunAdvances(
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
    CARAPI GetTextRunAdvances(
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
    CARAPI GetTextRunCursor(
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
    CARAPI GetTextRunCursor(
        /* [in] */ const String& text,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 cursorOpt,
        /* [out] */ Int32* position);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CPAINT_H__
