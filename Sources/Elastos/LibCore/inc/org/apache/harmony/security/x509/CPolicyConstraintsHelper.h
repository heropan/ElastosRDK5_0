
#ifndef __CPOLICYCONSTRAINTSHELPER_H__
#define __CPOLICYCONSTRAINTSHELPER_H__

#include "_CPolicyConstraintsHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CPolicyConstraintsHelper)
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

#endif // __CPOLICYCONSTRAINTSHELPER_H__
