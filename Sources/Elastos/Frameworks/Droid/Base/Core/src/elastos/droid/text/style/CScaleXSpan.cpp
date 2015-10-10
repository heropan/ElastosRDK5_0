
#include "elastos/droid/text/style/CScaleXSpan.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CScaleXSpan, ScaleXSpan, ScaleXSpan)
IMETRICAFFECTINGSPAN_METHODS_IMPL(CScaleXSpan, ScaleXSpan, ScaleXSpan)
IPARCELABLESPAN_METHODS_IMPL(CScaleXSpan, ScaleXSpan, ScaleXSpan)

PInterface CScaleXSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CScaleXSpan::Probe(riid);
}

ECode CScaleXSpan::GetScaleX(
    /* [out] */ Float* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=ScaleXSpan::GetScaleX();
    return NOERROR;
}

ECode CScaleXSpan::constructor()
{
    return NOERROR;
}

ECode CScaleXSpan::constructor(
    /* [in] */ Float proportion)
{
    Init(proportion);
    return NOERROR;
}

ECode CScaleXSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

