
#include "Int32ArrayBuffer.h"
#include "CByteOrderHelper.h"

namespace Elastos {
namespace IO {

Int32ArrayBuffer::Int32ArrayBuffer(
    /* [in] */ ArrayOf<Int32>* array)
    : Int32Buffer(array->GetLength(), 0)
    , mBackingArray(array)
    , mArrayOffset(0)
    , mIsReadOnly(FALSE)
{}

Int32ArrayBuffer::Int32ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int32>* backingArray,
    /* [in] */ Int32 offset,
    /* [in] */ Boolean isReadOnly)
    : Int32Buffer(capacity, 0)
    , mBackingArray(backingArray)
    , mArrayOffset(offset)
    , mIsReadOnly(isReadOnly)
{}

ECode Int32ArrayBuffer::Get(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = (*mBackingArray)[mArrayOffset + mPosition++];
    return NOERROR;
}

ECode Int32ArrayBuffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index))
    *value = (*mBackingArray)[mArrayOffset + index];
    return NOERROR;
}

ECode Int32ArrayBuffer::Get(
    /* [out] */ ArrayOf<Int32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int32Count)
{
    VALIDATE_NOT_NULL(dst);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (int32Count > remaining) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }

    dst->Copy(dstOffset, mBackingArray, mArrayOffset + mPosition, int32Count);
    mPosition += int32Count;
    return NOERROR;
}

ECode Int32ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode Int32ArrayBuffer::GetOrder(
   /* [out] */ ByteOrder* byteOrder)
{
    VALIDATE_NOT_NULL(byteOrder)
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    return helper->GetNativeOrder(byteOrder);
}


ECode Int32ArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    *buffer = (IInt32Buffer*) Copy(this, mMark, TRUE);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode Int32ArrayBuffer::Compact()
{
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    // System.arraycopy(backingArray, mPosition + mArrayOffset, backingArray, mArrayOffset, remaining());
    Int32 reaminvalue = 0;
    GetRemaining(&reaminvalue);
    mBackingArray->Copy(mArrayOffset, mBackingArray, mPosition + mArrayOffset, reaminvalue);
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = UNSET_MARK;
    return NOERROR;
}

ECode Int32ArrayBuffer::Duplicate(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    *buffer = (IInt32Buffer*) Copy(this, mMark, mIsReadOnly);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode Int32ArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    VALIDATE_NOT_NULL(array)

    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    *array = mBackingArray;
    return NOERROR;
}

ECode Int32ArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset)

    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    *offset = mArrayOffset;
    return NOERROR;
}

ECode Int32ArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    VALIDATE_NOT_NULL(hasArray)

    if (mIsReadOnly) {
        *hasArray = FALSE;
    }
    else {
        *hasArray = TRUE;
    }
    return NOERROR;
}

ECode Int32ArrayBuffer::Put(
    /* [in] */ Int32 d)
{
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    (*mBackingArray)[mArrayOffset + mPosition++] = d;
    return NOERROR;
}

ECode Int32ArrayBuffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Int32 d)
{
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    FAIL_RETURN(CheckIndex(index));
    (*mBackingArray)[mArrayOffset + index] = d;
    return NOERROR;
}

ECode Int32ArrayBuffer::Put(
    /* [in] */ const ArrayOf<Int32>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 doubleCount)
{
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    Int32 remainvalue = 0;
    GetRemaining(&remainvalue);
    if (doubleCount > remainvalue) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVERFLOW_EXCEPTION;
    }
    // System.arraycopy(src, srcOffset, backingArray, mArrayOffset + mPosition, doubleCount);
    mBackingArray->Copy(mArrayOffset + mPosition, &src, srcOffset, doubleCount);
    mPosition += doubleCount;
    return NOERROR;
}

ECode Int32ArrayBuffer::Slice(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    Int32 remainvalue = 0;
    GetRemaining(&remainvalue);
    *buffer = (IInt32Buffer*) new Int32ArrayBuffer(remainvalue, mBackingArray, mArrayOffset + mPosition, mIsReadOnly);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode Int32ArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mIsReadOnly;
    return NOERROR;
}

AutoPtr<Int32ArrayBuffer> Int32ArrayBuffer::Copy(
    /* [in] */ Int32ArrayBuffer* other,
    /* [in] */ Int32 mMarkOfOther,
    /* [in] */ Boolean mIsReadOnly)
{
    Int32 capvalue = 0;
    other->GetCapacity(&capvalue);
    AutoPtr<Int32ArrayBuffer> buf = new Int32ArrayBuffer(capvalue, other->mBackingArray, other->mArrayOffset, mIsReadOnly);
    buf->mLimit = other->mLimit;
    other->GetPosition(&buf->mPosition);
    buf->mMark = mMarkOfOther;
    return buf;
}

} // namespace IO
} // namespace Elastos
