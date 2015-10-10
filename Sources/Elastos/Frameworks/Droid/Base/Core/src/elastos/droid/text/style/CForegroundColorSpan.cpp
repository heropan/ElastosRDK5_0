
#include "elastos/droid/text/style/CForegroundColorSpan.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CForegroundColorSpan, ForegroundColorSpan, ForegroundColorSpan)
IPARCELABLESPAN_METHODS_IMPL(CForegroundColorSpan, ForegroundColorSpan, ForegroundColorSpan)

PInterface CForegroundColorSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CForegroundColorSpan::Probe(riid);
}

ECode CForegroundColorSpan::GetForegroundColor(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result=ForegroundColorSpan::GetForegroundColor();
    return NOERROR;
}

ECode CForegroundColorSpan::constructor(
    /* [in] */ Int32 color)
{
    Init(color);
    return NOERROR;
}

ECode CForegroundColorSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

