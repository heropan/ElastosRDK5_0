
#include "text/util/CLinkify.h"
#include "text/util/Linkify.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Utility {

ECode CLinkify::GetStaticUrlMatchFilter(
    /* [out] */ ILinkifyMatchFilter** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ILinkifyMatchFilter> lmf = Linkify::GetStaticUrlMatchFilter();
    *ret = lmf;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CLinkify::GetStaticPhoneNumberMatchFilter(
    /* [out] */ ILinkifyMatchFilter** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ILinkifyMatchFilter> lmf = Linkify::GetStaticPhoneNumberMatchFilter();
    *ret = lmf;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CLinkify::GetStaticPhoneNumberTransformFilter(
    /* [out] */ ILinkifyTransformFilter** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr< ILinkifyTransformFilter > ltf = Linkify::GetStaticPhoneNumberTransformFilter();
    *ret = ltf;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CLinkify::AddLinks(
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 mask,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    VALIDATE_NOT_NULL(text);
    *ret = Linkify::AddLinks(text, mask);
    return NOERROR;
}

ECode CLinkify::AddLinksEx(
    /* [in] */ ITextView* text,
    /* [in] */ Int32 mask,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    VALIDATE_NOT_NULL(text);
    *ret = Linkify::AddLinks(text, mask);
    return NOERROR;
}

ECode CLinkify::AddLinksEx2(
    /* [in] */ ITextView* text,
    /* [in] */ IPattern* pattern,
    /* [in] */ const String& scheme)
{
    VALIDATE_NOT_NULL(text);
    Linkify::AddLinks(text, pattern, scheme);
    return NOERROR;
}

ECode CLinkify::AddLinksEx3(
    /* [in] */ ITextView* text,
    /* [in] */ IPattern* p,
    /* [in] */ const String& scheme,
    /* [in] */ ILinkifyMatchFilter* matchFilter,
    /* [in] */ ILinkifyTransformFilter* transformFilter)
{
    VALIDATE_NOT_NULL(text);
    Linkify::AddLinks(text, p, scheme, matchFilter, transformFilter);
    return NOERROR;
}

ECode CLinkify::AddLinksEx4(
    /* [in] */ ISpannable* text,
    /* [in] */ IPattern* pattern,
    /* [in] */ const String& scheme,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    VALIDATE_NOT_NULL(text);
    *ret = Linkify::AddLinks(text, pattern, scheme);
    return NOERROR;
}

ECode CLinkify::AddLinksEx5(
    /* [in] */ ISpannable* s,
    /* [in] */ IPattern* p,
    /* [in] */ const String& scheme,
    /* [in] */ ILinkifyMatchFilter* matchFilter,
    /* [in] */ ILinkifyTransformFilter* transformFilter,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    VALIDATE_NOT_NULL(s);
    *ret = Linkify::AddLinks(s, p, scheme, matchFilter, transformFilter);
    return NOERROR;
}

} // namespace Utility
} // namespace Text
} // namepsace Droid
} // namespace Elastos
