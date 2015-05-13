#include "app/backup/CWallpaperBackupHelperHelper.h"
#include "app/backup/CWallpaperBackupHelper.h"

namespace Elastos {
namespace Droid {
namespace App {
namespace Backup {

ECode CWallpaperBackupHelperHelper::GetWALLPAPER_IMAGE(
    /* [out] */ String* image)
{
    VALIDATE_NOT_NULL(image);
    *image = CWallpaperBackupHelper::GetWALLPAPER_IMAGE();
    return NOERROR;
}

ECode CWallpaperBackupHelperHelper::GetWALLPAPER_INFO(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = CWallpaperBackupHelper::GetWALLPAPER_INFO();
    return NOERROR;
}
}
}
}
}
