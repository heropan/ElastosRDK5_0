#include "database/CCursorWrapper.h"

namespace Elastos {
namespace Droid {
namespace Database {

ECode CCursorWrapper::constructor(
    /* [in] */ ICursor* cursor)
{
    CursorWrapper::Init(cursor);
    return NOERROR;
}

ECode CCursorWrapper::GetWrappedCursor(
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor)
    return CursorWrapper::GetWrappedCursor(cursor);
}

ECode CCursorWrapper::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    return CursorWrapper::GetCount(count);
}

ECode CCursorWrapper::GetPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position)
    return CursorWrapper::GetPosition(position);
}

ECode CCursorWrapper::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CursorWrapper::Move(offset, succeeded);
}

ECode CCursorWrapper::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CursorWrapper::MoveToPosition(position, succeeded);
}

ECode CCursorWrapper::MoveToFirst(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CursorWrapper::MoveToFirst(succeeded);
}

ECode CCursorWrapper::MoveToLast(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::MoveToLast(succeeded);
}

ECode CCursorWrapper::MoveToNext(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CursorWrapper::MoveToNext(succeeded);
}

ECode CCursorWrapper::MoveToPrevious(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CursorWrapper::MoveToPrevious(succeeded);
}

ECode CCursorWrapper::IsFirst(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return CursorWrapper::IsFirst(result);
}

ECode CCursorWrapper::IsLast(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return CursorWrapper::IsLast(result);
}

ECode CCursorWrapper::IsBeforeFirst(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return CursorWrapper::IsBeforeFirst(result);
}

ECode CCursorWrapper::IsAfterLast(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return CursorWrapper::IsAfterLast(result);
}

ECode CCursorWrapper::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex)
    return CursorWrapper::GetColumnIndex(columnName, columnIndex);
}

ECode CCursorWrapper::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex)
    return CursorWrapper::GetColumnIndexOrThrow(columnName, columnIndex);
}

ECode CCursorWrapper::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnName)
{
    VALIDATE_NOT_NULL(columnName)
    return CursorWrapper::GetColumnName(columnIndex, columnName);
}

ECode CCursorWrapper::GetColumnNames(
    /* [out, callee] */ ArrayOf<String>** columnNames)
{
    VALIDATE_NOT_NULL(columnNames)
    return CursorWrapper::GetColumnNames(columnNames);
}

ECode CCursorWrapper::GetColumnCount(
    /* [out] */ Int32* columnCount)
{
    VALIDATE_NOT_NULL(columnCount)
    return CursorWrapper::GetColumnCount(columnCount);
}

ECode CCursorWrapper::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out, callee] */ ArrayOf<Byte>** blob)
{
    VALIDATE_NOT_NULL(blob)
    return CursorWrapper::GetBlob(columnIndex, blob);
}

ECode CCursorWrapper::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnValue)
{
    VALIDATE_NOT_NULL(columnValue);

    return CursorWrapper::GetString(columnIndex, columnValue);
}

ECode CCursorWrapper::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer* buffer)
{
    return CursorWrapper::CopyStringToBuffer(columnIndex, buffer);
}

ECode CCursorWrapper::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return CursorWrapper::GetInt16(columnIndex, columnValue);
}

ECode CCursorWrapper::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return CursorWrapper::GetInt32(columnIndex, columnValue);
}

ECode CCursorWrapper::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return CursorWrapper::GetInt64(columnIndex, columnValue);
}

ECode CCursorWrapper::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return CursorWrapper::GetFloat(columnIndex, columnValue);
}

ECode CCursorWrapper::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)
    return CursorWrapper::GetDouble(columnIndex, columnValue);
}

ECode CCursorWrapper::GetType(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return CursorWrapper::GetType(columnIndex, index);
}

ECode CCursorWrapper::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return CursorWrapper::IsNull(columnIndex, result);
}

ECode CCursorWrapper::Deactivate()
{
    return CursorWrapper::Deactivate();
}

ECode CCursorWrapper::Requery(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CursorWrapper::Requery(succeeded);
}

ECode CCursorWrapper::Close()
{
    return CursorWrapper::Close();
}

ECode CCursorWrapper::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    VALIDATE_NOT_NULL(isClosed)
    return CursorWrapper::IsClosed(isClosed);
}

ECode CCursorWrapper::RegisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return CursorWrapper::RegisterContentObserver(observer);
}

ECode CCursorWrapper::UnregisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return CursorWrapper::UnregisterContentObserver(observer);
}

ECode CCursorWrapper::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return CursorWrapper::RegisterDataSetObserver(observer);
}

ECode CCursorWrapper::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return CursorWrapper::UnregisterDataSetObserver(observer);
}

ECode CCursorWrapper::SetNotificationUri(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri)
{
    return CursorWrapper::SetNotificationUri(cr, uri);
}

ECode CCursorWrapper::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return CursorWrapper::GetWantsAllOnMoveCalls(result);
}

ECode CCursorWrapper::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)
    return CursorWrapper::GetExtras(extras);
}

ECode CCursorWrapper::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** result)
{
    VALIDATE_NOT_NULL(result)
    return CursorWrapper::Respond(extras, result);
}

} //Database
} //Droid
} //Elastos