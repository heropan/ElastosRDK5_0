#ifndef __ELASTOS_DROID_TEXT_STYLE_ForegroundColorSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_ForegroundColorSpan_H__

#include "elastos/droid/text/style/CharacterStyle.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class ForegroundColorSpan : public CharacterStyle
{
public:
    ForegroundColorSpan(
        /* [in] */ Int32 color);

    ForegroundColorSpan(
        /* [in] */ IParcel* src);

    CARAPI_(void) Init(
        /* [in] */ Int32 color);

    CARAPI_(void) Init(
        /* [in] */ IParcel* src);

    CARAPI_(Int32) GetSpanTypeId();

    CARAPI_(Int32) DescribeContents();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI_(Int32) GetForegroundColor();

    //@Override
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* ds);

protected:
    ForegroundColorSpan();

private:
    /*const*/ Int32 mColor;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_ForegroundColorSpan_H__
