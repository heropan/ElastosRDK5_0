
#include "elastos/droid/os/Binder.h"
#include "elastos/droid/os/Process.h"
//#include "utils/Xml.h"
//#include "utils/XmlUtils.h"
//#include "elastos/droid/text/TextUtils.h"
#include <eltypes.h>
#include <elstring.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuffer.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <utils/String8.h>
#include <utils/ArrayUtils.h>
#include <cutils/config_utils.h>
#include <cutils/sockets.h>
#include <cutils/log.h>
#include <linux/netlink.h>
#include <elastos/core/StringBuilder.h>

#include "elastos/droid/service/wallpaper/CWallpaperSettingsActivity.h"
#include "elastos/droid/content/CResourcesTheme.h"


/*
import android.content.res.Resources;
import android.os.Bundle;
import android.preference.PreferenceActivity;
*/

namespace Elastos {
namespace Droid {
namespace Service {
namespace Wallpaper {

/**
 * Base class for activities that will be used to configure the settings of
 * a wallpaper.  You should derive from this class to allow it to select the
 * proper theme of the activity depending on how it is being used.
 * @hide
 */

/**
 * This boolean extra in the launch intent indicates that the settings
 * are being used while the wallpaper is in preview mode.
 */
const String CWallpaperSettingsActivity::EXTRA_PREVIEW_MODE = "android.service.wallpaper.PREVIEW_MODE";

//@Override
ECode CWallpaperSettingsActivity::OnCreate(
    /* [in] */ IBundle* icicle)
{
    if (FALSE)
    {
        AutoPtr<IResourcesTheme> theme = GetTheme();
        if (GetIntent().GetBooleanExtra(EXTRA_PREVIEW_MODE, FALSE))
        {
            theme->ApplyStyle(com.android.internal.R.style.PreviewWallpaperSettings, TRUE);
        }
        else
        {
            theme->ApplyStyle(com.android.internal.R.style.ActiveWallpaperSettings, TRUE);
        }
    }
    super->OnCreate(icicle);
}


} // namespace Wallpaper
} // namespace Service
} // namepsace Droid
} // namespace Elastos
