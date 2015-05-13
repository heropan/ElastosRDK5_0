
#include "text/style/CLocaleSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CLocaleSpan, LocaleSpan, LocaleSpan)
IMETRICAFFECTINGSPAN_METHODS_IMPL(CLocaleSpan, LocaleSpan, LocaleSpan)
IPARCELABLESPAN_METHODS_IMPL(CLocaleSpan, LocaleSpan, LocaleSpan)

PInterface CLocaleSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CLocaleSpan::Probe(riid);
}

ECode CLocaleSpan::GetLocale(
    /* [out] */ ILocale** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ILocale> l = LocaleSpan::GetLocale();
    *ret = l;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CLocaleSpan::constructor()
{
    return NOERROR;
}

ECode CLocaleSpan::constructor(
    /* [in] */ ILocale* locale)
{
    LocaleSpan::Init(locale);
    return NOERROR;
}

ECode CLocaleSpan::constructor(
    /* [in] */ IParcel* src)
{
    LocaleSpan::Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos
