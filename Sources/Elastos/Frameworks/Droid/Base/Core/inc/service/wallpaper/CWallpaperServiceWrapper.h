
#ifndef __ELASTOS_DROID_SERVICE_WALLPAPER_CWALLPAPERSERVICEWRAPPER_H__
#define __ELASTOS_DROID_SERVICE_WALLPAPER_CWALLPAPERSERVICEWRAPPER_H__

#include "_CWallpaperServiceWrapper.h"

using Elastos::Droid::Os::IBinder;

namespace Elastos {
namespace Droid {
namespace Service {
namespace Wallpaper {

/**
 * Implements the internal {@link IWallpaperService} interface to convert
 * incoming calls to it back to calls on an {@link WallpaperService}.
 */
CarClass(CWallpaperServiceWrapper)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 context);

    CARAPI Attach(
        /* [in] */ IWallpaperConnection* conn,
        /* [in] */ IBinder* windowToken,
        /* [in] */ Int32 windowType,
        /* [in] */ Boolean isPreview,
        /* [in] */ Int32 reqWidth,
        /* [in] */ Int32 reqHeight);

    CARAPI ToString(
        /* [out] */ String* description);

private:
    Handle32 mTarget;
    AutoPtr<IWallpaperEngineWrapper> mEngine;
};

} // namespace Wallpaper
} // namespace Service
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVICE_WALLPAPER_CWALLPAPERSERVICEWRAPPER_H__
