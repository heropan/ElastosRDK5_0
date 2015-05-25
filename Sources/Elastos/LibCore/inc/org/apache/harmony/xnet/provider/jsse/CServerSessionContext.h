
#ifndef __CSERVERSESSIONCONTEXT_H__
#define __CSERVERSESSIONCONTEXT_H__

#include "_CServerSessionContext.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CServerSessionContext)
{
public:
    CARAPI GetIds(
        /* [out] */ Elastos::Utility::IEnumeration ** ppIds);

    CARAPI GetSession(
        /* [in] */ ArrayOf<Byte> * pSessionId,
        /* [out] */ Elastosx::Net::Ssl::ISSLSession ** ppSession);

    CARAPI GetSessionCacheSize(
        /* [out] */ Int32 * pSize);

    CARAPI GetSessionTimeout(
        /* [out] */ Int32 * pTimeout);

    CARAPI SetSessionCacheSize(
        /* [in] */ Int32 size);

    CARAPI SetSessionTimeout(
        /* [in] */ Int32 seconds);

    CARAPI SetPersistentCache(
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::ISSLServerSessionCache * pPersistentCache);

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

#endif // __CSERVERSESSIONCONTEXT_H__
