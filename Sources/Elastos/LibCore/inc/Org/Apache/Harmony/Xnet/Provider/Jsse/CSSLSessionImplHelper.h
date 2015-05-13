
#ifndef __CSSLSESSIONIMPLHELPER_H__
#define __CSSLSESSIONIMPLHELPER_H__

#include "_CSSLSessionImplHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CSSLSessionImplHelper)
{
public:
    CARAPI GetNULL_SESSION(
        /* [out] */ Elastosx::Net::Ssl::ISSLSession ** ppNullSession);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CSSLSESSIONIMPLHELPER_H__
