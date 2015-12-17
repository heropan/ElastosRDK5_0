
#include <Elastos.CoreLibrary.Utility.h>
#include "elastos/droid/database/sqlite/SQLiteDatabaseConfiguration.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;
using Elastos::Utility::Regex::IMatcher;
using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Regex::CPatternHelper;

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

const String SQLiteDatabaseConfiguration::MEMORY_DB_PATH(":memory:");

static AutoPtr<IPattern> InitEmailInDbPattern()
{
    AutoPtr<IPatternHelper> helper;
    CPatternHelper::AcquireSingleton((IPatternHelper**)&helper);
    AutoPtr<IPattern> pattern;
    helper->Compile(String("[\\w\\.\\-]+@[\\w\\.\\-]+"), (IPattern**)&pattern);
    return pattern;
}
const AutoPtr<IPattern> SQLiteDatabaseConfiguration::EMAIL_IN_DB_PATTERN = InitEmailInDbPattern();

SQLiteDatabaseConfiguration::SQLiteDatabaseConfiguration(
    /* [in] */ const String& path,
    /* [in] */ Int32 openFlags)
    : mOpenFlags(openFlags)
    , mMaxSqlCacheSize(25)
    , mForeignKeyConstraintsEnabled(FALSE)
{
    if (path.IsNull()) {
        Slogger::E(String("SQLiteDatabaseConfiguration"), "path must not be null.");
        assert(0);
        // throw new IllegalArgumentException("path must not be null.");
    }

    mPath = path;
    mLabel = StripPathForLogs(path);

    // Set default values for optional parameters.
    AutoPtr<ILocaleHelper> helper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
    helper->GetDefault((ILocale**)&mLocale);
}

SQLiteDatabaseConfiguration::SQLiteDatabaseConfiguration(
    /* [in] */ SQLiteDatabaseConfiguration* other)
    : mOpenFlags(0)
    , mMaxSqlCacheSize(0)
    , mForeignKeyConstraintsEnabled(FALSE)
{
    if (other == NULL) {
        Slogger::E(String("SQLiteDatabaseConfiguration"), "other must not be null.");
        assert(0);
        //throw new IllegalArgumentException("other must not be null.");
    }

    mPath  = other->mPath;
    mLabel = other->mLabel;
    ASSERT_SUCCEEDED(UpdateParametersFrom(other));
}

ECode SQLiteDatabaseConfiguration::UpdateParametersFrom(
    /* [in] */ SQLiteDatabaseConfiguration* other)
{
    if (other == NULL) {
        Slogger::E(String("SQLiteDatabaseConfiguration"), "other must not be null.");
        //throw new IllegalArgumentException("other must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (!mPath.Equals(other->mPath)) {
        Slogger::E(String("SQLiteDatabaseConfiguration"), "other configuration must refer to the same database.");
        //throw new IllegalArgumentException("other configuration must refer to "
        //        + "the same database.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mOpenFlags = other->mOpenFlags;
    mMaxSqlCacheSize = other->mMaxSqlCacheSize;
    mLocale = other->mLocale;
    mForeignKeyConstraintsEnabled = other->mForeignKeyConstraintsEnabled;
    mCustomFunctions.Clear();
    List<AutoPtr<SQLiteCustomFunction> >::Iterator it = other->mCustomFunctions.Begin();
    for (; it != other->mCustomFunctions.End(); ++it) {
        mCustomFunctions.PushBack(*it);
    }
    return NOERROR;
}

Boolean SQLiteDatabaseConfiguration::IsInMemoryDb()
{
    return mPath.EqualsIgnoreCase(MEMORY_DB_PATH);
}

String SQLiteDatabaseConfiguration::StripPathForLogs(
    /* [in] */ const String& path)
{
    if (path.IndexOf('@') == -1) {
        return path;
    }
    AutoPtr<IMatcher> matcher;
    EMAIL_IN_DB_PATTERN->Matcher(path, (IMatcher**)&matcher);
    String result;
    matcher->ReplaceAll(String("XX@YY"), &result);
    return result;
}

} //Sqlite
} //Database
} //Droid
} //Elastos
