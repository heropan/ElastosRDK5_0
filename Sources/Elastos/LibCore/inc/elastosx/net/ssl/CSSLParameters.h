
#ifndef __CSSLPARAMETERS_H__
#define __CSSLPARAMETERS_H__

#include "_CSSLParameters.h"

namespace Elastosx {
namespace Net {
namespace Ssl {

CarClass(CSSLParameters)
{
public:
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

#endif // __CSSLPARAMETERS_H__
