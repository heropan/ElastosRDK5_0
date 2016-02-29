
#ifndef  __ELASTOS_DROID_PACKAGES_LAUNCHER2_CWALLPAPERCHOOSERDIALOGFRAGMENT_H__
#define  __ELASTOS_DROID_PACKAGES_LAUNCHER2_CWALLPAPERCHOOSERDIALOGFRAGMENT_H__

#include "_Elastos_Droid_Packages_Launcher2_CWallpaperChooserDialogFragment.h"
#include "elastos/droid/packages/launcher2/WallpaperChooserDialogFragment.h"

using Elastos::Droid::Packages::Launcher2::WallpaperChooserDialogFragment;

namespace Elastos {
namespace Droid {
namespace Packages {
namespace Launcher2 {

CarClass(CWallpaperChooserDialogFragment)
    , public WallpaperChooserDialogFragment
{
public:
    CAR_OBJECT_DECL();
};

} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_PACKAGES_LAUNCHER2_CWALLPAPERCHOOSERDIALOGFRAGMENT_H__
