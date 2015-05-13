
#ifndef __CWMSUSERSWITCHOBSERVER_H__
#define __CWMSUSERSWITCHOBSERVER_H__

#include "_CWMSUserSwitchObserver.h"

using Elastos::Droid::Os::IRemoteCallback;

namespace Elastos {
namespace Droid {
namespace Server {

class CWallpaperManagerService;

CarClass(CWMSUserSwitchObserver)
{
public:
    CARAPI constructor(
            /* [in] */ Handle32 owner);

    CARAPI OnUserSwitching(
        /* [in] */ Int32 newUserId,
        /* [in] */ IRemoteCallback* reply);

    CARAPI OnUserSwitchComplete(
        /* [in] */ Int32 newUserId);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    CWallpaperManagerService* mOwner;
};

} // Server
} // Droid
} // Elastos

#endif //__CWMSUSERSWITCHOBSERVER_H__