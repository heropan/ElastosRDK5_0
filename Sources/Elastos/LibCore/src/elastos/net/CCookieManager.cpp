
#include "CCookieManager.h"
#include "CookieStoreImpl.h"
#include "CHttpCookie.h"
#include "CCookiePolicyHelper.h"
//#include "CCollections.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/Math.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::ICollections;
//using Elastos::Utility::CCollections;
using Elastos::Utility::IMapEntry;
using Elastos::Core::StringBuilder;
using Elastos::Core::Math;

namespace Elastos {
namespace Net {

const String CCookieManager::VERSION_ZERO_HEADER = String("Set-cookie");
const String CCookieManager::VERSION_ONE_HEADER = String("Set-cookie2");

CAR_INTERFACE_IMPL(CCookieManager, CookieHandler, ICookieManager)

CAR_OBJECT_IMPL(CCookieManager)

ECode CCookieManager::constructor()
{
    return constructor(NULL, NULL);
}

ECode CCookieManager::constructor(
    /* [in] */ ICookieStore* store,
    /* [in] */ ICookiePolicy* cookiePolicy)
{
    mStore = store == NULL ? (ICookieStore*) new CookieStoreImpl() : store;

    if (cookiePolicy == NULL) {
        mPolicy = CCookiePolicyHelper::GetCookiePolicy(
                CookiePolicyKind_ACCEPT_ORIGINAL_SERVER);
    }
    else mPolicy = cookiePolicy;

    return NOERROR;
}

ECode CCookieManager::Get(
    /* [in] */ IURI* uri,
    /* [in] */ IMap* requestHeaders,
    /* [out] */ IMap** cookiesMap)
{
    VALIDATE_NOT_NULL(cookiesMap);

    if (uri == NULL || requestHeaders == NULL) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    List< AutoPtr<IHttpCookie> > result;
    AutoPtr<IList> cookies;
    mStore->Get(uri, (IList**)&cookies);
    assert(cookies != NULL);
    AutoPtr<IIterator> enumerator;
    cookies->GetIterator((IIterator**)&enumerator);
    Boolean hasNext;
    while(enumerator->HasNext(&hasNext), hasNext) {
        AutoPtr<IHttpCookie> cookie;
        enumerator->Next((IInterface**)&cookie);
        if (CHttpCookie::PathMatches(cookie, uri)
                && CHttpCookie::SecureMatches(cookie, uri)
                && CHttpCookie::PortMatches(cookie, uri)) {
            result.PushBack(cookie);
        }
    }

    return CookiesToHeaders(result, cookiesMap);
}

ECode CCookieManager::CookiesToHeaders(
    /* [in] */ List<AutoPtr<IHttpCookie> >& cookies,
    /* [out] */ IMap** cookiesMap)
{
    VALIDATE_NOT_NULL(cookiesMap)

    AutoPtr<ICollections> outcol;
    /////////////////////CCollections::AcquireSingleton((ICollections**)&outcol);
    if (cookies.IsEmpty()) {
        return outcol->GetEmptyMap(cookiesMap);
    }

    StringBuilder result;

    // If all cookies are version 1, add a version 1 header. No header for version 0 cookies.
    Int32 minVersion = 1;
    List<AutoPtr<IHttpCookie> >::Iterator it;
    for(it = cookies.Begin(); it != cookies.End(); ++it) {
        IHttpCookie* cookie = *it;
        Int32 version;
        cookie->GetVersion(&version);
        minVersion = Math::Min(minVersion, version);
    }
    if (minVersion == 1) {
        result += "$Version=\"1\"; ";
    }

    it = cookies.Begin();
    CHttpCookie* cookieObj = (CHttpCookie*)(*it).Get();
    result += cookieObj->ToString();
    for(++it; it != cookies.End(); ++it) {
        cookieObj = (CHttpCookie*)(*it).Get();
        result += "; ";
        result += cookieObj->ToString();
    }
    AutoPtr<IMap> resmap;
    AutoPtr<IList> outlist;
    outcol->NewSingletonList(result.ToCharSequence(), (IList**)&outlist);
    AutoPtr<ICharSequence> Cookie;
    CStringWrapper::New(String("Cookie"), (ICharSequence**)&Cookie);
    return outcol->NewSingletonMap(Cookie, outlist, cookiesMap);
}

ECode CCookieManager::Put(
    /* [in] */ IURI* uri,
    /* [in] */ IMap* responseHeaders)
{
    if (uri == NULL || responseHeaders == NULL) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    //parse and construct cookies according to the map
    //ToDo: when to delete cookies?
    AutoPtr<List< AutoPtr<IHttpCookie> > > cookies = ParseCookie(responseHeaders);
    List< AutoPtr<IHttpCookie> >::Iterator it;
    for (it = cookies->Begin(); it != cookies->End(); ++it) {
        AutoPtr<IHttpCookie> cookie = *it;

        // if the cookie doesn't have a domain, set one. The policy will do validation.
        String domain;
        cookie->GetDomainAttr(&domain);
        if (domain.IsNull()) {
            String host;
            uri->GetHost(&host);
            cookie->SetDomain(host);
        }

        // if the cookie doesn't have a path, set one. If it does, validate it.
        String path;
        cookie->GetPath(&path);
        if (path.IsNull()) {
            String uriPath;
            uri->GetPath(&uriPath);
            cookie->SetPath(PathToCookiePath(uriPath));
        }
        else if (!CHttpCookie::PathMatches(cookie, uri)) {
            continue;
        }

        // if the cookie has the placeholder port list "", set the port. Otherwise validate it.
        String portList;
        cookie->GetPortList(&portList);
        if (portList.Equals("")) {
            Int32 port;
            uri->GetEffectivePort(&port);
            cookie->SetPortList(StringUtils::ToString(port));
        }
        else if (!portList.IsNull() && !CHttpCookie::PortMatches(cookie, uri)) {
            continue;
        }

        // if the cookie conforms to the policy, add it into the store
        Boolean result;
        mPolicy->ShouldAccept(uri, cookie, &result);
        if (result) {
            mStore->Add(uri, cookie);
        }
    }
    return NOERROR;
}

String CCookieManager::PathToCookiePath(
    /* [in] */ const String& path)
{
    if (path.IsNull()) {
        return String("/");
    }
    Int32 lastSlash = path.LastIndexOf('/'); // -1 yields the empty string
    return path.Substring(0, lastSlash + 1);
}

AutoPtr<List<AutoPtr<IHttpCookie> > > CCookieManager::ParseCookie(
    /* [in] */ IMap* responseHeaders)
{
    AutoPtr<List<AutoPtr<IHttpCookie> > > cookies = new List< AutoPtr<IHttpCookie> >;
    AutoPtr<ISet> outset;
    responseHeaders->EntrySet((ISet**)&outset);
    AutoPtr< ArrayOf<IInterface*> > outarr;
    outset->ToArray((ArrayOf<IInterface*>**)&outarr);
    for(Int32 i = 0; i < outarr->GetLength(); ++i) {
        AutoPtr<IMapEntry> entry = IMapEntry::Probe((*outarr)[i]);
        AutoPtr<IInterface> keyface;
        entry->GetKey((IInterface**)&keyface);
        String key;
        ICharSequence::Probe(keyface)->ToString(&key);
        // Only "Set-cookie" and "Set-cookie2" pair will be parsed
        if (!key.IsNull() && (key.EqualsIgnoreCase(VERSION_ZERO_HEADER)
                || key.EqualsIgnoreCase(VERSION_ONE_HEADER))) {
            // parse list elements one by one
            AutoPtr<IInterface> valueface;
            entry->GetValue((IInterface**)&valueface);
            AutoPtr<IList> outlist = IList::Probe(valueface);
            AutoPtr< ArrayOf<IInterface*> > outarr2;
            outlist->ToArray((ArrayOf<IInterface*>**)&outarr2);
            for (Int32 j = 0; j < outarr2->GetLength(); j++) {
                // try {
                    String cookieStr;
                    ICharSequence::Probe((*outarr2)[j])->ToString(&cookieStr);
                    List< AutoPtr<IHttpCookie> > listhttp;
                    CHttpCookie::Parse(cookieStr, listhttp);
                    for (Int32 k = 0; k < listhttp.GetSize(); k++) {
                        cookies->PushBack(listhttp[k]);
                    }
                // } catch (IllegalArgumentException ignored) {
                //     // this string is invalid, jump to the next one.
                // }
            }
        }
    }

    return cookies;
}

ECode CCookieManager::SetCookiePolicy(
    /* [in] */ ICookiePolicy* cookiePolicy)
{
    if (cookiePolicy != NULL) {
        mPolicy = cookiePolicy;
    }

    return NOERROR;
}

ECode CCookieManager::GetCookieStore(
    /* [out] */ ICookieStore** cookieStore)
{
    VALIDATE_NOT_NULL(cookieStore);

    *cookieStore = mStore;
    REFCOUNT_ADD(*cookieStore);
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
