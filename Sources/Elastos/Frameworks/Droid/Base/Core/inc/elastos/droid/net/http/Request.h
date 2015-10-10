
#ifndef __ELASTOS_DROID_NET_HTTP_REQUEST_H__
#define __ELASTOS_DROID_NET_HTTP_REQUEST_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>
#include "Connection.h"

using namespace Elastos::Core;
using namespace Org::Apache::Http;
using namespace Org::Apache::Http::Protocol;

using Elastos::IO::IInputStream;
using Org::Apache::Http::Message::IBasicHttpRequest;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * Represents an HTTP request for a given host.
 *
 * {@hide}
 */
class Request
    : public ElRefBase
    , public IObject
    , public IRequest
{
public:
    CAR_INTERFACE_DECL();

    CARAPI Aggregate(
        /* [in] */ AggrType aggrType,
        /* [in] */ PInterface pObject);

    CARAPI GetDomain(
        /* [out] */ PInterface *ppObject);

    CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean * result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    CARAPI ToString(
        /* [out] */ String* info);

    /**
     * Instantiates a new Request.
     * @param method GET/POST/PUT
     * @param host The server that will handle this request
     * @param path path part of URI
     * @param bodyProvider InputStream providing HTTP body, null if none
     * @param bodyLength length of body, must be 0 if bodyProvider is null
     * @param eventHandler request will make progress callbacks on
     * this interface
     * @param headers reqeust headers
     */
    Request(
        /* [in] */ const String& method,
        /* [in] */ IHttpHost* host,
        /* [in] */ IHttpHost* proxyHost,
        /* [in] */ const String& path,
        /* [in] */ IInputStream* bodyProvider,
        /* [in] */ Int32 bodyLength,
        /* [in] */ IEventHandler* eventHandler,
        /* [in] */ HashMap<String, String>* headers);

    ~Request();

    /**
     * @param pause True if the load should be paused.
     */
     CARAPI SetLoadingPaused(
         /* [in] */ Boolean pause);

    /**
     * @param connection Request served by this connection
     */
    CARAPI SetConnection(
         /* [in] */ Connection* connection);

    IEventHandler* GetEventHandler();

    /**
     * Add header represented by given pair to request.  Header will
     * be formatted in request as "name: value\r\n".
     * @param name of header
     * @param value of header
     */
    CARAPI AddHeader(
        /* [in] */ const String& name,
        /* [in] */ const String& value);

    /**
     * Add all headers in given map to this request.  This is a helper
     * method: it calls addHeader for each pair in the map.
     */
    CARAPI AddHeaders(
        /* [in] */ HashMap<String, String>* headers);

    /**
     * Send the request line and headers
     */
    CARAPI SendRequest(
        /* [in] */ IAndroidHttpClientConnection* httpClientConnection);

    /**
     * Receive a single http response.
     *
     * @param httpClientConnection the request to receive the response for.
     */
    CARAPI ReadResponse(
        /* [in] */ IAndroidHttpClientConnection* httpClientConnection);

    /**
     * Data will not be sent to or received from server after cancel()
     * call.  Does not close connection--use close() below for that.
     *
     * Called by RequestHandle from non-network thread
     */
    CARAPI Cancel();

    String GetHostPort();

    String GetUri();

    /**
     * for debugging
     */
    CARAPI ToString(
        /* [out] */ String* str);

    /**
     * If this request has been sent once and failed, it must be reset
     * before it can be sent again.
     */
    CARAPI Reset();

    /**
     * Pause thread request completes.  Used for synchronous requests,
     * and testing
     */
    CARAPI WaitUntilComplete();

    CARAPI Complete();

    /**
     * Handles SSL error(s) on the way down from the user (the user
     * has already provided their feedback).
     */
    CARAPI HandleSslErrorResponse(
        /* [in] */ Boolean proceed);

    /**
     * Helper: calls error() on eventhandler with appropriate message
     * This should not be called before the mConnection is set.
     */
    CARAPI Error(
        /* [in] */ Int32 errorId,
        /* [in] */ Int32 resourceId);

private:
    static AutoPtr<IRequestContent> InitRequestContentProcessor();

    /**
     * Decide whether a response comes with an entity.
     * The implementation in this class is based on RFC 2616.
     * Unknown methods and response codes are supposed to
     * indicate responses with an entity.
     * <br/>
     * Derived executors can override this method to handle
     * methods and response codes not specified in RFC 2616.
     *
     * @param request   the request, to obtain the executed method
     * @param response  the response, to obtain the status code
     */

    static CARAPI_(Boolean) CanResponseHaveBody(
        /* [in] */ IHttpRequest* request,
        /* [in] */ Int32 status);

    /**
     * Supply an InputStream that provides the body of a request.  It's
     * not great that the caller must also provide the length of the data
     * returned by that InputStream, but the client needs to know up
     * front, and I'm not sure how to get this out of the InputStream
     * itself without a costly readthrough.  I'm not sure skip() would
     * do what we want.  If you know a better way, please let me know.
     */
    CARAPI SetBodyProvider(
        /* [in] */ IInputStream* bodyProvider,
        /* [in] */ Int32 bodyLength);

public:
    /** The eventhandler to call as the request progresses */
    AutoPtr<IEventHandler> mEventHandler;

    /** The Apache http request */
    AutoPtr<IBasicHttpRequest> mHttpRequest;

    /** The path component of this request */
    String mPath;

    /** Host serving this request */
    AutoPtr<IHttpHost> mHost;

    /** Set if I'm using a proxy server */
    AutoPtr<IHttpHost> mProxyHost;

    /** True if request has been cancelled */
    /*volatile*/ Boolean mCancelled;

    Int32 mFailCount;

private:
    Connection* mConnection;

    // This will be used to set the Range field if we retry a connection. This
    // is http/1.1 feature.
    Int32 mReceivedBytes;

    AutoPtr<IInputStream> mBodyProvider;

    Int32 mBodyLength;

    static const String HOST_HEADER;
    static const String ACCEPT_ENCODING_HEADER;
    static const String CONTENT_LENGTH_HEADER;

    /* Used to synchronize waitUntilComplete() requests */
    Object mClientResource;

    /** True if loading should be paused **/
    Boolean mLoadingPaused;

    /**
     * Processor used to set content-length and transfer-encoding
     * headers.
     */
    static AutoPtr<IRequestContent> mRequestContentProcessor;

};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_REQUEST_H__
