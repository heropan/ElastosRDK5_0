
#ifndef __CCLIENTHELLO_H__
#define __CCLIENTHELLO_H__

#include "_CClientHello.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CClientHello)
{
public:
    CARAPI GetLength(
        /* [out] */ Int32 * pLength);

    CARAPI GetRandom(
        /* [out, callee] */ ArrayOf<Byte> ** ppRandom);

    CARAPI constructor(
        /* [in] */ Elastos::Security::ISecureRandom * pSr,
        /* [in] */ ArrayOf<Byte> * pVer,
        /* [in] */ ArrayOf<Byte> * pSes_id,
        /* [in] */ ArrayOf<Org::Apache::Harmony::Xnet::Provider::Jsse::ICipherSuite *> * pCipher_suite);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::IHandshakeIODataStream * pIds,
        /* [in] */ Int32 length);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::IHandshakeIODataStream * pIds);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CCLIENTHELLO_H__
