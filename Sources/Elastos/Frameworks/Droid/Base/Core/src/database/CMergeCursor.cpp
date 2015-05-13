
#include "database/CMergeCursor.h"

namespace Elastos {
namespace Droid {
namespace Database {

ECode CMergeCursor::constructor(
    /* [in] */ ArrayOf<ICursor*>* cursors)
{
    Init(cursors);
    return NOERROR;
}

PInterface CMergeCursor::Probe(
    /* [in]  */ REIID riid)
{
    return _CMergeCursor::Probe(riid);
}

ECode CMergeCursor::SetNotificationUriEx(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* notifyUri,
    /* [in] */ Int32 userHandle)
{
    return MergeCursor::SetNotificationUriEx(cr, notifyUri, userHandle);
}

ECode CMergeCursor::GetNotificationUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)

    return MergeCursor::GetNotificationUri(uri);
}

ECode CMergeCursor::SetExtras(
    /* [in] */ IBundle* extras)
{
    return MergeCursor::SetExtras(extras);
}

ECode CMergeCursor::OnChange(
    /* [in] */ Boolean selfChange)
{
    return MergeCursor::OnChange(selfChange);
}

ECode CMergeCursor::GetWindow(
    /* [out] */ ICursorWindow** window)
{
    VALIDATE_NOT_NULL(window)

    return MergeCursor::GetWindow(window);
}

ECode CMergeCursor::FillWindow(
    /* [in] */ Int32 pos,
    /* [in] */ ICursorWindow* window)
{
    return MergeCursor::FillWindow(pos, window);
}

ECode CMergeCursor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MergeCursor::OnMove(oldPosition, newPosition, succeeded);
}

ECode CMergeCursor::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)

    return MergeCursor::GetCount(count);
}

ECode CMergeCursor::GetPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position)

    return MergeCursor::GetPosition(position);
}

ECode CMergeCursor::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MergeCursor::Move(offset, succeeded);
}

ECode CMergeCursor::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MergeCursor::MoveToPosition(position, succeeded);
}

ECode CMergeCursor::MoveToFirst(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MergeCursor::MoveToFirst(succeeded);
}

ECode CMergeCursor::MoveToLast(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MergeCursor::MoveToLast(succeeded);
}

ECode CMergeCursor::MoveToNext(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MergeCursor::MoveToNext(succeeded);
}

ECode CMergeCursor::MoveToPrevious(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MergeCursor::MoveToPrevious(succeeded);
}

ECode CMergeCursor::IsFirst(
    /* [out] */ Boolean* isFirst)
{
    VALIDATE_NOT_NULL(isFirst)

    return MergeCursor::IsFirst(isFirst);
}

ECode CMergeCursor::IsLast(
    /* [out] */ Boolean* isLast)
{
    VALIDATE_NOT_NULL(isLast)

    return MergeCursor::IsLast(isLast);
}

ECode CMergeCursor::IsBeforeFirst(
    /* [out] */ Boolean* isBeforeFirst)
{
    VALIDATE_NOT_NULL(isBeforeFirst)

    return MergeCursor::IsBeforeFirst(isBeforeFirst);
}

ECode CMergeCursor::IsAfterLast(
    /* [out] */ Boolean* isAfterLast)
{
    VALIDATE_NOT_NULL(isAfterLast)

    return MergeCursor::IsAfterLast(isAfterLast);
}

ECode CMergeCursor::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex)

    return MergeCursor::GetColumnIndex(columnName, columnIndex);
}

ECode CMergeCursor::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex)

    return MergeCursor::GetColumnIndexOrThrow(columnName, columnIndex);
}

ECode CMergeCursor::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnName)
{
    VALIDATE_NOT_NULL(columnName)

    return MergeCursor::GetColumnName(columnIndex, columnName);
}

ECode CMergeCursor::GetColumnNames(
    /* [out,callee] */ ArrayOf<String>** columnNames)
{
    VALIDATE_NOT_NULL(columnNames)

    return MergeCursor::GetColumnNames(columnNames);
}

ECode CMergeCursor::GetColumnCount(
    /* [out] */ Int32* columnCount)
{
    VALIDATE_NOT_NULL(columnCount)

    return MergeCursor::GetColumnCount(columnCount);
}

ECode CMergeCursor::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out,callee] */ ArrayOf<Byte>** blob)
{
    VALIDATE_NOT_NULL(blob)

    return MergeCursor::GetBlob(columnIndex, blob);
}

ECode CMergeCursor::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)

    return MergeCursor::GetString(columnIndex, columnValue);
}

ECode CMergeCursor::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer* buf)
{
    return MergeCursor::CopyStringToBuffer(columnIndex, buf);
}

ECode CMergeCursor::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)

    return MergeCursor::GetInt16(columnIndex, columnValue);
}

ECode CMergeCursor::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)

    return MergeCursor::GetInt32(columnIndex, columnValue);
}

ECode CMergeCursor::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)

    return MergeCursor::GetInt64(columnIndex, columnValue);
}

ECode CMergeCursor::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)

    return MergeCursor::GetFloat(columnIndex, columnValue);
}

ECode CMergeCursor::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)

    return MergeCursor::GetDouble(columnIndex, columnValue);
}

ECode CMergeCursor::GetType(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)

    return MergeCursor::GetType(columnIndex, index);
}

ECode CMergeCursor::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isNull)
{
    VALIDATE_NOT_NULL(isNull)

    return MergeCursor::IsNull(columnIndex, isNull);
}

ECode CMergeCursor::Deactivate()
{
    return MergeCursor::Deactivate();
}

ECode CMergeCursor::Requery(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MergeCursor::Requery(succeeded);
}

ECode CMergeCursor::Close()
{
    return MergeCursor::Close();
}

ECode CMergeCursor::IsClosed(
    /* [out] */ Boolean* closed)
{
    VALIDATE_NOT_NULL(closed)

    return MergeCursor::IsClosed(closed);
}

ECode CMergeCursor::RegisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return MergeCursor::RegisterContentObserver(observer);
}

ECode CMergeCursor::UnregisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return MergeCursor::UnregisterContentObserver(observer);
}

ECode CMergeCursor::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return MergeCursor::RegisterDataSetObserver(observer);
}

ECode CMergeCursor::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return MergeCursor::UnregisterDataSetObserver(observer);
}

ECode CMergeCursor::SetNotificationUri(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri)
{
    return MergeCursor::SetNotificationUri(cr, uri);
}

ECode CMergeCursor::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return MergeCursor::GetWantsAllOnMoveCalls(result);
}

ECode CMergeCursor::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)

    return MergeCursor::GetExtras(extras);
}

ECode CMergeCursor::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** result)
{
    VALIDATE_NOT_NULL(result)

    return MergeCursor::Respond(extras, result);
}

} //Database
} //Droid
} //Elastos