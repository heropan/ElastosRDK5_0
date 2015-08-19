
#include "CCertificateFactory.h"
#include "CSecurity.h"

using Elastos::Security::CSecurity;

namespace Elastos {
namespace Security {
namespace Cert {

// Store CertificateFactory service name
static const String sSERVICE = String("CertificateFactory");
CAR_OBJECT_IMPL(CCertificateFactory);
CAR_INTERFACE_IMPL(CCertificateFactory, Object, ICertificateFactory);

ECode CCertificateFactory::GetInstance(
    /* [in] */ const String& type,
    /* [out] */ ICertificateFactory** certFact)
{
    if (type.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    /*
    try {
        Engine.SpiAndProvider sap = ENGINE.getInstance(type, null);
        return new CertificateFactory((CertificateFactorySpi) sap.spi, sap.provider, type);
    } catch (NoSuchAlgorithmException e) {
        throw new CertificateException(e);
    }
    */
    return NOERROR;
}

ECode CCertificateFactory::GetInstance(
    /* [in] */ const String& type,
    /* [in] */ const String& provider,
    /* [out] */ ICertificateFactory** certFact)
{
    VALIDATE_NOT_NULL(certFact)
    if (provider.IsNullOrEmpty()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ISecurity> security;
    FAIL_RETURN(CSecurity::AcquireSingleton((ISecurity**)&security))
    AutoPtr<IProvider> impProvider;
    security->GetProvider(provider, (IProvider**)&impProvider);
    if (impProvider == NULL) {
        return E_NO_SUCH_PROVIDER_EXCEPTION;
    }
    return GetInstance(type, impProvider, certFact);
}

ECode CCertificateFactory::GetInstance(
    /* [in] */ const String& type,
    /* [in] */ IProvider* provider,
    /* [out] */ ICertificateFactory** certFact)
{
    if (provider == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (type.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
/*
    try {
        Object spi = ENGINE.getInstance(type, provider, null);
        return new CertificateFactory((CertificateFactorySpi) spi, provider, type);
    } catch (NoSuchAlgorithmException e) {
        throw new CertificateException(e);
    }
*/
}

//with apache
// Used to access common engine functionality
//static const Engine ENGINE = new Engine(SERVICE);

ECode CCertificateFactory::GetProvider(
    /* [out] */ IProvider** provider)
{
    VALIDATE_NOT_NULL(provider)
    *provider = mProvider;
    return NOERROR;
}

ECode CCertificateFactory::GetType(
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type)
    *type = mType;
    return NOERROR;
}

ECode CCertificateFactory::GenerateCertificate(
    /* [in] */ IInputStream* inStream,
    /* [out] */ ICertificate** cert)
{
    VALIDATE_NOT_NULL(cert)
    return mSpiImpl->EngineGenerateCertificate(inStream, cert);
}

ECode CCertificateFactory::GetCertPathEncodings(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it)
    return mSpiImpl->EngineGetCertPathEncodings(it);
}

ECode CCertificateFactory::GenerateCertPath(
    /* [in] */ IInputStream* inStream,
    /* [out] */ ICertPath** certPath)
{
    VALIDATE_NOT_NULL(certPath)
    AutoPtr<IIterator> it;
    FAIL_RETURN(GetCertPathEncodings((IIterator**)&it))
    Boolean hasNext;
    if ((it->HasNext(&hasNext), !hasNext)) {
        return E_CERTIFICATE_EXCEPTION;
    }
    AutoPtr<IInterface> next;
    it->GetNext((IInterface**)&next);
    String nextString;
    //FAIL_RETURN(ICharSequence::Probe(next)->ToString(&nextString))
    return mSpiImpl->EngineGenerateCertPath(inStream, nextString, certPath);
}

ECode CCertificateFactory::GenerateCertPath(
    /* [in] */ IInputStream* inStream,
    /* [in] */ const String& encoding,
    /* [out] */ ICertPath** certPath)
{
    VALIDATE_NOT_NULL(certPath)
    return mSpiImpl->EngineGenerateCertPath(inStream, encoding, certPath);
}

ECode CCertificateFactory::GenerateCertPath(
    /* [in] */ IList* certificates,
    /* [out] */ ICertPath** certPath)
{
    VALIDATE_NOT_NULL(certPath)
    return mSpiImpl->EngineGenerateCertPath(certificates, certPath);
}

ECode CCertificateFactory::GenerateCertificates(
    /* [in] */ IInputStream* inStream,
    /* [out] */ ICollection** certs)
{
    VALIDATE_NOT_NULL(certs)
    return mSpiImpl->EngineGenerateCertificates(inStream, certs);
}

ECode CCertificateFactory::GenerateCRL(
    /* [in] */ IInputStream* inStream,
    /* [out] */ ICRL** crl)
{
    VALIDATE_NOT_NULL(crl)
    return mSpiImpl->EngineGenerateCRL(inStream, crl);
}

ECode CCertificateFactory::GenerateCRLs(
    /* [in] */ IInputStream* inStream,
    /* [out] */ ICollection** crls)
{
    VALIDATE_NOT_NULL(crls)
    return mSpiImpl->EngineGenerateCRLs(inStream, crls);
}

ECode CCertificateFactory::constructor()
{
    return NOERROR;
}

ECode CCertificateFactory::constructor(
    /* [in] */ ICertificateFactorySpi* certFacSpi,
    /* [in] */ IProvider* provider,
    /* [in] */ const String& type)
{
    mProvider = provider;
    mType = type;
    mSpiImpl = certFacSpi;
    return NOERROR;
}

} // end Cert
} // end Security
} // end Elastos
