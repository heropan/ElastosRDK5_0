
#include "text/style/CStrikethroughSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CStrikethroughSpan, StrikethroughSpan, StrikethroughSpan)
IPARCELABLESPAN_METHODS_IMPL(CStrikethroughSpan, StrikethroughSpan, StrikethroughSpan)

PInterface CStrikethroughSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CStrikethroughSpan::Probe(riid);
}

ECode CStrikethroughSpan::constructor()
{
    Init();
    return NOERROR;
}

ECode CStrikethroughSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

