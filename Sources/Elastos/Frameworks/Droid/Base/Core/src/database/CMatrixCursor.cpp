
#include "database/CMatrixCursor.h"

namespace Elastos {
namespace Droid {
namespace Database {

PInterface CMatrixCursor::Probe(
    /* [in]  */ REIID riid)
{
    return _CMatrixCursor::Probe(riid);
}

ECode CMatrixCursor::NewRow(
    /* [out] */ IRowBuilder** builder)
{
    VALIDATE_NOT_NULL(builder)

    return MatrixCursor::NewRow(builder);
}

ECode CMatrixCursor::AddRow(
    /* [in] */ const ArrayOf<IInterface*>& columnValues)
{
    return MatrixCursor::AddRow(columnValues);
}

ECode CMatrixCursor::AddRow(
    /* [in] */ IObjectContainer* columnValues)
{
    return MatrixCursor::AddRow(columnValues);
}

ECode CMatrixCursor::SetNotificationUri(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* notifyUri,
    /* [in] */ Int32 userHandle)
{
    return MatrixCursor::SetNotificationUri(cr, notifyUri, userHandle);
}

ECode CMatrixCursor::GetNotificationUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)

    return MatrixCursor::GetNotificationUri(uri);
}

ECode CMatrixCursor::SetExtras(
    /* [in] */ IBundle* extras)
{
    return MatrixCursor::SetExtras(extras);
}

ECode CMatrixCursor::OnChange(
    /* [in] */ Boolean selfChange)
{
    return MatrixCursor::OnChange(selfChange);
}

ECode CMatrixCursor::GetWindow(
    /* [out] */ ICursorWindow** window)
{
    VALIDATE_NOT_NULL(window)

    return MatrixCursor::GetWindow(window);
}

ECode CMatrixCursor::FillWindow(
    /* [in] */ Int32 pos,
    /* [in] */ ICursorWindow* window)
{
    return MatrixCursor::FillWindow(pos, window);
}

ECode CMatrixCursor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MatrixCursor::OnMove(oldPosition, newPosition, succeeded);
}

ECode CMatrixCursor::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)

    return MatrixCursor::GetCount(count);
}

ECode CMatrixCursor::GetPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position)

    return MatrixCursor::GetPosition(position);
}

ECode CMatrixCursor::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MatrixCursor::Move(offset, succeeded);
}

ECode CMatrixCursor::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MatrixCursor::MoveToPosition(position, succeeded);
}

ECode CMatrixCursor::MoveToFirst(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MatrixCursor::MoveToFirst(succeeded);
}

ECode CMatrixCursor::MoveToLast(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MatrixCursor::MoveToLast(succeeded);
}

ECode CMatrixCursor::MoveToNext(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MatrixCursor::MoveToNext(succeeded);
}

ECode CMatrixCursor::MoveToPrevious(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MatrixCursor::MoveToPrevious(succeeded);
}

ECode CMatrixCursor::IsFirst(
    /* [out] */ Boolean* isFirst)
{
    VALIDATE_NOT_NULL(isFirst)

    return MatrixCursor::IsFirst(isFirst);
}

ECode CMatrixCursor::IsLast(
    /* [out] */ Boolean* isLast)
{
    VALIDATE_NOT_NULL(isLast)

    return MatrixCursor::IsLast(isLast);
}

ECode CMatrixCursor::IsBeforeFirst(
    /* [out] */ Boolean* isBeforeFirst)
{
    VALIDATE_NOT_NULL(isBeforeFirst)

    return MatrixCursor::IsBeforeFirst(isBeforeFirst);
}

ECode CMatrixCursor::IsAfterLast(
    /* [out] */ Boolean* isAfterLast)
{
    VALIDATE_NOT_NULL(isAfterLast)

    return MatrixCursor::IsAfterLast(isAfterLast);
}

ECode CMatrixCursor::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex)

    return MatrixCursor::GetColumnIndex(columnName, columnIndex);
}

ECode CMatrixCursor::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex)

    return MatrixCursor::GetColumnIndexOrThrow(columnName, columnIndex);
}

ECode CMatrixCursor::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnName)
{
    VALIDATE_NOT_NULL(columnName)

    return MatrixCursor::GetColumnName(columnIndex, columnName);
}

ECode CMatrixCursor::GetColumnNames(
    /* [out,callee] */ ArrayOf<String>** columnNames)
{
    VALIDATE_NOT_NULL(columnNames)

    return MatrixCursor::GetColumnNames(columnNames);
}

ECode CMatrixCursor::GetColumnCount(
    /* [out] */ Int32* columnCount)
{
    VALIDATE_NOT_NULL(columnCount)

    return MatrixCursor::GetColumnCount(columnCount);
}

ECode CMatrixCursor::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out,callee] */ ArrayOf<Byte>** blob)
{
    VALIDATE_NOT_NULL(blob)

    return MatrixCursor::GetBlob(columnIndex, blob);
}

ECode CMatrixCursor::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)

    return MatrixCursor::GetString(columnIndex, columnValue);
}

ECode CMatrixCursor::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer* buf)
{
    return MatrixCursor::CopyStringToBuffer(columnIndex, buf);
}

ECode CMatrixCursor::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)

    return MatrixCursor::GetInt16(columnIndex, columnValue);
}

ECode CMatrixCursor::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)

    return MatrixCursor::GetInt32(columnIndex, columnValue);
}

ECode CMatrixCursor::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)

    return MatrixCursor::GetInt64(columnIndex, columnValue);
}

ECode CMatrixCursor::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)

    return MatrixCursor::GetFloat(columnIndex, columnValue);
}

ECode CMatrixCursor::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double* columnValue)
{
    VALIDATE_NOT_NULL(columnValue)

    return MatrixCursor::GetDouble(columnIndex, columnValue);
}

ECode CMatrixCursor::GetType(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    return MatrixCursor::GetType(columnIndex, index);
}

ECode CMatrixCursor::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isNull)
{
    VALIDATE_NOT_NULL(isNull)

    return MatrixCursor::IsNull(columnIndex, isNull);
}

ECode CMatrixCursor::Deactivate()
{
    return MatrixCursor::Deactivate();
}

ECode CMatrixCursor::Requery(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)

    return MatrixCursor::Requery(succeeded);
}

ECode CMatrixCursor::Close()
{
    return MatrixCursor::Close();
}

ECode CMatrixCursor::IsClosed(
    /* [out] */ Boolean* closed)
{
    VALIDATE_NOT_NULL(closed)

    return MatrixCursor::IsClosed(closed);
}

ECode CMatrixCursor::RegisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return MatrixCursor::RegisterContentObserver(observer);
}

ECode CMatrixCursor::UnregisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return MatrixCursor::UnregisterContentObserver(observer);
}

ECode CMatrixCursor::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return MatrixCursor::RegisterDataSetObserver(observer);
}

ECode CMatrixCursor::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return MatrixCursor::UnregisterDataSetObserver(observer);
}

ECode CMatrixCursor::SetNotificationUri(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri)
{
    return MatrixCursor::SetNotificationUri(cr, uri);
}

ECode CMatrixCursor::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return MatrixCursor::GetWantsAllOnMoveCalls(result);
}

ECode CMatrixCursor::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)

    return MatrixCursor::GetExtras(extras);
}

ECode CMatrixCursor::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** result)
{
    VALIDATE_NOT_NULL(result)

    return MatrixCursor::Respond(extras, result);
}

ECode CMatrixCursor::constructor(
    /* [in] */ ArrayOf<String>* columnNames,
    /* [in] */ Int32 initialCapacity)
{
    Init(columnNames, initialCapacity);
    return NOERROR;
}

ECode CMatrixCursor::constructor(
    /* [in] */ ArrayOf<String>* columnNames)
{
    Init(columnNames);
    return NOERROR;
}

} //Database
} //Droid
} //Elastos