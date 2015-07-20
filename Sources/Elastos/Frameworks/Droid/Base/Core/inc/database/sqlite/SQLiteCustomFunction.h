
#ifndef __SQLITECUSTOMFUNCTION_H__
#define __SQLITECUSTOMFUNCTION_H__

#include "ext/frameworkdef.h"
#include "database/sqlite/SQLiteDatabase.h"

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

/**
 * Describes a custom SQL function.
 *
 * @hide
 */
class SQLiteCustomFunction : public ElRefBase
{
public:
    /**
     * Create custom function.
     *
     * @param name The name of the sqlite3 function.
     * @param numArgs The number of arguments for the function, or -1 to
     * support any number of arguments.
     * @param callback The callback to invoke when the function is executed.
     */
    SQLiteCustomFunction(
        /* [in] */ const String& name,
        /* [in] */ Int32 numArgs,
        /* [in] */ SQLiteDatabase::ICustomFunction* callBack);

    // Called from native.
    //@SuppressWarnings("unused")
    CARAPI_(void) DispatchCallback(
        /* [in] */ ArrayOf<String>* args);

public:
    String mName;
    Int32 mNumArgs;
    AutoPtr<SQLiteDatabase::ICustomFunction> mCallback;
};

} //Sqlite
} //Database
} //Droid
} //Elastos

#endif  //__SQLITECUSTOMFUNCTION_H__