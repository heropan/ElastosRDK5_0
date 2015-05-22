
#ifndef __CDATABASEX_H__
#define __CDATABASEX_H__

#include "_Elastos_Sql_SQLite_JDBC_CDatabaseX.h"
#include "Database.h"

using Elastos::Sql::SQLite::Database;

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

CarClass(CDatabaseX)
    , public Database
{
public:
    CAR_OBJECT_DECL()

    CDatabaseX();

    CARAPI constructor();

    CARAPI Exec(
        /* [in] */ const String& sql,
        /* [in] */ ICallback * cb);

    CARAPI Exec(
        /* [in] */ const String& sql,
        /* [in] */ ICallback * cb,
        /* [in] */ const ArrayOf<String>& args);

    CARAPI GetTable(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String>& args,
        /* [out] */ ITableResult ** tbl);

    CARAPI GetTable(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String>& args,
        /* [in] */ ITableResult * tbl);

protected:
    CARAPI_(void) Wait(Int32 ms);

private:
    static AutoPtr<IObject> mLock;
};

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CDATABASEX_H__
