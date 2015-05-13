
#ifndef __COPENSSLDSAPRIVATEKEY_H__
#define __COPENSSLDSAPRIVATEKEY_H__

#include "_COpenSSLDSAPrivateKey.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(COpenSSLDSAPrivateKey)
{
public:
    CARAPI GetX(
        /* [out] */ Elastos::Math::IBigInteger ** ppX);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __COPENSSLDSAPRIVATEKEY_H__
