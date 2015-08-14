
#ifndef __ELASTOS_DROID_NET_HTTP_CCERTIFICATECHAINVALIDATORHELPER_H__
#define __ELASTOS_DROID_NET_HTTP_CCERTIFICATECHAINVALIDATORHELPER_H__

#include "_CCertificateChainValidatorHelper.h"
#include "CertificateChainValidator.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CarClass(CCertificateChainValidatorHelper)
{
public:
    /**
     * @return The singleton instance of the certificates chain validator
     */
    CARAPI GetInstance(
        /* [out] */ Elastos::Droid::Net::Http::ICertificateChainValidator** instance);

    /**
     * Similar to doHandshakeAndValidateServerCertificates but exposed to JNI for use
     * by Chromium HTTPS stack to validate the cert chain.
     * @param certChain The bytes for certificates in ASN.1 DER encoded certificates format.
     * @param domain The full website hostname and domain
     * @param authType The authentication type for the cert chain
     * @return An SSL error object if there is an error and null otherwise
     */
    CARAPI VerifyServerCertificates(
        /* [in] */ IObjectContainer* certChain,
        /* [in] */ const String& sDomain,
        /* [in] */ const String& authType,
        /* [out] */ Elastos::Droid::Net::Http::ISslError** err);

    /**
     * Handles updates to credential storage.
     */
    CARAPI HandleTrustStorageUpdate();

private:
    /**
     * The singleton instance of the certificate chain validator
     */
    static AutoPtr<ICertificateChainValidator> mInstance;

};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_CCERTIFICATECHAINVALIDATORHELPER_H__
