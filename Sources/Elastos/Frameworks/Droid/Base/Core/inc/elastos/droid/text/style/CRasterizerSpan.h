
#ifndef __ELASTOS_DROID_TEXT_STYLE_CRASTERIZERSPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CRASTERIZERSPAN_H__

#include "_Elastos_Droid_Text_Style_CRasterizerSpan.h"
#include "elastos/droid/text/style/RasterizerSpan.h"


namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CRasterizerSpan), public RasterizerSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetRasterizer(
        /* [out] */ IRasterizer** ret);

    CARAPI constructor(
        /* [in] */ IRasterizer* r);

};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CRASTERIZERSPAN_H__
