
#ifndef __CKEYSTOREPRIVATEKEYENTRY_H__
#define __CKEYSTOREPRIVATEKEYENTRY_H__

#include "_CKeyStorePrivateKeyEntry.h"

namespace Elastos {
namespace Security {

CarClass(CKeyStorePrivateKeyEntry)
{
public:
    CARAPI GetPrivateKey(
        /* [out] */ IPrivateKey **privateKey);

    CARAPI GetCertificateChain(
        /* [out, callee] */ ArrayOf<Elastos::Security::Cert::ICertificate*> **cc);

    CARAPI GetCertificate(
        /* [out] */ Elastos::Security::Cert::ICertificate **cert);

    CARAPI ToString(
        /* [out] */ String *str);

    CARAPI constructor(
        /* [in] */ IPrivateKey *privateKey,
        /* [in] */ ArrayOf<Elastos::Security::Cert::ICertificate*> *chain);

private:
    // Store Certificate chain
    AutoPtr<ArrayOf<Elastos::Security::Cert::ICertificate*> > mChain;

    // Store PrivateKey
    AutoPtr<IPrivateKey> mPrivateKey;

};

}
}

#endif // __CKEYSTOREPRIVATEKEYENTRY_H__
