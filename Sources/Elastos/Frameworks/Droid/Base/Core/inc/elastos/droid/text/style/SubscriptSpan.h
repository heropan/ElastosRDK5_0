#ifndef __ELASTOS_DROID_TEXT_STYLE_SubscriptSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_SubscriptSpan_H__

#include "elastos/droid/text/style/MetricAffectingSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class SubscriptSpan : public MetricAffectingSpan
{
public:
    SubscriptSpan();

    SubscriptSpan(
    /* [in] */ IParcel* src);

    CARAPI_(void) Init();

    CARAPI_(void) Init(
    /* [in] */ IParcel* src);

    CARAPI_(Int32) GetSpanTypeId();

    CARAPI_(Int32) DescribeContents();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    //@Override
    CARAPI UpdateDrawState(
    /* [in] */ ITextPaint* tp);

    //@Override
    CARAPI UpdateMeasureState(
    /* [in] */ ITextPaint* tp);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_SubscriptSpan_H__
