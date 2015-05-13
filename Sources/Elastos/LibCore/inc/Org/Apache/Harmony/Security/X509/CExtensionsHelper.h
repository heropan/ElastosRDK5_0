
#ifndef __CEXTENSIONSHELPER_H__
#define __CEXTENSIONSHELPER_H__

#include "_CExtensionsHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CExtensionsHelper)
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

#endif // __CEXTENSIONSHELPER_H__
