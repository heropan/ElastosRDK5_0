
#include "elastos/droid/text/style/CAbsoluteSizeSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CAbsoluteSizeSpan, AbsoluteSizeSpan, AbsoluteSizeSpan)
IMETRICAFFECTINGSPAN_METHODS_IMPL(CAbsoluteSizeSpan, AbsoluteSizeSpan, AbsoluteSizeSpan)
IPARCELABLESPAN_METHODS_IMPL(CAbsoluteSizeSpan, AbsoluteSizeSpan, AbsoluteSizeSpan)

PInterface CAbsoluteSizeSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CAbsoluteSizeSpan::Probe(riid);
}

ECode CAbsoluteSizeSpan::GetSize(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=AbsoluteSizeSpan::GetSize();
    return NOERROR;
}

ECode CAbsoluteSizeSpan::GetDip(
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=AbsoluteSizeSpan::GetDip();
    return NOERROR;
}

ECode CAbsoluteSizeSpan::constructor()
{
    return NOERROR;
}

ECode CAbsoluteSizeSpan::constructor(
    /* [in] */ Int32 size)
{
    Init(size);
    return NOERROR;
}

ECode CAbsoluteSizeSpan::constructor(
    /* [in] */ Int32 size,
    /* [in] */ Boolean dip)
{
    Init(size, dip);
    return NOERROR;
}

ECode CAbsoluteSizeSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos
