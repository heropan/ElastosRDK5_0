
#ifndef __ELASTOS_DROID_NET_HTTP_CERTIFICATECHAINVALIDATOR_H__
#define __ELASTOS_DROID_NET_HTTP_CERTIFICATECHAINVALIDATOR_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * Class responsible for all server certificate validation functionality
 *
 * {@hide}
 */
class CertificateChainValidator
    : public Object
    , public ICertificateChainValidator
{
private:
    class NoPreloadHolder
        : public Object
    {
#if 0 // TODO: Translate codes below.
        static final CertificateChainValidator sInstance = new CertificateChainValidator();
            /**
             * The singleton instance of the hostname verifier.
             */
            private static final HostnameVerifier sVerifier = HttpsURLConnection
                    .getDefaultHostnameVerifier();
#endif
    };

public:
    CAR_INTERFACE_DECL()

    /**
     * @return The singleton instance of the certificates chain validator
     */
    static CARAPI GetInstance(
        /* [out] */ ICertificateChainValidator** result);

    /**
     * Performs the handshake and server certificates validation
     * Notice a new chain will be rebuilt by tracing the issuer and subject
     * before calling checkServerTrusted().
     * And if the last traced certificate is self issued and it is expired, it
     * will be dropped.
     * @param sslSocket The secure connection socket
     * @param domain The website domain
     * @return An SSL error object if there is an error and null otherwise
     */
    CARAPI DoHandshakeAndValidateServerCertificates(
        /* [in] */ IHttpsConnection* connection,
        /* [in] */ ISSLSocket* sslSocket,
        /* [in] */ String domain,
        /* [out] */ ISslError** result);

    /**
     * Similar to doHandshakeAndValidateServerCertificates but exposed to JNI for use
     * by Chromium HTTPS stack to validate the cert chain.
     * @param certChain The bytes for certificates in ASN.1 DER encoded certificates format.
     * @param domain The full website hostname and domain
     * @param authType The authentication type for the cert chain
     * @return An SSL error object if there is an error and null otherwise
     */
    static CARAPI VerifyServerCertificates(
        /* [in] */ ArrayOf<IArrayOf>* certChain,
        /* [in] */ String domain,
        /* [in] */ String authType,
        /* [out] */ ISslError** result);

    /**
     * Handles updates to credential storage.
     */
    static CARAPI HandleTrustStorageUpdate();

private:
    /**
     * Creates a new certificate chain validator. This is a private constructor.
     * If you need a Certificate chain validator, call getInstance().
     */
    CertificateChainValidator();

    /**
     * Common code of doHandshakeAndValidateServerCertificates and verifyServerCertificates.
     * Calls DomainNamevalidator to verify the domain, and TrustManager to verify the certs.
     * @param chain the cert chain in X509 cert format.
     * @param domain The full website hostname and domain
     * @param authType The authentication type for the cert chain
     * @return An SSL error object if there is an error and null otherwise
     */
    static CARAPI VerifyServerDomainAndCertificates(
        /* [in] */ ArrayOf<IX509Certificate*>* chain,
        /* [in] */ String domain,
        /* [in] */ String authType,
        /* [out] */ ISslError** result);

    /**
     * Returns the platform default {@link X509TrustManager}.
     */
    CARAPI GetTrustManager(
        /* [out] */ IX509TrustManager** result);

    CARAPI CloseSocketThrowException(
        /* [in] */ ISSLSocket* socket,
        /* [in] */ String errorMessage,
        /* [in] */ String defaultErrorMessage);

    CARAPI CloseSocketThrowException(
        /* [in] */ ISSLSocket* socket,
        /* [in] */ String errorMessage);

    static const String TAG;

    AutoPtr<IX509TrustManager> mTrustManager;

};

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_HTTP_CERTIFICATECHAINVALIDATOR_H__
