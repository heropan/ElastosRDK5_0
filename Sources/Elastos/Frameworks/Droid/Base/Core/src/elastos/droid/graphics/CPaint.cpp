
#include "graphics/CPaint.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_OBJECT_IMPL(CPaint);
ECode CPaint::constructor()
{
    return Paint::constructor();
}

ECode CPaint::constructor(
    /* [in] */ Int32 flags)
{
    return Paint::constructor(flags);
}

ECode CPaint::constructor(
    /* [in] */ IPaint* paint)
{
    return Paint::constructor(paint);
}

PInterface CPaint::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Paint) {
        return reinterpret_cast<PInterface>((Paint*)this);
    }
    return Paint::Probe(riid);
}

ECode CPaint::Reset()
{
    return Paint::Reset();
}

ECode CPaint::Set(
    /* [in] */ IPaint* src)
{
    return Paint::Set(src);
}

ECode CPaint::SetCompatibilityScaling(
    /* [in] */ Float factor)
{
    return Paint::SetCompatibilityScaling(factor);
}

ECode CPaint::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    return Paint::GetFlags(flags);
}

/**
 * Return the paint's flags. Use the Flag enum to test flag values.
 *
 * @return the paint's flags (see enums ending in _Flag for bit masks)
 */
ECode CPaint::SetFlags(
    /* [in] */ Int32 flags)
{
    return Paint::SetFlags(flags);
}

ECode CPaint::IsAntiAlias(
    /* [out] */ Boolean* isAntiAlias)
{
    VALIDATE_NOT_NULL(isAntiAlias);
    return Paint::IsAntiAlias(isAntiAlias);
}

ECode CPaint::SetAntiAlias(
    /* [in] */ Boolean aa)
{
    return Paint::SetAntiAlias(aa);
}

ECode CPaint::IsDither(
    /* [out] */ Boolean* isDither)
{
    VALIDATE_NOT_NULL(isDither);
    return Paint::IsDither(isDither);
}

ECode CPaint::SetDither(
    /* [in] */ Boolean dither)
{
    return Paint::SetDither(dither);
}

ECode CPaint::IsLinearText(
    /* [out] */ Boolean* isLinearText)
{
    VALIDATE_NOT_NULL(isLinearText);
    return Paint::IsLinearText(isLinearText);
}

ECode CPaint::SetLinearText(
    /* [in] */ Boolean linearText)
{
    return Paint::SetLinearText(linearText);
}

ECode CPaint::IsSubpixelText(
    /* [out] */ Boolean* isSubpixelText)
{
    VALIDATE_NOT_NULL(isSubpixelText);
    return Paint::IsSubpixelText(isSubpixelText);
}

ECode CPaint::SetSubpixelText(
    /* [in] */ Boolean subpixelText)
{
    return Paint::SetSubpixelText(subpixelText);
}

ECode CPaint::IsUnderlineText(
    /* [out] */ Boolean* isUnderlineText)
{
    VALIDATE_NOT_NULL(isUnderlineText);
    return Paint::IsUnderlineText(isUnderlineText);
}

ECode CPaint::SetUnderlineText(
    /* [in] */ Boolean underlineText)
{
    return Paint::SetUnderlineText(underlineText);
}

ECode CPaint::IsStrikeThruText(
    /* [out] */ Boolean* isStrikeThruText)
{
    VALIDATE_NOT_NULL(isStrikeThruText);
    return Paint::IsStrikeThruText(isStrikeThruText);
}

ECode CPaint::SetStrikeThruText(
    /* [in] */ Boolean strikeThruText)
{
    return Paint::SetStrikeThruText(strikeThruText);
}

ECode CPaint::IsFakeBoldText(
    /* [out] */ Boolean* isFakeBoldText)
{
    VALIDATE_NOT_NULL(isFakeBoldText);
    return Paint::IsFakeBoldText(isFakeBoldText);
}

ECode CPaint::SetFakeBoldText(
    /* [in] */ Boolean fakeBoldText)
{
    return Paint::SetFakeBoldText(fakeBoldText);
}

ECode CPaint::IsFilterBitmap(
    /* [out] */ Boolean* isFilterBitmap)
{
    VALIDATE_NOT_NULL(isFilterBitmap);
    return Paint::IsFilterBitmap(isFilterBitmap);
}

ECode CPaint::SetFilterBitmap(
    /* [in] */ Boolean filter)
{
    return Paint::SetFilterBitmap(filter);
}

ECode CPaint::GetStyle(
    /* [out] */ PaintStyle* style)
{
    VALIDATE_NOT_NULL(style);
    return Paint::GetStyle(style);
}

ECode CPaint::SetStyle(
    /* [in] */ PaintStyle style)
{
    return Paint::SetStyle(style);
}

ECode CPaint::GetColor(
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    return Paint::GetColor(color);
}

/**
 * Set the paint's color. Note that the color is an int containing alpha
 * as well as r,g,b. This 32bit value is not premultiplied, meaning that
 * its alpha can be any value, regardless of the values of r,g,b.
 * See the Color class for more details.
 *
 * @param color The new color (including alpha) to set in the paint.
 */
ECode CPaint::SetColor(
    /* [in] */ Int32 color)
{
    return Paint::SetColor(color);
}

ECode CPaint::GetAlpha(
    /* [out] */ Int32* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    return Paint::GetAlpha(alpha);
}

ECode CPaint::SetAlpha(
    /* [in] */ Int32 a)
{
    return Paint::SetAlpha(a);
}

ECode CPaint::SetARGB(
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return Paint::SetARGB(a, r, g, b);
}

ECode CPaint::GetStrokeWidth(
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    return Paint::GetStrokeWidth(width);
}

ECode CPaint::SetStrokeWidth(
    /* [in] */ Float width)
{
    return Paint::SetStrokeWidth(width);
}

ECode CPaint::GetStrokeMiter(
    /* [out] */ Float* strokeMiter)
{
    VALIDATE_NOT_NULL(strokeMiter);
    return Paint::GetStrokeMiter(strokeMiter);
}

ECode CPaint::SetStrokeMiter(
    /* [in] */ Float miter)
{
    return Paint::SetStrokeMiter(miter);
}

ECode CPaint::GetStrokeCap(
    /* [out] */ PaintCap* cap)
{
    VALIDATE_NOT_NULL(cap);
    return Paint::GetStrokeCap(cap);
}

ECode CPaint::SetStrokeCap(
    /* [in] */ PaintCap cap)
{
    return Paint::SetStrokeCap(cap);
}

ECode CPaint::GetStrokeJoin(
    /* [out] */ PaintJoin* join)
{
    VALIDATE_NOT_NULL(join);
    return Paint::GetStrokeJoin(join);
}

ECode CPaint::SetStrokeJoin(
    /* [in] */ PaintJoin join)
{
    return Paint::SetStrokeJoin(join);
}

ECode CPaint::GetFillPath(
    /* [in] */ IPath * src,
    /* [in] */ IPath * dst,
    /* [out] */ Boolean * isShouldFilled)
{
    VALIDATE_NOT_NULL(isShouldFilled);
    return Paint::GetFillPath(src, dst, isShouldFilled);
}

ECode CPaint::GetShader(
    /* [out] */ IShader** shader)
{
    VALIDATE_NOT_NULL(shader);
    return Paint::GetShader(shader);
}

ECode CPaint::SetShader(
    /* [in] */ IShader* shader)
{
    return Paint::SetShader(shader);
}

ECode CPaint::GetColorFilter(
    /* [out] */ IColorFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    return Paint::GetColorFilter(filter);
}

ECode CPaint::SetColorFilter(
    /* [in] */ IColorFilter* filter)
{
    return Paint::SetColorFilter(filter);
}

ECode CPaint::GetXfermode(
    /* [out] */ IXfermode** xfermode)
{
    VALIDATE_NOT_NULL(xfermode);
    return Paint::GetXfermode(xfermode);
}

ECode CPaint::SetXfermode(
    /* [in] */ IXfermode* xfermode)
{
    return Paint::SetXfermode(xfermode);
}

ECode CPaint::GetPathEffect(
    /* [out] */ IPathEffect** effect)
{
    VALIDATE_NOT_NULL(effect);
    return Paint::GetPathEffect(effect);
}

ECode CPaint::SetPathEffect(
    /* [in] */ IPathEffect* effect)
{
    return Paint::SetPathEffect(effect);
}

ECode CPaint::GetMaskFilter(
    /* [out] */ IMaskFilter** maskfilter)
{
    VALIDATE_NOT_NULL(maskfilter);
    return Paint::GetMaskFilter(maskfilter);
}

ECode CPaint::SetMaskFilter(
    /* [in] */ IMaskFilter* maskfilter)
{
    return Paint::SetMaskFilter(maskfilter);
}

ECode CPaint::GetTypeface(
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);
    return Paint::GetTypeface(typeface);
}

ECode CPaint::SetTypeface(
    /* [in] */ ITypeface* typeface)
{
    return Paint::SetTypeface(typeface);
}

ECode CPaint::GetRasterizer(
    /* [out] */ IRasterizer** rasterizer)
{
    VALIDATE_NOT_NULL(rasterizer);
    return Paint::GetRasterizer(rasterizer);
}

ECode CPaint::SetRasterizer(
    /* [in] */ IRasterizer* rasterizer)
{
    return Paint::SetRasterizer(rasterizer);
}

ECode CPaint::SetShadowLayer(
    /* [in] */ Float radius,
    /* [in] */ Float rdx,
    /* [in] */ Float rdy,
    /* [in] */ Int32 color)
{
    return Paint::SetShadowLayer(radius, rdx, rdy, color);
}

ECode CPaint::ClearShadowLayer()
{
    return Paint::ClearShadowLayer();
}

ECode CPaint::GetTextAlign(
    /* [out] */ PaintAlign* align)
{
    VALIDATE_NOT_NULL(align);
    return Paint::GetTextAlign(align);
}

ECode CPaint::SetTextAlign(
    /* [in] */ PaintAlign align)
{
    return Paint::SetTextAlign(align);
}

ECode CPaint::GetTextSize(
    /* [out] */ Float* size)
{
    VALIDATE_NOT_NULL(size);
    return Paint::GetTextSize(size);
}

ECode CPaint::SetTextSize(
    /* [in] */ Float textSize)
{
    return Paint::SetTextSize(textSize);
}

ECode CPaint::GetTextScaleX(
    /* [out] */ Float* scaleX)
{
    VALIDATE_NOT_NULL(scaleX);
    return Paint::GetTextScaleX(scaleX);
}

ECode CPaint::SetTextScaleX(
    /* [in] */ Float scaleX)
{
    return Paint::SetTextScaleX(scaleX);
}

ECode CPaint::GetTextSkewX(
    /* [out] */ Float* skewX)
{
    VALIDATE_NOT_NULL(skewX);
    return Paint::GetTextSkewX(skewX);
}

ECode CPaint::SetTextSkewX(
    /* [in] */ Float skewX)
{
    return Paint::SetTextSkewX(skewX);
}

ECode CPaint::Ascent(
    /* [out] */ Float* distance)
{
    VALIDATE_NOT_NULL(distance);
    return Paint::Ascent(distance);
}

ECode CPaint::Descent(
    /* [out] */ Float* distance)
{
    VALIDATE_NOT_NULL(distance);
    return Paint::Descent(distance);
}

ECode CPaint::GetFontMetrics(
    /* [in] */ IPaintFontMetrics* metrics,
    /* [out] */ Float* spacing)
{
    VALIDATE_NOT_NULL(spacing);
    return Paint::GetFontMetrics(metrics, spacing);
}

ECode CPaint::GetFontMetrics(
    /* [out] */ IPaintFontMetrics** metrics)
{
    VALIDATE_NOT_NULL(metrics);
    return Paint::GetFontMetrics(metrics);
}

ECode CPaint::GetFontMetricsInt(
    /* [in] */ IPaintFontMetricsInt* fmi,
    /* [out] */ Int32* spacing)
{
    VALIDATE_NOT_NULL(spacing);
    return Paint::GetFontMetricsInt(fmi, spacing);
}

ECode CPaint::GetFontMetricsInt(
    /* [out] */ IPaintFontMetricsInt** fmi)
{
    VALIDATE_NOT_NULL(fmi);
    return Paint::GetFontMetricsInt(fmi);
}

ECode CPaint::GetFontSpacing(
    /* [out] */ Float* spacing)
{
    VALIDATE_NOT_NULL(spacing);
    return Paint::GetFontSpacing(spacing);
}

ECode CPaint::MeasureText(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    return Paint::MeasureText(text, index, count, width);
}

ECode CPaint::MeasureText(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    return Paint::MeasureText(text, start, end, width);
}

ECode CPaint::MeasureText(
    /* [in] */ const String& text,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    return Paint::MeasureText(text, width);
}

ECode CPaint::MeasureText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    return Paint::MeasureText(text, start, end, width);
}

ECode CPaint::BreakText(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return Paint::BreakText(text, index, count, maxWidth, measuredWidth, number);
}

ECode CPaint::BreakText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Boolean measureForwards,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return Paint::BreakText(text, start, end, measureForwards, maxWidth, measuredWidth, number);
}

ECode CPaint::BreakText(
    /* [in] */ const String& text,
    /* [in] */ Boolean measureForwards,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return Paint::BreakText(text, measureForwards, maxWidth, measuredWidth, number);
}

ECode CPaint::GetTextWidths(
    /* [in] */ const ArrayOf<Char32> & text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32 * width)
{
    VALIDATE_NOT_NULL(width);
    return Paint::GetTextWidths(text, index, count, widths, width);
}

ECode CPaint::GetTextWidths(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    return Paint::GetTextWidths(text, start, end, widths, width);
}

ECode CPaint::GetTextWidths(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    return Paint::GetTextWidths(text, start, end, widths, width);
}

ECode CPaint::GetTextWidths(
    /* [in] */ const String& text,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    return Paint::GetTextWidths(text, widths, width);
}

ECode CPaint::GetTextPath(
    /* [in] */ const ArrayOf<Char32> & text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPath* path)
{
    return Paint::GetTextPath(text, index, count, x, y, path);
}

ECode CPaint::GetTextPath(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPath* path)
{
    return Paint::GetTextPath(text, start, end, x, y, path);
}

ECode CPaint::GetTextBounds(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IRect* bounds)
{
    return Paint::GetTextBounds(text, start, end, bounds);
}

ECode CPaint::GetTextBounds(
    /* [in] */ const ArrayOf<Char32> & text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ IRect* bounds)
{
    return Paint::GetTextBounds(text, index, count, bounds);
}

ECode CPaint::SetTextLocale(
    /* [in] */ ILocale* locale)
{
    return Paint::SetTextLocale(locale);
}

ECode CPaint::GetTextLocale(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);
    return Paint::GetTextLocale(locale);
}

ECode CPaint::GetBidiFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    return Paint::GetBidiFlags(flags);
}

ECode CPaint::SetBidiFlags(
    /* [in] */ Int32 flags)
{
    return Paint::SetBidiFlags(flags);
}

ECode CPaint::GetHinting(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode)
    return Paint::GetHinting(mode);
}

ECode CPaint::SetHinting(
    /* [in] */ Int32 flags)
{
    return Paint::SetBidiFlags(flags);
}

ECode CPaint::GetTextGlyphs(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [out] */ ArrayOf<Char32>* glyphs,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return Paint::GetTextGlyphs(text, start, end, contextStart, contextEnd,
            flags, glyphs, number);
}

ECode CPaint::GetTextRunAdvances(
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextIndex,
    /* [in] */ Int32 contextCount,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [out] */ Float* advance)
{
    VALIDATE_NOT_NULL(advance);
    return Paint::GetTextRunAdvances(chars, index, count, contextIndex,
                contextCount, flags, advances, advancesIndex, advance);
}

ECode CPaint::GetTextRunAdvances(
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextIndex,
    /* [in] */ Int32 contextCount,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [in] */ Int32 reserved,
    /* [out] */ Float* advance)
{
    VALIDATE_NOT_NULL(advance);
    return Paint::GetTextRunAdvances(chars, index, count, contextIndex, contextCount,
            flags, advances, advancesIndex, reserved, advance);
}

ECode CPaint::GetTextRunAdvances(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [out] */ Float* advance)
{
    VALIDATE_NOT_NULL(advance);
    return Paint::GetTextRunAdvances(text, start, end, contextStart, contextEnd,
            flags, advances, advancesIndex, advance);
}

ECode CPaint::GetTextRunAdvances(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [in] */ Int32 reserved,
    /* [out] */ Float* advance)
{
    VALIDATE_NOT_NULL(advance);
     return Paint::GetTextRunAdvances(text, start, end, contextStart, contextEnd,
            flags, advances, advancesIndex, reserved, advance);
}

ECode CPaint::GetTextRunAdvances(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [out] */ Float* advance)
{
    VALIDATE_NOT_NULL(advance);
    return Paint::GetTextRunAdvances(text, start, end, contextStart,
            contextEnd, flags, advances, advancesIndex, advance);
}

ECode CPaint::GetTextRunAdvances(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [in] */ Int32 reserved,
    /* [out] */ Float* advance)
{
    VALIDATE_NOT_NULL(advance);
    return Paint::GetTextRunAdvances(text, start, end, contextStart,
            contextEnd, flags, advances, advancesIndex, reserved, advance);
}

ECode CPaint::GetTextRunCursor(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextLength,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return Paint::GetTextRunCursor(text, contextStart, contextLength,
            flags, offset, cursorOpt, position);
}

ECode CPaint::GetTextRunCursor(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt,
   /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return Paint::GetTextRunCursor(text, contextStart, contextEnd, flags,
            offset, cursorOpt, position);
}

ECode CPaint::GetTextRunCursor(
    /* [in] */ const String& text,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return Paint::GetTextRunCursor(text, contextStart, contextEnd, flags,
            offset, cursorOpt, position);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
