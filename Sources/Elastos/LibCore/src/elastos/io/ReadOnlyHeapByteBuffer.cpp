
#include "coredef.h"
#include "ReadOnlyHeapByteBuffer.h"
#include "elastos/core/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

ReadOnlyHeapByteBuffer::ReadOnlyHeapByteBuffer(
    /* [in] */ ArrayOf<Byte>* backingArray,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 arrayOffset)
    : HeapByteBuffer(backingArray, capacity, arrayOffset)
{}

AutoPtr<ReadOnlyHeapByteBuffer> ReadOnlyHeapByteBuffer::Copy(
    /* [in] */ HeapByteBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    AutoPtr<ReadOnlyHeapByteBuffer> buf = new ReadOnlyHeapByteBuffer(
            other->mBackingArray, other->mCapacity, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

CAR_INTERFACE_IMPL(ReadOnlyHeapByteBuffer, Object, IByteBuffer)

ECode ReadOnlyHeapByteBuffer::GetPrimitiveArray(
    /* [out] */ Handle32* arrayHandle)
{
    mArrayTemp = NULL;
    GetArray((ArrayOf<Byte>**)&mArrayTemp);
    if (mArrayTemp == NULL)
    {
        *arrayHandle = 0;
        return NOERROR;
    }
    Byte* primitiveArray = mArrayTemp->GetPayload();
    *arrayHandle = reinterpret_cast<Handle32>(primitiveArray);
    return NOERROR;
}

ECode ReadOnlyHeapByteBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return HeapByteBuffer::GetArray(array);
}

ECode ReadOnlyHeapByteBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return HeapByteBuffer::GetArrayOffset(offset);
}

ECode ReadOnlyHeapByteBuffer::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return HeapByteBuffer::AsCharBuffer(buffer);
}

ECode ReadOnlyHeapByteBuffer::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    return HeapByteBuffer::AsDoubleBuffer(buffer);
}

ECode ReadOnlyHeapByteBuffer::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    return HeapByteBuffer::AsFloatBuffer(buffer);
}

ECode ReadOnlyHeapByteBuffer::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    return HeapByteBuffer::AsInt16Buffer(buffer);
}

ECode ReadOnlyHeapByteBuffer::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    return HeapByteBuffer::AsInt32Buffer(buffer);
}

ECode ReadOnlyHeapByteBuffer::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    return HeapByteBuffer::AsInt64Buffer(buffer);
}

ECode ReadOnlyHeapByteBuffer::AsReadOnlyBuffer(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    *buffer = (IByteBuffer*)Copy((HeapByteBuffer*)this, mMark);
    REFCOUNT_ADD(*buffer);
    return NOERROR;
}

ECode ReadOnlyHeapByteBuffer::Compact()
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::CompareTo(
    /* [in] */ IByteBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return HeapByteBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyHeapByteBuffer::Duplicate(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<ReadOnlyHeapByteBuffer> buf = Copy((HeapByteBuffer*)this, mMark);
    *buffer = (IByteBuffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadOnlyHeapByteBuffer::Get(
    /* [out] */ Byte* value)
{
    return HeapByteBuffer::Get(value);
}

ECode ReadOnlyHeapByteBuffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    return HeapByteBuffer::Get(index, value);
}

ECode ReadOnlyHeapByteBuffer::Get(
    /* [out] */ ArrayOf<Byte>* dst)
{
    return HeapByteBuffer::Get(dst, 0, dst->GetLength());
}

ECode ReadOnlyHeapByteBuffer::Get(
    /* [out] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    return HeapByteBuffer::Get(dst, off, len);
}

ECode ReadOnlyHeapByteBuffer::GetChar(
    /* [out] */ Char32* value)
{
    return HeapByteBuffer::GetChar(value);
}

ECode ReadOnlyHeapByteBuffer::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return HeapByteBuffer::GetChar(index, value);
}

ECode ReadOnlyHeapByteBuffer::GetDouble(
    /* [out] */ Double* value)
{
    return HeapByteBuffer::GetDouble(value);
}

ECode ReadOnlyHeapByteBuffer::GetDouble(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return HeapByteBuffer::GetDouble(index, value);
}

ECode ReadOnlyHeapByteBuffer::GetFloat(
    /* [out] */ Float* value)
{
    return HeapByteBuffer::GetFloat(value);
}

ECode ReadOnlyHeapByteBuffer::GetFloat(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return HeapByteBuffer::GetFloat(index, value);
}

ECode ReadOnlyHeapByteBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    return HeapByteBuffer::GetInt32(value);
}

ECode ReadOnlyHeapByteBuffer::GetInt32(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return HeapByteBuffer::GetInt32(index, value);
}

ECode ReadOnlyHeapByteBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    return HeapByteBuffer::GetInt64(value);
}

ECode ReadOnlyHeapByteBuffer::GetInt64(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return HeapByteBuffer::GetInt64(index, value);
}

ECode ReadOnlyHeapByteBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    return HeapByteBuffer::GetInt16(value);
}

ECode ReadOnlyHeapByteBuffer::GetInt16(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return HeapByteBuffer::GetInt16(index, value);
}

ECode ReadOnlyHeapByteBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    return HeapByteBuffer::GetOrder(byteOrder);
}

ECode ReadOnlyHeapByteBuffer::SetOrder(
    /* [in] */ ByteOrder byteOrder)
{
    return HeapByteBuffer::SetOrder(byteOrder);
}

ECode ReadOnlyHeapByteBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    VALIDATE_NOT_NULL(hasArray);
    *hasArray = FALSE;
    return NOERROR;
}

ECode ReadOnlyHeapByteBuffer::Put(
    /* [in] */ Byte b)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::Put(
    /* [in] */ ArrayOf<Byte>* src)
{
    return HeapByteBuffer::Put(src);
}

ECode ReadOnlyHeapByteBuffer::Put(
    /* [in] */ ArrayOf<Byte>* src,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::PutByteBuffer(
    /* [in] */ IByteBuffer* src)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::PutChar(
    /* [in] */ Char32 value)
{
    return HeapByteBuffer::PutChar(value);
}

ECode ReadOnlyHeapByteBuffer::PutChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    return HeapByteBuffer::PutChar(index, value);
}

ECode ReadOnlyHeapByteBuffer::PutDouble(
    /* [in] */ Double value)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::PutDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::PutFloat(
    /* [in] */ Float value)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::PutFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::PutInt16(
    /* [in] */ Int16 value)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::PutInt16(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::PutInt32(
    /* [in] */ Int32 value)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::PutInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::PutInt64(
    /* [in] */ Int64 value)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::PutInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyHeapByteBuffer::Slice(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    *buffer = (IByteBuffer*)new ReadOnlyHeapByteBuffer(mBackingArray,
            remaining, mOffset + mPosition);
    REFCOUNT_ADD(*buffer);
    return NOERROR;
}

ECode ReadOnlyHeapByteBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return HeapByteBuffer::GetCapacity(cap);
}

ECode ReadOnlyHeapByteBuffer::Clear()
{
    return HeapByteBuffer::Clear();
}

ECode ReadOnlyHeapByteBuffer::Flip()
{
    return HeapByteBuffer::Flip();
}

ECode ReadOnlyHeapByteBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return HeapByteBuffer::HasArray(hasArray);
}

ECode ReadOnlyHeapByteBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return HeapByteBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyHeapByteBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return HeapByteBuffer::IsDirect(isDirect);
}

ECode ReadOnlyHeapByteBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

ECode ReadOnlyHeapByteBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return HeapByteBuffer::GetLimit(limit);
}

ECode ReadOnlyHeapByteBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return HeapByteBuffer::SetLimit(newLimit);
}

ECode ReadOnlyHeapByteBuffer::Mark()
{
    return HeapByteBuffer::Mark();
}

ECode ReadOnlyHeapByteBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return HeapByteBuffer::GetPosition(pos);
}

ECode ReadOnlyHeapByteBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return HeapByteBuffer::SetPosition(newPosition);
}

ECode ReadOnlyHeapByteBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return HeapByteBuffer::GetRemaining(remaining);
}

ECode ReadOnlyHeapByteBuffer::Reset()
{
    return HeapByteBuffer::Reset();
}

ECode ReadOnlyHeapByteBuffer::Rewind()
{
    return HeapByteBuffer::Rewind();
}

ECode ReadOnlyHeapByteBuffer::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* isEquals)
{
    ByteBuffer::Equals(other, isEquals);
    return NOERROR;
}

ECode ReadOnlyHeapByteBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadOnlyHeapByteBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadOnlyHeapByteBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadOnlyHeapByteBuffer";
    buf += ", status: capacity=";
    buf += mCapacity;
    buf += " position=";
    buf += mPosition;
    buf += " limit=";
    buf += mLimit;
    return buf.ToString(str);
}

} // namespace IO
} // namespace Elastos
