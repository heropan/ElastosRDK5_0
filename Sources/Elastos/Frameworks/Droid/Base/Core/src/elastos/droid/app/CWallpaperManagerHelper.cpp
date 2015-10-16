
#include "elastos/droid/app/CWallpaperManagerHelper.h"
#include "elastos/droid/app/CWallpaperManager.h"

namespace Elastos{
namespace Droid{
namespace App{

ECode CWallpaperManagerHelper::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ IWallpaperManager** manager)
{
    VALIDATE_NOT_NULL(manager);
    return CWallpaperManager::GetInstance(context, manager);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
