#ifndef __ELASTOS_DROID_APP_BACKUP_CWALLPAPERBACKUPHELPERHELPER_H__
#define __ELASTOS_DROID_APP_BACKUP_CWALLPAPERBACKUPHELPERHELPER_H__

#include "_Elastos_Droid_App_Backup_CWallpaperBackupHelperHelper.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace App {
namespace Backup {

CarClass(CWallpaperBackupHelperHelper)
{
public:
    CARAPI GetWALLPAPER_IMAGE(
        /* [out] */ String* image);

    CARAPI GetWALLPAPER_INFO(
        /* [out] */ String* info);
};
}
}
}
}

#endif
