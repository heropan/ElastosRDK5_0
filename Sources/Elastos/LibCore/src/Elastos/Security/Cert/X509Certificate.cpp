
#include "cmdef.h"
#include "X509Certificate.h"
#include "CByteArrayInputStream.h"
#include "CCertificateFactoryHelper.h"

using Elastos::IO::IInputStream;
using Elastos::IO::IByteArrayInputStream;
using Elastos::IO::CByteArrayInputStream;

namespace Elastos {
namespace Security {
namespace Cert {

static const Int64 serialVersionUID = -2491127588187038216L;

CAR_INTERFACE_IMPL(X509Certificate, IX509Extension)

X509Certificate::X509Certificate()
    : Certificate(String("X.509"))
{}

ECode X509Certificate::GetIssuerX500Principal(
    /* [out] */ IX500Principal** issuer)
{
    VALIDATE_NOT_NULL(issuer)
    // TODO if there is no X.509 certificate provider installed
    // should we try to access Harmony X509CertImpl via classForName?
    AutoPtr<ICertificateFactoryHelper> helper;
    AutoPtr<ICertificateFactory> factory;
    CCertificateFactoryHelper::AcquireSingleton((ICertificateFactoryHelper**)&helper);
    helper->GetInstance(String("X.509"), (ICertificateFactory**)&factory);
    AutoPtr<ArrayOf<Byte> > encode;
    AutoPtr<IByteArrayInputStream> bais;
    AutoPtr<ICertificate> cert;
    AutoPtr<IX509Certificate> x509Cert;
    AutoPtr<IX500Principal> principal;
    FAIL_GOTO(GetEncoded((ArrayOf<Byte>**)&encode), FAIL_PROCESS)
    CByteArrayInputStream::New(encode, (IByteArrayInputStream**)&bais);
    FAIL_GOTO(factory->GenerateCertificate((IInputStream*)bais.Get(), (ICertificate**)&cert), FAIL_PROCESS)
    x509Cert = cert.Get();
    FAIL_GOTO(x509Cert->GetIssuerX500Principal(issuer), FAIL_PROCESS)
    return NOERROR;
FAIL_PROCESS:
    return E_RUNTIME_EXCEPTION;
}

ECode X509Certificate::GetSubjectX500Principal(
    /* [out] */ IX500Principal** principal)
{
    VALIDATE_NOT_NULL(principal)
    AutoPtr<ICertificateFactoryHelper> helper;
    AutoPtr<ICertificateFactory> factory;
    CCertificateFactoryHelper::AcquireSingleton((ICertificateFactoryHelper**)&helper);
    helper->GetInstance(String("X.509"), (ICertificateFactory**)&factory);
    AutoPtr<ArrayOf<Byte> > encode;
    AutoPtr<IByteArrayInputStream> bais;
    AutoPtr<ICertificate> cert;
    AutoPtr<IX509Certificate> x509Cert;
    FAIL_GOTO(GetEncoded((ArrayOf<Byte>**)&encode), FAIL_PROCESS)
    CByteArrayInputStream::New(encode, (IByteArrayInputStream**)&bais);
    FAIL_GOTO(factory->GenerateCertificate((IInputStream*)bais.Get(), (ICertificate**)&cert), FAIL_PROCESS)
    x509Cert = cert.Get();
    FAIL_GOTO(x509Cert->GetSubjectX500Principal(principal), FAIL_PROCESS)
    return NOERROR;
FAIL_PROCESS:
    return E_RUNTIME_EXCEPTION;
}

ECode X509Certificate::GetExtendedKeyUsage(
    /* [out] */ IList** usage)
{
    return NOERROR;
}

ECode X509Certificate::GetSubjectAlternativeNames(
    /* [out] */ ICollection** names)
{
    return NOERROR;
}

ECode X509Certificate::GetIssuerAlternativeNames(
    /* [out] */ ICollection** names)
{
    return NOERROR;
}

} // end Cert
} // end Security
} // end Elastos
