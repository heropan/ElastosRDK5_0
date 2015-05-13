#ifndef __StrikethroughSpan_H__
#define __StrikethroughSpan_H__

#include "text/style/CharacterStyle.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class StrikethroughSpan : public CharacterStyle
{
public:
    StrikethroughSpan();

    StrikethroughSpan(
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
        /* [in] */ ITextPaint* ds);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __StrikethroughSpan_H__
