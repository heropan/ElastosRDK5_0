
#include "CSslCertificateHelper.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elautoptr.h"
#include "CSslCertificate.h"

using Elastos::IO::IByteArrayInputStream;
using Elastos::Security::Cert::ICertificate;
using Elastos::Security::Cert::ICertificateFactory;
using Elastos::Security::Cert::ICertificateFactoryHelper;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

ECode CSslCertificateHelper::SaveState(
    /* [in] */ ISslCertificate* certificate,
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle);

    if (certificate == NULL) {
        *bundle = NULL;
        return NOERROR;
    }

    AutoPtr<IBundle> localbundle;
    // TODO:
    // CBundle::New((IBundle**)&localbundle);
    AutoPtr<ISslCertificateDName> dNameTo;
    certificate->GetIssuedTo((ISslCertificateDName**)&dNameTo);
    String strTo;
    dNameTo->GetDName(&strTo);
    localbundle->PutString(CSslCertificate::ISSUED_TO, strTo);
    AutoPtr<ISslCertificateDName> dNameBy;
    certificate->GetIssuedTo((ISslCertificateDName**)&dNameBy);
    String strBy;
    dNameBy->GetDName(&strBy);
    localbundle->PutString(CSslCertificate::ISSUED_BY, strBy);
    String strNotBefore;
    certificate->GetValidNotBefore(&strNotBefore);
    localbundle->PutString(CSslCertificate::VALID_NOT_BEFORE, strNotBefore);
    String strNotAfter;
    certificate->GetValidNotAfter(&strNotAfter);
    localbundle->PutString(CSslCertificate::VALID_NOT_AFTER, strNotAfter);
    AutoPtr<IX509Certificate> x509Certificate = ((CSslCertificate*)certificate)->mX509Certificate;
    if (x509Certificate != NULL) {
        AutoPtr<ArrayOf<Byte> > bytesEncoded;
        // TODO:
        // x509Certificate->GetEncoded(&bytesEncoded);
        // localbundle->PutByteArray(CSslCertificate::X509_CERTIFICATE, bytesEncoded);

    }
    *bundle = localbundle;
    REFCOUNT_ADD(*bundle);
    return NOERROR;
}

ECode CSslCertificateHelper::RestoreState(
    /* [in] */ IBundle* bundle,
    /* [out] */ ISslCertificate** certificate)
{
    VALIDATE_NOT_NULL(certificate);

    if (bundle == NULL) {
        *certificate = NULL;
        return NOERROR;
    }

    AutoPtr<IX509Certificate> x509Certificate;
    AutoPtr<ArrayOf<Byte> > bytes;
    // TODO:
    // bundle->GetByteArray(CSslCertificate::X509_CERTIFICATE, &bytes);
    if (bytes != NULL) {
        AutoPtr<ICertificateFactoryHelper> helper;
        // CCertificateFactoryHelper::AcquireSingleton((ICertificateFactoryHelper**)&helper);
        AutoPtr<ICertificateFactory> certFactory;
        // helper->GetInstance(String("X.509"), (ICertificateFactory**)&certFactory);
        AutoPtr<ICertificate> cert;
        AutoPtr<IByteArrayInputStream> stream;
        // CByteArrayInputStream::New(bytes, (IByteArrayInputStream**)&stream);
        // if (!FAILED(certFactory->GenerateCertificate(stream, (ICertificate**)&cert))) {
        //     x509Certificate = (IX509Certificate*)cert->Probe(EIID_IX509Certificate);
        // }
    }

    String issuedTo;
    bundle->GetString(CSslCertificate::ISSUED_TO, &issuedTo);
    String issuedBy;
    bundle->GetString(CSslCertificate::ISSUED_BY, &issuedBy);
    String strNotBefore;
    bundle->GetString(CSslCertificate::VALID_NOT_BEFORE, &issuedBy);
    AutoPtr<IDate> dateNOtBefore = CSslCertificate::ParseDate(strNotBefore);
    String strNotAfter;
    bundle->GetString(CSslCertificate::VALID_NOT_AFTER, &strNotAfter);
    AutoPtr<IDate> dateNOtAfter = CSslCertificate::ParseDate(strNotBefore);

    CSslCertificate* cert = new CSslCertificate(issuedTo, issuedBy, dateNOtBefore, dateNOtAfter, x509Certificate);
    *certificate = (ISslCertificate*)cert->Probe(EIID_ISslCertificate);
    REFCOUNT_ADD(*certificate);
    return NOERROR;
}

}
}
}
}

