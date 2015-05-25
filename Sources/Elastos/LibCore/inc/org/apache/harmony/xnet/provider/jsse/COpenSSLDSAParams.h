
#ifndef __COPENSSLDSAPARAMS_H__
#define __COPENSSLDSAPARAMS_H__

#include "_COpenSSLDSAParams.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(COpenSSLDSAParams)
{
public:
    CARAPI GetG(
        /* [out] */ Elastos::Math::IBigInteger ** ppBase);

    CARAPI GetP(
        /* [out] */ Elastos::Math::IBigInteger ** ppPrime);

    CARAPI GetQ(
        /* [out] */ Elastos::Math::IBigInteger ** ppSubprime);

    CARAPI GetY(
        /* [out] */ Elastos::Math::IBigInteger ** ppY);

    CARAPI GetX(
        /* [out] */ Elastos::Math::IBigInteger ** ppX);

    CARAPI Equals(
        /* [in] */ IInterface * pObj,
        /* [out] */ Boolean * pIsEqual);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __COPENSSLDSAPARAMS_H__
