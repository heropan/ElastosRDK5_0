
#include "elastos/droid/server/am/BackupRecord.h"

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
    sb.Append("BackupRecord{");
    sb.Append((Int32)this);  // (Integer.toHexString(System.identityHashCode(this)))
    sb.AppendChar(' ');
    String pkgName;
    mAppInfo->GetPackageName(&pkgName);
    sb.Append(pkgName);
    sb.AppendChar(' ');
    String name;
    mAppInfo->GetName(&name);
    sb.Append(name);
    sb.AppendChar(' ');
    String bName;
    mAppInfo->GetBackupAgentName(&bName);
    sb.Append(bName);
    sb.AppendChar('}');
    return mStringName = sb.ToString();
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
