#include "text/util/Linkify.h"
#include "text/CSpannableStringHelper.h"
#include "text/method/CLinkMovementMethod.h"
#include "text/style/CURLSpan.h"
//#include "webkit/CWebView.h"
#include "util/Patterns.h"
#include <elastos/Character.h>

using Elastos::Core::Character;
using Elastos::Net::CURLEncoder;
using Elastos::Net::CURLEncoder;
using Elastos::Net::IURLEncoder;
using Elastos::Core::EIID_IComparator;
using Elastos::Utility::Regex::IPattern;
using Elastos::Droid::Utility::Patterns;
using Elastos::Droid::Text::ISpannableStringHelper;
using Elastos::Droid::Text::CSpannableStringHelper;
using Elastos::Droid::Text::Method::IMovementMethod;
using Elastos::Droid::Text::Method::ILinkMovementMethod;
using Elastos::Droid::Text::Method::EIID_ILinkMovementMethod;
using Elastos::Droid::Text::Method::CLinkMovementMethod;
using Elastos::Droid::Text::Style::IURLSpan;
using Elastos::Droid::Text::Style::EIID_IURLSpan;
using Elastos::Droid::Text::Style::CURLSpan;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Utility {

const AutoPtr<ILinkifyMatchFilter> Linkify::sUrlMatchFilter = new Linkify::MatchFilterUrl();
const AutoPtr<ILinkifyMatchFilter> Linkify::sPhoneNumberMatchFilter = new Linkify::MatchFilterPhoneNumber();
const AutoPtr<ILinkifyTransformFilter> Linkify::sPhoneNumberTransformFilter = new Linkify::TransformFilterPhoneNumber();

const Int32 Linkify::PHONE_NUMBER_MINIMUM_DIGITS = 5;



/*****************************Linkify::MatchFilterUrl*****************************/
CAR_INTERFACE_IMPL(Linkify::MatchFilterUrl, ILinkifyMatchFilter)

ECode Linkify::MatchFilterUrl::AcceptMatch(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TRUE;
    VALIDATE_NOT_NULL(s);

    if (start == 0) {
        return NOERROR;
    }

    Char32 c;
    s->GetCharAt(start - 1, &c);
    if (c == '@') {
        *ret = FALSE;
        return NOERROR;
    }

    return NOERROR;
}

/*****************************Linkify::MatchFilterPhoneNumber*****************************/
CAR_INTERFACE_IMPL(Linkify::MatchFilterPhoneNumber, ILinkifyMatchFilter)

ECode Linkify::MatchFilterPhoneNumber::AcceptMatch(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = FALSE;
    VALIDATE_NOT_NULL(s);

    Int32 digitCount = 0;
    Char32 c;
    for (Int32 i = start; i < end; i++) {
        s->GetCharAt(i, &c);
        if (Character::IsDigit(c)) {
            digitCount++;
            if (digitCount >= PHONE_NUMBER_MINIMUM_DIGITS) {
                *ret = TRUE;
                return NOERROR;
            }
        }
    }

    return NOERROR;
}

/*****************************Linkify::TransformFilterPhoneNumber*****************************/
CAR_INTERFACE_IMPL(Linkify::TransformFilterPhoneNumber, ILinkifyTransformFilter)

ECode Linkify::TransformFilterPhoneNumber::TransformUrl(
    /* [in] */ IMatcher* match,
    /* [in] */ const String & url,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Patterns::DigitsAndPlusOnly(match);
    return NOERROR;
}

/*****************************Linkify::ComparatorLinkSpec*****************************/

Int32 Linkify::ComparatorLinkSpec::ComparatorLinkSpecFunc(
    /* [in] */ LinkSpec* a,
    /* [in] */ LinkSpec* b)
{
    assert(a && b);
    LinkSpec* aT = (LinkSpec*)a;
    LinkSpec* bT = (LinkSpec*)b;

    if (aT->start < bT->start) {
        return -1;
    }

    if (aT->start > bT->start) {
        return 1;
    }

    if (aT->end < bT->end) {
        return 1;
    }

    if (aT->end > bT->end) {
        return -1;
    }
    return 0;
}

/*****************************Linkify*****************************/
Boolean Linkify::AddLinks(
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 mask)
{
    if (mask == 0) {
        return FALSE;
    }

    Int32 textLen;
    text->GetLength(&textLen);
    AutoPtr< ArrayOf<IURLSpan*> > old;
    text->GetSpans(0, textLen, EIID_IURLSpan, (ArrayOf<IInterface*>**)&old);

    if (old != NULL) {
        for (Int32 i = old->GetLength() - 1; i >= 0; i--) {
            text->RemoveSpan((*old)[i]);
        }
    }

    List< AutoPtr<LinkSpec> > links;

    if ((mask & ILinkify::WEB_URLS) != 0) {
        AutoPtr< ArrayOf<String> > ary = ArrayOf<String>::Alloc(3);
        (*ary)[0] = String("http://");
        (*ary)[1] = String("https://");
        (*ary)[2] = String("rtsp://");
        GatherLinks(&links, text, Patterns::WEB_URL, ary, sUrlMatchFilter, NULL);
    }

    if ((mask & ILinkify::EMAIL_ADDRESSES) != 0) {
        AutoPtr< ArrayOf<String> > ary = ArrayOf<String>::Alloc(1);
        (*ary)[0] = String("mailto:");
        GatherLinks(&links, text, Patterns::EMAIL_ADDRESS, ary, NULL, NULL);
    }

    if ((mask & ILinkify::PHONE_NUMBERS) != 0) {
        AutoPtr< ArrayOf<String> > ary = ArrayOf<String>::Alloc(1);
        (*ary)[0] = String("tel:");
        GatherLinks(&links, text, Patterns::PHONE, ary,
            sPhoneNumberMatchFilter, sPhoneNumberTransformFilter);
    }

    if ((mask & ILinkify::MAP_ADDRESSES) != 0) {
        GatherMapLinks(&links, text);
    }

    PruneOverlaps(&links);

    Int32 linksSize = links.GetSize();
    if (linksSize == 0) {
        return FALSE;
    }

    List< AutoPtr<LinkSpec> >::ReverseIterator it = links.RBegin();
    for (; it != links.REnd(); ++it) {
        AutoPtr<LinkSpec> link = *it;
        ApplyLink(link->url, link->start, link->end, text);
    }

    return TRUE;
}

Boolean Linkify::AddLinks(
    /* [in] */ ITextView* text,
    /* [in] */ Int32 mask)
{
    if (mask == 0) {
        return FALSE;
    }

    AutoPtr<ICharSequence> t;
    text->GetText((ICharSequence**)&t);

    AutoPtr<ISpannable> sp = ISpannable::Probe(t);

    if (sp != NULL) {
        if (AddLinks(sp, mask)) {
            AddLinkMovementMethod(text);
            return TRUE;
        }

        return FALSE;
    }
    else {
        AutoPtr<ISpannableStringHelper> helper;
        CSpannableStringHelper::AcquireSingleton((ISpannableStringHelper**)&helper);
        AutoPtr<ISpannableString> s;
        helper->ValueOf(t, (ISpannableString**)&s);

        if (AddLinks(ISpannable::Probe(s), mask)) {
            AddLinkMovementMethod(text);
            text->SetText(ICharSequence::Probe(s));

            return TRUE;
        }

        return FALSE;
    }
}

void Linkify::AddLinkMovementMethod(
    /* [in] */ ITextView* t)
{
    AutoPtr<IMovementMethod> m;
    t->GetMovementMethod((IMovementMethod**)&m);

    if (ILinkMovementMethod::Probe(m) == NULL) {
        Boolean bLinksClickable;
        t->GetLinksClickable(&bLinksClickable);
        if (bLinksClickable) {
            m = CLinkMovementMethod::GetInstance();
            t->SetMovementMethod(m);
        }
    }
}

void Linkify::AddLinks(
    /* [in] */ ITextView* text,
    /* [in] */ IPattern* pattern,
    /* [in] */ const String& scheme)
{
    AddLinks(text, pattern, scheme, NULL, NULL);
}

void Linkify::AddLinks(
    /* [in] */ ITextView* text,
    /* [in] */ IPattern* p,
    /* [in] */ const String& scheme,
    /* [in] */ ILinkifyMatchFilter* matchFilter,
    /* [in] */ ILinkifyTransformFilter* transformFilter)
{
    AutoPtr<ICharSequence> cs;
    text->GetText((ICharSequence**)&cs);

    AutoPtr<ISpannableStringHelper> helper;
    CSpannableStringHelper::AcquireSingleton((ISpannableStringHelper**)&helper);
    AutoPtr<ISpannableString> s;
    helper->ValueOf(cs, (ISpannableString**)&s);

    if (AddLinks(s, p, scheme, matchFilter, transformFilter)) {
        text->SetText(s);
        AddLinkMovementMethod(text);
    }
}

Boolean Linkify::AddLinks(
    /* [in] */ ISpannable* text,
    /* [in] */ IPattern* pattern,
    /* [in] */ const String& scheme)
{
    return AddLinks(text, pattern, scheme, NULL, NULL);
}

Boolean Linkify::AddLinks(
    /* [in] */ ISpannable* s,
    /* [in] */ IPattern* p,
    /* [in] */ const String& scheme,
    /* [in] */ ILinkifyMatchFilter* matchFilter,
    /* [in] */ ILinkifyTransformFilter* transformFilter)
{
    Boolean hasMatches = FALSE;
    String prefix("");
    if (!scheme.IsNull()) {
        prefix = scheme.ToLowerCase();
    }

    AutoPtr<IMatcher> m;
    p->MatcherEx(s, (IMatcher**)&m);
    Boolean bFind, allowed;
    Int32 start, end;
    String group, url;

    while ((m->Find(&bFind), bFind)) {
        m->Start(&start);
        m->End(&end);
        allowed = TRUE;
        if (matchFilter != NULL) {
            matchFilter->AcceptMatch(s, start, end, &allowed);
        }

        if (allowed) {
            AutoPtr< ArrayOf<String> > ary = ArrayOf<String>::Alloc(1);
            (*ary)[0] = prefix;

            m->GroupEx(0, &group);
            url = MakeUrl(group, ary, m, transformFilter);

            ApplyLink(url, start, end, s);
            hasMatches = TRUE;
        }
    }

    return hasMatches;
}

void Linkify::ApplyLink(
    /* [in] */ const String& url,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ISpannable* text)
{
    AutoPtr<IURLSpan> span;
    CURLSpan::New(url, (IURLSpan**)&span);

    text->SetSpan(span, start, end, ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
}

String Linkify::MakeUrl(
    /* [in] */ const String& _url,
    /* [in] */ ArrayOf<String>* prefixes,
    /* [in] */ IMatcher* m,
    /* [in] */ ILinkifyTransformFilter* filter)
{
    String url(_url);
    if (filter != NULL) {
        filter->TransformUrl(m, url, &url);
    }

    Boolean hasPrefix = FALSE;

    for (Int32 i = 0; i < prefixes->GetLength(); i++) {
        if ( url.StartWith( (*prefixes)[i])) {
            hasPrefix = TRUE;

            // Fix capitalization if necessary
            if ( !(url.StartWith( (*prefixes)[i]))) {
                url = (*prefixes)[i] + url.Substring(((*prefixes)[i]).GetLength());
            }

            break;
        }
    }

    if (!hasPrefix) {
        url = (*prefixes)[0] + url;
    }

    return url;
}

void Linkify::GatherLinks(
    /* [in] */ List< AutoPtr<LinkSpec> >* links,
    /* [in] */ ISpannable* s,
    /* [in] */ IPattern* pattern,
    /* [in] */ ArrayOf<String>* schemes,
    /* [in] */ ILinkifyMatchFilter* matchFilter,
    /* [in] */ ILinkifyTransformFilter* transformFilter)
{
    AutoPtr<IMatcher> m;
    pattern->MatcherEx(s, (IMatcher**)&m);

    Boolean mFind, bAcceptMatch;
    Int32 start, end;
    String group, url;
    while ((m->Find(&mFind), mFind)) {
        m->Start(&start);
        m->End(&end);

        if (matchFilter == NULL || (matchFilter->AcceptMatch(s, start, end, &bAcceptMatch), bAcceptMatch)) {
            m->GroupEx(0, &group);
            url = MakeUrl(group, schemes, m, transformFilter);

            AutoPtr<LinkSpec> spec = new LinkSpec();
            spec->url = url;
            spec->start = start;
            spec->end = end;

            links->PushBack(spec);
        }
    }
}

void Linkify::GatherMapLinks(
    /* [in] */ List< AutoPtr<LinkSpec> >* links,
    /* [in] */ ISpannable* s)
{
    String string;
    s->ToString(&string);
    String address;
    Int32 base = 0;
    Int32 length, end;

    String encodedAddress;

//    address = CWebView::FindAddress(string);
    while (!address.IsNull()) {
        Int32 start = string.IndexOf(address);

        if (start < 0) {
            break;
        }

        AutoPtr<LinkSpec> spec = new LinkSpec();
        length = address.GetLength();
        end = start + length;

        spec->start = base + start;
        spec->end = base + end;
        string = string.Substring(end);
        base += end;

        //try {
            AutoPtr<IURLEncoder> sUrlEncoder;
            CURLEncoder::AcquireSingleton((IURLEncoder**)&sUrlEncoder);
            sUrlEncoder->EncodeEx(address, String("UTF-8"), &encodedAddress);
        //} catch (UnsupportedEncodingException e) {
            //continue;
        //}

        spec->url = String("geo:0,0?q=") + encodedAddress;
        links->PushBack(spec);
    }
}

void Linkify::PruneOverlaps(
    /* [in] */ List< AutoPtr<LinkSpec> >* links)
{
//    AutoPtr<ComparatorLinkSpec> c = new ComparatorLinkSpec();

    links->Sort(Linkify::ComparatorLinkSpec::ComparatorLinkSpecFunc);

    Int32 len = links->GetSize();
    Int32 i = 0;
    Int32 remove;

    while (i < len - 1) {
        LinkSpec* a = ( (*links)[i] );
        LinkSpec* b = ( (*links)[i + 1] );
        remove = -1;

        if ((a->start <= b->start) && (a->end > b->start)) {
            if (b->end <= a->end) {
                remove = i + 1;
            }
            else if ((a->end - a->start) > (b->end - b->start)) {
                remove = i + 1;
            }
            else if ((a->end - a->start) < (b->end - b->start)) {
                remove = i;
            }

            if (remove != -1) {
                links->Remove(remove);
                len--;
                continue;
            }

        }

        i++;
    }
}

AutoPtr<ILinkifyMatchFilter> Linkify::GetStaticUrlMatchFilter()
{
    return sUrlMatchFilter;
}

AutoPtr<ILinkifyMatchFilter> Linkify::GetStaticPhoneNumberMatchFilter()
{
    return sPhoneNumberMatchFilter;
}

AutoPtr<ILinkifyTransformFilter> Linkify::GetStaticPhoneNumberTransformFilter()
{
    return sPhoneNumberTransformFilter;
}

} // namespace Utility
} // namespace Text
} // namepsace Droid
} // namespace Elastos
