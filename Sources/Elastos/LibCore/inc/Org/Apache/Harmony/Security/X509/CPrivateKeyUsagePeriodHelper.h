
#ifndef __CPRIVATEKEYUSAGEPERIODHELPER_H__
#define __CPRIVATEKEYUSAGEPERIODHELPER_H__

#include "_CPrivateKeyUsagePeriodHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CPrivateKeyUsagePeriodHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence ** ppAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CPRIVATEKEYUSAGEPERIODHELPER_H__
