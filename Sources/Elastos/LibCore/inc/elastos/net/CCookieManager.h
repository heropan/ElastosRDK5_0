
#ifndef __ELASTOS_NET_CCOOKIEMANAGER_H__
#define __ELASTOS_NET_CCOOKIEMANAGER_H__

#include "_Elastos_Net_CCookieManager.h"
#include "CStringWrapper.h"
#include "CookieHandler.h"
#include <elastos/utility/etl/List.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::IMap;
using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Net {

CarClass(CCookieManager)
    , public CookieHandler
    , public ICookieManager
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI SetCookiePolicy(
        /* [in] */ ICookiePolicy* cookiePolicy);

    CARAPI GetCookieStore(
        /* [out] */ ICookieStore** cookieStore);

    CARAPI Get(
        /* [in] */ IURI* uri,
        /* [in] */ IMap* requestHeaders,
        /* [out] */ IMap** cookiesMap);

    CARAPI Put(
        /* [in] */ IURI* uri,
        /* [in] */ IMap* responseHeaders);

    static CARAPI_(String) PathToCookiePath(
        /* [in] */ const String& path);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ICookieStore* store,
        /* [in] */ ICookiePolicy* cookiePolicy);

private:
    static CARAPI CookiesToHeaders(
        /* [in] */ List< AutoPtr<IHttpCookie> >& cookies,
        /* [out] */ IMap** cookiesMap);

    static CARAPI_(AutoPtr<List< AutoPtr<IHttpCookie> > >) ParseCookie(
        /* [in] */ IMap* responseHeaders);

private:
    AutoPtr<ICookieStore> mStore;

    AutoPtr<ICookiePolicy> mPolicy;

    static const String VERSION_ZERO_HEADER;

    static const String VERSION_ONE_HEADER;
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_CCOOKIEMANAGER_H__
