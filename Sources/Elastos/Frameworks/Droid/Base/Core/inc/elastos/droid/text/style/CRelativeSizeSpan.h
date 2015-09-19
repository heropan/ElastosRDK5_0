
#ifndef __ELASTOS_DROID_TEXT_STYLE_CRELATIVESIZESPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CRELATIVESIZESPAN_H__

#include "_Elastos_Droid_Text_Style_CRelativeSizeSpan.h"
#include "text/style/RelativeSizeSpan.h"




namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CRelativeSizeSpan)
    , public RelativeSizeSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IMETRICAFFECTINGSPAN_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetSizeChange(
        /* [out] */ Float* ret);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Float proportion);

    CARAPI constructor(
        /* [in] */ IParcel* src);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CRELATIVESIZESPAN_H__
