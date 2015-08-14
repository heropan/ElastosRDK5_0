#ifndef __ELASTOS_DROID_TEXT_STYLE_CLEADINGMARGINSPANSTANDARD_H__
#define __ELASTOS_DROID_TEXT_STYLE_CLEADINGMARGINSPANSTANDARD_H__

#include "_CLeadingMarginSpanStandard.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPaint;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/**
 * The standard implementation of LeadingMarginSpan, which adjusts the
 * margin but does not do any rendering.
 */
CarClass(CLeadingMarginSpanStandard)
{
public:
    CARAPI DescribeContents(
        /* [out] */ Int32* ret);

    CARAPI ReadFromParcel(
    /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetSpanTypeId(
        /* [out] */ Int32* ret);

    CARAPI GetLeadingMargin(
        /* [in] */ Boolean first,
        /* [out] */ Int32* offset);

    CARAPI DrawLeadingMargin(
        /* [in] */ ICanvas* c,
        /* [in] */ IPaint* p,
        /* [in] */ Int32 x,
        /* [in] */ Int32 dir,
        /* [in] */ Int32 top,
        /* [in] */ Int32 baseline,
        /* [in] */ Int32 bottom,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Boolean first,
        /* [in] */ ILayout* layout);

    CARAPI constructor();

    /**
     * Constructor taking separate indents for the first and subsequent
     * lines.
     *
     * @param first the indent for the first line of the paragraph
     * @param rest the indent for the remaining lines of the paragraph
     */
    CARAPI constructor(
        /* [in] */ Int32 first,
        /* [in] */ Int32 rest);

    /**
     * Constructor taking an indent for all lines.
     * @param every the indent of each line
     */
    CARAPI constructor(
        /* [in] */ Int32 every);

    CARAPI constructor(
        /* [in] */ IParcel* src);

private:
    /*const*/ Int32 mFirst, mRest;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CLEADINGMARGINSPANSTANDARD_H__
