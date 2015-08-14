
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CVALIDITYHELPER_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CVALIDITYHELPER_H__

#include "_CValidityHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CValidityHelper)
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

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CVALIDITYHELPER_H__
