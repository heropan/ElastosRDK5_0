
#ifndef __CCERTIFICATE_H__
#define __CCERTIFICATE_H__

#include "_CCertificate.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CCertificate)
{
public:
    CARAPI GetTbsCertificate(
        /* [out] */ Org::Apache::Harmony::Security::X509::ITBSCertificate ** ppTbsCertificate);

    CARAPI GetSignatureValue(
        /* [out, callee] */ ArrayOf<Byte> ** ppSignatureValue);

    CARAPI ToString(
        /* [out] */ String * pStr);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncode);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Security::X509::ITBSCertificate * pTbsCertificate,
        /* [in] */ Org::Apache::Harmony::Security::X509::IAlgorithmIdentifier * pSignatureAlgorithm,
        /* [in] */ ArrayOf<Byte> * pSignatureValue);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CCERTIFICATE_H__
