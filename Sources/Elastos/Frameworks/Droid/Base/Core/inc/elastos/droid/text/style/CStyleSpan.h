
#ifndef __ELASTOS_DROID_TEXT_STYLE_CSTYLESPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CSTYLESPAN_H__

#include "_Elastos_Droid_Text_Style_CStyleSpan.h"
#include "elastos/droid/text/style/StyleSpan.h"




namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CStyleSpan), public StyleSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IMETRICAFFECTINGSPAN_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetStyle(
        /* [out] */ Int32* ret);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 style);

    CARAPI constructor(
        /* [in] */ IParcel* src);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CSTYLESPAN_H__
