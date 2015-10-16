#ifndef __ELASTOS_DROID_TEXT_STYLE_ScaleXSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_ScaleXSpan_H__

#include "elastos/droid/text/style/MetricAffectingSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class ScaleXSpan
    : public MetricAffectingSpan
    , public IScaleXSpan
    , public IParcelableSpan
    , public IParcelable

{
public:
    ScaleXSpan(
        /* [in] */ Float proportion);

    ScaleXSpan(
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

    CARAPI_(Float) GetScaleX();

    //@Override
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* ds);

    //@Override
    CARAPI UpdateMeasureState(
        /* [in] */ ITextPaint* ds);

protected:
    ScaleXSpan();

private:
    Float mProportion;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_ScaleXSpan_H__
