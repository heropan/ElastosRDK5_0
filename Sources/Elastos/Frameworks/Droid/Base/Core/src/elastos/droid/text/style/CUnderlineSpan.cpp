
#include "elastos/droid/text/style/CUnderlineSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CUnderlineSpan, UnderlineSpan, UnderlineSpan)
IPARCELABLESPAN_METHODS_IMPL(CUnderlineSpan, UnderlineSpan, UnderlineSpan)

ECode CUnderlineSpan::constructor()
{
    Init();
    return NOERROR;
}

ECode CUnderlineSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

PInterface CUnderlineSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CUnderlineSpan::Probe(riid);
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos


