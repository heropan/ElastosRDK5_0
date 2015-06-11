
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
    TableResult::Init();
    sql_type = ArrayOf<Int32>::Alloc(ncolumns);
    for (Int32 i = 0; i < ncolumns; i++) {
        (*sql_type)[i] = ITypes::VARCHAR;
    }
    return NOERROR;
}

ECode CTableResultX::constructor(
    /* [in] */ Int32 maxrows)
{
    TableResult::Init(maxrows);
    sql_type = ArrayOf<Int32>::Alloc(ncolumns);
    for (Int32 i = 0; i < ncolumns; i++) {
        (*sql_type)[i] = ITypes::VARCHAR;
    }
    return NOERROR;
}

ECode CTableResultX::constructor(
    /* [in] */ Elastos::Sql::SQLite::ITableResult* tr)
{
    column = ((CTableResult* )tr)->column;
    rows = ((CTableResult* )tr)->rows;
    ncolumns = ((CTableResult* )tr)->ncolumns;
    nrows = ((CTableResult* )tr)->nrows;
    types = ((CTableResult* )tr)->types;
    maxrows = ((CTableResult* )tr)->maxrows;
    sql_type = ArrayOf<Int32>::Alloc(ncolumns);
    for (Int32 i = 0; i < ncolumns; i++) {
        (*sql_type)[i] = ITypes::VARCHAR;
    }
    if (((CTableResult* )tr)->types != NULL) {
        for (Int32 i = 0; i < ((CTableResult* )tr)->types->GetLength(); i++) {
            (*sql_type)[i] = CJDBCDatabaseMetaData::MapSqlType((*((CTableResult* )tr)->types)[i]);
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
