
#ifndef __ELASTOS_DROID_SERVER_AM_BACKUPRECORD_H__
#define __ELASTOS_DROID_SERVER_AM_BACKUPRECORD_H__

#include "am/ProcessRecord.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class BackupRecord : public ElRefBase
{
public:
    BackupRecord(
        /* [in] */ BatteryStatsImpl::Uid::Pkg::Serv* agentStats,
        /* [in] */ IApplicationInfo* appInfo,
        /* [in] */ Int32 backupMode)
        : mStats(agentStats)
        , mAppInfo(appInfo)
        , mBackupMode(backupMode)
    {}


    CARAPI_(String) ToString();

public:
    // backup/restore modes
    static const Int32 BACKUP_NORMAL = 0;
    static const Int32 BACKUP_FULL = 1;
    static const Int32 RESTORE = 2;
    static const Int32 RESTORE_FULL = 3;

    AutoPtr<BatteryStatsImpl::Uid::Pkg::Serv> mStats;
    String mStringName;                     // cached toString() output
    AutoPtr<IApplicationInfo> mAppInfo;         // information about BackupAgent's app
    Int32 mBackupMode;                  // full backup / incremental / restore
    AutoPtr<ProcessRecord> mApp;                     // where this agent is running or null
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_AM_BACKUPRECORD_H__
