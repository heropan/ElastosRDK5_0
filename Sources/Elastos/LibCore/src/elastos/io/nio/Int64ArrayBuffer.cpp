
#include "Int64ArrayBuffer.h"
#include "CByteOrderHelper.h"

namespace Elastos {
namespace IO {

Int64ArrayBuffer::Int64ArrayBuffer(
    /* [in] */ ArrayOf<Int64>* array)
    : Int64Buffer(array->GetLength(), 0)
    , mBackingArray(array)
    , mOffset(0)
    , mIsReadOnly(FALSE)
{}

Int64ArrayBuffer::Int64ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int64>* backingArray,
    /* [in] */ Int32 offset,
    /* [in] */ Boolean isReadOnly)
    : Int64Buffer(capacity, 0)
    , mBackingArray(backingArray)
    , mOffset(offset)
    , mIsReadOnly(isReadOnly)
{}

ECode Int64ArrayBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = (*mBackingArray)[mOffset + mPosition++];
    return NOERROR;
}

ECode Int64ArrayBuffer::GetInt64(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index))
    *value = (*mBackingArray)[mOffset + index];
    return NOERROR;
}

ECode Int64ArrayBuffer::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int64Count)
{
    VALIDATE_NOT_NULL(dst);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (int64Count > remaining) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }

    dst->Copy(dstOffset, mBackingArray, mOffset + mPosition, int64Count);
    mPosition += int64Count;
    return NOERROR;
}

ECode Int64ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode Int64ArrayBuffer::GetOrder(
   /* [out] */ ByteOrder* byteOrder)
{
    VALIDATE_NOT_NULL(byteOrder)
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    return helper->GetNativeOrder(byteOrder);
}

} // namespace IO
} // namespace Elastos
