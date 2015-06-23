
#include "database/CCursorWindow.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Database {

ECode CCursorWindow::constructor()
{
    return NOERROR;
}

ECode CCursorWindow::constructor(
        /* [in] */ const String& name)
{
    return CursorWindow::Init(name);
}

ECode CCursorWindow::constructor(
    /* [in] */ Boolean localWindow)
{
    return CursorWindow::Init(localWindow);
}

ECode CCursorWindow::AcquireReference()
{
    return CursorWindow::AcquireReference();
}

ECode CCursorWindow::ReleaseReference()
{
    return CursorWindow::ReleaseReference();
}

ECode CCursorWindow::ReleaseReferenceFromContainer()
{
    return CursorWindow::ReleaseReferenceFromContainer();
}

ECode CCursorWindow::Close()
{
    return CursorWindow::Close();
}

ECode CCursorWindow::Clear()
{
    return CursorWindow::Clear();
}

ECode CCursorWindow::GetStartPosition(
    /* [out] */ Int32* pos)
{
    VALIDATE_NOT_NULL(pos)
    return CursorWindow::GetStartPosition(pos);
}

ECode CCursorWindow::SetStartPosition(
    /* [in] */ Int32 pos)
{
    return CursorWindow::SetStartPosition(pos);
}

ECode CCursorWindow::GetNumRows(
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    return CursorWindow::GetNumRows(num);
}

ECode CCursorWindow::SetNumColumns(
    /* [in] */ Int32 columnNum,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CursorWindow::SetNumColumns(columnNum, succeeded);
}

ECode CCursorWindow::AllocRow(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CursorWindow::AllocRow(succeeded);
}

ECode CCursorWindow::FreeLastRow()
{
    return CursorWindow::FreeLastRow();
}

ECode CCursorWindow::PutBlob(
    /* [in] */ const ArrayOf<Byte>& value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CursorWindow::PutBlob(value, row, col, succeeded);
}

ECode CCursorWindow::PutString(
    /* [in] */ const String& value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CursorWindow::PutString(value, row, col, succeeded);
}

ECode CCursorWindow::PutInt64(
    /* [in] */ Int64 value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CursorWindow::PutInt64(value, row, col, succeeded);
}

ECode CCursorWindow::PutDouble(
    /* [in] */ Double value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CursorWindow::PutDouble(value, row, col, succeeded);
}

ECode CCursorWindow::PutNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return CursorWindow::PutNull(row, col, succeeded);
}

ECode CCursorWindow::IsNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isNull)
{
    VALIDATE_NOT_NULL(isNull)
    return CursorWindow::IsNull(row, col, isNull);
}

ECode CCursorWindow::GetType(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column,
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type)
    return CursorWindow::GetType(row, column, type);
}

ECode CCursorWindow::GetBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out, callee] */ ArrayOf<Byte>** blob)
{
    VALIDATE_NOT_NULL(blob)
    return CursorWindow::GetBlob(row, col, blob);
}

ECode CCursorWindow::IsBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isBlob)
{
    VALIDATE_NOT_NULL(isBlob)
    return CursorWindow::IsBlob(row, col, isBlob);
}

ECode CCursorWindow::IsInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isInt64)
{
    VALIDATE_NOT_NULL(isInt64)
    return CursorWindow::IsInt64(row, col, isInt64);
}

ECode CCursorWindow::IsFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isFloat)
{
    VALIDATE_NOT_NULL(isFloat)
    return CursorWindow::IsFloat(row, col, isFloat);
}

ECode CCursorWindow::IsString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isString)
{
    VALIDATE_NOT_NULL(isString)
    return CursorWindow::IsString(row, col, isString);
}

ECode CCursorWindow::GetString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    return CursorWindow::GetString(row, col, str);
}

ECode CCursorWindow::CopyStringToBuffer(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [in] */ ICharArrayBuffer* buffer)
{
    return CursorWindow::CopyStringToBuffer(row, col, buffer);
}

ECode CCursorWindow::GetInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    return CursorWindow::GetInt64(row, col, value);
}

ECode CCursorWindow::GetDouble(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value)
    return CursorWindow::GetDouble(row, col, value);
}

ECode CCursorWindow::GetInt16(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value)
    return CursorWindow::GetInt16(row, col, value);
}

ECode CCursorWindow::GetInt32(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return CursorWindow::GetInt32(row, col, value);
}

ECode CCursorWindow::GetFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    return CursorWindow::GetFloat(row, col, value);
}

ECode CCursorWindow::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    return CursorWindow::DescribeContents(result);
}

ECode CCursorWindow::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mStartPos);
    mWindowPtr = NativeCreateFromParcel(source);
    if (mWindowPtr == 0) {
        Slogger::E(STATS_TAG, "Cursor window could not be created from binder.");
        return E_CURSOR_WINDOW_ALLOCATION_EXCEPTION;
        // throw new CursorWindowAllocationException("Cursor window could not be "
        //         + "created from binder.");
    }
    mName = NativeGetName(mWindowPtr);
    // mCloseGuard.open("close");
    return NOERROR;
}

ECode CCursorWindow::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return CursorWindow::WriteToParcel(dest);
}

ECode CCursorWindow::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    return CursorWindow::ToString(str);
}

} //Database
} //Droid
} //Elastos
