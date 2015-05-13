
#include "CTableResultX.h"
#include "CTableResult.h"
#include "CJDBCDatabaseMetaData.h"

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

ECode CTableResultX::ToString(
    /* [out] */ String * pStr)
{
    *pStr = TableResult::ToString();
    return NOERROR;
}

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
    TableResult::InitEx(maxrows);
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

ECode CTableResultX::Clear()
{
    TableResult::Clear();
    return NOERROR;
}

ECode CTableResultX::Columns(
    /* [in] */ const ArrayOf<String>& coldata)
{
    TableResult::Columns(coldata);
    return NOERROR;
}

ECode CTableResultX::Types(
    /* [in] */ const ArrayOf<String>& intypes)
{
    TableResult::Types(intypes);
    return NOERROR;
}

ECode CTableResultX::Newrow(
    /* [in] */ const ArrayOf<String>& rowdata,
    /* [out] */ Boolean * value)
{
    *value = TableResult::Newrow(rowdata);
    return NOERROR;
}

void CTableResultX::Sql_types(AutoPtr<ArrayOf<Int32> > types)
{
    sql_type = types;
}

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos
