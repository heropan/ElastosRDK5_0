#include "database/CCrossProcessCursorWrapper.h"

namespace Elastos {
namespace Droid {
namespace Database {

ECode CCrossProcessCursorWrapper::constructor(
    /* [in] */ ICursor* cursor)
{
    CrossProcessCursorWrapper::Init(cursor);
    return NOERROR;
}

ECode CCrossProcessCursorWrapper::GetWindow(
    /* [out] */ ICursorWindow** window)
{
    return CrossProcessCursorWrapper::GetWindow(window);
}

ECode CCrossProcessCursorWrapper::FillWindow(
    /* [in] */ Int32 pos,
    /* [in] */ ICursorWindow* window)
{
    return CrossProcessCursorWrapper::FillWindow(pos, window);
}

ECode CCrossProcessCursorWrapper::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition,
    /* [out] */ Boolean* succeeded)
{
    return CrossProcessCursorWrapper::OnMove(oldPosition, newPosition, succeeded);
}
ECode CCrossProcessCursorWrapper::GetWrappedCursor(
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor)
    return CrossProcessCursorWrapper::GetWrappedCursor(cursor);
}

ECode CCrossProcessCursorWrapper::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    return CrossProcessCursorWrapper::GetCount(count);
}

ECode CCrossProcessCursorWrapper::GetPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position)
    return CrossProcessCursorWrapper::GetPosition(position);
}

ECode CCrossProcessCursorWrapper::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CrossProcessCursorWrapper::Move(offset, succeeded);
}

ECode CCrossProcessCursorWrapper::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CrossProcessCursorWrapper::MoveToPosition(position, succeeded);
}

ECode CCrossProcessCursorWrapper::MoveToFirst(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CrossProcessCursorWrapper::MoveToFirst(succeeded);
}

ECode CCrossProcessCursorWrapper::MoveToLast(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CrossProcessCursorWrapper::MoveToLast(succeeded);
}

ECode CCrossProcessCursorWrapper::MoveToNext(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CrossProcessCursorWrapper::MoveToNext(succeeded);
}

ECode CCrossProcessCursorWrapper::MoveToPrevious(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CrossProcessCursorWrapper::MoveToPrevious(succeeded);
}

ECode CCrossProcessCursorWrapper::IsFirst(
    /* [out] */ Boolean* isFirst)
{
    VALIDATE_NOT_NULL(isFirst)
    return CrossProcessCursorWrapper::IsFirst(isFirst);
}

ECode CCrossProcessCursorWrapper::IsLast(
    /* [out] */ Boolean* isLast)
{
    VALIDATE_NOT_NULL(isLast)
    return CrossProcessCursorWrapper::IsLast(isLast);
}

ECode CCrossProcessCursorWrapper::IsBeforeFirst(
    /* [out] */ Boolean* isBeforeFirst)
{
    VALIDATE_NOT_NULL(isBeforeFirst)
    return CrossProcessCursorWrapper::IsBeforeFirst(isBeforeFirst);
}

ECode CCrossProcessCursorWrapper::IsAfterLast(
    /* [out] */ Boolean* isAfterLast)
{
    VALIDATE_NOT_NULL(isAfterLast)
    return CrossProcessCursorWrapper::IsAfterLast(isAfterLast);
}

ECode CCrossProcessCursorWrapper::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex)
    return CrossProcessCursorWrapper::GetColumnIndex(columnName, columnIndex);
}

ECode CCrossProcessCursorWrapper::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex)
    return CrossProcessCursorWrapper::GetColumnIndexOrThrow(columnName, columnIndex);
}

ECode CCrossProcessCursorWrapper::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnName)
{
    VALIDATE_NOT_NULL(columnName)
    return CrossProcessCursorWrapper::GetColumnName(columnIndex, columnName);
}

ECode CCrossProcessCursorWrapper::GetColumnNames(
    /* [out,callee] */ ArrayOf<String>** columnNames)
{
    VALIDATE_NOT_NULL(columnNames)
    return CrossProcessCursorWrapper::GetColumnNames(columnNames);
}

ECode CCrossProcessCursorWrapper::GetColumnCount(
    /* [out] */ Int32* columnCount)
{
    VALIDATE_NOT_NULL(columnCount)
    return CrossProcessCursorWrapper::GetColumnCount(columnCount);
}

ECode CCrossProcessCursorWrapper::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out,callee] */ ArrayOf<Byte>** blob)
{
    VALIDATE_NOT_NULL(blob)
    return CrossProcessCursorWrapper::GetBlob(columnIndex, blob);
}

ECode CCrossProcessCursorWrapper::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return CrossProcessCursorWrapper::GetString(columnIndex, columnValue);
}

ECode CCrossProcessCursorWrapper::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer* buf)
{
    return CrossProcessCursorWrapper::CopyStringToBuffer(columnIndex, buf);
}

ECode CCrossProcessCursorWrapper::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return CrossProcessCursorWrapper::GetInt16(columnIndex, columnValue);
}

ECode CCrossProcessCursorWrapper::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return CrossProcessCursorWrapper::GetInt32(columnIndex, columnValue);
}

ECode CCrossProcessCursorWrapper::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return CrossProcessCursorWrapper::GetInt64(columnIndex, columnValue);
}

ECode CCrossProcessCursorWrapper::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return CrossProcessCursorWrapper::GetFloat(columnIndex, columnValue);
}

ECode CCrossProcessCursorWrapper::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return CrossProcessCursorWrapper::GetDouble(columnIndex, columnValue);
}

ECode CCrossProcessCursorWrapper::GetType(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return CrossProcessCursorWrapper::GetType(columnIndex, index);
}

ECode CCrossProcessCursorWrapper::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isNull)
{
    VALIDATE_NOT_NULL(isNull)
    return CrossProcessCursorWrapper::IsNull(columnIndex, isNull);
}

ECode CCrossProcessCursorWrapper::Deactivate()
{
    return CrossProcessCursorWrapper::Deactivate();
}

ECode CCrossProcessCursorWrapper::Requery(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CrossProcessCursorWrapper::Requery(succeeded);
}

ECode CCrossProcessCursorWrapper::Close()
{
    return CrossProcessCursorWrapper::Close();
}

ECode CCrossProcessCursorWrapper::IsClosed(
    /* [out] */ Boolean* closed)
{
    VALIDATE_NOT_NULL(closed)
    return CrossProcessCursorWrapper::IsClosed(closed);
}

ECode CCrossProcessCursorWrapper::RegisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return CrossProcessCursorWrapper::RegisterContentObserver(observer);
}

ECode CCrossProcessCursorWrapper::UnregisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return CrossProcessCursorWrapper::UnregisterContentObserver(observer);
}

ECode CCrossProcessCursorWrapper::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return CrossProcessCursorWrapper::RegisterDataSetObserver(observer);
}

ECode CCrossProcessCursorWrapper::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return CrossProcessCursorWrapper::UnregisterDataSetObserver(observer);
}

ECode CCrossProcessCursorWrapper::SetNotificationUri(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri)
{
    return CrossProcessCursorWrapper::SetNotificationUri(cr, uri);
}

ECode CCrossProcessCursorWrapper::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return CrossProcessCursorWrapper::GetWantsAllOnMoveCalls(result);
}

ECode CCrossProcessCursorWrapper::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)
    return CrossProcessCursorWrapper::GetExtras(extras);
}

ECode CCrossProcessCursorWrapper::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** result)
{
    VALIDATE_NOT_NULL(result)
    return CrossProcessCursorWrapper::Respond(extras, result);
}

} //Database
} //Droid
} //Elastos