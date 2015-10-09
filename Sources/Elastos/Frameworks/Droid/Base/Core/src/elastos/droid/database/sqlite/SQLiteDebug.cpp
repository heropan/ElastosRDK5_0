
#include "database/sqlite/SQLiteDebug.h"
#include "database/sqlite/SQLiteDatabase.h"
#include "os/Build.h"
#include "os/SystemProperties.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Logger.h>
#include <sqlite3.h>

using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

SQLiteDebug::DbStats::DbStats(
    /* [in] */ const String& dbName,
    /* [in] */ Int64 pageCount,
    /* [in] */ Int64 pageSize,
    /* [in] */ Int32 lookaside,
    /* [in] */ Int32 hits,
    /* [in] */ Int32 misses,
    /* [in] */ Int32 cachesize)
    : mDbName(dbName)
    , mPageSize(pageSize / 1024)
    , mDbSize((pageCount * pageSize) / 1024)
    , mLookaside(lookaside)
{
    StringBuilder builder(hits);
    builder += "/";
    builder += misses;
    builder += "/";
    builder += cachesize;
    mCache = builder.ToString();
}


SQLiteDebug::PagerStats::~PagerStats()
{
    mDbStats = NULL;
}


const Boolean SQLiteDebug::DEBUG_SQL_LOG = Logger::IsLoggable(String("SQLiteLog"), Logger::VERBOSE);
const Boolean SQLiteDebug::DEBUG_SQL_STATEMENTS  = Logger::IsLoggable(String("SQLiteStatements"), Logger::VERBOSE);
const Boolean SQLiteDebug::DEBUG_SQL_TIME = Logger::IsLoggable(String("SQLiteTime"), Logger::VERBOSE);
const Boolean SQLiteDebug::DEBUG_LOG_SLOW_QUERIES  = Build::IS_DEBUGGABLE;

void SQLiteDebug::NativeGetPagerStats(
    /* [in] */ PagerStats* stats)
{
    Int32 memoryUsed;
    Int32 pageCacheOverflow;
    Int32 largestMemAlloc;
    Int32 unused;

    sqlite3_status(SQLITE_STATUS_MEMORY_USED, &memoryUsed, &unused, 0);
    sqlite3_status(SQLITE_STATUS_MALLOC_SIZE, &unused, &largestMemAlloc, 0);
    sqlite3_status(SQLITE_STATUS_PAGECACHE_OVERFLOW, &pageCacheOverflow, &unused, 0);
    stats->mMemoryUsed = memoryUsed;
    stats->mPageCacheOverflow = pageCacheOverflow;
    stats->mLargestMemAlloc = largestMemAlloc;
}

SQLiteDebug::SQLiteDebug()
{}

Boolean SQLiteDebug::ShouldLogSlowQuery(
    /* [in] */ Int64 elapsedTimeMillis)
{
    Int32 slowQueryMillis;
    SystemProperties::GetInt32(String("db.log.slow_query_threshold"), -1, &slowQueryMillis);
    return slowQueryMillis >= 0 && elapsedTimeMillis >= slowQueryMillis;
}

AutoPtr<SQLiteDebug::PagerStats> SQLiteDebug::GetDatabaseInfo()
{
    AutoPtr<PagerStats> stats = new PagerStats();
    NativeGetPagerStats(stats);
    stats->mDbStats = SQLiteDatabase::GetDbStats();
    return stats;
}

void SQLiteDebug::Dump(
    /* [in] */ IPrinter* printer,
    /* [in] */ const ArrayOf<String>& args)
{
    Boolean verbose = FALSE;
    for(Int32 i=0; i < args.GetLength(); i++) {
        if((args)[i].Equals("-v")) {
            verbose = TRUE;
        }
    }
    SQLiteDatabase::DumpAll(printer, verbose);
}

} //Sqlite
} //Database
} //Droid
} //Elastos
