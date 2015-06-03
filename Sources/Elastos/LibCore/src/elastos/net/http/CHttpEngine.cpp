
#include "CHttpEngine.h"

namespace Elastos {
namespace Net {
namespace Http {

PInterface CHttpEngine::Probe(
    /* [in] */ REIID riid)
{
    return _CHttpEngine::Probe(riid);
}

ECode CHttpEngine::constructor(
    /* [in] */ IHttpURLConnectionImpl* policy,
    /* [in] */ const String& method,
    /* [in] */ IRawHeaders* requestHeaders,
    /* [in] */ IHttpConnection* connection,
    /* [in] */ IRetryableOutputStream* requestBodyOut)
{
    return HttpEngine::Init(policy, method, requestHeaders, connection, requestBodyOut);
}

ECode CHttpEngine::GetUri(
    /* [out] */ IURI** url)
{
    return HttpEngine::GetUri(url);
}

ECode CHttpEngine::SendRequest()
{
    return HttpEngine::SendRequest();
}

ECode CHttpEngine::GetRequestBody(
    /* [out] */ IOutputStream** os)
{
    return HttpEngine::GetRequestBody(os);
}

ECode CHttpEngine::HasResponse(
    /* [out] */ Boolean* value)
{
    return HttpEngine::HasResponse(value);
}

ECode CHttpEngine::GetRequestHeaders(
    /* [out] */ IRequestHeaders** requesh)
{
    return HttpEngine::GetRequestHeaders(requesh);
}

ECode CHttpEngine::GetResponseHeaders(
    /* [out] */ IResponseHeaders** rponh)
{
    return HttpEngine::GetResponseHeaders(rponh);
}

ECode CHttpEngine::GetResponseCode(
    /* [out] */ Int32* value)
{
    return HttpEngine::GetResponseCode(value);
}

ECode CHttpEngine::GetResponseBody(
    /* [out] */ IInputStream** is)
{
    return HttpEngine::GetResponseBody(is);
}

ECode CHttpEngine::GetCacheResponse(
    /* [out] */ ICacheResponse** cr)
{
    return HttpEngine::GetCacheResponse(cr);
}

ECode CHttpEngine::GetConnection(
    /* [out] */ IHttpConnection** hc)
{
    return HttpEngine::GetConnection(hc);
}

ECode CHttpEngine::HasRecycledConnection(
    /* [out] */ Boolean* value)
{
    return HttpEngine::HasRecycledConnection(value);
}

ECode CHttpEngine::AutomaticallyReleaseConnectionToPool()
{
    return HttpEngine::AutomaticallyReleaseConnectionToPool();
}

ECode CHttpEngine::ToRelease(
    /* [in] */ Boolean reusable)
{
    return HttpEngine::ToRelease(reusable);
}

ECode CHttpEngine::HasResponseBody(
    /* [out] */ Boolean* value)
{
    return HttpEngine::HasResponseBody(value);
}

ECode CHttpEngine::ReadTrailers()
{
    return HttpEngine::ReadTrailers();
}

ECode CHttpEngine::ReadResponse()
{
    return HttpEngine::ReadResponse();
}

} // namespace Http
} // namespace Net
} // namespace Elastos
