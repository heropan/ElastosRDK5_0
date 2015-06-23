
#include "database/sqlite/CSQLiteCursor.h"

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

ECode CSQLiteCursor::constructor(
    /* [in] */ ISQLiteDatabase* db,
    /* [in] */ ISQLiteCursorDriver* driver,
    /* [in] */ const String& editTable,
    /* [in] */ ISQLiteQuery* query)
{
    return SQLiteCursor::Init(db, driver, editTable, query);
}

ECode CSQLiteCursor::constructor(
    /* [in] */ ISQLiteCursorDriver* driver,
    /* [in] */ const String& editTable,
    /* [in] */ ISQLiteQuery* query)
{
    return SQLiteCursor::Init(driver, editTable, query);
}

PInterface CSQLiteCursor::Probe(
    /* [in]  */ REIID riid)
{
    return _CSQLiteCursor::Probe(riid);
}

ECode CSQLiteCursor::GetDatabase(
    /* [out] */ ISQLiteDatabase** database)
{
    VALIDATE_NOT_NULL(database)
    return SQLiteCursor::GetDatabase(database);
}

ECode CSQLiteCursor::SetSelectionArguments(
    /* [in] */ ArrayOf<String>* selectionArgs)
{
    return SQLiteCursor::SetSelectionArguments(selectionArgs);
}

ECode CSQLiteCursor::IsBlob(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return SQLiteCursor::IsBlob(columnIndex, result);
}

ECode CSQLiteCursor::IsString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return SQLiteCursor::IsString(columnIndex, result);
}

ECode CSQLiteCursor::IsInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return SQLiteCursor::IsInt64(columnIndex, result);
}

ECode CSQLiteCursor::IsFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return SQLiteCursor::IsFloat(columnIndex, result);
}

ECode CSQLiteCursor::SetWindow(
    /* [in] */ ICursorWindow* window)
{
    return SQLiteCursor::SetWindow(window);
}

ECode CSQLiteCursor::HasWindow(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return SQLiteCursor::HasWindow(result);
}

ECode CSQLiteCursor::GetInt16(
    /* [in] */ Int32 column,
    /* [out] */ Int16* result)
{
    VALIDATE_NOT_NULL(result)
    return SQLiteCursor::GetInt16(column, result);
}

ECode CSQLiteCursor::GetInt32(
    /* [in] */ Int32 column,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    return SQLiteCursor::GetInt32(column, result);
}

ECode CSQLiteCursor::GetInt64(
    /* [in] */ Int32 column,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)
    return SQLiteCursor::GetInt64(column, result);
}

ECode CSQLiteCursor::SetNotificationUri(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* notifyUri,
    /* [in] */ Int32 userHandle)
{
    return SQLiteCursor::SetNotificationUri(cr, notifyUri, userHandle);
}

ECode CSQLiteCursor::GetNotificationUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    return SQLiteCursor::GetNotificationUri(uri);
}

ECode CSQLiteCursor::SetExtras(
    /* [in] */ IBundle* extras)
{
    return SQLiteCursor::SetExtras(extras);
}

ECode CSQLiteCursor::OnChange(
    /* [in] */ Boolean selfChange)
{
    return SQLiteCursor::OnChange(selfChange);
}

ECode CSQLiteCursor::GetWindow(
    /* [out] */ ICursorWindow** window)
{
    VALIDATE_NOT_NULL(window);
    return SQLiteCursor::GetWindow(window);
}

ECode CSQLiteCursor::FillWindow(
    /* [in] */ Int32 pos,
    /* [in] */ ICursorWindow* window)
{
    return SQLiteCursor::FillWindow(pos, window);
}

ECode CSQLiteCursor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return SQLiteCursor::OnMove(oldPosition, newPosition, succeeded);
}

ECode CSQLiteCursor::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    return SQLiteCursor::GetCount(count);
}

ECode CSQLiteCursor::GetPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position)
    return SQLiteCursor::GetPosition(position);
}

ECode CSQLiteCursor::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return SQLiteCursor::Move(offset, succeeded);
}

ECode CSQLiteCursor::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return SQLiteCursor::MoveToPosition(position, succeeded);
}

ECode CSQLiteCursor::MoveToFirst(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return SQLiteCursor::MoveToFirst(succeeded);
}

ECode CSQLiteCursor::MoveToLast(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return SQLiteCursor::MoveToLast(succeeded);
}

ECode CSQLiteCursor::MoveToNext(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return SQLiteCursor::MoveToNext(succeeded);
}

ECode CSQLiteCursor::MoveToPrevious(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return SQLiteCursor::MoveToPrevious(succeeded);
}

ECode CSQLiteCursor::IsFirst(
    /* [out] */ Boolean* isFirst)
{
    VALIDATE_NOT_NULL(isFirst)
    return SQLiteCursor::IsFirst(isFirst);
}

ECode CSQLiteCursor::IsLast(
    /* [out] */ Boolean* isLast)
{
    VALIDATE_NOT_NULL(isLast)
    return SQLiteCursor::IsLast(isLast);
}

ECode CSQLiteCursor::IsBeforeFirst(
    /* [out] */ Boolean* isBeforeFirst)
{
    VALIDATE_NOT_NULL(isBeforeFirst)
    return SQLiteCursor::IsBeforeFirst(isBeforeFirst);
}

ECode CSQLiteCursor::IsAfterLast(
    /* [out] */ Boolean* isAfterLast)
{
    VALIDATE_NOT_NULL(isAfterLast)
    return SQLiteCursor::IsAfterLast(isAfterLast);
}

ECode CSQLiteCursor::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex)
    return SQLiteCursor::GetColumnIndex(columnName, columnIndex);
}

ECode CSQLiteCursor::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex)
    return SQLiteCursor::GetColumnIndexOrThrow(columnName, columnIndex);
}

ECode CSQLiteCursor::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnName)
{
    VALIDATE_NOT_NULL(columnName)
    return SQLiteCursor::GetColumnName(columnIndex, columnName);
}

ECode CSQLiteCursor::GetColumnNames(
    /* [out, callee] */ ArrayOf<String>** columnNames)
{
    VALIDATE_NOT_NULL(columnNames)
    return SQLiteCursor::GetColumnNames(columnNames);
}

ECode CSQLiteCursor::GetColumnCount(
    /* [out] */ Int32* columnCount)
{
    VALIDATE_NOT_NULL(columnCount)
    return SQLiteCursor::GetColumnCount(columnCount);
}

ECode CSQLiteCursor::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out, callee] */ ArrayOf<Byte>** blob)
{
    VALIDATE_NOT_NULL(blob)
    return SQLiteCursor::GetBlob(columnIndex, blob);
}

ECode CSQLiteCursor::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return SQLiteCursor::GetString(columnIndex, columnValue);
}

ECode CSQLiteCursor::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer* buf)
{
    return SQLiteCursor::CopyStringToBuffer(columnIndex, buf);
}

ECode CSQLiteCursor::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return SQLiteCursor::GetFloat(columnIndex, columnValue);
}

ECode CSQLiteCursor::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return SQLiteCursor::GetDouble(columnIndex, columnValue);
}

ECode CSQLiteCursor::GetType(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return SQLiteCursor::GetType(columnIndex, index);
}

ECode CSQLiteCursor::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isNull)
{
    VALIDATE_NOT_NULL(isNull)
    return SQLiteCursor::IsNull(columnIndex, isNull);
}

ECode CSQLiteCursor::Deactivate()
{
    return SQLiteCursor::Deactivate();
}

ECode CSQLiteCursor::Requery(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return SQLiteCursor::Requery(succeeded);
}

ECode CSQLiteCursor::Close()
{
    return SQLiteCursor::Close();
}

ECode CSQLiteCursor::IsClosed(
    /* [out] */ Boolean* closed)
{
    VALIDATE_NOT_NULL(closed)
    return SQLiteCursor::IsClosed(closed);
}

ECode CSQLiteCursor::RegisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return SQLiteCursor::RegisterContentObserver(observer);
}

ECode CSQLiteCursor::UnregisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return SQLiteCursor::UnregisterContentObserver(observer);
}

ECode CSQLiteCursor::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return SQLiteCursor::RegisterDataSetObserver(observer);
}

ECode CSQLiteCursor::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return SQLiteCursor::UnregisterDataSetObserver(observer);
}

ECode CSQLiteCursor::SetNotificationUri(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri)
{
    return SQLiteCursor::SetNotificationUri(cr, uri);
}

ECode CSQLiteCursor::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return SQLiteCursor::GetWantsAllOnMoveCalls(result);
}

ECode CSQLiteCursor::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)
    return SQLiteCursor::GetExtras(extras);
}

ECode CSQLiteCursor::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** result)
{
    VALIDATE_NOT_NULL(result)
    return SQLiteCursor::Respond(extras, result);
}

} //Sqlite
} //Database
} //Droid
} //Elastos