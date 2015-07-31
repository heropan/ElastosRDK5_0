
#include "BestMatchSpec.h"
#include <elastos/Logger.h>

using Elastos::Core::Math;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHeaderElement;
using Org::Apache::Http::INameValuePair;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Cookie {

BestMatchSpec::BestMatchSpec(
    /* [in] */ ArrayOf<String>* datepatterns,
    /* [in] */ Boolean oneHeader)
    : mDatepatterns(datepatterns)
    , mOneHeader(oneHeader)
{}

BestMatchSpec::BestMatchSpec()
    : mOneHeader(FALSE)
{}

CAR_INTERFACE_IMPL(BestMatchSpec, Object, ICookieSpec)

AutoPtr<RFC2965Spec> BestMatchSpec::GetStrict()
{
    if (mStrict == null) {
         mStrict = new RFC2965Spec(mDatepatterns, mOneHeader);
    }
    return mStrict;
}

AutoPtr<BrowserCompatSpec> BestMatchSpec::GetCompat()
{
    if (mCompat == NULL) {
        mCompat = new BrowserCompatSpec(mDatepatterns);
    }
    return mCompat;
}

AutoPtr<NetscapeDraftSpec> BestMatchSpec::GetNetscape()
{
    if (mNetscape == NULL) {
        AutoPtr< ArrayOf<String> > patterns = mDatepatterns;
        if (patterns == NULL) {
            patterns = BrowserCompatSpec::DATE_PATTERNS;
        }
        mNetscape = new NetscapeDraftSpec(patterns);
    }
    return mNetscape;
}

ECode BestMatchSpec::Parse(
    /* [in] */ IHeader* header,
    /* [in] */ ICookieOrigin* origin,
    /* [out] */ IList** cookies)
{
    VALIDATE_NOT_NULL(cookies)
    *cookies = NULL;

    if (header == NULL) {
        Logger::E("BestMatchSpec", "Header may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (origin == NULL) {
        Logger::E("BestMatchSpec", "Cookie origin may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr< ArrayOf<IHeaderElement> > helems;
    header->GetElements((ArrayOf<IHeaderElement>**)&helems);
    Boolean versioned = FALSE;
    Boolean netscape = FALSE;
    for (Int32 i = 0; i < helems->GetLength(); ++i) {
        AutoPtr<IHeaderElement> helem = (*helems)[i];
        AutoPtr<INameValuePair> pair;
        helem->GetParameterByName(String("version"), (INameValuePair**)&pair);
        if (pair != NULL) {
            versioned = TRUE;
        }
        pair = NULL;
        helem->GetParameterByName(String("expires"), (INameValuePair**)&pair);
        if (pair != NULL) {
            netscape = TRUE;
        }
    }
    if (netscape) {

    }
    // Do we have a cookie with a version attribute?
    if (versioned) {
        return GetStrict()->Parse(helems, origin);
    }
    else if (netscape) {
        // Need to parse the header again,
        // because Netscape draft cannot handle
        // comma separators
        return GetNetscape()->Parse(header, origin);
    }
    else {
        return GetCompat()->Parse(helems, origin);
    }
}

ECode BestMatchSpec::Validate(
    /* [in] */ IHeader* header,
    /* [in] */ ICookieOrigin* origin)
{
    if (cookie == NULL) {
        Logger::E("BestMatchSpec", "Cookie may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (origin == NULL) {
        Logger::E("BestMatchSpec", "Cookie origin may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 version;
    if (cookie->GetVersion(&version), version > 0) {
        return GetStrict()->Validate(cookie, origin);
    }
    else {
        return GetCompat()->Validate(cookie, origin);
    }
}

ECode BestMatchSpec::Match(
    /* [in] */ IHeader* header,
    /* [in] */ ICookieOrigin* origin,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    if (cookie == NULL) {
        Logger::E("BestMatchSpec", "Cookie may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (origin == NULL) {
        Logger::E("BestMatchSpec", "Cookie origin may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 version;
    if (cookie->GetVersion(&version), version > 0) {
        return GetStrict()->Match(cookie, origin);
    }
    else {
        return GetCompat()->Match(cookie, origin);
    }
}

ECode BestMatchSpec::FormatCookies(
    /* [in] */ IList* cookies,
    /* [out] */ IList** headers)
{
    VALIDATE_NOT_NULL(headers)
    *headers = NULL;

    if (cookies == NULL) {
        Logger::E("BestMatchSpec", "List of cookie may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 version = Math::INT32_MAX_VALUE;
    AutoPtr<IIterator> it;
    cookies->GetIterator((IIterator**)&it);
    Boolean hasNext;
    While (it->HasNext(), hasNext) {
        AutoPtr<IInterface> value;
        it->GetNext((IInterface**)&value);
        AutoPtr<ICookie> cookie = ICookie::Probe(value);
        Int32 ver;
        if (cookie->GetVersion(&ver), ver < version) {
            version = ver;
        }
    }
    if (version > 0) {
        return GetStrict()->FormatCookies(cookies);
    }
    else {
        return GetCompat()->FormatCookies(cookies);
    }
}

ECode BestMatchSpec::GetVersion(
    /* [out] */ Int32* version)
{
    VALIDATE_NOT_NULL(version)
    *version = 0;
    return NOERROR;
}

ECode BestMatchSpec::GetVersionHeader(
    /* [out] */ IHeader* header)
{
    VALIDATE_NOT_NULL(header)
    *header = NULL;
    return NOERROR;
}

} // namespace Cookie
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org