
#ifndef __ELASTOS_NET_COOKIEHANDLER_H__
#define __ELASTOS_NET_COOKIEHANDLER_H__

#include "Object.h"

using Elastos::Utility::IMap;
using Elastos::Net::ICookieHandler;

namespace Elastos {
namespace Net {

class CookieHandler : public Object
{
public:
    static CARAPI GetDefault(
        /* [out] */ ICookieHandler** handler);

    static CARAPI SetDefault(
        /* [in] */ ICookieHandler* cHandler);

    virtual CARAPI Get(
        /* [in] */ IURI* uri,
        /* [in] */ IMap* requestHeaders,
        /* [out]*/ IMap** cookiesMap) = 0;

    virtual CARAPI Put(
        /* [in] */ IURI* uri,
        /* [in] */ IMap* requestHeaders) = 0;

    static CARAPI StaticInit();

private:
    static AutoPtr<ICookieHandler> sSystemWideCookieHandler;
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_COOKIEHANDLER_H__
