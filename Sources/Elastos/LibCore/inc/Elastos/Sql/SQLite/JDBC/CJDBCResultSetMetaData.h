
#ifndef __CJDBCRESULTSETMETADATA_H__
#define __CJDBCRESULTSETMETADATA_H__

#include "_CJDBCResultSetMetaData.h"

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

CarClass(CJDBCResultSetMetaData)
{
public:
    CARAPI GetCatalogName(
        /* [in] */ Int32 column,
        /* [out] */ String * str);

    CARAPI GetColumnClassName(
        /* [in] */ Int32 column,
        /* [out] */ String * str);

    CARAPI GetColumnCount(
        /* [out] */ Int32 * value);

    CARAPI GetColumnDisplaySize(
        /* [in] */ Int32 column,
        /* [out] */ Int32 * value);

    CARAPI GetColumnLabel(
        /* [in] */ Int32 column,
        /* [out] */ String * str);

    CARAPI GetColumnName(
        /* [in] */ Int32 column,
        /* [out] */ String * str);

    CARAPI GetColumnType(
        /* [in] */ Int32 column,
        /* [out] */ Int32 * value);

    CARAPI GetColumnTypeName(
        /* [in] */ Int32 column,
        /* [out] */ String * str);

    CARAPI GetPrecision(
        /* [in] */ Int32 column,
        /* [out] */ Int32 * value);

    CARAPI GetScale(
        /* [in] */ Int32 column,
        /* [out] */ Int32 * value);

    CARAPI GetSchemaName(
        /* [in] */ Int32 column,
        /* [out] */ String * str);

    CARAPI GetTableName(
        /* [in] */ Int32 column,
        /* [out] */ String * str);

    CARAPI IsAutoIncrement(
        /* [in] */ Int32 column,
        /* [out] */ Boolean * value);

    CARAPI IsCaseSensitive(
        /* [in] */ Int32 column,
        /* [out] */ Boolean * value);

    CARAPI IsCurrency(
        /* [in] */ Int32 column,
        /* [out] */ Boolean * value);

    CARAPI IsDefinitelyWritable(
        /* [in] */ Int32 column,
        /* [out] */ Boolean * value);

    CARAPI IsNullable(
        /* [in] */ Int32 column,
        /* [out] */ Int32 * value);

    CARAPI IsReadOnly(
        /* [in] */ Int32 column,
        /* [out] */ Boolean * value);

    CARAPI IsSearchable(
        /* [in] */ Int32 column,
        /* [out] */ Boolean * value);

    CARAPI IsSigned(
        /* [in] */ Int32 column,
        /* [out] */ Boolean * value);

    CARAPI IsWritable(
        /* [in] */ Int32 column,
        /* [out] */ Boolean * value);
    CARAPI IsWrapperFor(
        /* [in] */ IInterface * iface,
        /* [out] */ Boolean * value);

    CARAPI constructor(
        /* [in] */ IJDBCResultSet * ir);

    CARAPI Unwrap(
        /* [in] */ PInterface iface,
        /* [out] */ IInterface ** oface);

    CARAPI FindColByName(
        /* [in] */ const String& columnName,
        /* [out] */ Int32 * value);

private:
    AutoPtr<IJDBCResultSet> r;
};

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CJDBCRESULTSETMETADATA_H__
