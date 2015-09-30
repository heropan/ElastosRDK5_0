
#include "elastos/droid/text/style/CTypefaceSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CTypefaceSpan, TypefaceSpan, TypefaceSpan)
IMETRICAFFECTINGSPAN_METHODS_IMPL(CTypefaceSpan, TypefaceSpan, TypefaceSpan)
IPARCELABLESPAN_METHODS_IMPL(CTypefaceSpan, TypefaceSpan, TypefaceSpan)

PInterface CTypefaceSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CTypefaceSpan::Probe(riid);
}

ECode CTypefaceSpan::GetFamily(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TypefaceSpan::GetFamily();
    return NOERROR;
}

ECode CTypefaceSpan::constructor(
    /* [in] */ const String& family)
{
    Init(family);
    return NOERROR;
}

ECode CTypefaceSpan::constructor()
{
    return NOERROR;
}

ECode CTypefaceSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos


