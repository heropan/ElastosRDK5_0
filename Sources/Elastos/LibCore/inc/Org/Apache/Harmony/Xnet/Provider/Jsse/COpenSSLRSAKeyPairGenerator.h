
#ifndef __COPENSSLRSAKEYPAIRGENERATOR_H__
#define __COPENSSLRSAKEYPAIRGENERATOR_H__

#include "_COpenSSLRSAKeyPairGenerator.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(COpenSSLRSAKeyPairGenerator)
{
public:
    CARAPI GenerateKeyPair(
        /* [out] */ Elastos::Security::IKeyPair ** ppKeyPair);

    CARAPI Initialize(
        /* [in] */ Int32 keysize,
        /* [in] */ Elastos::Security::ISecureRandom * pRandom);

    CARAPI InitializeEx(
        /* [in] */ Elastos::Security::Spec::IAlgorithmParameterSpec * pParams,
        /* [in] */ Elastos::Security::ISecureRandom * pRandom);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __COPENSSLRSAKEYPAIRGENERATOR_H__
