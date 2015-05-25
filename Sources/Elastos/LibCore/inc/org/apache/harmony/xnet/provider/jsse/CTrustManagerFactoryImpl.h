
#ifndef __CTRUSTMANAGERFACTORYIMPL_H__
#define __CTRUSTMANAGERFACTORYIMPL_H__

#include "_CTrustManagerFactoryImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CTrustManagerFactoryImpl)
{
public:
    CARAPI EngineInit(
        /* [in] */ Elastos::Security::IKeyStore * pKs);

    CARAPI EngineInitEx(
        /* [in] */ Elastosx::Net::Ssl::IManagerFactoryParameters * pSpec);

    CARAPI EngineGetTrustManagers(
        /* [out, callee] */ ArrayOf<Elastosx::Net::Ssl::ITrustManager *> ** ppManagers);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CTRUSTMANAGERFACTORYIMPL_H__
