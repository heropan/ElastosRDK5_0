
#include "elastos/droid/text/style/CURLSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CURLSpan, URLSpan, URLSpan)
IPARCELABLESPAN_METHODS_IMPL(CURLSpan, URLSpan, URLSpan)

PInterface CURLSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CURLSpan::Probe(riid);
}

ECode CURLSpan::OnClick(
    /* [in] */ IView* widget)
{
    URLSpan::OnClick(widget);
    return NOERROR;
}

ECode CURLSpan::GetURL(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    *url=URLSpan::GetURL();
    return NOERROR;
}

ECode CURLSpan::constructor()
{
    return NOERROR;
}

ECode CURLSpan::constructor(
    /* [in] */ const String& url)
{
    Init(url);
    return NOERROR;
}

ECode CURLSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos


