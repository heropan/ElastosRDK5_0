
#ifndef __HTTPENGINE_H__
#define __HTTPENGINE_H__

#include "Elastos.CoreLibrary_server.h"

using Elastos::IO::IOutputStream;
using Elastos::IO::IInputStream;
using Elastos::Net::ResponseSource;

namespace Elastos {
namespace Net {
namespace Http {

class HttpEngine
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    CARAPI Init(
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

protected:
    CARAPI_(void) Connect();

    CARAPI_(AutoPtr<IHttpConnection>) OpenSocketConnection();

    CARAPI InitRequestBodyOut();

    CARAPI_(Boolean) AcceptCacheResponseType(
        /* [in] */ ICacheResponse* cacheResponse);

    CARAPI_(AutoPtr<IHttpURLConnection>) GetHttpConnectionToCache();

    CARAPI_(AutoPtr<IRawHeaders>) GetNetworkRequestHeaders();

    CARAPI_(Boolean) IncludeAuthorityInRequestLine();

    // SSLSocketFactory getSslSocketFactory()

    CARAPI_(String) GetDefaultUserAgent();

    CARAPI_(String) GetOriginAddress(
        /* [in] */ IURL* url);

    CARAPI_(Boolean) RequiresTunnel();

private:
    CARAPI InitResponseSource();

    CARAPI SendSocketRequest();

    CARAPI SetResponse(
        /* [in] */ IResponseHeaders* headers,
        /* [in] */ IInputStream* body);

    CARAPI_(Boolean) HasRequestBody();

    CARAPI_(void) MaybeCache();

    CARAPI_(void) InitContentStream(
        /* [in] */ IInputStream* transferStream);

    CARAPI_(AutoPtr<IInputStream>) GetTransferStream();

    CARAPI ReadResponseHeaders();

    CARAPI_(void) ReadHeaders(
        /* [in] */ IRawHeaders* headers);

    CARAPI WriteRequestHeaders(
        /* [in] */ Int32 contentLength);

    CARAPI_(void) PrepareRawRequestHeaders();

    CARAPI_(String) GetRequestLine();

    CARAPI_(String) RequestString();

public:
    static const Int32 DEFAULT_CHUNK_LENGTH;

    static const String OPTIONS;
    static const String GET;
    static const String HEAD;
    static const String POST;
    static const String PUT;
    static const String DELETE;
    static const String TRACE;
    static const String CONNECT;

    static const Int32 HTTP_CONTINUE;

    /**
     * HTTP 1.1 doesn't specify how many redirects to follow, but HTTP/1.0
     * recommended 5. http://www.w3.org/Protocols/HTTP/1.0/spec.html#Code3xx
     */
    static const Int32 MAX_REDIRECTS;

protected:
    AutoPtr<IHttpURLConnectionImpl> mPolicy;

    String mMethod;

    AutoPtr<IHttpConnection> mConnection;

private:
    static const AutoPtr<ICacheResponse> GATEWAY_TIMEOUT_RESPONSE;

    /**
     * The maximum number of bytes to buffer when sending headers and a request
     * body. When the headers and body can be sent in a single write, the
     * request completes sooner. In one WiFi benchmark, using a large enough
     * buffer sped up some uploads by half.
     */
    static const Int32 MAX_REQUEST_BUFFER_LENGTH;

    ResponseSource mResponseSource;

    AutoPtr<IInputStream> mSocketIn;
    AutoPtr<IOutputStream> mSocketOut;

    /**
     * This stream buffers the request headers and the request body when their
     * combined size is less than MAX_REQUEST_BUFFER_LENGTH. By combining them
     * we can save socket writes, which in turn saves a packet transmission.
     * This is socketOut if the request size is large or unknown.
     */
    AutoPtr<IOutputStream> mRequestOut;
    AutoPtr<IAbstractHttpOutputStream> mRequestBodyOut;

    AutoPtr<IInputStream> mResponseBodyIn;

    AutoPtr<IResponseCache> mResponseCache; // = ResponseCache.getDefault();
    AutoPtr<ICacheResponse> mCacheResponse;
    AutoPtr<ICacheRequest> mCacheRequest;

    /** The time when the request headers were written, or -1 if they haven't been written yet. */
    Int64 mSentRequestMillis;

    /**
     * True if this client added an "Accept-Encoding: gzip" header field and is
     * therefore responsible for also decompressing the transfer stream.
     */
    Boolean mTransparentGzip;

    Boolean mSendChunked;

    /**
     * The version this client will use. Either 0 for HTTP/1.0, or 1 for
     * HTTP/1.1. Upon receiving a non-HTTP/1.1 response, this client
     * automatically sets its version to HTTP/1.0.
     */
    // TODO: is HTTP minor version tracked across HttpEngines?
    Int32 mHttpMinorVersion; // Assume HTTP/1.1

    AutoPtr<IURI> mUri;

    AutoPtr<IRequestHeaders> mRequestHeaders;

    /** Null until a response is received from the network or the cache */
    AutoPtr<IResponseHeaders> mResponseHeaders;

    /*
     * The cache response currently being validated on a conditional get. Null
     * if the cached response doesn't exist or doesn't need validation. If the
     * conditional get succeeds, these will be used for the response headers and
     * body. If it fails, these be closed and set to null.
     */
    AutoPtr<IResponseHeaders> mCachedResponseHeaders;
    AutoPtr<IInputStream> mCachedResponseBody;

    /**
     * True if the socket connection should be released to the connection pool
     * when the response has been fully read.
     */
    Boolean mAutomaticallyReleaseConnectionToPool;

    /** True if the socket connection is no longer needed by this engine. */
    Boolean mConnectionReleased;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__HTTPENGINE_H__
