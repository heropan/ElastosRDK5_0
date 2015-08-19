
#include "CJarUtils.h"

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
    // BerInputStream bis = new BerInputStream(signatureBlock);
//    ContentInfo info = (ContentInfo)ContentInfo.ASN1.decode(bis);
//    SignedData signedData = info.getSignedData();
//    if (signedData == null) {
//        throw new IOException("No SignedData found");
//    }
//    Collection<org.apache.harmony.security.x509.Certificate> encCerts
//            = signedData.getCertificates();
//    if (encCerts.isEmpty()) {
//        return null;
//    }
//    X509Certificate[] certs = new X509Certificate[encCerts.size()];
//    CertificateFactory cf = CertificateFactory.getInstance("X.509");
//    int i = 0;
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
