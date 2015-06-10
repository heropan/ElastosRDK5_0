
#include "CCertificateFactory.h"
#include "CSecurity.h"

using Elastos::Security::CSecurity;

namespace Elastos {
namespace Security {
namespace Cert {

// Store CertificateFactory service name
static const String sSERVICE = String("CertificateFactory");

/**
 * Creates a new {@code CertificateFactory} instance that provides the
 * requested certificate type.
 *
 * @param type
 *            the certificate type.
 * @return the new {@code CertificateFactory} instance.
 * @throws CertificateException
 *             if the specified certificate type is not available at any
 *             installed provider.
 * @throws NullPointerException if {@code type == null}
 */
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

/**
 * Creates a new {@code CertificateFactory} instance from the specified
 * provider that provides the requested certificate type.
 *
 * @param type
 *            the certificate type.
 * @param provider
 *            the name of the provider providing certificates of the
 *            specified type.
 * @return the new {@code CertificateFactory} instance.
 * @throws CertificateException
 *             if the specified certificate type is not available by the
 *             specified provider.
 * @throws NoSuchProviderException
 *             if no provider with the specified name can be found.
 * @throws IllegalArgumentException if {@code provider == null || provider.isEmpty()}
 * @throws NullPointerException
 *             it {@code type} is {@code null}.
 */
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

/**
 * Creates a new {@code CertificateFactory} instance from the specified
 * provider that provides the requested certificate type.
 *
 * @param type
 *            the certificate type.
 * @param provider
 *            the name of the provider providing certificates of the
 *            specified type.
 * @return the new {@code CertificateFactory} instance.
 * @throws CertificateException
 *             if the specified certificate type is not available at the
 *             specified provider.
 * @throws IllegalArgumentException
 *             if the specified provider is {@code null}.
 * @throws NullPointerException if {@code type == null}
 * @throws IllegalArgumentException if {@code provider == null}
 */
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

/**
 * Returns the {@code Provider} of the certificate factory represented by
 * the certificate.
 *
 * @return the provider of this certificate factory.
 */
ECode CCertificateFactory::GetProvider(
    /* [out] */ IProvider** provider)
{
    VALIDATE_NOT_NULL(provider)
    *provider = mProvider;
    return NOERROR;
}

/**
 * Returns the Certificate type.
 *
 * @return type of certificate being used.
 */
ECode CCertificateFactory::GetType(
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type)
    *type = mType;
    return NOERROR;
}

/**
 * Generates and initializes a {@code Certificate} from the provided input
 * stream.
 *
 * @param inStream
 *            the stream from where data is read to create the {@code
 *            Certificate}.
 * @return an initialized Certificate.
 * @throws CertificateException
 *             if parsing problems are detected.
 */
ECode CCertificateFactory::GenerateCertificate(
    /* [in] */ IInputStream* inStream,
    /* [out] */ ICertificate** cert)
{
    VALIDATE_NOT_NULL(cert)
    return mSpiImpl->EngineGenerateCertificate(inStream, cert);
}

/**
 * Returns an {@code Iterator} over the supported {@code CertPath} encodings
 * (as Strings). The first element is the default encoding scheme to apply.
 *
 * @return an iterator over supported {@link CertPath} encodings (as
 *         Strings).
 */
ECode CCertificateFactory::GetCertPathEncodings(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it)
    return mSpiImpl->EngineGetCertPathEncodings(it);
}

/**
 * Generates a {@code CertPath} (a certificate chain) from the provided
 * {@code InputStream}. The default encoding scheme is applied.
 *
 * @param inStream
 *            {@code InputStream} with encoded data.
 * @return a {@code CertPath} initialized from the provided data.
 * @throws CertificateException
 *             if parsing problems are detected.
 */
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
    return mSpiImpl->EngineGenerateCertPathEx(inStream, nextString, certPath);
}

/**
 * Generates a {@code CertPath} (a certificate chain) from the provided
 * {@code InputStream} and the specified encoding scheme.
 *
 * @param inStream
 *            {@code InputStream} containing certificate path data in
 *            specified encoding.
 * @param encoding
 *            encoding of the data in the input stream.
 * @return a {@code CertPath} initialized from the provided data.
 * @throws CertificateException
 *             if parsing problems are detected.
 * @throws UnsupportedOperationException
 *             if the provider does not implement this method.
 */
ECode CCertificateFactory::GenerateCertPathEx(
    /* [in] */ IInputStream* inStream,
    /* [in] */ const String& encoding,
    /* [out] */ ICertPath** certPath)
{
    VALIDATE_NOT_NULL(certPath)
    return mSpiImpl->EngineGenerateCertPathEx(inStream, encoding, certPath);
}

/**
 * Generates a {@code CertPath} from the provided list of certificates. The
 * encoding is the default encoding.
 *
 * @param certificates
 *            the list containing certificates in a format supported by the
 *            {@code CertificateFactory}.
 * @return a {@code CertPath} initialized from the provided data.
 * @throws CertificateException
 *             if parsing problems are detected.
 * @throws UnsupportedOperationException
 *             if the provider does not implement this method.
 */
ECode CCertificateFactory::GenerateCertPathEx2(
    /* [in] */ IList* certificates,
    /* [out] */ ICertPath** certPath)
{
    VALIDATE_NOT_NULL(certPath)
    return mSpiImpl->EngineGenerateCertPathEx2(certificates, certPath);
}

/**
 * Generates and initializes a collection of (unrelated) certificates from
 * the provided input stream.
 *
 * @param inStream
 *            the stream from which the data is read to create the
 *            collection.
 * @return an initialized collection of certificates.
 * @throws CertificateException
 *             if parsing problems are detected.
 */
ECode CCertificateFactory::GenerateCertificates(
    /* [in] */ IInputStream* inStream,
    /* [out] */ ICollection** certs)
{
    VALIDATE_NOT_NULL(certs)
    return mSpiImpl->EngineGenerateCertificates(inStream, certs);
}

/**
 * Generates and initializes a <i>Certificate Revocation List</i> (CRL) from
 * the provided input stream.
 *
 * @param inStream
 *            the stream from where data is read to create the CRL.
 * @return an initialized CRL.
 * @exception CRLException
 *                if parsing problems are detected.
 */
ECode CCertificateFactory::GenerateCRL(
    /* [in] */ IInputStream* inStream,
    /* [out] */ ICRL** crl)
{
    VALIDATE_NOT_NULL(crl)
    return mSpiImpl->EngineGenerateCRL(inStream, crl);
}

/**
 * Generates and initializes a collection of <i>Certificate Revocation
 * List</i> (CRL) from the provided input stream.
 *
 * @param inStream
 *            the stream from which the data is read to create the CRLs.
 * @return an initialized collection of CRLs.
 * @exception CRLException
 *                if parsing problems are detected.
 */
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

/**
 * Creates a new {@code CertificateFactory} instance.
 *
 * @param certFacSpi
 *            the implementation delegate.
 * @param provider
 *            the associated provider.
 * @param type
 *            the certificate type.
 */
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
