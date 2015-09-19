
#ifndef __ELASTOS_DROID_TEXT_STYLE_CSCALEXSPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CSCALEXSPAN_H__

#include "_Elastos_Droid_Text_Style_CScaleXSpan.h"
#include "text/style/ScaleXSpan.h"




namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CScaleXSpan), public ScaleXSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IMETRICAFFECTINGSPAN_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetScaleX(
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

#endif // __ELASTOS_DROID_TEXT_STYLE_CSCALEXSPAN_H__
