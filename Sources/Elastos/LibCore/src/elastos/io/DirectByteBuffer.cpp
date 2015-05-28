
#include "coredef.h"
#include "DirectByteBuffer.h"
#include "CByteOrderHelper.h"
#include <elastos/Math.h>

using Elastos::Core::Math;

namespace Elastos {
namespace IO {

// {A2A31377-84A4-30B4-30D9-55F3369DD50F}
extern "C" const InterfaceID EIID_DirectByteBuffer =
    { 0xa2a31377, 0x84a4, 0x30b4, { 0x30, 0xd9, 0x55, 0xf3, 0x36, 0x9d, 0xd5, 0x0f } };

DirectByteBuffer::DirectByteBuffer(
    /* [in] */ MemoryBlock* block,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 offset)
    : BaseByteBuffer(capacity, block)
{
    Int64 baseSize = block->GetSize();
    if (baseSize >= 0 && (capacity + offset) > baseSize) {
        // throw new IllegalArgumentException("capacity + offset > baseSize");
        assert(0);
    }

    mOffset = offset;
    mEffectiveDirectAddress = block->ToInt32() + offset;
}

ECode DirectByteBuffer::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 byteCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 bounds;
    FAIL_RETURN(CheckGetBounds(1, dst->GetLength(), dstOffset, byteCount, &bounds));
    mBlock->PeekByteArray(mOffset + mPosition, dst, dstOffset, byteCount);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::GetChars(
    /* [out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Char32), dst->GetLength(), dstOffset, charCount, &byteCount));
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    mBlock->PeekCharArray(mOffset + mPosition, dst, dstOffset, charCount, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 doubleCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Double), dst->GetLength(), dstOffset, doubleCount, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    mBlock->PeekDoubleArray(mOffset + mPosition, dst, dstOffset, doubleCount, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::GetFloats(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 floatCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Float), dst->GetLength(), dstOffset, floatCount, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    mBlock->PeekFloatArray(mOffset + mPosition, dst, dstOffset, floatCount, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::GetInt32s(
    /* [out] */ ArrayOf<Int32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int32Count)
{
    VALIDATE_NOT_NULL(dst);
    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Int32), dst->GetLength(), dstOffset, int32Count, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    mBlock->PeekInt32Array(mOffset + mPosition, dst, dstOffset, int32Count, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int64Count)
{
    VALIDATE_NOT_NULL(dst);
    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Int64), dst->GetLength(), dstOffset, int64Count, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    mBlock->PeekInt64Array(mOffset + mPosition, dst, dstOffset, int64Count, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int16Count)
{
    VALIDATE_NOT_NULL(dst);
    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Int16), dst->GetLength(), dstOffset, int16Count, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    mBlock->PeekInt16Array(mOffset + mPosition, dst, dstOffset, int16Count, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::GetByte(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = mBlock->PeekByte(mOffset + mPosition++);
    return NOERROR;
}

ECode DirectByteBuffer::GetByte(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index))
    *value = mBlock->PeekByte(mOffset + index);
    return NOERROR;
}

ECode DirectByteBuffer::GetChar(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value)
    Int32 newPosition = mPosition + sizeof(Char32);
    if (newPosition > mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = (Char32) mBlock->PeekInt32(mOffset + mPosition, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value)
    FAIL_RETURN(CheckIndex(index, sizeof(Char32)))
    *value = (Char32) mBlock->PeekInt32(mOffset + index, mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::GetDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 newPosition = mPosition + sizeof(Double);
    if (newPosition > mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = Elastos::Core::Math::Int64BitsToDouble(mBlock->PeekInt64(mOffset + mPosition, mOrder));
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::GetDouble(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index, sizeof(Double)))
    *value = Elastos::Core::Math::Int64BitsToDouble(mBlock->PeekInt64(mOffset + index, mOrder));
    return NOERROR;
}

ECode DirectByteBuffer::GetFloat(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 newPosition = mPosition + sizeof(Float);
    if (newPosition > mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = Elastos::Core::Math::Int32BitsToFloat(mBlock->PeekInt32(mOffset + mPosition, mOrder));
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::GetFloat(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index, sizeof(Float)))
    *value = Elastos::Core::Math::Int32BitsToFloat(mBlock->PeekInt32(mOffset + index, mOrder));
    return NOERROR;
}

ECode DirectByteBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 newPosition = mPosition + sizeof(Int32);
    if (newPosition > mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = mBlock->PeekInt32(mOffset + mPosition, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::GetInt32(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index, sizeof(Int32)))
    *value = mBlock->PeekInt32(mOffset + index, mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 newPosition = mPosition + sizeof(Int64);
    if (newPosition > mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = mBlock->PeekInt64(mOffset + mPosition, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::GetInt64(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index, sizeof(Int64)))
    *value = mBlock->PeekInt64(mOffset + index, mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 newPosition = mPosition + sizeof(Int16);
    if (newPosition > mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = mBlock->PeekInt16(mOffset + mPosition, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::GetInt16(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index, sizeof(Int16)))
    *value = mBlock->PeekInt16(mOffset + index, mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = TRUE;
    return NOERROR;
}

ECode DirectByteBuffer::Free()
{
    mBlock->Free();
    return NOERROR;
}

ECode DirectByteBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    VALIDATE_NOT_NULL(array);
    AutoPtr< ArrayOf<Byte> > arr = mBlock->GetArray();
    if (arr == NULL) {
        // throw new UnsupportedOperationException();
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }
    *array = arr;
    REFCOUNT_ADD(*array);
    return NOERROR;
}

ECode DirectByteBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    AutoPtr< ArrayOf<Byte> > array;
    FAIL_RETURN(ProtectedArray((ArrayOf<Byte>**)&array)); // Throw if we don't have an array.
    *offset = mOffset;
    return NOERROR;
}

ECode DirectByteBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    VALIDATE_NOT_NULL(hasArray);
    *hasArray = mBlock->GetArray() != NULL;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
