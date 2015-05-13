
#ifndef __COPENSSLDSAPUBLICKEY_H__
#define __COPENSSLDSAPUBLICKEY_H__

#include "_COpenSSLDSAPublicKey.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(COpenSSLDSAPublicKey)
{
public:
    CARAPI GetParams(
        /* [out] */ Elastos::Security::Interfaces::IDSAParams ** ppParameters);

    CARAPI GetY(
        /* [out] */ Elastos::Math::IBigInteger ** ppY);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __COPENSSLDSAPUBLICKEY_H__
