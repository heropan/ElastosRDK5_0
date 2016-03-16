
#ifndef __ELASTOS_DROID_SETTINGS_SEARCH_INDEXDATABASEHELPER_H__
#define __ELASTOS_DROID_SETTINGS_SEARCH_INDEXDATABASEHELPER_H__

#include "elastos/droid/database/sqlite/SQLiteOpenHelper.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabase;
using Elastos::Droid::Database::Sqlite::SQLiteOpenHelper;

namespace Elastos {
namespace Droid {
namespace Settings {
namespace Search {

class IndexDatabaseHelper
    : public SQLiteOpenHelper
{
public:
    IndexDatabaseHelper();

    ~IndexDatabaseHelper();

    CARAPI constructor(
        /* [in] */ IContext* context);

    static CARAPI_(AutoPtr<IndexDatabaseHelper>) GetInstance(
        /* [in] */ IContext* context);

    //@Override
    CARAPI OnCreate(
        /* [in] */ ISQLiteDatabase* db);

    //@Override
    CARAPI OnOpen(
        /* [in] */ ISQLiteDatabase* db);

    //@Override
    CARAPI OnUpgrade(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ Int32 oldVersion,
        /* [in] */ Int32 newVersion);

    //@Override
    CARAPI OnDowngrade(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ Int32 oldVersion,
        /* [in] */ Int32 newVersion);

private:
    CARAPI_(void) BootstrapDB(
        /* [in] */ ISQLiteDatabase* db);

    CARAPI_(void) Reconstruct(
        /* [in] */ ISQLiteDatabase* db);

    CARAPI GetBuildVersion(
        /* [in] */ ISQLiteDatabase* db,
        /* [out] */ String* version);

    CARAPI_(void) DropTables(
        /* [in] */ ISQLiteDatabase* db);

private:
    static const String TAG;

    static const String DATABASE_NAME;
    static const Int32 DATABASE_VERSION;

    static const String CREATE_INDEX_TABLE;

    static const String CREATE_META_TABLE;

    static const String CREATE_SAVED_QUERIES_TABLE;

    static const String INSERT_BUILD_VERSION;

    static const String SELECT_BUILD_VERSION;

    static AutoPtr<IndexDatabaseHelper> sSingleton;

    static Object sLock;
};

} // namespace Search
} // namespace Settings
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SETTINGS_SEARCH_INDEXDATABASEHELPER_H__
