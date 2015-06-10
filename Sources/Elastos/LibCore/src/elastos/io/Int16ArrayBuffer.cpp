
#include "Int16ArrayBuffer.h"
#include "CByteOrderHelper.h"

namespace Elastos {
namespace IO {

Int16ArrayBuffer::Int16ArrayBuffer(
    /* [in] */ ArrayOf<Int16>* array)
    : Int16Buffer(array->GetLength())
    , mBackingArray(array)
    , mOffset(0)
{}

Int16ArrayBuffer::Int16ArrayBuffer(
    /* [in] */ Int32 capacity)
    : Int16Buffer(capacity)
    , mBackingArray(ArrayOf<Int16>::Alloc(capacity))
    , mOffset(0)
{}

Int16ArrayBuffer::Int16ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int16>* backingArray,
    /* [in] */ Int32 offset)
    : Int16Buffer(capacity)
    , mBackingArray(backingArray)
    , mOffset(offset)
{}

ECode Int16ArrayBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = (*mBackingArray)[mOffset + mPosition++];
    return NOERROR;
}

ECode Int16ArrayBuffer::GetInt16(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index))
    *value = (*mBackingArray)[mOffset + index];
    return NOERROR;
}

ECode Int16ArrayBuffer::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int16Count)
{
    VALIDATE_NOT_NULL(dst);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (int16Count > remaining) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }

    dst->Copy(dstOffset, mBackingArray, mOffset + mPosition, int16Count);
    mPosition += int16Count;
    return NOERROR;
}

ECode Int16ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode Int16ArrayBuffer::GetOrder(
   /* [out] */ ByteOrder* byteOrder)
{
    VALIDATE_NOT_NULL(byteOrder)
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    return helper->GetNativeOrder(byteOrder);
}

} // namespace IO
} // namespace Elastos
