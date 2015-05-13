
#include "am/BackupRecord.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

const Int32 BackupRecord::BACKUP_NORMAL;
const Int32 BackupRecord::BACKUP_FULL;
const Int32 BackupRecord::RESTORE;
const Int32 BackupRecord::RESTORE_FULL;

String BackupRecord::ToString()
{
    if (!mStringName.IsNull()) {
        return mStringName;
    }
    StringBuilder sb(128);
    sb.AppendCStr("BackupRecord{");
    sb.AppendInt32((Int32)this);  // (Integer.toHexString(System.identityHashCode(this)))
    sb.AppendChar(' ');
    String pkgName;
    mAppInfo->GetPackageName(&pkgName);
    sb.AppendString(pkgName);
    sb.AppendChar(' ');
    String name;
    mAppInfo->GetName(&name);
    sb.AppendString(name);
    sb.AppendChar(' ');
    String bName;
    mAppInfo->GetBackupAgentName(&bName);
    sb.AppendString(bName);
    sb.AppendChar('}');
    return mStringName = sb.ToString();
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
