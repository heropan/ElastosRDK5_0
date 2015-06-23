
#include "database/sqlite/CSQLiteQueryBuilder.h"

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

ECode CSQLiteQueryBuilder::constructor()
{
    return NOERROR;
}

ECode CSQLiteQueryBuilder::SetDistinct(
    /* [in] */ Boolean distinct)
{
   return SQLiteQueryBuilder::SetDistinct(distinct);
}

ECode CSQLiteQueryBuilder::GetTables(
    /* [out] */ String* tables)
{
    VALIDATE_NOT_NULL(tables)
    *tables = SQLiteQueryBuilder::GetTables();
    return NOERROR;
}

ECode CSQLiteQueryBuilder::SetTables(
    /* [in] */ const String& inTables)
{
    return SQLiteQueryBuilder::SetTables(inTables);
}

ECode CSQLiteQueryBuilder::AppendWhere(
    /* [in] */ ICharSequence *inWhere)
{
    return SQLiteQueryBuilder::AppendWhere(inWhere);
}

ECode CSQLiteQueryBuilder::AppendWhereEscapeString(
    /* [in] */ const String& inWhere)
{
    return SQLiteQueryBuilder::AppendWhereEscapeString(inWhere);
}

ECode CSQLiteQueryBuilder::SetProjectionMap(
    /* [in] */ IObjectStringMap* columnMap)
{
    return SQLiteQueryBuilder::SetProjectionMap(columnMap);
}

ECode CSQLiteQueryBuilder::SetCursorFactory(
    /* [in] */ ISQLiteDatabaseCursorFactory* factory)
{
    return SQLiteQueryBuilder::SetCursorFactory(factory);
}

ECode CSQLiteQueryBuilder::SetStrict(
    /* [in] */ Boolean flag)
{
    return SQLiteQueryBuilder::SetStrict(flag);
}

ECode CSQLiteQueryBuilder::Query(
    /* [in] */ ISQLiteDatabase* db,
    /* [in] */ ArrayOf<String>* projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);
    return SQLiteQueryBuilder::Query(db, projectionIn, selection,
            selectionArgs, groupBy, having, sortOrder, cursor);
}

ECode CSQLiteQueryBuilder::Query(
    /* [in] */ ISQLiteDatabase *db,
    /* [in] */ ArrayOf<String>* projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [out] */ ICursor **cursor)
{
    VALIDATE_NOT_NULL(cursor);
    return SQLiteQueryBuilder::Query(db, projectionIn, selection,
            selectionArgs, groupBy, having, sortOrder, limit, cursor);
}

ECode CSQLiteQueryBuilder::Query(
    /* [in] */ ISQLiteDatabase* db,
    /* [in] */ ArrayOf<String>* projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [in] */ ICancellationSignal* cancellationSignal,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor)
    return SQLiteQueryBuilder::Query(db, projectionIn, selection,
            selectionArgs, groupBy, having, sortOrder, limit, cancellationSignal, cursor);
}

ECode CSQLiteQueryBuilder::BuildQuery(
    /* [in] */ ArrayOf<String>* projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [out] */ String *str)
{
    VALIDATE_NOT_NULL(str);
    return SQLiteQueryBuilder::BuildQuery(projectionIn, selection, groupBy, having, sortOrder, limit, str);
}

ECode CSQLiteQueryBuilder::BuildQuery(
    /* [in] */ ArrayOf<String>* projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [out] */ String *str)
{
    VALIDATE_NOT_NULL(str);
    return SQLiteQueryBuilder::BuildQuery(projectionIn, selection,
            selectionArgs, groupBy, having, sortOrder, limit, str);
}

ECode CSQLiteQueryBuilder::BuildUnionSubQuery(
    /* [in] */ const String& typeDiscriminatorColumn,
    /* [in] */ const ArrayOf<String>& unionColumns,
    /* [in] */ IObjectStringMap* columnsPresentInTable,
    /* [in] */ Int32 computedColumnsOffset,
    /* [in] */ const String& typeDiscriminatorValue,
    /* [in] */ const String& selection,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [out] */ String *str)
{
    VALIDATE_NOT_NULL(str);
    return SQLiteQueryBuilder::BuildUnionSubQuery(typeDiscriminatorColumn ,
            unionColumns, columnsPresentInTable, computedColumnsOffset, typeDiscriminatorValue,
            selection, groupBy, having, str);
}

ECode CSQLiteQueryBuilder::BuildUnionSubQuery(
    /* [in] */ const String& typeDiscriminatorColumn,
    /* [in] */ const ArrayOf<String>& unionColumns,
    /* [in] */ IObjectStringMap* columnsPresentInTable,
    /* [in] */ Int32 computedColumnsOffset,
    /* [in] */ const String& typeDiscriminatorValue,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [out] */ String *str)
{
    VALIDATE_NOT_NULL(str);
    return SQLiteQueryBuilder::BuildUnionSubQuery(typeDiscriminatorColumn ,
            unionColumns, columnsPresentInTable, computedColumnsOffset, typeDiscriminatorValue,
            selection, selectionArgs, groupBy, having, str);
}

ECode CSQLiteQueryBuilder::BuildUnionQuery(
    /* [in] */ const ArrayOf<String>& subQueries,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [out] */ String *str)
{
    VALIDATE_NOT_NULL(str);
    return SQLiteQueryBuilder::BuildUnionQuery(subQueries, sortOrder, limit, str);
}

} //Sqlite
} //Database
} //Droid
} //Elastos