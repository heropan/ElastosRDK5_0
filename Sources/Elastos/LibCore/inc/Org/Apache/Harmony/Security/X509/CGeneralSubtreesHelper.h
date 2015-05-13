
#ifndef __CGENERALSUBTREESHELPER_H__
#define __CGENERALSUBTREESHELPER_H__

#include "_CGeneralSubtreesHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CGeneralSubtreesHelper)
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

#endif // __CGENERALSUBTREESHELPER_H__
