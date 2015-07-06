
#ifndef __ELASTOSX_NET_SSL_CSSLPARAMETERS_H__
#define __ELASTOSX_NET_SSL_CSSLPARAMETERS_H__

#include "_Elastosx_Net_Ssl_CSSLParameters.h"
#include <elastos/core/Object.h>

namespace Elastosx {
namespace Net {
namespace Ssl {

CarClass(CSSLParameters)
    , public Object
    , public ISSLParameters
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI GetCipherSuites(
        /* [out, callee] */ ArrayOf<String>** suites);

    CARAPI SetCipherSuites(
        /* [in] */ ArrayOf<String>* cipherSuites);

    CARAPI GetProtocols(
        /* [out, callee] */ ArrayOf<String>** protocols);

    CARAPI SetProtocols(
        /* [in] */ ArrayOf<String>* protocols);

    CARAPI GetNeedClientAuth(
        /* [out] */ Boolean* auth);

    CARAPI SetNeedClientAuth(
        /* [in] */ Boolean needClientAuth);

    CARAPI GetWantClientAuth(
        /* [out] */ Boolean* auth);

    CARAPI SetWantClientAuth(
        /* [in] */ Boolean wantClientAuth);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ArrayOf<String>* cipherSuites);

    CARAPI constructor(
        /* [in] */ ArrayOf<String>* cipherSuites,
        /* [in] */ ArrayOf<String>* protocols);

private:
    AutoPtr<ArrayOf<String> > mCipherSuites;
    AutoPtr<ArrayOf<String> > mProtocols;
    Boolean mNeedClientAuth;
    Boolean mWantClientAuth;
};

} // namespace Ssl
} // namespace Net
} // namespace Elastosx

#endif // __ELASTOSX_NET_SSL_CSSLPARAMETERS_H__
