
#include "text/style/CStyleSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CStyleSpan, StyleSpan, StyleSpan)
IMETRICAFFECTINGSPAN_METHODS_IMPL(CStyleSpan, StyleSpan, StyleSpan)
IPARCELABLESPAN_METHODS_IMPL(CStyleSpan, StyleSpan, StyleSpan)

PInterface CStyleSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CStyleSpan::Probe(riid);
}

ECode CStyleSpan::GetStyle(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = StyleSpan::GetStyle();
    return NOERROR;
}

ECode CStyleSpan::constructor()
{
    return NOERROR;
}

ECode CStyleSpan::constructor(
    /* [in] */ Int32 style)
{
    Init(style);
    return NOERROR;
}

ECode CStyleSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

