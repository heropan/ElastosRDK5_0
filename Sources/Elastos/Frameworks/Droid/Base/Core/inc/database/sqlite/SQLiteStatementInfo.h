#ifndef __SQLITESTATEMENTINFO_H__
#define __SQLITESTATEMENTINFO_H__

#include "ext/frameworkdef.h"
#include <elrefbase.h>

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

/**
 * Describes a SQLite statement.
 *
 * @hide
 */
class SQLiteStatementInfo : public ElRefBase
{
public:
    SQLiteStatementInfo()
        : mNumParameters(0)
        , mReadOnly(FALSE)
    {}

public:
    /**
     * The number of parameters that the statement has.
     */
    Int32 mNumParameters;

    /**
     * The names of all columns in the result set of the statement.
     */
    AutoPtr< ArrayOf<String> > mColumnNames;

    /**
     * True if the statement is read-only.
     */
    Boolean mReadOnly;
};

} //Sqlite
} //Database
} //Droid
} //Elastos

#endif // __SQLITESTATEMENTINFO_H__
