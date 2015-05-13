
#ifndef __COPENSSLENGINEHELPER_H__
#define __COPENSSLENGINEHELPER_H__

#include "_COpenSSLEngineHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(COpenSSLEngineHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ const String& engine,
        /* [out] */ Org::Apache::Harmony::Xnet::Provider::Jsse::IOpenSSLEngine ** ppInstance);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __COPENSSLENGINEHELPER_H__
