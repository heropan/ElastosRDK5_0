
#ifndef __CPRIVATEKEYUSAGEPERIOD_H__
#define __CPRIVATEKEYUSAGEPERIOD_H__

#include "_CPrivateKeyUsagePeriod.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CPrivateKeyUsagePeriod)
{
public:
    CARAPI GetNotBefore(
        /* [out] */ Elastos::Utility::IDate ** ppNotBefore);

    CARAPI GetNotAfter(
        /* [out] */ Elastos::Utility::IDate ** ppNotAfter);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

    CARAPI constructor(
        /* [in] */ Elastos::Utility::IDate * pNotBeforeDate,
        /* [in] */ Elastos::Utility::IDate * pNotAfterDate);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CPRIVATEKEYUSAGEPERIOD_H__
