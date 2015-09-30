
#include "elastos/droid/text/style/CSuperscriptSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CSuperscriptSpan, SuperscriptSpan, SuperscriptSpan)
IMETRICAFFECTINGSPAN_METHODS_IMPL(CSuperscriptSpan, SuperscriptSpan, SuperscriptSpan)
IPARCELABLESPAN_METHODS_IMPL(CSuperscriptSpan, SuperscriptSpan, SuperscriptSpan)

PInterface CSuperscriptSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CSuperscriptSpan::Probe(riid);
}

ECode CSuperscriptSpan::constructor()
{
    Init();
    return NOERROR;
}

ECode CSuperscriptSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos


