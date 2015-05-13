
#ifndef __CFILECLIENTSESSIONCACHEHELPER_H__
#define __CFILECLIENTSESSIONCACHEHELPER_H__

#include "_CFileClientSessionCacheHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CFileClientSessionCacheHelper)
{
public:
    CARAPI UsingDirectory(
        /* [in] */ Elastos::IO::IFile * pDirectory,
        /* [out] */ Org::Apache::Harmony::Xnet::Provider::Jsse::ISSLClientSessionCache ** ppCache);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CFILECLIENTSESSIONCACHEHELPER_H__
