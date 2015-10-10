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
    virtual CARAPI SetBgColor(
        /* [in] */ Int32 bgColor);

    virtual CARAPI GetBgColor(
        /* [out] */ Int32* color);

    virtual CARAPI SetBaselineShift(
        /* [in] */ Int32 baselineShift);

    virtual CARAPI GetBaselineShift(
        /* [out] */ Int32* shift);

    virtual CARAPI SetLinkColor(
        /* [in] */ Int32 linkColor);

    virtual CARAPI GetLinkColor(
        /* [out] */ Int32* color);

    virtual CARAPI SetDrawableState(
        /* [in] */ ArrayOf<Int32>* drawableState);

    virtual CARAPI GetDrawableState(
        /* [out, callee] */ ArrayOf<Int32>** state);

    virtual CARAPI SetDensity(
        /* [in] */ Float density);

    virtual CARAPI GetDensity(
        /* [out] */ Float* density);

    /**
     * Special value 0 means no custom underline
     * @hide
     */
    virtual CARAPI SetUnderlineColor(
        /* [in] */ Int32 underlineColor);

    virtual CARAPI GetUnderlineColor(
        /* [out] */ Int32* color);

    /**
     * Defined as a multiplier of the default underline thickness. Use 1.0f for default thickness.
     * @hide
     */
    virtual CARAPI SetUnderlineThickness(
        /* [in] */ Float underlineThickness);

    virtual CARAPI GetUnderlineThickness(
        /* [out] */ Float* underlineThickness);

    /**
     * Copy the fields from tp into this TextPaint, including the
     * fields inherited from Paint.
     */
    virtual CARAPI Set(
        /* [in] */ ITextPaint* tp);

    /**
     * Defines a custom underline for this Paint.
     * @param color underline solid color
     * @param thickness underline thickness
     * @hide
     */
    virtual CARAPI SetUnderlineText(
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
