
#include "CCookiePolicyHelper.h"
#include "CHttpCookie.h"

namespace Elastos {
namespace Net {

PInterface CCookiePolicyHelper::CookiePolicyAcceptAll::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ICookiePolicy*)this;
    }
    else if (riid == EIID_ICookiePolicy) {
        return (ICookiePolicy*)this;
    }

    return NULL;
}

UInt32 CCookiePolicyHelper::CookiePolicyAcceptAll::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCookiePolicyHelper::CookiePolicyAcceptAll::Release()
{
    return ElRefBase::Release();
}

ECode CCookiePolicyHelper::CookiePolicyAcceptAll::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(ICookiePolicy*)this) {
        *pIID = EIID_ICookiePolicy;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CCookiePolicyHelper::CookiePolicyAcceptAll::ShouldAccept(
    /* [in] */ IURI* uri,
    /* [in] */ IHttpCookie* cookie,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = TRUE;
    return NOERROR;
}


PInterface CCookiePolicyHelper::CookiePolicyAcceptNone::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ICookiePolicy*)this;
    }
    else if (riid == EIID_ICookiePolicy) {
        return (ICookiePolicy*)this;
    }

    return NULL;
}

UInt32 CCookiePolicyHelper::CookiePolicyAcceptNone::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCookiePolicyHelper::CookiePolicyAcceptNone::Release()
{
    return ElRefBase::Release();
}

ECode CCookiePolicyHelper::CookiePolicyAcceptNone::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(ICookiePolicy*)this) {
        *pIID = EIID_ICookiePolicy;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CCookiePolicyHelper::CookiePolicyAcceptNone::ShouldAccept(
    /* [in] */ IURI* uri,
    /* [in] */ IHttpCookie* cookie,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = FALSE;
    return NOERROR;
}


PInterface CCookiePolicyHelper::CookiePolicyAcceptOriginalServer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ICookiePolicy*)this;
    }
    else if (riid == EIID_ICookiePolicy) {
        return (ICookiePolicy*)this;
    }

    return NULL;
}

UInt32 CCookiePolicyHelper::CookiePolicyAcceptOriginalServer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCookiePolicyHelper::CookiePolicyAcceptOriginalServer::Release()
{
    return ElRefBase::Release();
}

ECode CCookiePolicyHelper::CookiePolicyAcceptOriginalServer::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(ICookiePolicy*)this) {
        *pIID = EIID_ICookiePolicy;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CCookiePolicyHelper::CookiePolicyAcceptOriginalServer::ShouldAccept(
    /* [in] */ IURI* uri,
    /* [in] */ IHttpCookie* cookie,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    String domain, host;
    cookie->GetDomainAttr(&domain);
    uri->GetHost(&host);
    *result = CHttpCookie::DomainMatches(domain, host);
    return NOERROR;
}


AutoPtr<ICookiePolicy> CCookiePolicyHelper::ACCEPT_ALL;
AutoPtr<ICookiePolicy> CCookiePolicyHelper::ACCEPT_NONE;
AutoPtr<ICookiePolicy> CCookiePolicyHelper::ACCEPT_ORIGINAL_SERVER;

ECode CCookiePolicyHelper::GetCookiePolicy(
    /* [in] */ CookiePolicyKind kind,
    /* [out] */ ICookiePolicy** policy)
{
    VALIDATE_NOT_NULL(policy);

    AutoPtr<ICookiePolicy> _policy = GetCookiePolicy(kind);
    if (_policy == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;

    *policy = _policy.Get();
    REFCOUNT_ADD(*policy);

    return NOERROR;
}

AutoPtr<ICookiePolicy> CCookiePolicyHelper::GetCookiePolicy(
    /* [in] */ CookiePolicyKind kind)
{
    if (kind == CookiePolicyKind_ACCEPT_ALL) {
        return ACCEPT_ALL;
    }
    else if (kind == CookiePolicyKind_ACCEPT_NONE) {
        return ACCEPT_NONE;
    }
    else if (kind == CookiePolicyKind_ACCEPT_ORIGINAL_SERVER) {
        return ACCEPT_ORIGINAL_SERVER;
    }
    else {
        return NULL;
    }
}

} // namespace Net
} // namespace Elastos
