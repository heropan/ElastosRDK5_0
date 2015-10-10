
#include "elastos/droid/text/style/CBackgroundColorSpan.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CBackgroundColorSpan, BackgroundColorSpan, BackgroundColorSpan)
IPARCELABLESPAN_METHODS_IMPL(CBackgroundColorSpan, BackgroundColorSpan, BackgroundColorSpan)

PInterface CBackgroundColorSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CBackgroundColorSpan::Probe(riid);
}

ECode CBackgroundColorSpan::GetBackgroundColor(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result=BackgroundColorSpan::GetBackgroundColor();
    return NOERROR;
}

ECode CBackgroundColorSpan::constructor()
{
    return NOERROR;
}

ECode CBackgroundColorSpan::constructor(
    /* [in] */ Int32 color)
{
    Init(color);
    return NOERROR;
}

ECode CBackgroundColorSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}


} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

