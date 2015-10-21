
#ifndef __ELASTOS_DROID_NET_HTTP_CELASTOSHTTPCLIENTCONNECTION_H__
#define __ELASTOS_DROID_NET_HTTP_CELASTOSHTTPCLIENTCONNECTION_H__

#include "_Elastos_Droid_Net_Http_CElastosHttpClientConnection.h"

using namespace Elastos::Core;
using namespace Elastos::Net;
using namespace Org::Apache::Http;
using namespace Org::Apache::Http::IO;
using namespace Org::Apache::Http::Entity;

using Org::Apache::Http::Impl::IHttpConnectionMetricsImpl;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * A alternate class for (@link DefaultHttpClientConnection).
 * It has better performance than DefaultHttpClientConnection
 *
 * {@hide}
 */
CarClass(CElastosHttpClientConnection)
{
public:
    CElastosHttpClientConnection();

    ~CElastosHttpClientConnection();

    CARAPI constructor();

    /**
     * Bind socket and set HttpParams to AndroidHttpClientConnection
     * @param socket outgoing socket
     * @param params HttpParams
     * @throws IOException
      */
    CARAPI Bind(
        /* [in] */ Elastos::Net::ISocket* socket,
        /* [in] */ Org::Apache::Http::Params::IHttpParams* params);

    CARAPI ToString(
        /* [out] */ String* str);

    /**
     * Sends the request line and all headers over the connection.
     * @param request the request whose headers to send.
     * @throws HttpException
     * @throws IOException
     */
    CARAPI SendRequestHeader(
        /* [in] */ Org::Apache::Http::IHttpRequest* request);

    /**
     * Sends the request entity over the connection.
     * @param request the request whose entity to send.
     * @throws HttpException
     * @throws IOException
     */
    CARAPI SendRequestEntity(
        /* [in] */ Org::Apache::Http::IHttpEntityEnclosingRequest* request);

    CARAPI Flush();

    /**
     * Parses the response headers and adds them to the
     * given {@code headers} object, and returns the response StatusLine
     * @param headers store parsed header to headers.
     * @throws IOException
     * @return StatusLine
     * @see HttpClientConnection#receiveResponseHeader()
      */
    CARAPI ParseResponseHeader(
        /* [in] */ Elastos::Droid::Net::Http::IHeaders* headers,
        /* [out] */ Org::Apache::Http::IStatusLine** statusLine);

    /**
     * Return the next response entity.
     * @param headers contains values for parsing entity
     * @see HttpClientConnection#receiveResponseEntity(HttpResponse response)
     */
    CARAPI ReceiveResponseEntity(
        /* [in] */ Elastos::Droid::Net::Http::IHeaders* headers,
        /* [out] */ Org::Apache::Http::IHttpEntity** entity);

    CARAPI IsOpen(
        /* [out] */ Boolean* isOpen);

    CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetLocalPort(
        /* [out] */ Int32* port);

    CARAPI GetRemoteAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetRemotePort(
        /* [out] */ Int32* port);

    CARAPI SetSocketTimeout(
        /* [in] */ Int32 timeout);

    CARAPI GetSocketTimeout(
    /* [out] */ Int32* timeout);

    CARAPI Shutdown();

    CARAPI Close();

    /**
     * Checks whether this connection has gone down.
     * Network connections may get closed during some time of inactivity
     * for several reasons. The next time a read is attempted on such a
     * connection it will throw an IOException.
     * This method tries to alleviate this inconvenience by trying to
     * find out if a connection is still usable. Implementations may do
     * that by attempting a read with a very small timeout. Thus this
     * method may block for a small amount of time before returning a result.
     * It is therefore an <i>expensive</i> operation.
     *
     * @return  <code>true</code> if attempts to use this connection are
     *          likely to succeed, or <code>false</code> if they are likely
     *          to fail and this connection should be closed
     */
    CARAPI IsStale(
        /* [out] */ Boolean* isStale);

    /**
     * Returns a collection of connection metrcis
     * @return HttpConnectionMetrics
     */
    CARAPI GetMetrics(
        /* [out] */  IHttpConnectionMetricsImpl** metrics);

protected:
    CARAPI DoFlush();

private:
    CARAPI AssertNotOpen();

    CARAPI AssertOpen();

    CARAPI_(Int64) DetermineLength(
        /* [in] */ IHeaders* headers);

private:
    AutoPtr<ISessionInputBuffer> mInbuffer;

    AutoPtr<ISessionOutputBuffer> mOutbuffer;

    Int32 mMaxHeaderCount;

    // store CoreConnectionPNames.MAX_LINE_LENGTH for performance
    Int32 mMaxLineLength;

    AutoPtr<IEntitySerializer> mEntityserializer;

    AutoPtr<IHttpMessageWriter> mRequestWriter;

    AutoPtr<IHttpConnectionMetricsImpl> mMetrics;

    /*volatile*/ Boolean mOpen;

    AutoPtr<ISocket> mSocket;

};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_CELASTOSHTTPCLIENTCONNECTION_H__
