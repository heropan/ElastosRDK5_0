
#ifndef __CWMSWALLPAPERCONNECTION_H__
#define __CWMSWALLPAPERCONNECTION_H__

#include "_CWMSWallpaperConnection.h"
#include "CWallpaperManagerService.h"

namespace Elastos {
namespace Droid {
namespace Server {

CarClass(CWMSWallpaperConnection)
{
    friend class CWallpaperManagerService;
public:
    CARAPI constructor(
        /* [in] */ IWallpaperInfo* info,
        /* [in] */ Handle32 wallpaper,
        /* [in] */ Handle32 owner);

    CARAPI OnServiceConnected(
        /* [in] */ IComponentName* name,
        /* [in] */ IBinder* service);

    CARAPI OnServiceDisconnected(
        /* [in] */ IComponentName* name);

    CARAPI AttachEngine(
        /* [in] */ IWallpaperEngine* engine);

    CARAPI EngineShown(
        /* [in] */ IWallpaperEngine* engine);

    CARAPI SetWallpaper(
        /* [in] */ const String& name,
        /* [out] */ IParcelFileDescriptor** result);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    AutoPtr<IWallpaperInfo> mInfo;
    AutoPtr<IBinder> mToken;
    AutoPtr<IIWallpaperService> mService;
    AutoPtr<IWallpaperEngine> mEngine;
    AutoPtr<CWallpaperManagerService::WallpaperData> mWallpaper;
    AutoPtr<IRemoteCallback> mReply;
    CWallpaperManagerService* mOwner;
};

} // Server
} // Droid
} // Elastos

#endif //__CWMSWALLPAPERCONNECTION_H__