#ifndef __ELASTOS_DROID_DATABASE_SQLITE_SQLITEPROGRAM_H__
#define __ELASTOS_DROID_DATABASE_SQLITE_SQLITEPROGRAM_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/database/sqlite/SQLiteClosable.h"
#include "elastos/droid/database/sqlite/SQLiteSession.h"
#include <sqlite3.h>

using Elastos::Droid::Database::Sqlite::SQLiteClosable;
using Elastos::Core::IArrayOf;

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

/**
 * A base class for compiled SQLite programs.
 *
 * SQLiteProgram is not internally synchronized so code using a SQLiteProgram from multiple
 * threads should perform its own synchronization when using the SQLiteProgram.
 */
class SQLiteProgram
    : public SQLiteClosable
    , public ISQLiteProgram
{
public:
    CAR_INTERFACE_DECL()

    SQLiteProgram();

    CARAPI Init(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ const String& sql,
        /* [in] */ ArrayOf<IInterface*>* bindArgs,
        /* [in] */ ICancellationSignal* cancellationSignalForPrepare);

    virtual ~SQLiteProgram() {}

    CARAPI_(AutoPtr<ISQLiteDatabase>) GetDatabase();

    CARAPI_(String) GetSql();

    CARAPI_(AutoPtr< ArrayOf<IInterface*> >) GetBindArgs();

    CARAPI_(AutoPtr< ArrayOf<String> >) GetColumnNames();

    /**
     * Returns a unique identifier for this program.
     *
     * @return a unique identifier for this program
     */
    virtual CARAPI GetUniqueId(
        /* [out] */ Int32* id);

    /**
     * Bind a NULL value to this statement. The value remains bound until
     * {@link #clearBindings} is called.
     *
     * @param index The 1-based index to the parameter to bind null to
     */
    virtual CARAPI BindNull(
        /* [in] */ Int32 index);

    /**
     * Bind a long value to this statement. The value remains bound until
     * {@link #clearBindings} is called.
     *
     * @param index The 1-based index to the parameter to bind
     * @param value The value to bind
     */
    virtual CARAPI BindInt64(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value);

    /**
     * Bind a double value to this statement. The value remains bound until
     * {@link #clearBindings} is called.
     *
     * @param index The 1-based index to the parameter to bind
     * @param value The value to bind
     */
    virtual CARAPI BindDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double value);

    /**
     * Bind a String value to this statement. The value remains bound until
     * {@link #clearBindings} is called.
     *
     * @param index The 1-based index to the parameter to bind
     * @param value The value to bind
     */
    virtual CARAPI BindString(
        /* [in] */ Int32 index,
        /* [in] */ const String& value);

    /**
     * Bind a byte array value to this statement. The value remains bound until
     * {@link #clearBindings} is called.
     *
     * @param index The 1-based index to the parameter to bind
     * @param value The value to bind
     */
    virtual CARAPI BindBlob(
        /* [in] */ Int32 index,
        /* [in] */ const ArrayOf<Byte>& value);

    /**
     * Clears all existing bindings. Unset bindings are treated as NULL.
     */
    virtual CARAPI ClearBindings();

    /**
     * Given an array of String bindArgs, this method binds all of them in one single call.
     *
     * @param bindArgs the String array of bind args, none of which must be null.
     */
    virtual CARAPI BindAllArgsAsStrings(
        /* [in] */ ArrayOf<String>* bindArgs);

protected:
    CARAPI GetSession(
        /* [out] */ SQLiteSession** session);

    CARAPI_(Int32) GetConnectionFlags();

    CARAPI OnCorruption();

    CARAPI_(void) OnAllReferencesReleased();

    CARAPI Bind(
        /* [in] */ Int32 index,
        /* [in] */ IInterface* value);

private:
    static const AutoPtr< ArrayOf<String> > EMPTY_STRING_ARRAY;

    AutoPtr<SQLiteDatabase> mDatabase;
    String mSql;
    Boolean mReadOnly;
    AutoPtr< ArrayOf<String> > mColumnNames;
    Int32 mNumParameters;
    AutoPtr< ArrayOf<IInterface*> > mBindArgs;
};

} //Sqlite
} //Database
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_DATABASE_SQLITE_SQLITEPROGRAM_H__
