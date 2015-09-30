#ifndef __ELASTOS_DROID_TEXT_STYLE_BackgroundColorSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_BackgroundColorSpan_H__

#include "elastos/droid/text/style/CharacterStyle.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class BackgroundColorSpan
    : public CharacterStyle
{
public:
    BackgroundColorSpan(
        /* [in] */ Int32 color);

    BackgroundColorSpan(
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

    CARAPI_(Int32) GetBackgroundColor();

    //@Override
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* ds);

protected:
    BackgroundColorSpan();

private:
    /*const*/ Int32 mColor;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_BackgroundColorSpan_H__
