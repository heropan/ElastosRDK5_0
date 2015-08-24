
#include "CJarUtils.h"
#include "io/CByteArrayInputStream.h"
#include "security/cert/CertificateFactory.h"
#include "utility/logging/Logger.h"
#include "asn1/CBerInputStream.h"
#include "pkcs7/CContentInfoHelper.h"

using Elastos::IO::CByteArrayInputStream;
using Elastos::Security::Cert::ICertificateFactory;
using Elastos::Security::Cert::CertificateFactory;
using Elastos::Utility::ICollection;
using Elastos::Utility::IList;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Harmony::Security::Asn1::IASN1Sequence;
using Org::Apache::Harmony::Security::Asn1::IASN1Type;
using Org::Apache::Harmony::Security::Asn1::IBerInputStream;
using Org::Apache::Harmony::Security::Asn1::CBerInputStream;
using Org::Apache::Harmony::Security::Pkcs7::IContentInfo;
using Org::Apache::Harmony::Security::Pkcs7::IContentInfoHelper;
using Org::Apache::Harmony::Security::Pkcs7::CContentInfoHelper;
using Org::Apache::Harmony::Security::Pkcs7::ISignedData;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Utils {

const AutoPtr< ArrayOf<Int32> > CJarUtils::MESSAGE_DIGEST_OID = CJarUtils::Init_MESSAGE_DIGEST_OID();

CAR_INTERFACE_IMPL(CJarUtils, Singleton, IJarUtils)

CAR_SINGLETON_IMPL(CJarUtils)

ECode CJarUtils::VerifySignature(
    /* [in] */ IInputStream* signature,
    /* [in] */ IInputStream* signatureBlock,
    /* [out, callee] */ ArrayOf<ICertificate*>** sign)
{
    VALIDATE_NOT_NULL(sign);
    *sign = NULL;
    AutoPtr<IBerInputStream> bis;
    FAIL_RETURN(CBerInputStream::New(signatureBlock, (IBerInputStream**)&bis));
    AutoPtr<IContentInfoHelper> helper;
    CContentInfoHelper::AcquireSingleton((IContentInfoHelper**)&helper);
    AutoPtr<IASN1Sequence> asn1;
    helper->GetASN1((IASN1Sequence**)&asn1);
    AutoPtr<IInterface> obj;
    IASN1Type::Probe(asn1)->Decode(bis, (IInterface**)&obj);
    IContentInfo* info = IContentInfo::Probe(obj);
    AutoPtr<ISignedData> signedData;
    info->GetSignedData((ISignedData**)&signedData);
    if (signedData == NULL) {
        Logger::E("CJarUtils", "No SignedData found");
        return E_IO_EXCEPTION;
    }
    AutoPtr<IList> certList;
    signedData->GetCertificates((IList**)&certList);
    ICollection* encCerts = ICollection::Probe(certList);
    Boolean isEmpty;
    if (encCerts->IsEmpty(&isEmpty), isEmpty) {
        return NOERROR;
    }
    Int32 size;
    encCerts->GetSize(&size);
    AutoPtr< ArrayOf<IX509Certificate*> > certs = ArrayOf<IX509Certificate*>::Alloc(size);
    AutoPtr<ICertificateFactory> cf;
    CertificateFactory::GetInstance(String("X.509"), (ICertificateFactory**)&cf);
    Int32 i = 0;
    AutoPtr<IIterator> it;
    encCerts->GetIterator((IIterator**)&it);
    Boolean hasNext;
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> certObj;
        it->GetNext((IInterface**)&certObj);
        Org::Apache::Harmony::Security::X509::ICertificate* encCert =
                Org::Apache::Harmony::Security::X509::ICertificate::Probe(certObj);
        AutoPtr< ArrayOf<Byte> > encoded;
        encCert->GetEncoded((ArrayOf<Byte>**)&encoded);
        AutoPtr<IInputStream> is;
        CByteArrayInputStream::New(encoded, (IInputStream**)&is);
    }

//    for (org.apache.harmony.security.x509.Certificate encCert : encCerts) {
//        final byte[] encoded = encCert.getEncoded();
//        final InputStream is = new ByteArrayInputStream(encoded);
//        certs[i++] = new VerbatimX509Certificate((X509Certificate) cf.generateCertificate(is),
//                encoded);
//    }
    return E_NOT_IMPLEMENTED;
}

AutoPtr< ArrayOf<IX509Certificate*> > CJarUtils::CreateChain(
    /* [in] */ IX509Certificate* signer,
    /* [in] */ const ArrayOf<IX509Certificate*>& candidates)
{
    return NULL;
}

AutoPtr<IX509Certificate> CJarUtils::FindCert(
    /* [in] */ IPrincipal* issuer,
    /* [in] */ const ArrayOf<IX509Certificate*>& candidates)
{
    return NULL;
}

AutoPtr< ArrayOf<Int32> > CJarUtils::Init_MESSAGE_DIGEST_OID()
{
    AutoPtr< ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(7);
    (*array)[0] = 1;
    (*array)[1] = 2;
    (*array)[2] = 840;
    (*array)[3] = 113549;
    (*array)[4] = 1;
    (*array)[5] = 9;
    (*array)[6] = 4;
    return array;
}

}
}
}
}
}
