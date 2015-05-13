
#ifndef __CREASONCODEHELPER_H__
#define __CREASONCODEHELPER_H__

#include "_CReasonCodeHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CReasonCodeHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Type ** ppAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CREASONCODEHELPER_H__
