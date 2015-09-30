#ifndef __ELASTOS_DROID_TEXT_STYLE_StyleSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_StyleSpan_H__

#include "elastos/droid/text/style/MetricAffectingSpan.h"
#include "Elastos.Droid.Core_server.h"

using Elastos::Droid::Graphics::IPaint;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/**
 *
 * Describes a style in a span.
 * Note that styles are cumulative -- if both bold and italic are set in
 * separate spans, or if the base style is bold and a span calls for italic,
 * you get bold italic.  You can't turn off a style from the base style.
 *
 */
class StyleSpan
    : public MetricAffectingSpan
{
public:
    /**
     *
     * @param style An integer constant describing the style for this span. Examples
     * include bold, italic, and normal. Values are constants defined
     * in {@link android.graphics.Typeface}.
     */
    StyleSpan(
        /* [in] */ Int32 style);

    StyleSpan(
        /* [in] */ IParcel* src);

    CARAPI_(void) Init(
        /* [in] */ Int32 style);

    CARAPI_(void) Init(
        /* [in] */ IParcel* src);

    CARAPI_(Int32) GetSpanTypeId();

    CARAPI_(Int32) DescribeContents();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    /**
     * Returns the style constant defined in {@link android.graphics.Typeface}.
     */
    CARAPI_(Int32) GetStyle();

    //@Override
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* ds);

    //@Override
    CARAPI UpdateMeasureState(
        /* [in] */ ITextPaint* paint);

protected:
    StyleSpan();

private:
    static CARAPI_(void) Apply(
        /* [in] */ IPaint* paint,
        /* [in] */ Int32 style);

private:
    /*const*/ Int32 mStyle;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_StyleSpan_H__
