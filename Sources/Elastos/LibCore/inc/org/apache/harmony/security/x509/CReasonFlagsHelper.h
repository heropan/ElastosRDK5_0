
#ifndef __CREASONFLAGSHELPER_H__
#define __CREASONFLAGSHELPER_H__

#include "_CReasonFlagsHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CReasonFlagsHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1BitString ** ppAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CREASONFLAGSHELPER_H__
