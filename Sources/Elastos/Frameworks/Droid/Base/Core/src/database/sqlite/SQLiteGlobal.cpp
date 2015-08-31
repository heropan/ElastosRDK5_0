
#include "database/sqlite/SQLiteGlobal.h"
#include "os/SystemProperties.h"
//#include "os/CStatFs.h"
//#include "content/res/CResources.h"
#include "R.h"
#include <elastos/core/Math.h>
#include <sqlite3.h>
#include <elastos/core/AutoLock.h>

using Elastos::Droid::R;
//using Elastos::Droid::Content::Res::CResources;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Os::IStatFs;
//using Elastos::Droid::Os::CStatFs;
using Elastos::Droid::Os::SystemProperties;

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

const String SQLiteGlobal::TAG("SQLiteGlobal");
Object SQLiteGlobal::mLock;
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
    Int32 result;
    synchronized(mLock) {
        if (sDefaultPageSize == 0) {
            AutoPtr<IStatFs> fs;
            assert(0 && "TODO");
            //CStatFs::New(String("/data"), (IStatFs**)&fs);
            fs->GetBlockSize(&sDefaultPageSize);
        }
        SystemProperties::GetInt32(String("ro.build.version.sdk"), sDefaultPageSize, &result);
    }
    return result;
}

String SQLiteGlobal::GetDefaultJournalMode()
{
    String str, result;
    assert(0 && "TODO");
    AutoPtr<IResources> r;// = CResources::GetSystem();
    r->GetString(R::string::db_default_journal_mode, &str);
    SystemProperties::Get(String("debug.sqlite.journalmode"), str, &result);
    return result;
}

Int32 SQLiteGlobal::GetJournalSizeLimit()
{
    Int32 num, result;
    assert(0 && "TODO");
    AutoPtr<IResources> r;// = CResources::GetSystem();
    r->GetInteger(R::integer::db_journal_size_limit, &num);
    SystemProperties::GetInt32(String("debug.sqlite.journalsizelimit"), num, &result);
    return result;
}

String SQLiteGlobal::GetDefaultSyncMode()
{
    String str, result;
    assert(0 && "TODO");
    AutoPtr<IResources> r;// = CResources::GetSystem();
    r->GetString(R::string::db_default_sync_mode, &str);
    SystemProperties::Get(String("debug.sqlite.syncmode"), str, &result);
    return result;
}

String SQLiteGlobal::GetWALSyncMode()
{
    String str, result;
    assert(0 && "TODO");
    AutoPtr<IResources> r;// = CResources::GetSystem();
    r->GetString(R::string::db_wal_sync_mode, &str);
    SystemProperties::Get(String("debug.sqlite.wal.syncmode"), str, &result);
    return result;
}

Int32 SQLiteGlobal::GetWALAutoCheckpoint()
{
    Int32 num, value;
    assert(0 && "TODO");
    AutoPtr<IResources> r;// = CResources::GetSystem();
    r->GetInteger(R::integer::db_wal_autocheckpoint, &num);
    SystemProperties::GetInt32(String("debug.sqlite.wal.autocheckpoint"), num, &value);
    return Elastos::Core::Math::Max(1, value);
}

Int32 SQLiteGlobal::GetWALConnectionPoolSize()
{
    Int32 num, value;
    assert(0 && "TODO");
    AutoPtr<IResources> r;// = CResources::GetSystem();
    r->GetInteger(R::integer::db_connection_pool_size, &num);
    SystemProperties::GetInt32(String("debug.sqlite.wal.poolsize"), num, &value);
    return Elastos::Core::Math::Max(2, value);
}

} //Sqlite
} //Database
} //Droid
} //Elastos