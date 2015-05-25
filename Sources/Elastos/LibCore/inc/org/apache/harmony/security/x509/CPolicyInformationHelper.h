
#ifndef __CPOLICYINFORMATIONHELPER_H__
#define __CPOLICYINFORMATIONHELPER_H__

#include "_CPolicyInformationHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CPolicyInformationHelper)
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

#endif // __CPOLICYINFORMATIONHELPER_H__
