
#ifndef __CATTRIBUTETYPEANDVALUEHELPER_H__
#define __CATTRIBUTETYPEANDVALUEHELPER_H__

#include "_CAttributeTypeAndValueHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X501 {

CarClass(CAttributeTypeAndValueHelper)
{
public:
    CARAPI GetAttributeValue(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Type ** ppAttributeValue);

    CARAPI SetAttributeValue(
        /* [in] */ Org::Apache::Harmony::Security::Asn1::IASN1Type * pAttributeValue);

    CARAPI GetASN1Sequence(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence ** ppAsn1);

    CARAPI SetASN1Sequence(
        /* [in] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence * pAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CATTRIBUTETYPEANDVALUEHELPER_H__
