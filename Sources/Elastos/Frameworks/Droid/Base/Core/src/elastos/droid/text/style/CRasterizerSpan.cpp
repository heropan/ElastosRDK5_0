
#include "elastos/droid/text/style/CRasterizerSpan.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CRasterizerSpan, RasterizerSpan, RasterizerSpan)

PInterface CRasterizerSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CRasterizerSpan::Probe(riid);
}

ECode CRasterizerSpan::GetRasterizer(
    /* [out] */ IRasterizer** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IRasterizer> r = RasterizerSpan::GetRasterizer();
    *ret = r;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CRasterizerSpan::constructor(
    /* [in] */ IRasterizer* r)
{
    Init(r);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

