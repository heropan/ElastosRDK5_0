
#ifndef __ELASTOS_DROID_PACKAGES_LAUNCHER2_CWALLPAPERCHOOSERDIALOGFRAGMENTHELPER_H__
#define __ELASTOS_DROID_PACKAGES_LAUNCHER2_CWALLPAPERCHOOSERDIALOGFRAGMENTHELPER_H__

#include "_Elastos_Droid_Packages_Launcher2_CWallpaperChooserDialogFragmentHelper.h"
#include <elastos/core/Singleton.h>

using Elastos::Droid::Packages::Launcher2::IWallpaperChooserDialogFragment;

namespace Elastos {
namespace Droid {
namespace Packages {
namespace Launcher2 {

CarClass(CWallpaperChooserDialogFragmentHelper)
    , public Singleton
    , public IWallpaperChooserDialogFragmentHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI NewInstance(
        /* [out] */ IWallpaperChooserDialogFragment** fragment);
};

} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_PACKAGES_LAUNCHER2_CWALLPAPERCHOOSERDIALOGFRAGMENTHELPER_H__
