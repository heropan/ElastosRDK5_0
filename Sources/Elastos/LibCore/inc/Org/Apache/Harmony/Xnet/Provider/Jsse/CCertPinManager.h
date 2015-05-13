
#ifndef __CCERTPINMANAGER_H__
#define __CCERTPINMANAGER_H__

#include "_CCertPinManager.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CCertPinManager)
{
public:
    CARAPI ChainIsNotPinned(
        /* [in] */ const String& hostname,
        /* [in] */ Elastos::Utility::IList * pChain,
        /* [out] */ Boolean * pIsNotPinned);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::ITrustedCertificateStore * pStore);

    CARAPI constructor(
        /* [in] */ const String& path,
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::ITrustedCertificateStore * pStore);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CCERTPINMANAGER_H__
