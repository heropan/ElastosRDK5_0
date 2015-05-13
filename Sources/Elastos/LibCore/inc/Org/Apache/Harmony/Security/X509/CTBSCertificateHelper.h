
#ifndef __CTBSCERTIFICATEHELPER_H__
#define __CTBSCERTIFICATEHELPER_H__

#include "_CTBSCertificateHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CTBSCertificateHelper)
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

#endif // __CTBSCERTIFICATEHELPER_H__
