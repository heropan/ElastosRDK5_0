
#ifndef __CSSLPARAMETERSIMPLHELPER_H__
#define __CSSLPARAMETERSIMPLHELPER_H__

#include "_CSSLParametersImplHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CSSLParametersImplHelper)
{
public:
    CARAPI GetDefaultTrustManager(
        /* [out] */ Elastosx::Net::Ssl::IX509TrustManager ** ppManager);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CSSLPARAMETERSIMPLHELPER_H__
