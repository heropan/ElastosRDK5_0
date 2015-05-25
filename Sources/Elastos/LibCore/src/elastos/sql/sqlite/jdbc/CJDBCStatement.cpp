
#include "CJDBCStatement.h"

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

CAR_OBJECT_IMPL(CJDBCStatement)

ECode CJDBCStatement::constructor(
    /* [in] */ AutoPtr<IJDBCConnection> conn)
{
    JDBCStatement::Init(conn);
    return NOERROR;
}

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos
