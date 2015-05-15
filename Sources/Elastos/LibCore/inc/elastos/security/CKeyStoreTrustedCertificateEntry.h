
#ifndef __CKEYSTORETRUSTEDCERTIFICATEENTRY_H__
#define __CKEYSTORETRUSTEDCERTIFICATEENTRY_H__

#include "_CKeyStoreTrustedCertificateEntry.h"

namespace Elastos {
namespace Security {

CarClass(CKeyStoreTrustedCertificateEntry)
{
public:
    CARAPI GetTrustedCertificate(
        /* [out] */ Elastos::Security::Cert::ICertificate **cert);

    CARAPI ToString(
        /* [out] */ String *str);

    CARAPI constructor(
        /* [in] */ Elastos::Security::Cert::ICertificate *trustCertificate);

private:
    // Store trusted Certificate
    AutoPtr<Elastos::Security::Cert::ICertificate> mTrustCertificate;
};

}
}

#endif // __CKEYSTORETRUSTEDCERTIFICATEENTRY_H__
