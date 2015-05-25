
#ifndef __CJDBCSTATEMENT_H__
#define __CJDBCSTATEMENT_H__

#include "_Elastos_Sql_SQLite_JDBC_CJDBCStatement.h"
#include "JDBCStatement.h"

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

CarClass(CJDBCStatement) , public JDBCStatement
{
public:
    CAR_OBJECT_DECL();

    CARAPI constructor(
        /* [in] */ AutoPtr<IJDBCConnection> conn);
};

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CJDBCSTATEMENT_H__
