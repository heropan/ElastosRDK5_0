
#ifndef __CPKISTATUSINFOHELPER_H__
#define __CPKISTATUSINFOHELPER_H__

#include "_CPKIStatusInfoHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {
namespace Tsp {

CarClass(CPKIStatusInfoHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence ** ppAsn1);

    CARAPI SetASN1(
        /* [in] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence * pAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CPKISTATUSINFOHELPER_H__
