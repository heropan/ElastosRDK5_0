
#include "CTableResultX.h"
#include "CTableResult.h"
#include "CJDBCDatabaseMetaData.h"

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

CAR_OBJECT_IMPL(CTableResultX)
CAR_INTERFACE_IMPL(CTableResultX, TableResult, ITableResultX);

ECode CTableResultX::constructor()
{
    FAIL_RETURN(TableResult::constructor())
    sql_type = ArrayOf<Int32>::Alloc(mNcolumns);
    for (Int32 i = 0; i < mNcolumns; i++) {
        (*sql_type)[i] = ITypes::VARCHAR;
    }
    return NOERROR;
}

ECode CTableResultX::constructor(
    /* [in] */ Int32 maxrows)
{
    FAIL_RETURN(TableResult::constructor(maxrows))
    sql_type = ArrayOf<Int32>::Alloc(mNcolumns);
    for (Int32 i = 0; i < mNcolumns; i++) {
        (*sql_type)[i] = ITypes::VARCHAR;
    }
    return NOERROR;
}

ECode CTableResultX::constructor(
    /* [in] */ Elastos::Sql::SQLite::ITableResult* tr)
{
    mColumn = ((CTableResult* )tr)->mColumn;
    mRows = ((CTableResult* )tr)->mRows;
    mNcolumns = ((CTableResult* )tr)->mNcolumns;
    mNrows = ((CTableResult* )tr)->mNrows;
    mTypes = ((CTableResult* )tr)->mTypes;
    mMaxRows = ((CTableResult* )tr)->mMaxRows;
    sql_type = ArrayOf<Int32>::Alloc(mNcolumns);
    for (Int32 i = 0; i < mNcolumns; i++) {
        (*sql_type)[i] = ITypes::VARCHAR;
    }
    if (((CTableResult* )tr)->mTypes != NULL) {
        for (Int32 i = 0; i < ((CTableResult* )tr)->mTypes->GetLength(); i++) {
            (*sql_type)[i] = CJDBCDatabaseMetaData::MapSqlType((*((CTableResult* )tr)->mTypes)[i]);
        }
    }
    return NOERROR;
}

void CTableResultX::Sql_types(
    /* [in] */ ArrayOf<Int32>* types)
{
    sql_type = types;
}

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos
