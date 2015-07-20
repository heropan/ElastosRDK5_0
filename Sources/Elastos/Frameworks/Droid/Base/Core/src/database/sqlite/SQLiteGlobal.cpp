
#include "database/sqlite/SQLiteGlobal.h"
#include "os/SystemProperties.h"
#include "os/CSystemProperties.h"
#include "os/CStatFs.h"
#include "content/res/CResources.h"
#include "R.h"
#include <elastos/core/Math.h>
#include <sqlite3_android.h>

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::CResources;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Os::IStatFs;
using Elastos::Droid::Os::CStatFs;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

const String SQLiteGlobal::TAG("SQLiteGlobal");
Mutex SQLiteGlobal::mLock;
Int32 SQLiteGlobal::sDefaultPageSize;

// Limit heap to 8MB for now.  This is 4 times the maximum cursor window
// size, as has been used by the original code in SQLiteDatabase for
// a long time.
static const Int32 SOFT_HEAP_LIMIT = 8 * 1024 * 1024;

Int32 SQLiteGlobal::NativeReleaseMemory()
{
    return sqlite3_release_memory(SOFT_HEAP_LIMIT);
}

Int32 SQLiteGlobal::ReleaseMemory()
{
    return NativeReleaseMemory();
}

Int32 SQLiteGlobal::GetDefaultPageSize()
{
    AutoLock lock(mLock);

    if (sDefaultPageSize == 0) {
        AutoPtr<IStatFs> fs;
        CStatFs::New(String("/data"), (IStatFs**)&fs);
        fs->GetBlockSize(&sDefaultPageSize);
    }
    return SystemProperties::GetInt32(String("ro.build.version.sdk"), sDefaultPageSize);
}

String SQLiteGlobal::GetDefaultJournalMode()
{
    String str, result;
    AutoPtr<IResources> r = CResources::GetSystem();
    r->GetString(R::string::db_default_journal_mode, &str);
    AutoPtr<ISystemProperties> sp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sp);
    sp->Get(String("debug.sqlite.journalmode"), str, &result);
    return result;
}

Int32 SQLiteGlobal::GetJournalSizeLimit()
{
    Int32 num;
    AutoPtr<IResources> r = CResources::GetSystem();
    r->GetInteger(R::integer::db_journal_size_limit, &num);
    return SystemProperties::GetInt32(String("debug.sqlite.journalsizelimit"), num);
}

String SQLiteGlobal::GetDefaultSyncMode()
{
    String str, result;
    AutoPtr<IResources> r = CResources::GetSystem();
    r->GetString(R::string::db_default_sync_mode, &str);
    AutoPtr<ISystemProperties> sp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sp);
    sp->Get(String("debug.sqlite.syncmode"), str, &result);
    return result;
}

String SQLiteGlobal::GetWALSyncMode()
{
    String str, result;
    AutoPtr<IResources> r = CResources::GetSystem();
    r->GetString(R::string::db_wal_sync_mode, &str);
    AutoPtr<ISystemProperties> sp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sp);
    sp->Get(String("debug.sqlite.wal.syncmode"), str, &result);
    return result;
}

Int32 SQLiteGlobal::GetWALAutoCheckpoint()
{
    Int32 num, value;
    AutoPtr<IResources> r = CResources::GetSystem();
    r->GetInteger(R::integer::db_wal_autocheckpoint, &num);
    value = SystemProperties::GetInt32(String("debug.sqlite.wal.autocheckpoint"), num);
    return Elastos::Core::Math::Max(1, value);
}

Int32 SQLiteGlobal::GetWALConnectionPoolSize()
{
    Int32 num, value;
    AutoPtr<IResources> r = CResources::GetSystem();
    r->GetInteger(R::integer::db_connection_pool_size, &num);
    value = SystemProperties::GetInt32(String("debug.sqlite.wal.poolsize"), num);
    return Elastos::Core::Math::Max(2, value);
}

} //Sqlite
} //Database
} //Droid
} //Elastos