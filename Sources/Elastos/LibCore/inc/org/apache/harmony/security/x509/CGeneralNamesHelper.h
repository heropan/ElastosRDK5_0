
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CGENERALNAMESHELPER_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CGENERALNAMESHELPER_H__

#include "_CGeneralNamesHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CGeneralNamesHelper)
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

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CGENERALNAMESHELPER_H__
