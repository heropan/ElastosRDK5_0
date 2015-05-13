
#ifndef __CSERVERKEYEXCHANGEHELPER_H__
#define __CSERVERKEYEXCHANGEHELPER_H__

#include "_CServerKeyExchangeHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CServerKeyExchangeHelper)
{
public:
    CARAPI ToUnsignedByteArray(
        /* [in] */ Elastos::Math::IBigInteger * pBi,
        /* [out, callee] */ ArrayOf<Byte> ** ppByteArray);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CSERVERKEYEXCHANGEHELPER_H__
