
#ifndef __CRASTERIZERSPAN_H__
#define __CRASTERIZERSPAN_H__

#include "_CRasterizerSpan.h"
#include "text/style/RasterizerSpan.h"


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

#endif // __CRASTERIZERSPAN_H__
