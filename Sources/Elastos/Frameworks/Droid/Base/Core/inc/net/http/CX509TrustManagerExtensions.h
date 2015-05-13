
#ifndef __CX509TRUSTMANAGEREXTENSIONS_H__
#define __CX509TRUSTMANAGEREXTENSIONS_H__

#include "_CX509TrustManagerExtensions.h"
#include "elautoptr.h"

using Org::Apache::Harmony::Xnet::Provider::Jsse::ITrustManagerImpl;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * X509TrustManager wrapper exposing Android-added features.
 *
 * <p> The checkServerTrusted method allows callers to perform additional
 * verification of certificate chains after they have been successfully
 * verified by the platform.</p>
 */
CarClass(CX509TrustManagerExtensions)
{
public:
    /**
     * Constructs a new X509TrustManagerExtensions wrapper.
     *
     * @param tm A {@link X509TrustManager} as returned by TrustManagerFactory.getInstance();
     * @throws IllegalArgumentException If tm is an unsupported TrustManager type.
     */
    CARAPI constructor(
        /* [in] */ Elastos::Net::Ssl::IX509TrustManager* tm);

    /**
     * Verifies the given certificate chain.
     *
     * <p>See {@link X509TrustManager#checkServerTrusted(X509Certificate[], String)} for a
     * description of the chain and authType parameters. The final parameter, host, should be the
     * hostname of the server.</p>
     *
     * @throws CertificateException if the chain does not verify correctly.
     * @return the properly ordered chain used for verification as a list of X509Certificates.
     */
    CARAPI CheckServerTrusted(
        /* [in] */ const ArrayOf<Elastos::Security::Cert::IX509Certificate *> & chain,
        /* [in] */ const String& authType,
        /* [in] */ const String& host,
        /* [out] */ IObjectContainer** certificates);

private:
    AutoPtr<ITrustManagerImpl> mDelegate;
};

}
}
}
}

#endif // __CX509TRUSTMANAGEREXTENSIONS_H__
