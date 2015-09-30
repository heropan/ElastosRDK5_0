
#ifndef __ELASTOS_DROID_TEXT_STYLE_CBACKGROUNDCOLORSPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CBACKGROUNDCOLORSPAN_H__

#include "_Elastos_Droid_Text_Style_CBackgroundColorSpan.h"
#include "elastos/droid/text/style/BackgroundColorSpan.h"



namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CBackgroundColorSpan), public BackgroundColorSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetBackgroundColor(
        /* [out] */ Int32* result);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 color);

    CARAPI constructor(
        /* [in] */ IParcel* src);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CBACKGROUNDCOLORSPAN_H__
