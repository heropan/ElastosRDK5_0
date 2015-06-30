
#ifndef __ELASTOS_NET_RESPONSECACHE_H__
#define __ELASTOS_NET_RESPONSECACHE_H__

#include "Object.h"

using Elastos::Utility::IMap;

namespace Elastos {
namespace Net {

class ResponseCache
    : public Object
    , public IResponseCache
{
public:
    CAR_INTERFACE_DECL()

    ResponseCache();

    virtual ~ResponseCache();

    /**
     * Returns the system's default response cache, or null.
     */
    static CARAPI GetDefault(
        /* [out] */ IResponseCache** responseCache);

    /**
     * Sets the system's default response cache. Use null to remove the response cache.
     */
    static CARAPI SetDefault(
        /* [in] */ IResponseCache* responseCache);

    virtual CARAPI Get(
        /* [in] */ IURI* uri,
        /* [in] */ const String& requestMethod,
        /* [in] */ IMap* requestHeaders,
        /* [out] */ ICacheResponse** cache) = 0;

    virtual CARAPI Put(
        /* [in] */ IURI* uri,
        /* [in] */ IURLConnection* conn,
        /* [out] */ ICacheRequest** cache) = 0;

private:
    static AutoPtr<IResponseCache> sDefaultResponseCache;
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_RESPONSECACHE_H__
