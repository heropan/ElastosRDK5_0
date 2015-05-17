
#include "cmdef.h"
#include "DoubleArrayBuffer.h"
#include "CByteOrderHelper.h"

namespace Elastos {
namespace IO {

DoubleArrayBuffer::DoubleArrayBuffer(
    /* [in] */ ArrayOf<Double>* array)
    : DoubleBuffer(array->GetLength())
    , mBackingArray(array)
    , mOffset(0)
{}

DoubleArrayBuffer::DoubleArrayBuffer(
    /* [in] */ Int32 capacity)
    : DoubleBuffer(capacity)
    , mBackingArray(ArrayOf<Double>::Alloc(capacity))
    , mOffset(0)
{}

DoubleArrayBuffer::DoubleArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Double>* backingArray,
    /* [in] */ Int32 offset)
    : DoubleBuffer(capacity)
    , mBackingArray(backingArray)
    , mOffset(offset)
{}

ECode DoubleArrayBuffer::GetDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = (*mBackingArray)[mOffset + mPosition++];
    return NOERROR;
}

ECode DoubleArrayBuffer::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index));
    *value = (*mBackingArray)[mOffset + index];
    return NOERROR;
}

ECode DoubleArrayBuffer::GetDoublesEx(
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
    dst->Copy(dstOffset, mBackingArray, mOffset + mPosition, doubleCount);
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
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    return helper->GetNativeOrder(byteOrder);
}

} // namespace IO
} // namespace Elastos
