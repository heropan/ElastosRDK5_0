
#ifndef __CMESSAGEIMPRINT_H__
#define __CMESSAGEIMPRINT_H__

#include "_CMessageImprint.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {
namespace Tsp {

CarClass(CMessageImprint)
{
public:
    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence ** ppAsn1);

    CARAPI SetASN1(
        /* [in] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence * pAsn1);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Security::X509::IAlgorithmIdentifier * pAlgId,
        /* [in] */ ArrayOf<Byte> * pHashedMessage);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CMESSAGEIMPRINT_H__
