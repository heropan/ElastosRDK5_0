#ifndef __ELASTOS_DROID_TEXT_STYLE_RelativeSizeSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_RelativeSizeSpan_H__

#include "text/style/MetricAffectingSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class RelativeSizeSpan : public MetricAffectingSpan
{
public:
    RelativeSizeSpan(
        /* [in] */ Float proportion);

    RelativeSizeSpan(
        /* [in] */ IParcel* src);

    CARAPI_(void) Init(
        /* [in] */ Float proportion);

    CARAPI_(void) Init(
        /* [in] */ IParcel* src);

    CARAPI_(Int32) GetSpanTypeId();

    CARAPI_(Int32) DescribeContents();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI_(Float) GetSizeChange();

    //@Override
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* ds);

    //@Override
    CARAPI UpdateMeasureState(
        /* [in] */ ITextPaint* ds);

protected:
    RelativeSizeSpan();

private:
    /*const*/ Float mProportion;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_RelativeSizeSpan_H__
