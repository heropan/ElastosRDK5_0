
#ifndef __RESPONSECACHE_H__
#define __RESPONSECACHE_H__

#include "Elastos.CoreLibrary_server.h"

using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Net {

class ResponseCache
{
public:
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
        /* [in] */ IObjectStringMap* requestHeaders,
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

#endif //__RESPONSECACHE_H__
