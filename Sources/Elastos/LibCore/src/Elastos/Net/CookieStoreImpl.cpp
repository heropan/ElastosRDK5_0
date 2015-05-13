
#include "CookieStoreImpl.h"
#include "CURI.h"
#include "CHttpCookie.h"
#include "CArrayList.h"
#include "CCollections.h"

using Elastos::Utility::IArrayList;
using Elastos::Utility::CArrayList;
using Elastos::Utility::CCollections;

namespace Elastos {
namespace Net {

CookieStoreImpl::~CookieStoreImpl()
{
    mMap.Clear();
}

PInterface CookieStoreImpl::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_ICookieStore) {
        return (ICookieStore*)this;
    }

    return NULL;
}

UInt32 CookieStoreImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CookieStoreImpl::Release()
{
    return ElRefBase::Release();
}

ECode CookieStoreImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CookieStoreImpl::Add(
    /* [in] */ IURI* uri,
    /* [in] */ IHttpCookie* cookie)
{
    Mutex::Autolock lock(mLock);

    if (cookie == NULL) {
        // throw new NullPointerException("cookie == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IURI> cookiesUri = GetCookiesUri(uri);

    AutoPtr<List< AutoPtr<IHttpCookie> > > cookies = NULL;
    HashMap<AutoPtr<IURI>, AutoPtr<List< AutoPtr<IHttpCookie> > > >::Iterator it =
            mMap.Find(cookiesUri);
    if (it != mMap.End()) {
        cookies = it->mSecond;
    }

    if (cookies == NULL) {
        cookies = new List< AutoPtr<IHttpCookie> >();
        mMap[uri] = cookies;
    }
    else {
        cookies->Remove(cookie);
    }
    cookies->PushBack(cookie);

    return NOERROR;
}

AutoPtr<IURI> CookieStoreImpl::GetCookiesUri(
    /* [in] */ IURI* uri)
{
    if (uri == NULL) {
        return NULL;
    }
//    try {
    String host;
    uri->GetHost(&host);
    AutoPtr<IURI> cookiesUri;
    ECode ec = CURI::New(String("http"), host, String(NULL), String(NULL), (IURI**)&cookiesUri);
    if (FAILED(ec)) {
        cookiesUri = uri;
    }

    return cookiesUri;
    // } catch (URISyntaxException e) {
    //     return uri; // probably a URI with no host
    // }
}

ECode CookieStoreImpl::Get(
    /* [in] */ IURI* uri,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mLock);

    if (uri == NULL) {
        // throw new NullPointerException("uri == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IList> outresult;
    CArrayList::New((IArrayList**)&outresult);

    // get cookies associated with given URI. If none, returns an empty list
    AutoPtr<List< AutoPtr<IHttpCookie> > > cookiesForUri;
    HashMap<AutoPtr<IURI>, AutoPtr<List< AutoPtr<IHttpCookie> > > >::Iterator it =
            mMap.Find(uri);
    if (it != mMap.End()) {
        cookiesForUri = it->mSecond;
    }
    if (cookiesForUri != NULL) {
        List<AutoPtr<IHttpCookie> >::Iterator cookieIt = cookiesForUri->Begin();
        while(cookieIt != cookiesForUri->End()) {
            AutoPtr<IHttpCookie> cookie = *cookieIt;
            Boolean hasExpired;
            if (cookie->HasExpired(&hasExpired), hasExpired) {
                cookieIt = cookiesForUri->Erase(cookieIt);
            }
            else {
                outresult->Add(cookie, &hasExpired);
                ++cookieIt;
            }
        }
    }

    // get all cookies that domain matches the URI
    for (it = mMap.Begin(); it != mMap.End(); ++it) {
        if (uri == it->mFirst/*uri.equals(entry.getKey()*/) {
            continue; // skip the given URI; we've already handled it
        }

        AutoPtr<List<AutoPtr<IHttpCookie> > > entryCookies = it->mSecond;
        List<AutoPtr<IHttpCookie> >::Iterator cookieIt = entryCookies->Begin();
        while (cookieIt != entryCookies->End()) {
            AutoPtr<IHttpCookie> cookie = *cookieIt;
            String attr, host;
            cookie->GetDomainAttr(&attr);
            uri->GetHost(&host);
            if (!CHttpCookie::DomainMatches(attr, host)) {
                ++cookieIt;
                continue;
            }
            Boolean hasExpired;
            if (cookie->HasExpired(&hasExpired), hasExpired) {
                cookieIt = entryCookies->Erase(cookieIt);
            }
            else {
                Boolean found = FALSE;
                outresult->Contains(cookie, &found);
                if (!found) {
                    outresult->Add(cookie, &found);
                }
                ++cookieIt;
            }
        }
    }

    return CCollections::_NewUnmodifiableList(outresult, result);
}

ECode CookieStoreImpl::GetCookies(
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mLock);
    AutoPtr<IList> outresult;
    CArrayList::New((IArrayList**)&outresult);

    HashMap<AutoPtr<IURI>, AutoPtr<List< AutoPtr<IHttpCookie> > > >::Iterator it = mMap.Begin();
    for (it = mMap.Begin(); it != mMap.End(); ++it) {
        AutoPtr<List< AutoPtr<IHttpCookie> > >  list = it->mSecond;
        List<AutoPtr<IHttpCookie> >::Iterator listIt = list->Begin();
        while (listIt!= list->End()) {
            AutoPtr<IHttpCookie> cookie = *listIt;
            Boolean hasExpired;
            if (cookie->HasExpired(&hasExpired), hasExpired) {
                listIt = list->Erase(listIt);
            }
            else {
                Boolean found = FALSE;
                outresult->Contains(cookie, &found);
                if (!found) {
                    outresult->Add(cookie, &found);
                }
                ++listIt;
            }
        }
    }
    return CCollections::_NewUnmodifiableList(outresult, result);
}

ECode CookieStoreImpl::GetURIs(
    /* [out] */ IList** URIs)
{
    VALIDATE_NOT_NULL(URIs);

    Mutex::Autolock lock(mLock);
    AutoPtr<IList> outresult;
    CArrayList::New((IArrayList**)&outresult);

    Boolean isflag = FALSE;
    HashMap<AutoPtr<IURI>, AutoPtr<List< AutoPtr<IHttpCookie> > > >::Iterator it;
    for (it = mMap.Begin(); it != mMap.End(); ++it) {
        AutoPtr<IURI> uri = it->mFirst;
        outresult->Add(uri, &isflag);
    }
    outresult->Remove(NULL, &isflag); // sigh
    return CCollections::_NewUnmodifiableList(outresult, URIs);
}

ECode CookieStoreImpl::Remove(
    /* [in] */ IURI* uri,
    /* [in] */ IHttpCookie* cookie,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(mLock);

    if (cookie == NULL) {
//        throw new NullPointerException("cookie == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<List< AutoPtr<IHttpCookie> > >  cookies = NULL;
    HashMap<AutoPtr<IURI>, AutoPtr<List< AutoPtr<IHttpCookie> > > >::Iterator it =
            mMap.Find(GetCookiesUri(uri));
    if (it != mMap.End()) {
        cookies = it->mSecond;
    }
    if (cookies != NULL) {
        cookies->Remove(cookie);
        *succeeded = TRUE;
    }
    else {
        *succeeded = FALSE;
    }

    return NOERROR;
}

ECode CookieStoreImpl::RemoveAll(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(mLock);

    *succeeded = mMap.Begin() != mMap.End();
    mMap.Clear();

    return NOERROR;
}

} // namespace Net
} // namespace Elastos
