
#ifndef __CSERVERKEYEXCHANGE_H__
#define __CSERVERKEYEXCHANGE_H__

#include "_CServerKeyExchange.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CServerKeyExchange)
{
public:
    CARAPI GetLength(
        /* [out] */ Int32 * pLength);

    CARAPI GetRSAPublicKey(
        /* [out] */ Elastos::Security::Interfaces::IRSAPublicKey ** ppKey);

    CARAPI constructor(
        /* [in] */ Elastos::Math::IBigInteger * pPar1,
        /* [in] */ Elastos::Math::IBigInteger * pPar2,
        /* [in] */ Elastos::Math::IBigInteger * pPar3,
        /* [in] */ ArrayOf<Byte> * pHash);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::IHandshakeIODataStream * pIds,
        /* [in] */ Int32 length,
        /* [in] */ Int32 keyExchange);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CSERVERKEYEXCHANGE_H__
