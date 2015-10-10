
#include "elastos/droid/text/style/CRelativeSizeSpan.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CRelativeSizeSpan, RelativeSizeSpan, RelativeSizeSpan)
IMETRICAFFECTINGSPAN_METHODS_IMPL(CRelativeSizeSpan, RelativeSizeSpan, RelativeSizeSpan)
IPARCELABLESPAN_METHODS_IMPL(CRelativeSizeSpan, RelativeSizeSpan, RelativeSizeSpan)

PInterface CRelativeSizeSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CRelativeSizeSpan::Probe(riid);
}

ECode CRelativeSizeSpan::GetSizeChange(
    /* [out] */ Float* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=RelativeSizeSpan::GetSizeChange();
    return NOERROR;
}

ECode CRelativeSizeSpan::constructor()
{
    return NOERROR;
}

ECode CRelativeSizeSpan::constructor(
    /* [in] */ Float proportion)
{
    Init(proportion);
    return NOERROR;
}

ECode CRelativeSizeSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos
