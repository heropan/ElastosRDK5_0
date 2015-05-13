
#include "text/style/CMaskFilterSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CMaskFilterSpan, MaskFilterSpan, MaskFilterSpan)

PInterface CMaskFilterSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CMaskFilterSpan::Probe(riid);
}

ECode CMaskFilterSpan::GetMaskFilter(
    /* [out] */ IMaskFilter** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IMaskFilter> mf = MaskFilterSpan::GetMaskFilter();
    *ret = mf;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CMaskFilterSpan::constructor(
    /* [in] */ IMaskFilter* filter)
{
    MaskFilterSpan::Init(filter);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

