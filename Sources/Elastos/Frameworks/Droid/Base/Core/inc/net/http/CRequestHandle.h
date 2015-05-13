
#ifndef __CREQUESTHANDLE_H__
#define __CREQUESTHANDLE_H__

#include "_CRequestHandle.h"
#include "ext/frameworkext.h"
#include <elastos/HashMap.h>
#include "Request.h"

using namespace Elastos::Core;

using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * RequestHandle: handles a request session that may include multiple
 * redirects, HTTP authentication requests, etc.
 *
 * {@hide}
 */
CarClass(CRequestHandle)
{
    friend class CRequestHandleHelper;

public:
    CRequestHandle();

    ~CRequestHandle();

    /**
     * Creates a new request session.
     */
    CARAPI constructor(
        /* [in] */ Elastos::Droid::Net::Http::IRequestQueue* requestQueue,
        /* [in] */ const String& url,
        /* [in] */ Elastos::Droid::Net::IWebAddress* uri,
        /* [in] */ const String& method,
        /* [in] */ IObjectStringMap* headers,
        /* [in] */ Elastos::IO::IInputStream* bodyProvider,
        /* [in] */ Int32 bodyLength,
        /* [in] */ IRequest* request);

    /**
     * Creates a new request session with a given Connection. This connection
     * is used during a synchronous load to handle this request.
     */
    CARAPI constructor(
        /* [in] */ Elastos::Droid::Net::Http::IRequestQueue* requestQueue,
        /* [in] */ const String& url,
        /* [in] */ Elastos::Droid::Net::IWebAddress* uri,
        /* [in] */ const String& method,
        /* [in] */ IObjectStringMap* headers,
        /* [in] */ Elastos::IO::IInputStream* bodyProvider,
        /* [in] */ Int32 bodyLength,
        /* [in] */ IRequest* request,
        /* [in] */ IConnection* conn);

    /**
     * Cancels this request
     */
    CARAPI Cancel();

    /**
     * Pauses the loading of this request. For example, called from the WebCore thread
     * when the plugin can take no more data.
     */
    CARAPI PauseRequest(
        /* [in] */ Boolean pause);

    /**
     * Handles SSL error(s) on the way down from the user (the user
     * has already provided their feedback).
     */
    CARAPI HandleSslErrorResponse(
        /* [in] */ Boolean proceed);

    /**
     * @return true if we've hit the max redirect count
     */
    CARAPI IsRedirectMax(
        /* [out] */ Boolean* isRedirectMax);

    CARAPI GetRedirectCount(
        /* [out] */ Int32* count);

    CARAPI SetRedirectCount(
        /* [in] */ Int32 count);

    /**
     * Create and queue a redirect request.
     *
     * @param redirectTo URL to redirect to
     * @param statusCode HTTP status code returned from original request
     * @param cacheHeaders Cache header for redirect URL
     * @return true if setup succeeds, false otherwise (redirect loop
     * count exceeded, body provider unable to rewind on 307 redirect)
     */
    CARAPI SetupRedirect(
        /* [in] */ const String& redirectTo,
        /* [in] */ Int32 statusCode,
        /* [in] */ IObjectStringMap* cacheHeaders,
        /* [out] */ Boolean* result);

    /**
     * Create and queue an HTTP authentication-response (basic) request.
     */
    CARAPI SetupBasicAuthResponse(
        /* [in] */ Boolean isProxy,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI SetupDigestAuthResponse(
        /* [in] */ Boolean isProxy,
        /* [in] */ const String& username,
        /* [in] */ const String& password,
        /* [in] */ const String& realm,
        /* [in] */ const String& nonce,
        /* [in] */ const String& QOP,
        /* [in] */ const String& algorithm,
        /* [in] */ const String& opaque);

    /**
     * @return HTTP request method (GET, PUT, etc).
     */
    CARAPI GetMethod(
        /* [out] */ String* method);

    CARAPI WaitUntilComplete();

    CARAPI ProcessRequest();

private:
    /**
     * @return Digest-scheme authentication response.
     */
    CARAPI ComputeDigestAuthResponse(
        /* [in] */ const String& username,
        /* [in] */ const String& password,
        /* [in] */ const String& realm,
        /* [in] */ const String& nonce,
        /* [in] */ const String& QOP,
        /* [in] */ const String& algorithm,
        /* [in] */ const String& opaque,
        /* [out] */ String* str);

    /**
     * @return Double-quoted MD5 digest.
     */
    CARAPI ComputeDigest(
        /* [in] */ const String& A1,
        /* [in] */ const String& A2,
        /* [in] */ const String& nonce,
        /* [in] */ const String& QOP,
        /* [in] */ const String& nc,
        /* [in] */ const String& cnonce,
        /* [out] */ String* str);

    /**
     * @return MD5 hash of concat(secret, ":", data).
     */
    CARAPI KD(
        /* [in] */ const String& secret,
        /* [in] */ const String& data,
        /* [out] */ String* str);

    /**
     * @return MD5 hash of param.
     */
    CARAPI H(
        /* [in] */ const String& param,
        /* [out] */ String* str);

    /**
     * @return HEX buffer representation.
     */
    CARAPI BufferToHex(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [out] */ String* str);

    /**
     * Computes a random cnonce value based on the current time.
     */
    CARAPI ComputeCnonce(
        /* [out] */ String* str);

    /**
     * "Double-quotes" the argument.
     */
    CARAPI_(String) DoubleQuote(
        /* [in] */ const String& param);

    /**
     * Creates and queues new request.
     */
    CARAPI CreateAndQueueNewRequest();

private:
    String mUrl;

    AutoPtr<IWebAddress> mUri;

    String mMethod;

    HashMap<String, String> mHeaders;

    AutoPtr<IRequestQueue> mRequestQueue;

    Request* mRequest;

    AutoPtr<IInputStream> mBodyProvider;

    Int32 mBodyLength;

    Int32 mRedirectCount;

    // Used only with synchronous requests.
    Connection* mConnection;

    static const String AUTHORIZATION_HEADER;
    static const String PROXY_AUTHORIZATION_HEADER;

};

}
}
}
}

#endif // __CREQUESTHANDLE_H__
