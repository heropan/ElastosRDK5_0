#include "elastos/droid/app/backup/CBackupManagerHelper.h"
#include "elastos/droid/app/backup/CBackupManager.h"

namespace Elastos {
namespace Droid {
namespace App {
namespace Backup {

ECode CBackupManagerHelper::DataChanged(
    /* [in] */ const String& packageName)
{
    return CBackupManager::DataChanged(packageName);
}

}
}
}
}
