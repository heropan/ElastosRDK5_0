
#include "webkit/CertTool.h"

using Elastos::Utility::Etl::HashMap;

namespace Elastos {
namespace Droid {
namespace Webkit {

static Boolean SCertificateTypeMap_Init()
{
    // TODO
//    sCertificateTypeMap.put("application/x-x509-ca-cert", KeyChain.EXTRA_CERTIFICATE);
//    sCertificateTypeMap.put("application/x-x509-user-cert", KeyChain.EXTRA_CERTIFICATE);
//    sCertificateTypeMap.put("application/x-pkcs12", KeyChain.EXTRA_PKCS12);
    return TRUE;
}

//static AutoPtr<IAlgorithmIdentifier> MD5_WITH_RSA_Create()
//{
//    AutoPtr<IAlgorithmIdentifier> algorithmIdentifier;
//    CAlgorithmIdentifier(PKCSObjectIdentifiers.md5WithRSAEncryption, (IAlgorithmIdentifier**)&algorithmIdentifier);
//    return algorithmIdentifier;
//}

const String CertTool::LOGTAG("CertTool");
//const AutoPtr<IAlgorithmIdentifier> CertTool::MD5_WITH_RSA;
HashMap<String, String> CertTool::sCertificateTypeMap;
static Boolean sCertificateTypeMapInit = SCertificateTypeMap_Init();

CertTool::CertTool()
{
}

AutoPtr< ArrayOf<String> > CertTool::GetKeyStrengthList()
{
    AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(2);
    (*array)[0] = "High Grade";
    (*array)[1] = "Medium Grade";
    return array;
}

String CertTool::GetSignedPublicKey(
    /* [in] */ IContext* context,
    /* [in] */ Int32 index,
    /* [in] */ const String& challenge)
{
    assert(0);
    // TODO
//    try {
//        KeyPairGenerator generator = KeyPairGenerator.getInstance("RSA");
//        generator.initialize((index == 0) ? 2048 : 1024);
//        KeyPair pair = generator.genKeyPair();
//
//        NetscapeCertRequest request = new NetscapeCertRequest(challenge,
//                MD5_WITH_RSA, pair.getPublic());
//        request.sign(pair.getPrivate());
//        byte[] signed = request.toASN1Primitive().getEncoded(ASN1Encoding.DER);
//
//        Credentials.getInstance().install(context, pair);
//        return String(Base64.encode(signed));
//    } catch (Exception e) {
//        Log.w(LOGTAG, e);
//    }

    return String(NULL);
}

void CertTool::AddCertificate(
    /* [in] */ IContext* context,
    /* [in] */ const String& type,
    /* [in] */ ArrayOf<Byte>* value)
{
    assert(0);
    // TODO
//    Credentials.getInstance().install(context, type, value);
}

String CertTool::GetCertType(
    /* [in] */ const String& mimeType)
{
    return sCertificateTypeMap[mimeType];
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
