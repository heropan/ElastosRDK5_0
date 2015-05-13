
#ifndef __CPOLICYQUALIFIERINFOHELPER_H__
#define __CPOLICYQUALIFIERINFOHELPER_H__

#include "_CPolicyQualifierInfoHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CPolicyQualifierInfoHelper)
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

#endif // __CPOLICYQUALIFIERINFOHELPER_H__
