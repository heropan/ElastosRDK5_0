#ifndef __ELASTOS_DROID_SERVICE_WALLPAPER_CWALLPAPERSETTINGSACTIVITY_H__
#define __ELASTOS_DROID_SERVICE_WALLPAPER_CWALLPAPERSETTINGSACTIVITY_H__

#include "_CWallpaperSettingsActivity.h"
#include <core/IPreferenceActivity.h>




namespace Elastos {
namespace Droid {
namespace Service {
namespace Wallpaper {

CarClass(CWallpaperSettingsActivity), public IPreferenceActivity
{
public:

    const static String EXTRA_PREVIEW_MODE;// = "android.service.wallpaper.PREVIEW_MODE";

protected:

    CARAPI OnCreate(
            /* [in] */ IBundle* icicle);
}

} // namespace Wallpaper
} // namespace Service
} // namepsace Droid
} // namespace Elastos

#endif // __CWALLPAPERSETTINGSACTIVITYH__
