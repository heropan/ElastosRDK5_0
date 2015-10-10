
#ifndef __ELASTOS_DROID_NET_HTTP_CONNECTION_H__
#define __ELASTOS_DROID_NET_HTTP_CONNECTION_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/List.h>
#include "RequestFeeder.h"

using namespace Elastos::Core;
using namespace Elastos::Utility;
using namespace Org::Apache::Http;

using Elastos::Droid::Content::IContext;
using Org::Apache::Http::Protocol::IHttpContext;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

class Request;

/**
 * {@hide}
 */
class Connection
{
public:
    CARAPI_(AutoPtr<IHttpHost>) GetHost();

    /**
     * connection factory: returns an HTTP or HTTPS connection as
     * necessary
     */
    static IConnection* GetConnection(
        /* [in] */ IContext* context,
        /* [in] */ IHttpHost* host,
        /* [in] */ IHttpHost* proxy,
        /* [in] */ RequestFeeder* requestFeeder);

    /**
     * Close current network connection
     * Note: this runs in non-network thread
     */
    CARAPI Cancel();

    /**
     * Process requests in queue
     * pipelines requests
     */
    CARAPI ProcessRequests(
        /* [in] */ Request* firstRequest);

    CARAPI GetHttpContext(
        /* [out] */ IHttpContext** context);

    CARAPI SetCanPersist(
        /* [in] */ IHttpEntity* entity,
        /* [in] */ IProtocolVersion* ver,
        /* [in] */ Int32 connType);

    CARAPI SetCanPersist(
        /* [in] */ Boolean canPersist);

    CARAPI GetCanPersist(
        /* [out] */ Boolean* canPersist);

    /** typically http or https... set by subclass */
    virtual CARAPI GetScheme(
        /* [out] */ String* scheme) = 0;

    virtual CARAPI CloseConnection() = 0;

    virtual CARAPI OpenConnection(
        /* [in] */ Request* req,
        /* [out] */ IAndroidHttpClientConnection** scheme) = 0;

    /**
     * Prints request queue to log, for debugging.
     * returns request count
     */
    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI GetBuf(
        /* [out, callee] */ ArrayOf<Byte>** buf);

protected:
    Connection(
        /* [in] */ IContext* context,
        /* [in] */ IHttpHost* host,
        /* [in] */ RequestFeeder* requestFeeder);

private:
    static AutoPtr<ArrayOf<String> > InitSTATES();

    /**
     * After a send/receive failure, any pipelined requests must be
     * cleared back to the mRequest queue
     * @return true if mRequests is empty after pipe cleared
     */
    CARAPI_(Boolean) ClearPipe(
        /* [in] */ /*LinkedList<Request>*/ List<AutoPtr<Request> >& pipe);

    /**
     * @return true on success
     */
    CARAPI_(Boolean) OpenHttpConnection(
        /* [in] */ Request* req);

    /**
     * Helper.  Calls the mEventHandler's error() method only if
     * request failed permanently.  Increments mFailcount on failure.
     *
     * Increments failcount only if the network is believed to be
     * connected
     *
     * @return true if request can be retried (less than
     * RETRY_REQUEST_LIMIT failures have occurred).
     */
    CARAPI_(Boolean) HttpFailure(
        /* [in] */ Request* req,
        /* [in] */ Int32 errorId,
        /* [in] */ Boolean isException);

    /**
     * Use same logic as ConnectionReuseStrategy
     * @see ConnectionReuseStrategy
     */
    CARAPI_(Boolean) KeepAlive(
        /* [in] */ IHttpEntity* entity,
        /* [in] */ IProtocolVersion* ver,
        /* [in] */ Int32 connType,
        /* [in] */ IHttpContext* context);

public:
    /**
     * Allow a TCP connection 60 idle seconds before erroring out
     */
    static const Int32 SOCKET_TIMEOUT;

    AutoPtr<IContext> mContext;

    /**
     * The host this connection is connected to.  If using proxy,
     * this is set to the proxy address
     */
    AutoPtr<IHttpHost> mHost;

    RequestFeeder* mRequestFeeder;

protected:
    /** The low level connection */
    AutoPtr<IAndroidHttpClientConnection> mHttpClientConnection;

    /**
     * The server SSL certificate associated with this connection
     * (null if the connection is not secure)
     * It would be nice to store the whole certificate chain, but
     * we want to keep things as light-weight as possible
     */
    AutoPtr<ISslCertificate> mCertificate;

private:
    static const Int32 SEND;

    static const Int32 READ;

    static const Int32 DRAIN;

    static const Int32 DONE;

    static const AutoPtr<ArrayOf<String> >STATES;

    /** true if the connection can be reused for sending more requests */
    Boolean mCanPersist;

    /** context required by ConnectionReuseStrategy. */
    AutoPtr<IHttpContext> mHttpContext;

    /** set when cancelled */
    static Int32 STATE_NORMAL;

    static Int32 STATE_CANCEL_REQUESTED;

    Int32 mActive;

    /** The number of times to try to re-connect (if connect fails). */
    static const Int32 RETRY_REQUEST_LIMIT;

    static const Int32 MIN_PIPE;

    static const Int32 MAX_PIPE;

    /**
     * Doesn't seem to exist anymore in the new HTTP client, so copied here.
     */
    static const String HTTP_CONNECTION;

    /**
     * Buffer for feeding response blocks to webkit.  One block per
     * connection reduces memory churn.
     */
    AutoPtr<ArrayOf<Byte> > mBuf;

};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_CONNECTION_H__
