#include "app/backup/CBackupManagerHelper.h"
#include "app/backup/CBackupManager.h"

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
