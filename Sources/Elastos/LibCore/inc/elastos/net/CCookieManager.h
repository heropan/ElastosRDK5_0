
#ifndef __CCOOKIEMANAGER_H__
#define __CCOOKIEMANAGER_H__

#include "_CCookieManager.h"
#include <elastos/utility/etl/List.h>
#include "CStringWrapper.h"

using Elastos::Utility::Etl::List;
using Elastos::Utility::IMap;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Net {

CarClass(CCookieManager)
{
public:
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

#endif //__CCOOKIEMANAGER_H__
