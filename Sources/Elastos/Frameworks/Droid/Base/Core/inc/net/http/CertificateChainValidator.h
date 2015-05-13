
#ifndef __CERTIFICATECHAINVALIDATOR_H__
#define __CERTIFICATECHAINVALIDATOR_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

using Elastos::Security::Cert::IX509Certificate;
using Elastos::Net::Ssl::IDefaultHostnameVerifier;
using Elastos::Net::Ssl::ISSLSocket;

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
    : public ElRefBase
    , public IObject
    , public ICertificateChainValidator
{
    friend class CCertificateChainValidatorHelper;

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
        /* [in] */ Elastos::Droid::Net::Http::IHttpsConnection* connection,
        /* [in] */ ISSLSocket* sslSocket,
        /* [in] */ const String& sDomain,
        /* [out] */ Elastos::Droid::Net::Http::ISslError** err);

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
        /* [in] */ const String& sDomain,
        /* [in] */ const String& authType,
        /* [out] */ ISslError** err);

    CARAPI CloseSocketThrowException(
        /* [in] */ ISSLSocket* socket,
        /* [in] */ const String& errorMessage,
        /* [in] */ const String& defaultErrorMessage);

    CARAPI CloseSocketThrowException(
        /* [in] */ ISSLSocket* socket,
        /* [in] */ const String& errorMessage);

private:
    static AutoPtr<IDefaultHostnameVerifier> mVerifier;

};

}
}
}
}

#endif // __CERTIFICATECHAINVALIDATOR_H__
