
#ifndef __COPENSSLENGINE_H__
#define __COPENSSLENGINE_H__

#include "_COpenSSLEngine.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(COpenSSLEngine)
{
public:
    CARAPI GetPrivateKeyById(
        /* [in] */ const String& id,
        /* [out] */ Elastos::Security::IPrivateKey ** ppKey);

    CARAPI Equals(
        /* [in] */ IInterface * pObj,
        /* [out] */ Boolean * pIsEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32 * pHashCode);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __COPENSSLENGINE_H__
