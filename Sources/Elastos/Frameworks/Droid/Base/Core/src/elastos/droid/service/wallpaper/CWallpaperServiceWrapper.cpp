
#include "elastos/droid/service/wallpaper/CWallpaperServiceWrapper.h"
#include "elastos/droid/service/wallpaper/WallpaperService.h"
#include "elastos/droid/service/wallpaper/CWallpaperEngineWrapper.h"

namespace Elastos {
namespace Droid {
namespace Service {
namespace Wallpaper {

ECode CWallpaperServiceWrapper::constructor(
    /* [in] */ Handle32 context)
{
    mTarget = context;
    return NOERROR;
}

ECode CWallpaperServiceWrapper::Attach(
    /* [in] */ IWallpaperConnection* conn,
    /* [in] */ IBinder* windowToken,
    /* [in] */ Int32 windowType,
    /* [in] */ Boolean isPreview,
    /* [in] */ Int32 reqWidth,
    /* [in] */ Int32 reqHeight)
{
    mEngine = NULL;
    CWallpaperEngineWrapper::New(
        mTarget, conn, windowToken, windowType,
        isPreview, reqWidth, reqHeight, (IWallpaperEngineWrapper**)&mEngine);

    return NOERROR;
}

ECode CWallpaperServiceWrapper::ToString(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

} // namespace Wallpaper
} // namespace Service
} // namepsace Droid
} // namespace Elastos
