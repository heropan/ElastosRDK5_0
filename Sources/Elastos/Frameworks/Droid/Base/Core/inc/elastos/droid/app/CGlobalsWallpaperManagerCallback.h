#ifndef __ELASTOS_DROID_APP_CGLOBALSWALLPAPERMANAGERCALLBACK_H__
#define __ELASTOS_DROID_APP_CGLOBALSWALLPAPERMANAGERCALLBACK_H__

#include "_Elastos_Droid_App_CGlobalsWallpaperManagerCallback.h"
#include "elastos/droid/os/HandlerBase.h"

using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Content::IContext;

namespace Elastos{
namespace Droid{
namespace App{

class CWallpaperManager;

CarClass(CGlobalsWallpaperManagerCallback)
{
private:
    class MyHandler : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ CGlobalsWallpaperManagerCallback* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        CGlobalsWallpaperManagerCallback* mHost;
    };

public:
    CARAPI OnWallpaperChanged();

    CARAPI_(AutoPtr<IBitmap>) PeekWallpaperBitmap(
        /* [in] */ IContext* context,
        /* [in] */ Boolean returnDefault);

    CARAPI_(void) ForgetLoadedWallpaper();

    CARAPI constructor(
        /* [in] */ ILooper* looper);

private:
    CARAPI_(AutoPtr<IBitmap>) GetCurrentWallpaperLocked(
        /* [in] */ IContext* context);

    CARAPI_(AutoPtr<IBitmap>) GetDefaultWallpaperLocked(
        /* [in] */ IContext* context);

    CARAPI_(void) HandleClearWallpaper();

private:
    static const Int32 MSG_CLEAR_WALLPAPER;

    AutoPtr<IIWallpaperManager> mService;
    AutoPtr<IBitmap> mWallpaper;
    AutoPtr<IBitmap> mDefaultWallpaper;
    AutoPtr<IHandler> mHandler;
    Object mLock;

    friend class CWallpaperManager;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_CGLOBALSWALLPAPERMANAGERCALLBACK_H__
