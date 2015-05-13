
#ifndef __CPINLISTENTRY_H__
#define __CPINLISTENTRY_H__

#include "_CPinListEntry.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CPinListEntry)
{
public:
    CARAPI GetCommonName(
        /* [out] */ String * pCommonName);

    CARAPI GetEnforcing(
        /* [out] */ Boolean * pEnforcing);

    CARAPI ChainIsNotPinned(
        /* [in] */ Elastos::Utility::IList * pChain,
        /* [out] */ Boolean * pIsNotPinned);

    CARAPI constructor(
        /* [in] */ const String& entry,
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

#endif // __CPINLISTENTRY_H__
