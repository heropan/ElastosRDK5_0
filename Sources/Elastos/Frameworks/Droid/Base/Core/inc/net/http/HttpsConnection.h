
#ifndef __HTTPSCONNECTION_H__
#define __HTTPSCONNECTION_H__

#include "Connection.h"

using namespace Elastos::Net::Ssl;

using Elastos::Security::Cert::IX509Certificate;
using Elastos::IO::IFile;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * A requestConnection connecting to a normal (non secure) http server
 *
 * {@hide}
 */
class HttpsConnection
    : public IHttpsConnection
    , public ElRefBase
    , public Connection
    , public IObject
{
public:
    class LocalCX509TrustManager
        : public ElRefBase
        , public IObject
        , public IX509TrustManager
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

        LocalCX509TrustManager();

        ~LocalCX509TrustManager();

        CARAPI GetAcceptedIssuers(
            /* [out, callee] */ ArrayOf<IX509Certificate>** certs);

        CARAPI CheckClientTrusted(
            /* [in] */ ArrayOf<IX509Certificate>* certs,
            /* [in] */ const String& authType);

        CARAPI CheckServerTrusted(
            /* [in] */ ArrayOf<IX509Certificate>* certs,
            /* [in] */ const String& authType);
    };

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
     * Contructor for a https connection.
     */
    HttpsConnection(
        /* [in] */ IContext* context,
        /* [in] */ IHttpHost* host,
        /* [in] */ IHttpHost* proxy,
        /* [in] */ RequestFeeder* requestFeeder);

    ~HttpsConnection();

    /**
     * @hide
     *
     * @param sessionDir directory to cache SSL sessions
     */
    static CARAPI InitializeEngine(
        /* [in] */ IFile* sessionDir);

    /**
     * Sets the server SSL certificate associated with this
     * connection.
     * @param certificate The SSL certificate
     */
    CARAPI SetCertificate(
        /* [in] */ ISslCertificate* certificate);

    /**
     * Opens the connection to a http server or proxy.
     *
     * @return the opened low level connection
     * @throws IOException if the connection fails for any reason.
     */
    CARAPI OpenConnection(
        /* [in] */ Request* req,
        /* [out] */ IAndroidHttpClientConnection** scheme);

    /**
     * Closes the low level connection.
     *
     * If an exception is thrown then it is assumed that the connection will
     * have been closed (to the extent possible) anyway and the caller does not
     * need to take any further action.
     *
     */
    CARAPI CloseConnection();

    /**
     * Restart a secure connection suspended waiting for user interaction.
     */
    CARAPI RestartConnection(
        /* [in] */ Boolean proceed);

    CARAPI GetScheme(
        /* [out] */ String* scheme);

private:
    static ISSLSocketFactory* GetSocketFactory();

    CARAPI CheckErrorAndClose(
        /* [in] */ ECode ec,
        /* [in] */ IAndroidHttpClientConnection* connection);

private:
    /**
     * SSL socket factory
     */
    static AutoPtr<ISSLSocketFactory> mSslSocketFactory;

    /**
     * Object to wait on when suspending the SSL connection
     */
    Mutex mSuspendLock;

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

};

}
}
}
}

#endif // __HTTPCONNECTION_H__
