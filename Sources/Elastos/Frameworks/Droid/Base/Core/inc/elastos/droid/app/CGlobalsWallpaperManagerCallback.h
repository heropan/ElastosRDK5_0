#ifndef __ELASTOS_DROID_APP_CGLOBALSWALLPAPERMANAGERCALLBACK_H__
#define __ELASTOS_DROID_APP_CGLOBALSWALLPAPERMANAGERCALLBACK_H__

#include "_Elastos_Droid_App_CGlobalsWallpaperManagerCallback.h"
#include "elastos/droid/os/Handler.h"

using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IBitmap;

namespace Elastos {
namespace Droid {
namespace App {

class CWallpaperManager;

CarClass(CGlobalsWallpaperManagerCallback)
    , public Object
    , public IIWallpaperManagerCallback
    , public IBinder
{
private:
    class MyHandler
        : public Handler
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
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ ILooper* looper);

    CARAPI OnWallpaperChanged();

    CARAPI_(AutoPtr<IBitmap>) PeekWallpaperBitmap(
        /* [in] */ IContext* context,
        /* [in] */ Boolean returnDefault);

    CARAPI_(void) ForgetLoadedWallpaper();

    CARAPI ToString(
        /* [out] */ String* str);
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

    friend class CWallpaperManager;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_CGLOBALSWALLPAPERMANAGERCALLBACK_H__
