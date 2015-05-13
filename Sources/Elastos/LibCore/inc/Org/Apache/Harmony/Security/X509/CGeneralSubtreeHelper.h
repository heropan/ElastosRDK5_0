
#ifndef __CGENERALSUBTREEHELPER_H__
#define __CGENERALSUBTREEHELPER_H__

#include "_CGeneralSubtreeHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CGeneralSubtreeHelper)
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

#endif // __CGENERALSUBTREEHELPER_H__
