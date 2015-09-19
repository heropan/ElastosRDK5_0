
#ifndef __ELASTOS_DROID_TEXT_STYLE_CFOREGROUNDCOLORSPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CFOREGROUNDCOLORSPAN_H__

#include "_Elastos_Droid_Text_Style_CForegroundColorSpan.h"
#include "text/style/ForegroundColorSpan.h"



namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CForegroundColorSpan), public ForegroundColorSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetForegroundColor(
        /* [out] */ Int32* result);

    CARAPI constructor(
        /* [in] */ Int32 color);

    CARAPI constructor(
        /* [in] */ IParcel* src);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CFOREGROUNDCOLORSPAN_H__
