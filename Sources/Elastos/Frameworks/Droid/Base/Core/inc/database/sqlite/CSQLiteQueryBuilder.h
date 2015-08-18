#ifndef __ELASTOS_DROID_DATABASE_SQLITE_CSQLITEQUERYBUILDER_H__
#define __ELASTOS_DROID_DATABASE_SQLITE_CSQLITEQUERYBUILDER_H__

#include "_Elastos_Droid_Database_Sqlite_CSQLiteQueryBuilder.h"
#include "database/sqlite/SQLiteQueryBuilder.h"

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

CarClass(CSQLiteQueryBuilder), public SQLiteQueryBuilder
{
public:
    CARAPI constructor();

    CARAPI SetDistinct(
        /* [in] */ Boolean distinct);

    CARAPI GetTables(
        /* [out] */ String* tables);

    CARAPI SetTables(
        /* [in] */ const String& inTables);

    CARAPI AppendWhere(
        /* [in] */ ICharSequence *inWhere);

    CARAPI AppendWhereEscapeString(
        /* [in] */ const String& inWhere);

    CARAPI SetProjectionMap(
        /* [in] */ IObjectStringMap* columnMap);

    CARAPI SetCursorFactory(
        /* [in] */ ISQLiteDatabaseCursorFactory* factory);

    CARAPI SetStrict(
        /* [in] */ Boolean flag);

    CARAPI Query(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ ArrayOf<String>* projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [out] */ ICursor** cursor);

    CARAPI Query(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ ArrayOf<String>* projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [out] */ ICursor** cursor);

    CARAPI Query(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ ArrayOf<String>* projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [in] */ ICancellationSignal* cancellationSignal,
        /* [out] */ ICursor** cursor);

    CARAPI BuildQuery(
        /* [in] */ ArrayOf<String>* projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [out] */ String *str);

    CARAPI BuildQuery(
        /* [in] */ ArrayOf<String>* projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [out] */ String *str);

    CARAPI BuildUnionSubQuery(
        /* [in] */ const String& typeDiscriminatorColumn,
        /* [in] */ const ArrayOf<String>& unionColumns,
        /* [in] */ IObjectStringMap* columnsPresentInTable,
        /* [in] */ Int32 computedColumnsOffset,
        /* [in] */ const String& typeDiscriminatorValue,
        /* [in] */ const String& selection,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [out] */ String *str);

    CARAPI BuildUnionSubQuery(
        /* [in] */ const String& typeDiscriminatorColumn,
        /* [in] */ const ArrayOf<String>& unionColumns,
        /* [in] */ IObjectStringMap* columnsPresentInTable,
        /* [in] */ Int32 computedColumnsOffset,
        /* [in] */ const String& typeDiscriminatorValue,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String>& selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [out] */ String *str);

    CARAPI BuildUnionQuery(
        /* [in] */ const ArrayOf<String>& subQueries,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [out] */ String *str);
};

} //Sqlite
} //Database
} //Droid
} //Elastos

#endif // __ELASTOS_DROID_DATABASE_SQLITE_CSQLITEQUERYBUILDER_H__
