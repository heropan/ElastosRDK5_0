
#ifndef __ELASTOS_DROID_TEXT_STYLE_CTYPEFACESPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CTYPEFACESPAN_H__

#include "_Elastos_Droid_Text_Style_CTypefaceSpan.h"
#include "elastos/droid/text/style/TypefaceSpan.h"




namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CTypefaceSpan), public TypefaceSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IMETRICAFFECTINGSPAN_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetFamily(
        /* [out] */ String* ret);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& family);

    CARAPI constructor(
        /* [in] */ IParcel* src);

};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CTYPEFACESPAN_H__
