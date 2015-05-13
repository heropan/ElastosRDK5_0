
#ifndef __CCERTIFICATELIST_H__
#define __CCERTIFICATELIST_H__

#include "_CCertificateList.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CCertificateList)
{
public:
    CARAPI GetTbsCertList(
        /* [out] */ Org::Apache::Harmony::Security::X509::ITBSCertList ** ppTbsCertList);

    CARAPI GetSignatureValue(
        /* [out, callee] */ ArrayOf<Byte> ** ppSignatureValue);

    CARAPI ToString(
        /* [out] */ String * pStr);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncode);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Security::X509::ITBSCertList * pTbsCertList,
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

#endif // __CCERTIFICATELIST_H__
