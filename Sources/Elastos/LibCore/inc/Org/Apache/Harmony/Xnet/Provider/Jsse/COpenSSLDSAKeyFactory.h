
#ifndef __COPENSSLDSAKEYFACTORY_H__
#define __COPENSSLDSAKEYFACTORY_H__

#include "_COpenSSLDSAKeyFactory.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(COpenSSLDSAKeyFactory)
{
public:
    CARAPI EngineGeneratePublic(
        /* [in] */ Elastos::Security::Spec::IKeySpec * pKeySpec,
        /* [out] */ Elastos::Security::IPublicKey ** ppPubKey);

    CARAPI EngineGeneratePrivate(
        /* [in] */ Elastos::Security::Spec::IKeySpec * pKeySpec,
        /* [out] */ Elastos::Security::IPrivateKey ** ppPriKey);

    CARAPI EngineGetKeySpec(
        /* [in] */ Elastos::Security::IKey * pKey,
        /* [in] */ const ClassID & keySpec,
        /* [out] */ Elastos::Security::Spec::IKeySpec ** ppRetkeySpec);

    CARAPI EngineTranslateKey(
        /* [in] */ Elastos::Security::IKey * pKey,
        /* [out] */ Elastos::Security::IKey ** ppTranslatedKey);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __COPENSSLDSAKEYFACTORY_H__
