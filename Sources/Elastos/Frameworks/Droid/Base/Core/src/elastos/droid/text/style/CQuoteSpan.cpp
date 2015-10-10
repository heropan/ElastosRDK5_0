
#include "elastos/droid/text/style/CQuoteSpan.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ILEADINGMARGINSPAN_METHODS_IMPL(CQuoteSpan, QuoteSpan, QuoteSpan)
IPARCELABLESPAN_METHODS_IMPL(CQuoteSpan, QuoteSpan, QuoteSpan)

ECode CQuoteSpan::GetColor(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=QuoteSpan::GetColor();
    return NOERROR;
}

ECode CQuoteSpan::constructor()
{
    Init();
    return NOERROR;
}

ECode CQuoteSpan::constructor(
    /* [in] */ Int32 color)
{
    Init(color);
    return NOERROR;
}

ECode CQuoteSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

