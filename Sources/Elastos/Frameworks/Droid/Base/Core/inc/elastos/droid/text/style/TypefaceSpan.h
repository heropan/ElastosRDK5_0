#ifndef __ELASTOS_DROID_TEXT_STYLE_TypefaceSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_TypefaceSpan_H__

#include "elastos/droid/text/style/MetricAffectingSpan.h"
#include "Elastos.Droid.Core_server.h"

using Elastos::Droid::Graphics::IPaint;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/**
 * Changes the typeface family of the text to which the span is attached.
 */
class TypefaceSpan : public MetricAffectingSpan
{
public:
    /**
     * @param family The font family for this typeface.  Examples include
     * "monospace", "serif", and "sans-serif".
     */
    TypefaceSpan(
        /* [in] */ const String& family);

    TypefaceSpan(
        /* [in] */ IParcel* src);

    CARAPI_(void) Init(
        /* [in] */ const String& family);

    CARAPI_(void) Init(
        /* [in] */ IParcel* src);

    CARAPI_(Int32) GetSpanTypeId();

    CARAPI_(Int32) DescribeContents();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    /**
     * Returns the font family name.
     */
    CARAPI_(String) GetFamily();

    //@Override
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* ds);

    //@Override
    CARAPI UpdateMeasureState(
        /* [in] */ ITextPaint* paint);

protected:
    TypefaceSpan();

private:
    static void Apply(
        /* [in] */ IPaint* paint,
        /* [in] */ const String& family);

private:
    /*const*/ String mFamily;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_TypefaceSpan_H__
