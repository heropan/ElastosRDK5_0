
#ifndef __ELASTOS_NET_CCOOKIEPOLICYHELPER_H__
#define __ELASTOS_NET_CCOOKIEPOLICYHELPER_H__

#include "_ELASTOS_NET_CCookiePolicyHelper.h"
#include "Singleton.h"


namespace Elastos {
namespace Net {

CarClass(CCookiePolicyHelper)
    , public Singleton
    , public ICookiePolicyHelper
{
private:
    class CookiePolicyAcceptAll
        : public ElRefBase
        , public ICookiePolicy
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [out] */ InterfaceID* pIID);

        CARAPI ShouldAccept(
            /* [in] */ IURI* uri,
            /* [in] */ IHttpCookie* cookie,
            /* [out] */ Boolean* result);
    };

    class CookiePolicyAcceptNone
        : public ElRefBase
        , public ICookiePolicy
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [out] */ InterfaceID* pIID);

        CARAPI ShouldAccept(
            /* [in] */ IURI* uri,
            /* [in] */ IHttpCookie* cookie,
            /* [out] */ Boolean* result);
    };

    class CookiePolicyAcceptOriginalServer
        : public ElRefBase
        , public ICookiePolicy
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [out] */ InterfaceID* pIID);

        CARAPI ShouldAccept(
            /* [in] */ IURI* uri,
            /* [in] */ IHttpCookie* cookie,
            /* [out] */ Boolean* result);
    };

public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI GetCookiePolicy(
        /* [in] */ CookiePolicyKind kind,
        /* [out] */ ICookiePolicy** policy);

    static CARAPI_(AutoPtr<ICookiePolicy>) GetCookiePolicy(
        /* [in] */ CookiePolicyKind kind);

private:
    static AutoPtr<ICookiePolicy> ACCEPT_ALL;
    static AutoPtr<ICookiePolicy> ACCEPT_NONE;
    static AutoPtr<ICookiePolicy> ACCEPT_ORIGINAL_SERVER;
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_CCOOKIEPOLICYHELPER_H__
