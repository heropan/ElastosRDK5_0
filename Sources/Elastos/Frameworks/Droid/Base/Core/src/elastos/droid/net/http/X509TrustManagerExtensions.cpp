
#include "elastos/droid/net/http/X509TrustManagerExtensions.h"

using Elastosx::Net::Ssl::ISSLParameters;
using Elastosx::Net::Ssl::ISSLSocket;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL(X509TrustManagerExtensions, Object, IX509TrustManagerExtensions)

ECode X509TrustManagerExtensions::constructor(
    /* [in] */ IX509TrustManager* tm)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(tm);

    AutoPtr<ITrustManagerImpl> impl;
    impl = (ITrustManagerImpl*)tm->Probe(Org::Apache::Harmony::Xnet::Provider::Jsse::EIID_ITrustManagerImpl);

    if (impl == NULL) {
            throw new IllegalArgumentException("tm is an instance of " + tm.getClass().getName() +
                    " which is not a supported type of X509TrustManager");
        Logger::E();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mDelegate = impl;
    return NOERROR;
#endif
}

ECode X509TrustManagerExtensions::CheckServerTrusted(
    /* [in] */ ArrayOf<IX509Certificate*>* chain,
    /* [in] */ const String& authType,
    /* [in] */ const String& host,
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // return mDelegate->CheckServerTrusted(chain, authType, host, certificates);
    return E_NOT_IMPLEMENTED;
#endif
}

ECode X509TrustManagerExtensions::IsUserAddedCertificate(
    /* [in] */ IX509Certificate* cert,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mDelegate.isUserAddedCertificate(cert);

#endif
}


} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
