
#include "text/style/CSubscriptSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CSubscriptSpan, SubscriptSpan, SubscriptSpan)
IMETRICAFFECTINGSPAN_METHODS_IMPL(CSubscriptSpan, SubscriptSpan, SubscriptSpan)
IPARCELABLESPAN_METHODS_IMPL(CSubscriptSpan, SubscriptSpan, SubscriptSpan)

PInterface CSubscriptSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CSubscriptSpan::Probe(riid);
}

ECode CSubscriptSpan::constructor()
{
    Init();
    return NOERROR;
}

ECode CSubscriptSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

