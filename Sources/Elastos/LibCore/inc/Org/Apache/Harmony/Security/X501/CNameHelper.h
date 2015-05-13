
#ifndef __CNAMEHELPER_H__
#define __CNAMEHELPER_H__

#include "_CNameHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X501 {

CarClass(CNameHelper)
{
public:
    CARAPI GetASN1_RDN(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1SetOf ** ppAsn1_rdn);

    CARAPI SetASN1_RDN(
        /* [in] */ Org::Apache::Harmony::Security::Asn1::IASN1SetOf * pAsn1_rdn);

    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1SequenceOf ** ppAsn1);

    CARAPI SetASN1(
        /* [in] */ Org::Apache::Harmony::Security::Asn1::IASN1SequenceOf * pAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CNAMEHELPER_H__
