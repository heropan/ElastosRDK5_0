
#ifndef __CCERTIFICATEFACTORY_H__
#define __CCERTIFICATEFACTORY_H__

#include "_CCertificateFactory.h"

#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

using Elastos::Security::IProvider;
using Elastos::Security::ISecurity;
using Elastos::IO::IInputStream;
using Elastos::Utility::IIterator;
using Elastos::Utility::IList;
using Elastos::Utility::ICollection;

namespace Elastos {
namespace Security {
namespace Cert {

CarClass(CCertificateFactory)
{
public:
    static CARAPI GetInstance(
    /* [in] */ const String& type,
    /* [out] */ ICertificateFactory** certFact);

    static CARAPI GetInstance(
        /* [in] */ const String& type,
        /* [in] */ const String& provider,
        /* [out] */ ICertificateFactory** certFact);

    static CARAPI GetInstance(
    /* [in] */ const String& type,
    /* [in] */ IProvider* provider,
    /* [out] */ ICertificateFactory** certFact);

    CARAPI GetProvider(
        /* [out] */ IProvider** provider);

    CARAPI GetType(
        /* [out] */ String* type);

    CARAPI GenerateCertificate(
        /* [in] */ IInputStream* inStream,
        /* [out] */ ICertificate** cert);

    CARAPI GetCertPathEncodings(
        /* [out] */ IIterator** it);

    CARAPI GenerateCertPath(
        /* [in] */ IInputStream* inStream,
        /* [out] */ ICertPath** certPath);

    CARAPI GenerateCertPathEx(
        /* [in] */ IInputStream* inStream,
        /* [in] */ const String& encoding,
        /* [out] */ ICertPath** certPath);

    CARAPI GenerateCertPathEx2(
        /* [in] */ IList* certificates,
        /* [out] */ ICertPath** certPath);

    CARAPI GenerateCertificates(
        /* [in] */ IInputStream* inStream,
        /* [out] */ ICollection** certs);

    CARAPI GenerateCRL(
        /* [in] */ IInputStream* inStream,
        /* [out] */ ICRL** crl);

    CARAPI GenerateCRLs(
        /* [in] */ IInputStream* inStream,
        /* [out] */ ICollection** crls);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ICertificateFactorySpi* certFacSpi,
        /* [in] */ IProvider* provider,
        /* [in] */ const String& type);

private:
    // Store used provider
    const AutoPtr<IProvider> mProvider;

    // Store used CertificateFactorySpi implementation
    const AutoPtr<ICertificateFactorySpi> mSpiImpl;

    // Store used type
    const String mType;
};

} // end Cert
} // end Security
} // end Elastos
#endif // __CCERTIFICATEFACTORY_H__
