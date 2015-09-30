#ifndef __ELASTOS_DROID_TEXT_STYLE_LocaleSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_LocaleSpan_H__

#include "elastos/droid/text/style/MetricAffectingSpan.h"

using Libcore::ICU::ILocale;
using Elastos::Droid::Graphics::IPaint;
//using IParcel;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/**
 * Changes the {@link Locale} of the text to which the span is attached.
 */
class LocaleSpan
    : public MetricAffectingSpan
{
public:
    /**
     * Creates a LocaleSpan.
     * @param locale The {@link Locale} of the text to which the span is
     * attached.
     */
    LocaleSpan(
        /* [in] */ ILocale* locale);

    LocaleSpan(
        /* [in] */ IParcel* src);

    CARAPI_(void) Init(
        /* [in] */ ILocale* locale);

    CARAPI_(void) Init(
        /* [in] */ IParcel* src);

    //@Override
    CARAPI_(Int32) GetSpanTypeId();

    //@Override
    CARAPI_(Int32) DescribeContents();

    //@Override
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    /**
     * Returns the {@link Locale}.
     *
     * @return The {@link Locale} for this span.
     */
    CARAPI_(AutoPtr<ILocale>) GetLocale();

    //@Override
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* ds);

    //@Override
    CARAPI UpdateMeasureState(
        /* [in] */ ITextPaint* paint);

protected:
    LocaleSpan();

private:
    static CARAPI_(void) Apply(
        /* [in] */ IPaint* paint,
        /* [in] */ ILocale* locale);

private:
    AutoPtr<ILocale> mLocale;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_LocaleSpan_H__
