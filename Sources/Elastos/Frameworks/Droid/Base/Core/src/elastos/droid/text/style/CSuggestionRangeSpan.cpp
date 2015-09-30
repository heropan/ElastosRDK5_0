
#include "elastos/droid/text/style/CSuggestionRangeSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CSuggestionRangeSpan, SuggestionRangeSpan, SuggestionRangeSpan)
IPARCELABLESPAN_METHODS_IMPL(CSuggestionRangeSpan, SuggestionRangeSpan, SuggestionRangeSpan)

PInterface CSuggestionRangeSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CSuggestionRangeSpan::Probe(riid);
}

ECode CSuggestionRangeSpan::SetBackgroundColor(
    /* [in] */ Int32 backgroundColor)
{
    SuggestionRangeSpan::SetBackgroundColor(backgroundColor);
    return NOERROR;
}

ECode CSuggestionRangeSpan::constructor()
{
    Init();
    return NOERROR;
}

ECode CSuggestionRangeSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

