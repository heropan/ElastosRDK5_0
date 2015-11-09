
#ifndef __ELASTOS_DROID_NET_HTTP_HTTPSCONNECTION_H__
#define __ELASTOS_DROID_NET_HTTP_HTTPSCONNECTION_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/net/http/Connection.h"

using Elastos::IO::IFile;
using Elastosx::Net::Ssl::ISSLSocketFactory;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * A Connection connecting to a secure http server or tunneling through
 * a http proxy server to a https server.
 *
 * @hide
 */
class HttpsConnection
    : public Connection
    , public IHttpsConnection
{
public:
    CAR_INTERFACE_DECL()

    HttpsConnection();

    /**
     * @hide
     *
     * @param sessionDir directory to cache SSL sessions
     */
    static CARAPI InitializeEngine(
        /* [in] */ IFile* sessionDir);

    /**
     * Contructor for a https connection.
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IHttpHost* host,
        /* [in] */ IHttpHost* proxy,
        /* [in] */ IRequestFeeder* requestFeeder);

    /* package */
    CARAPI SetCertificate(
        /* [in] */ ISslCertificate* certificate);

    /**
     * Opens the connection to a http server or proxy.
     *
     * @return the opened low level connection
     * @throws IOException if the connection fails for any reason.
     */
    // @Override
    CARAPI OpenConnection(
        /* [in] */ IRequest* req,
        /* [out] */ IElastosHttpClientConnection** result);

    /**
     * Closes the low level connection.
     *
     * If an exception is thrown then it is assumed that the connection will
     * have been closed (to the extent possible) anyway and the caller does not
     * need to take any further action.
     *
     */
    // @Override
    CARAPI CloseConnection();

    /**
     * Restart a secure connection suspended waiting for user interaction.
     */
    CARAPI RestartConnection(
        /* [in] */ Boolean proceed);

    // @Override
    CARAPI GetScheme(
        /* [out] */ String* result);

private:
    static CARAPI GetSocketFactory(
        /* [out] */ ISSLSocketFactory** result);

private:
    /**
     * SSL socket factory
     */
    static AutoPtr<ISSLSocketFactory> sSslSocketFactory;

    /**
     * Object to wait on when suspending the SSL connection
     */
    AutoPtr<IInterface> mSuspendLock;

    /**
     * True if the connection is suspended pending the result of asking the
     * user about an error.
     */
    Boolean mSuspended;

    /**
     * True if the connection attempt should be aborted due to an ssl
     * error.
     */
    Boolean mAborted;

    // Used when connecting through a proxy.
    AutoPtr<IHttpHost> mProxyHost;

    static ECode mEnableStaticBlock;
};

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_HTTP_HTTPSCONNECTION_H__
