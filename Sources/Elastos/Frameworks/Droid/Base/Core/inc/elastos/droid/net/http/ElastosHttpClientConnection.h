
#ifndef __ELASTOS_DROID_NET_HTTP_ELASTOSHTTPCLIENTCONNECTION_H__
#define __ELASTOS_DROID_NET_HTTP_ELASTOSHTTPCLIENTCONNECTION_H__

#include "elastos/droid/ext/frameworkext.h"

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
class ElastosHttpClientConnection
    : public $SUPER_CLASS$
    , public IElastosHttpClientConnection
{
public:
    CAR_INTERFACE_DECL()

    public:
    CARAPI constructor();

    /**
     * Bind socket and set HttpParams to AndroidHttpClientConnection
     * @param socket outgoing socket
     * @param params HttpParams
     * @throws IOException
      */
    CARAPI Bind(
        /* [in] */  ,
        /* [in] */  );

    // @Override
    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI IsOpen(
        /* [out] */ Boolean* result);

    CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** result);

    CARAPI GetLocalPort(
        /* [out] */ Int32* result);

    CARAPI GetRemoteAddress(
        /* [out] */ IInetAddress** result);

    CARAPI GetRemotePort(
        /* [out] */ Int32* result);

    CARAPI SetSocketTimeout(
        /* [in] */ Int32 timeout);

    CARAPI GetSocketTimeout(
        /* [out] */ Int32* result);

    CARAPI Shutdown();

    CARAPI Close();

    /**
     * Sends the request line and all headers over the connection.
     * @param request the request whose headers to send.
     * @throws HttpException
     * @throws IOException
     */
    CARAPI SendRequestHeader(
        /* [in] */  );

    /**
     * Sends the request entity over the connection.
     * @param request the request whose entity to send.
     * @throws HttpException
     * @throws IOException
     */
    CARAPI SendRequestEntity(
        /* [in] */  );

    CARAPI DoFlush();

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
        /* [in] */ IHeaders* headers,
        /* [out] */ IStatusLine** result);

    /**
     * Return the next response entity.
     * @param headers contains values for parsing entity
     * @see HttpClientConnection#receiveResponseEntity(HttpResponse response)
     */
    CARAPI ReceiveResponseEntity(
        /* [in] */  ,
        /* [out] */ IHttpEntity** result);

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
        /* [out] */ Boolean* result);

    /**
     * Returns a collection of connection metrcis
     * @return HttpConnectionMetrics
     */
    CARAPI GetMetrics(
        /* [out] */ IHttpConnectionMetrics** result);

    private:
    CARAPI AssertNotOpen();

    CARAPI AssertOpen();

    CARAPI DetermineLength(
        /* [in] */  ,
        /* [out] */ Int64* result);

    ISessionInputBuffer* mInbuffer;

    ISessionOutputBuffer* mOutbuffer;

    Int32 mMaxHeaderCount;

    // store CoreConnectionPNames.MAX_LINE_LENGTH for performance
    Int32 mMaxLineLength;

    const IEntitySerializer* entityserializer;

    IHttpMessageWriter* mRequestWriter;

    IHttpConnectionMetricsImpl* mMetrics;

    Ivolatile* mOpen;

    ISocket* mSocket;

};

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_HTTP_ELASTOSHTTPCLIENTCONNECTION_H__
