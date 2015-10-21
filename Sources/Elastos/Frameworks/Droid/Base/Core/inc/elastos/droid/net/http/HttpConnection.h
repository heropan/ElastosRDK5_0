
#ifndef __ELASTOS_DROID_NET_HTTP_HTTPCONNECTION_H__
#define __ELASTOS_DROID_NET_HTTP_HTTPCONNECTION_H__

#include "Connection.h"
#include "RequestFeeder.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * A requestConnection connecting to a normal (non secure) http server
 *
 * {@hide}
 */
class HttpConnection
    : public IConnection
    , public ElRefBase
    , public Connection
    , public IObject
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

    HttpConnection(
        /* [in] */ IContext* context,
        /* [in] */ IHttpHost* host,
        /* [in] */ RequestFeeder* requestFeeder);

    CARAPI GetScheme(
        /* [out] */ String* scheme);

    /**
     * Closes the low level connection.
     *
     * If an exception is thrown then it is assumed that the
     * connection will have been closed (to the extent possible)
     * anyway and the caller does not need to take any further action.
     *
     */
    CARAPI CloseConnection();

    /**
     * Opens the connection to a http server
     *
     * @return the opened low level connection
     * @throws IOException if the connection fails for any reason.
     */
    CARAPI OpenConnection(
        /* [in] */ Request* req,
        /* [out] */ IElastosHttpClientConnection** scheme);

    /**
     * Restart a secure connection suspended waiting for user interaction.
     */
    CARAPI RestartConnection(
        /* [in] */ Boolean abort);

    CARAPI ToString(
        /* [out] */ String* str);

private:
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_HTTPCONNECTION_H__
