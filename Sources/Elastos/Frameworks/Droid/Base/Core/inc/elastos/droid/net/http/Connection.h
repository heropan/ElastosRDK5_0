
#ifndef __ELASTOS_DROID_NET_HTTP_CONNECTION_H__
#define __ELASTOS_DROID_NET_HTTP_CONNECTION_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Content::IContext;

using Elastos::Utility::ILinkedList;
using Org::Apache::Http::IHttpEntity;
using Org::Apache::Http::IHttpHost;
using Org::Apache::Http::IProtocolVersion;
using Org::Apache::Http::Protocol::IHttpContext;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * {@hide}
 */
class Connection
    : public Object
    , public IConnection
{
public:
    CAR_INTERFACE_DECL()

    Connection();

    virtual ~Connection();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IHttpHost* host,
        /* [in] */ IRequestFeeder* requestFeeder);

    CARAPI GetHost(
        /* [out] */ IHttpHost** result);

    /**
     * connection factory: returns an HTTP or HTTPS connection as
     * necessary
     */
    static CARAPI GetConnection(
        /* [in] */ IContext* context,
        /* [in] */ IHttpHost* host,
        /* [in] */ IHttpHost* proxy,
        /* [in] */ IRequestFeeder* requestFeeder,
        /* [out] */ IConnection** result);

    /* package */
    CARAPI GetCertificate(
        /* [out] */ ISslCertificate** result);

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
        /* [in] */ IRequest* firstRequest);

    CARAPI GetHttpContext(
        /* [out] */ IHttpContext** result);

    CARAPI SetCanPersist(
        /* [in] */ IHttpEntity* entity,
        /* [in] */ IProtocolVersion* ver,
        /* [in] */ Int32 connType);

    CARAPI SetCanPersist(
        /* [in] */ Boolean canPersist);

    CARAPI GetCanPersist(
        /* [out] */ Boolean* result);

    /** typically http or https... set by subclass */
    virtual CARAPI GetScheme(
        /* [out] */ String* result) = 0;

    virtual CARAPI CloseConnection() = 0;

    virtual CARAPI OpenConnection(
        /* [in] */ IRequest* req,
        /* [out] */ IElastosHttpClientConnection** result) = 0;

    /**
     * Prints request queue to log, for debugging.
     * returns request count
     */
    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI GetBuf(
        /* [out, callee] */ ArrayOf<Byte>** result);

private:
    /**
     * After a send/receive failure, any pipelined requests must be
     * cleared back to the mRequest queue
     * @return true if mRequests is empty after pipe cleared
     */
    CARAPI ClearPipe(
        /* [in] */ ILinkedList* pipe,
        /* [out] */ Boolean* result);

    /**
     * @return true on success
     */
    CARAPI OpenHttpConnection(
        /* [in] */ IRequest* req,
        /* [out] */ Boolean* result);

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
    CARAPI HttpFailure(
        /* [in] */ IRequest* req,
        /* [in] */ Int32 errorId,
        /* [in] */ ECode e,
        /* [out] */ Boolean* result);

    /**
     * Use same logic as ConnectionReuseStrategy
     * @see ConnectionReuseStrategy
     */
    CARAPI KeepAlive(
        /* [in] */ IHttpEntity* entity,
        /* [in] */ IProtocolVersion* ver,
        /* [in] */ Int32 connType,
        /* [in] */ const IHttpContext* context,
        /* [out] */ Boolean* result);

    static CARAPI_(AutoPtr<ArrayOf<String> >) InitSTATES();

public:
    /**
     * Allow a TCP connection 60 idle seconds before erroring out
     */
    static const Int32 SOCKET_TIMEOUT;

    AutoPtr<IContext> mContext;

    /** The low level connection */
    AutoPtr<IElastosHttpClientConnection> mHttpClientConnection;

    /**
     * The server SSL certificate associated with this connection
     * (null if the connection is not secure)
     * It would be nice to store the whole certificate chain, but
     * we want to keep things as light-weight as possible
     */
    AutoPtr<ISslCertificate> mCertificate;

    /**
     * The host this connection is connected to.  If using proxy,
     * this is set to the proxy address
     */
    AutoPtr<IHttpHost> mHost;

    AutoPtr<IRequestFeeder> mRequestFeeder;

private:
    static const AutoPtr<ArrayOf<String> > STATES;

    static const Int32 SEND;

    static const Int32 READ;

    static const Int32 DRAIN;

    static const Int32 DONE;

    /** true if the connection can be reused for sending more requests */
    Boolean mCanPersist;

    /** context required by ConnectionReuseStrategy. */
    AutoPtr<IHttpContext> mHttpContext;

    /** set when cancelled */
    static Int32 sSTATE_NORMAL;

    static Int32 sSTATE_CANCEL_REQUESTED;

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

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_HTTP_CONNECTION_H__
