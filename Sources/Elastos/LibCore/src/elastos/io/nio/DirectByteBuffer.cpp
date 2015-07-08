
#include "DirectByteBuffer.h"
#include "CByteOrderHelper.h"
#include "Math.h"

using Elastos::Core::Math;
using Libcore::IO::ISizeOf;

namespace Elastos {
namespace IO {

// {A2A31377-84A4-30B4-30D9-55F3369DD50F}
extern "C" const InterfaceID EIID_DirectByteBuffer =
    { 0xa2a31377, 0x84a4, 0x30b4, { 0x30, 0xd9, 0x55, 0xf3, 0x36, 0x9d, 0xd5, 0x0f } };

DirectByteBuffer::DirectByteBuffer(
    /* [in] */ Int64 address,
    /* [in] */ Int32 capacity)
{
    // this(MemoryBlock.wrapFromJni(address, capacity), capacity, 0, false, null);
}

DirectByteBuffer::DirectByteBuffer(
    /* [in] */ MemoryBlock* block,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 offset,
    /* [in] */ Boolean isReadOnly,
    /* [in] */ FileChannelMapMode mapMode)
{
    // super(block, capacity, mapMode, block.toLong() + offset);

    // long baseSize = block.getSize();
    // // We're throwing this exception after we passed a bogus value
    // // to the superclass constructor, but it doesn't make any
    // // difference in this case.
    // if (baseSize >= 0 && (capacity + offset) > baseSize) {
    //   throw new IllegalArgumentException("capacity + offset > baseSize");
    // }

    // this.offset = offset;
    // this.isReadOnly = isReadOnly;
}

ECode DirectByteBuffer::Get(
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
    FAIL_RETURN(CheckGetBounds(ISizeOf::LONG, dst->GetLength(), dstOffset, int64Count, &byteCount))
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

ECode DirectByteBuffer::Get(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = mBlock->PeekByte(mOffset + mPosition++);
    return NOERROR;
}

ECode DirectByteBuffer::Get(
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
    Int32 newPosition = mPosition + ISizeOf::LONG;
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
    FAIL_RETURN(CheckIndex(index, ISizeOf::LONG))
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

ECode DirectByteBuffer::Put(
    /* [in] */ Byte b)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // if (position == limit) {
    //   throw new BufferOverflowException();
    // }
    // this.block.pokeByte(offset + position++, value);
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // checkIndex(index);
    // this.block.pokeByte(offset + index, value);
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::Put(
    /* [in] */ ArrayOf<Byte>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 byteCount)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // checkPutBounds(1, src.length, srcOffset, byteCount);
    // this.block.pokeByteArray(offset + position, src, srcOffset, byteCount);
    // position += byteCount;
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::Put(
    /* [in] */ IByteBuffer* src)
{
    return NOERROR;
}

ECode DirectByteBuffer::PutChar(
    /* [in] */ Char32 value)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // int newPosition = position + ISizeOf::CHAR;
    // if (newPosition > limit) {
    //   throw new BufferOverflowException();
    // }
    // this.block.pokeShort(offset + position, (short) value, order);
    // position = newPosition;
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::PutChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // checkIndex(index, ISizeOf::CHAR);
    // this.block.pokeShort(offset + index, (short) value, order);
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::PutDouble(
    /* [in] */ Double value)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // int newPosition = position + ISizeOf::DOUBLE;
    // if (newPosition > limit) {
    //   throw new BufferOverflowException();
    // }
    // this.block.pokeLong(offset + position, Double.doubleToRawLongBits(value), order);
    // position = newPosition;
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::PutDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // checkIndex(index, ISizeOf::DOUBLE);
    // this.block.pokeLong(offset + index, Double.doubleToRawLongBits(value), order);
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::PutFloat(
    /* [in] */ Float value)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // int newPosition = position + ISizeOf::FLOAT;
    // if (newPosition > limit) {
    //   throw new BufferOverflowException();
    // }
    // this.block.pokeInt(offset + position, Float.floatToRawIntBits(value), order);
    // position = newPosition;
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::PutFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // checkIndex(index, ISizeOf::FLOAT);
    // this.block.pokeInt(offset + index, Float.floatToRawIntBits(value), order);
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt16(
    /* [in] */ Int16 value)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // int newPosition = position + ISizeOf::SHORT;
    // if (newPosition > limit) {
    //   throw new BufferOverflowException();
    // }
    // this.block.pokeShort(offset + position, value, order);
    // position = newPosition;
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt16(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // checkIndex(index, ISizeOf::SHORT);
    // this.block.pokeShort(offset + index, value, order);
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt32(
    /* [in] */ Int32 value)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // int newPosition = position + ISizeOf::INT;
    // if (newPosition > limit) {
    //   throw new BufferOverflowException();
    // }
    // this.block.pokeInt(offset + position, value, order);
    // position = newPosition;
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // checkIndex(index, ISizeOf::INT);
    // this.block.pokeInt(offset + index, value, order);
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt64(
    /* [in] */ Int64 value)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // int newPosition = position + ISizeOf::LONG;
    // if (newPosition > limit) {
    //   throw new BufferOverflowException();
    // }
    // this.block.pokeLong(offset + position, value, order);
    // position = newPosition;
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // checkIndex(index, ISizeOf::LONG);
    // this.block.pokeLong(offset + index, value, order);
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::Slice(
    /* [out] */ IByteBuffer** buffer)
{
    // checkNotFreed();
    // return new DirectByteBuffer(block, remaining(), offset + position, isReadOnly, mapMode);
    return NOERROR;
}

ECode DirectByteBuffer::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    // checkNotFreed();
    // return ByteBufferAsCharBuffer.asCharBuffer(this);
    return NOERROR;
}

ECode DirectByteBuffer::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    // checkNotFreed();
    // return ByteBufferAsDoubleBuffer.asDoubleBuffer(this);
    return NOERROR;
}

ECode DirectByteBuffer::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    // checkNotFreed();
    // return ByteBufferAsFloatBuffer.asFloatBuffer(this);
    return NOERROR;
}

ECode DirectByteBuffer::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    // checkNotFreed();
    // return ByteBufferAsShortBuffer.asShortBuffer(this);
    return NOERROR;
}

ECode DirectByteBuffer::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    // checkNotFreed();
    // return ByteBufferAsIntBuffer.asIntBuffer(this);
    return NOERROR;
}

ECode DirectByteBuffer::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    // checkNotFreed();
    // return ByteBufferAsLongBuffer.asLongBuffer(this);
    return NOERROR;
}

ECode DirectByteBuffer::AsReadOnlyBuffer(
    /* [out] */ IByteBuffer** buffer)
{
    // return copy(this, mark, true);
    return NOERROR;
}

ECode DirectByteBuffer::Compact()
{
    // checkIsAccessible();
    // if (isReadOnly) {
    //   throw new ReadOnlyBufferException();
    // }
    // Memory.memmove(this, 0, this, position, remaining());
    // position = limit - position;
    // limit = capacity;
    // mark = UNSET_MARK;
    // return this;
    return NOERROR;
}

ECode DirectByteBuffer::Duplicate(
    /* [out] */ IByteBuffer** buffer)
{
    // return copy(this, mark, isReadOnly);
    return NOERROR;
}

ECode DirectByteBuffer::IsReadOnly(
    /* [out] */ Boolean* value)
{
    // return isReadOnly;
    return NOERROR;
}

ECode DirectByteBuffer::PutChars(
    /* [in] */ ArrayOf<Char32>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    // checkIsAccessible();
    // int byteCount = checkPutBounds(ISizeOf::CHAR, src.length, srcOffset, charCount);
    // this.block.pokeCharArray(offset + position, src, srcOffset, charCount, order.needsSwap);
    // position += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::PutDoubles(
    /* [in] */ ArrayOf<Double>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 doubleCount)
{
    // checkIsAccessible();
    // int byteCount = checkPutBounds(ISizeOf::DOUBLE, src.length, srcOffset, doubleCount);
    // this.block.pokeDoubleArray(offset + position, src, srcOffset, doubleCount, order.needsSwap);
    // position += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::PutFloats(
    /* [in] */ ArrayOf<Float>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 floatCount)
{
    // checkIsAccessible();
    // int byteCount = checkPutBounds(ISizeOf::FLOAT, src.length, srcOffset, floatCount);
    // this.block.pokeFloatArray(offset + position, src, srcOffset, floatCount, order.needsSwap);
    // position += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt32s(
    /* [in] */ ArrayOf<Int32>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 intCount)
{
    // checkIsAccessible();
    // int byteCount = checkPutBounds(ISizeOf::INT, src.length, srcOffset, intCount);
    // this.block.pokeIntArray(offset + position, src, srcOffset, intCount, order.needsSwap);
    // position += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt64s(
    /* [in] */ ArrayOf<Int64>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 longCount)
{
    // checkIsAccessible();
    // int byteCount = checkPutBounds(ISizeOf::LONG, src.length, srcOffset, longCount);
    // this.block.pokeLongArray(offset + position, src, srcOffset, longCount, order.needsSwap);
    // position += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt16s(
    /* [in] */ ArrayOf<Int16>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 shortCount)
{
    // checkIsAccessible();
    // int byteCount = checkPutBounds(ISizeOf::SHORT, src.length, srcOffset, shortCount);
    // this.block.pokeShortArray(offset + position, src, srcOffset, shortCount, order.needsSwap);
    // position += byteCount;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
