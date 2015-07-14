
#include "DoubleArrayBuffer.h"
#include "CByteOrderHelper.h"

namespace Elastos {
namespace IO {

DoubleArrayBuffer::DoubleArrayBuffer(
    /* [in] */ ArrayOf<Double>* array)
    : DoubleBuffer(array->GetLength(), 0)
    , mBackingArray(array)
    , mArrayOffset(0)
{}

DoubleArrayBuffer::DoubleArrayBuffer(
    /* [in] */ Int32 mCapacity,
    /* [in] */ ArrayOf<Double>* backingArray,
    /* [in] */ Int32 offset,
    /* [in] */ Boolean mIsReadOnly)
    : DoubleBuffer(mCapacity, 0)
    , mBackingArray(backingArray)
    , mArrayOffset(offset)
{}

ECode DoubleArrayBuffer::Get(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = (*mBackingArray)[mArrayOffset + mPosition++];
    return NOERROR;
}

ECode DoubleArrayBuffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index));
    *value = (*mBackingArray)[mArrayOffset + index];
    return NOERROR;
}

ECode DoubleArrayBuffer::Get(
    /* [out] */ ArrayOf<Double>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 doubleCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (doubleCount > remaining) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    dst->Copy(dstOffset, mBackingArray, mArrayOffset + mPosition, doubleCount);
    mPosition += doubleCount;
    return NOERROR;
}

ECode DoubleArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode DoubleArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    VALIDATE_NOT_NULL(byteOrder)

    *byteOrder = CByteOrderHelper::_GetNativeOrder();
    return NOERROR;
}

ECode DoubleArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    *buffer = (IDoubleBuffer*) Copy(this, mMark, TRUE);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DoubleArrayBuffer::Compact()
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

ECode DoubleArrayBuffer::Duplicate(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    *buffer = (IDoubleBuffer*) Copy(this, mMark, mIsReadOnly);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DoubleArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    VALIDATE_NOT_NULL(array)

    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    *array = mBackingArray;
    return NOERROR;
}

ECode DoubleArrayBuffer::ProtectedArrayOffset(
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

ECode DoubleArrayBuffer::ProtectedHasArray(
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

ECode DoubleArrayBuffer::Put(
    /* [in] */ Double d)
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

ECode DoubleArrayBuffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Double d)
{
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    FAIL_RETURN(CheckIndex(index));
    (*mBackingArray)[mArrayOffset + index] = d;
    return NOERROR;
}

ECode DoubleArrayBuffer::Put(
    /* [in] */ const ArrayOf<Double>& src,
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

ECode DoubleArrayBuffer::Slice(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    Int32 remainvalue = 0;
    GetRemaining(&remainvalue);
    *buffer = (IDoubleBuffer*) new DoubleArrayBuffer(remainvalue, mBackingArray, mArrayOffset + mPosition, mIsReadOnly);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DoubleArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mIsReadOnly;
    return NOERROR;
}

AutoPtr<DoubleArrayBuffer> DoubleArrayBuffer::Copy(
    /* [in] */ DoubleArrayBuffer* other,
    /* [in] */ Int32 mMarkOfOther,
    /* [in] */ Boolean mIsReadOnly)
{
    Int32 capvalue = 0;
    other->GetCapacity(&capvalue);
    AutoPtr<DoubleArrayBuffer> buf = new DoubleArrayBuffer(capvalue, other->mBackingArray, other->mArrayOffset, mIsReadOnly);
    buf->mLimit = other->mLimit;
    other->GetPosition(&buf->mPosition);
    buf->mMark = mMarkOfOther;
    return buf;
}

} // namespace IO
} // namespace Elastos
