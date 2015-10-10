#ifndef __ELASTOS_DROID_TEXT_TEXTPAINT_H__
#define __ELASTOS_DROID_TEXT_TEXTPAINT_H__

#include "elastos/droid/graphics/Paint.h"

using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::Paint;

namespace Elastos {
namespace Droid {
namespace Text {

/**
 * TextPaint is an extension of Paint that leaves room for some extra
 * data used during text measuring and drawing.
 */
class TextPaint
    : public Elastos::Droid::Graphics::Paint
{
public:
    TextPaint();

    virtual ~TextPaint();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ IPaint* p);

    // Special value 0 means no background paint
    CARAPI SetBgColor(
        /* [in] */ Int32 bgColor);

    CARAPI GetBgColor(
        /* [out] */ Int32* color);

    CARAPI SetBaselineShift(
        /* [in] */ Int32 baselineShift);

    CARAPI GetBaselineShift(
        /* [out] */ Int32* shift);

    CARAPI SetLinkColor(
        /* [in] */ Int32 linkColor);

    CARAPI GetLinkColor(
        /* [out] */ Int32* color);

    CARAPI SetDrawableState(
        /* [in] */ ArrayOf<Int32>* drawableState);

    CARAPI GetDrawableState(
        /* [out, callee] */ ArrayOf<Int32>** state);

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
        /* [out] */ Int32* color);

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

    using Paint::SetUnderlineText;

public:
    // Special value 0 means no background paint
    Int32 mBgColor;
    Int32 mBaselineShift;
    Int32 mLinkColor;
    AutoPtr< ArrayOf<Int32> > mDrawableState;
    Float mDensity;// = 1.0f;
    /**
     * Special value 0 means no custom underline
     * @hide
     */
    Int32 mUnderlineColor;// = 0;

    /**
     * Defined as a multiplier of the default underline thickness. Use 1.0f for default thickness.
     * @hide
     */
    Float mUnderlineThickness;
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_TEXT_TEXTPAINT_H__
