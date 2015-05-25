
#ifndef __CASN1ANYHELPER_H__
#define __CASN1ANYHELPER_H__

#include "_CASN1AnyHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CASN1AnyHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IASN1Any** instance);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CASN1ANYHELPER_H__
