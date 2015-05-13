
#ifndef __CPINFAILURELOGGERHELPER_H__
#define __CPINFAILURELOGGERHELPER_H__

#include "_CPinFailureLoggerHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CPinFailureLoggerHelper)
{
public:
    CARAPI Log(
        /* [in] */ const String& cn,
        /* [in] */ Boolean chainContainsUserCert,
        /* [in] */ Boolean pinIsEnforcing,
        /* [in] */ Elastos::Utility::IList * pChain);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CPINFAILURELOGGERHELPER_H__
