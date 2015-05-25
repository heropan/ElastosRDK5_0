
#ifndef __COPENSSLRSAPUBLICKEY_H__
#define __COPENSSLRSAPUBLICKEY_H__

#include "_COpenSSLRSAPublicKey.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(COpenSSLRSAPublicKey)
{
public:
    CARAPI GetPublicExponent(
        /* [out] */ Elastos::Math::IBigInteger ** ppPublicExponent);

    CARAPI Equals(
        /* [in] */ IInterface * pObj,
        /* [out] */ Boolean * pIsEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32 * pHashCode);

    CARAPI ToString(
        /* [out] */ String * pStr);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __COPENSSLRSAPUBLICKEY_H__
