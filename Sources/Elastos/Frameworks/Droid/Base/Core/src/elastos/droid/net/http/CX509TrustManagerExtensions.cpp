
#include "CX509TrustManagerExtensions.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

ECode CX509TrustManagerExtensions::constructor(
    /* [in] */ Elastos::Net::Ssl::IX509TrustManager* tm)
{
    VALIDATE_NOT_NULL(tm);

    AutoPtr<ITrustManagerImpl> impl;
    impl = (ITrustManagerImpl*)tm->Probe(Org::Apache::Harmony::Xnet::Provider::Jsse::EIID_ITrustManagerImpl);

    if (impl == NULL) {
        Logger::E(String("CX509TrustManagerExtensions"), String("tm is not a supported type of X509TrustManager"));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mDelegate = impl;
    return NOERROR;
}

ECode CX509TrustManagerExtensions::CheckServerTrusted(
    /* [in] */ const ArrayOf<Elastos::Security::Cert::IX509Certificate *> & chain,
    /* [in] */ const String& authType,
    /* [in] */ const String& host,
    /* [out] */ IObjectContainer** certificates)
{
    // return mDelegate->CheckServerTrusted(chain, authType, host, certificates);
    return E_NOT_IMPLEMENTED;
}

}
}
}
}

