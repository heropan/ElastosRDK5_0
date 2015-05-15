
#ifndef __CHTTPENGINE_H__
#define __CHTTPENGINE_H__

#include "_CHttpEngine.h"
#include "HttpEngine.h"

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CHttpEngine), public HttpEngine
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IHttpURLConnectionImpl* policy,
        /* [in] */ const String& method,
        /* [in] */ IRawHeaders* requestHeaders,
        /* [in] */ IHttpConnection* connection,
        /* [in] */ IRetryableOutputStream* requestBodyOut);

    CARAPI GetUri(
        /* [out] */ IURI** url);

    CARAPI SendRequest();

    CARAPI GetRequestBody(
        /* [out] */ IOutputStream** os);

    CARAPI HasResponse(
        /* [out] */ Boolean* value);

    CARAPI GetRequestHeaders(
        /* [out] */ IRequestHeaders** requesh);

    CARAPI GetResponseHeaders(
        /* [out] */ IResponseHeaders** rponh);

    CARAPI GetResponseCode(
        /* [out] */ Int32* value);

    CARAPI GetResponseBody(
        /* [out] */ IInputStream** is);

    CARAPI GetCacheResponse(
        /* [out] */ ICacheResponse** cr);

    CARAPI GetConnection(
        /* [out] */ IHttpConnection** hc);

    CARAPI HasRecycledConnection(
        /* [out] */ Boolean* value);

    CARAPI AutomaticallyReleaseConnectionToPool();

    CARAPI ToRelease(
        /* [in] */ Boolean reusable);

    CARAPI HasResponseBody(
        /* [out] */ Boolean* value);

    CARAPI ReadTrailers();

    CARAPI ReadResponse();
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CHTTPENGINE_H__
