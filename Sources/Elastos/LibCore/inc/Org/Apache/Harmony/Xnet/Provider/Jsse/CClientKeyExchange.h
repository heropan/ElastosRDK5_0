
#ifndef __CCLIENTKEYEXCHANGE_H__
#define __CCLIENTKEYEXCHANGE_H__

#include "_CClientKeyExchange.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CClientKeyExchange)
{
public:
    CARAPI GetLength(
        /* [out] */ Int32 * pLength);

    CARAPI IsEmpty(
        /* [out] */ Boolean * pIsEmpty);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte> * pEncrypted_pre_master_secret,
        /* [in] */ Boolean isTLS);

    CARAPI constructor(
        /* [in] */ Elastos::Math::IBigInteger * pDh_Yc);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::IHandshakeIODataStream * pIds,
        /* [in] */ Int32 length,
        /* [in] */ Boolean isTLS,
        /* [in] */ Boolean isRSA);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CCLIENTKEYEXCHANGE_H__
