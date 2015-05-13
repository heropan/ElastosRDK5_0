
#ifndef __COTHERNAMEHELPER_H__
#define __COTHERNAMEHELPER_H__

#include "_COtherNameHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(COtherNameHelper)
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

#endif // __COTHERNAMEHELPER_H__
