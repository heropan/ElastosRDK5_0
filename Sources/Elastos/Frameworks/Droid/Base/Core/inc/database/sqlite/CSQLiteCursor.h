#ifndef __CSQLITECURSOR_H__
#define __CSQLITECURSOR_H__

#include "_CSQLiteCursor.h"
#include "database/sqlite/SQLiteCursor.h"

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

CarClass(CSQLiteCursor) , public SQLiteCursor
{
public:
    CARAPI constructor(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ ISQLiteCursorDriver* driver,
        /* [in] */ const String& editTable,
        /* [in] */ ISQLiteQuery* query);

    CARAPI constructor(
        /* [in] */ ISQLiteCursorDriver* driver,
        /* [in] */ const String& editTable,
        /* [in] */ ISQLiteQuery* query);

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

    CARAPI GetDatabase(
        /* [out] */ ISQLiteDatabase** database);

    CARAPI SetSelectionArguments(
        /* [in] */ ArrayOf<String>* selectionArgs);

    CARAPI IsBlob(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* result);

    CARAPI IsString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* result);

    CARAPI IsInt64(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* result);

    CARAPI IsFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* result);

    CARAPI SetWindow(
        /* [in] */ ICursorWindow* window);

    CARAPI HasWindow(
        /* [out] */ Boolean* result);

    CARAPI GetInt16(
        /* [in] */ Int32 column,
        /* [out] */ Int16* result);

    CARAPI GetInt32(
        /* [in] */ Int32 column,
        /* [out] */ Int32* result);

    CARAPI GetInt64(
        /* [in] */ Int32 column,
        /* [out] */ Int64* result);

    CARAPI SetNotificationUriEx(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IUri* notifyUri,
        /* [in] */ Int32 userHandle);

    CARAPI GetNotificationUri(
        /* [out] */ IUri** uri);

    CARAPI SetExtras(
        /* [in] */ IBundle* extras);

    CARAPI OnChange(
        /* [in] */ Boolean selfChange);

    CARAPI GetWindow(
        /* [out] */ ICursorWindow** window);

    CARAPI FillWindow(
        /* [in] */ Int32 pos,
        /* [in] */ ICursorWindow* window);

    CARAPI OnMove(
        /* [in] */ Int32 oldPosition,
        /* [in] */ Int32 newPosition,
        /* [out] */ Boolean* succeeded);

    CARAPI GetCount(
        /* [out] */ Int32* count);

    CARAPI GetPosition(
        /* [out] */ Int32* position);

    CARAPI Move(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* succeeded);

    CARAPI MoveToPosition(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* succeeded);

    CARAPI MoveToFirst(
        /* [out] */ Boolean* succeeded);

    CARAPI MoveToLast(
        /* [out] */ Boolean* succeeded);

    CARAPI MoveToNext(
        /* [out] */ Boolean* succeeded);

    CARAPI MoveToPrevious(
        /* [out] */ Boolean* succeeded);

    CARAPI IsFirst(
        /* [out] */ Boolean* isFirst);

    CARAPI IsLast(
        /* [out] */ Boolean* isLast);

    CARAPI IsBeforeFirst(
        /* [out] */ Boolean* isBeforeFirst);

    CARAPI IsAfterLast(
        /* [out] */ Boolean* isAfterLast);

    CARAPI GetColumnIndex(
        /* [in] */ const String& columnName,
        /* [out] */ Int32* columnIndex);

    CARAPI GetColumnIndexOrThrow(
        /* [in] */ const String& columnName,
        /* [out] */ Int32* columnIndex);

    CARAPI GetColumnName(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* columnName);

    CARAPI GetColumnNames(
        /* [out, callee] */ ArrayOf<String>** columnNames);

    CARAPI GetColumnCount(
        /* [out] */ Int32* columnCount);

    CARAPI GetBlob(
        /* [in] */ Int32 columnIndex,
        /* [out, callee] */ ArrayOf<Byte>** blob);

    CARAPI GetString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* columnValue);

    CARAPI CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in] */ ICharArrayBuffer* buf);

    CARAPI GetFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Float* columnValue);

    CARAPI GetDouble(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Double* columnValue);

    CARAPI GetType(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int32* index);

    CARAPI IsNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* isNull);

    CARAPI Deactivate();

    CARAPI Requery(
        /* [out] */ Boolean* succeeded);

    CARAPI Close();

    CARAPI IsClosed(
        /* [out] */ Boolean* closed);

    CARAPI RegisterContentObserver(
        /* [in] */ IContentObserver* observer);

    CARAPI UnregisterContentObserver(
        /* [in] */ IContentObserver* observer);

    CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI SetNotificationUri(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IUri* uri);

    CARAPI GetWantsAllOnMoveCalls(
        /* [out] */ Boolean* result);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** result);
};

} //Sqlite
} //Database
} //Droid
} //Elastos

#endif // __CSQLITECURSOR_H__