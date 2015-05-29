
#include "coredef.h"
#include "ReadWriteHeapByteBuffer.h"
#include "ReadOnlyHeapByteBuffer.h"
#include "Memory.h"
#include <elastos/Math.h>
#include "CByteOrderHelper.h"
#include "elastos/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

ReadWriteHeapByteBuffer::ReadWriteHeapByteBuffer(
    /* [in] */ ArrayOf<Byte>* backingArray)
    : HeapByteBuffer(backingArray)
{}

ReadWriteHeapByteBuffer::ReadWriteHeapByteBuffer(
    /* [in] */ Int32 capacity)
    : HeapByteBuffer(capacity)
{}

ReadWriteHeapByteBuffer::ReadWriteHeapByteBuffer(
    /* [in] */ ArrayOf<Byte>* backingArray,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 arrayOffset)
    : HeapByteBuffer(backingArray, capacity, arrayOffset)
{}

PInterface ReadWriteHeapByteBuffer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IByteBuffer) {
        return (IByteBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_ByteBuffer) {
        return reinterpret_cast<PInterface>((ByteBuffer*)this);
    }
    else if (riid == EIID_HeapByteBuffer) {
        return reinterpret_cast<PInterface>((HeapByteBuffer*)this);
    }

    return NULL;
}

UInt32 ReadWriteHeapByteBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteHeapByteBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteHeapByteBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(IByteBuffer*)this) {
        *pIID = EIID_IByteBuffer;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

AutoPtr<ReadWriteHeapByteBuffer> ReadWriteHeapByteBuffer::Copy(
    /* [in] */ HeapByteBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 cap;
    other->GetCapacity(&cap);
    AutoPtr<ReadWriteHeapByteBuffer> buf = new ReadWriteHeapByteBuffer(
            other->mBackingArray, cap, other->mOffset);
    buf->mLimit = other->mLimit;
    other->GetPosition(&(buf->mPosition));
    buf->mMark = markOfOther;
    return buf;
}

ECode ReadWriteHeapByteBuffer::GetPrimitiveArray(
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

ECode ReadWriteHeapByteBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    VALIDATE_NOT_NULL(array);
    return HeapByteBuffer::GetArray(array);
}

ECode ReadWriteHeapByteBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    return HeapByteBuffer::GetArrayOffset(offset);
}

ECode ReadWriteHeapByteBuffer::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    return HeapByteBuffer::AsCharBuffer(buffer);
}

ECode ReadWriteHeapByteBuffer::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    return HeapByteBuffer::AsDoubleBuffer(buffer);
}

ECode ReadWriteHeapByteBuffer::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    return HeapByteBuffer::AsFloatBuffer(buffer);
}

ECode ReadWriteHeapByteBuffer::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    return HeapByteBuffer::AsInt16Buffer(buffer);
}

ECode ReadWriteHeapByteBuffer::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    return HeapByteBuffer::AsInt32Buffer(buffer);
}

ECode ReadWriteHeapByteBuffer::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    return HeapByteBuffer::AsInt64Buffer(buffer);
}

ECode ReadWriteHeapByteBuffer::AsReadOnlyBuffer(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    AutoPtr<ReadOnlyHeapByteBuffer> buf = ReadOnlyHeapByteBuffer::Copy((HeapByteBuffer*)this, mMark);
    *buffer = (IByteBuffer*)buf.Get();
    REFCOUNT_ADD(*buffer);
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::Compact()
{
    Int32 remaining;
    GetRemaining(&remaining);
    mBackingArray->Copy(mOffset, mBackingArray, mOffset + mPosition, remaining);
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::CompareTo(
    /* [in] */ IByteBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return HeapByteBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteHeapByteBuffer::Duplicate(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    AutoPtr<ReadWriteHeapByteBuffer> buf = Copy((HeapByteBuffer*)this, mMark);
    *buffer = (IByteBuffer*)buf.Get();
    REFCOUNT_ADD(*buffer);
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::GetByte(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetByte(value);
}

ECode ReadWriteHeapByteBuffer::GetByte(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetByte(index, value);
}

ECode ReadWriteHeapByteBuffer::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst)
{
    VALIDATE_NOT_NULL(dst);
    return HeapByteBuffer::GetBytes(dst);
}

ECode ReadWriteHeapByteBuffer::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    VALIDATE_NOT_NULL(dst);
    return HeapByteBuffer::GetBytes(dst, off, len);
}

ECode ReadWriteHeapByteBuffer::GetChar(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetChar(value);
}

ECode ReadWriteHeapByteBuffer::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetChar(index, value);
}

ECode ReadWriteHeapByteBuffer::GetDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetDouble(value);
}

ECode ReadWriteHeapByteBuffer::GetDouble(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetDouble(index, value);
}

ECode ReadWriteHeapByteBuffer::GetFloat(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetFloat(value);
}

ECode ReadWriteHeapByteBuffer::GetFloat(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetFloat(index, value);
}

ECode ReadWriteHeapByteBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetInt32(value);
}

ECode ReadWriteHeapByteBuffer::GetInt32(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetInt32(index, value);
}

ECode ReadWriteHeapByteBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetInt64(value);
}

ECode ReadWriteHeapByteBuffer::GetInt64(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetInt64(index, value);
}

ECode ReadWriteHeapByteBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetInt16(value);
}

ECode ReadWriteHeapByteBuffer::GetInt16(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    return HeapByteBuffer::GetInt16(index, value);
}

ECode ReadWriteHeapByteBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    VALIDATE_NOT_NULL(byteOrder);
    return HeapByteBuffer::GetOrder(byteOrder);
}

ECode ReadWriteHeapByteBuffer::SetOrder(
    /* [in] */ ByteOrder byteOrder)
{
    VALIDATE_NOT_NULL(byteOrder);
    return HeapByteBuffer::SetOrder(byteOrder);
}

ECode ReadWriteHeapByteBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    REFCOUNT_ADD(*array);
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    VALIDATE_NOT_NULL(hasArray);
    *hasArray = TRUE;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutByte(
    /* [in] */ Byte b)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    (*mBackingArray)[mOffset + mPosition++] = b;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutByte(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    FAIL_RETURN(CheckIndex(index));
    (*mBackingArray)[mOffset + index] = b;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src)
{
    return HeapByteBuffer::PutBytes(src);
}

ECode ReadWriteHeapByteBuffer::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 byteCount)
{
    Int32 bounds;
    FAIL_RETURN(CheckPutBounds(1, src.GetLength(), srcOffset, byteCount, &bounds));
    mBackingArray->Copy(mOffset + mPosition, &src, srcOffset, byteCount);
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutByteBuffer(
    /* [in] */ IByteBuffer* src)
{
    return HeapByteBuffer::PutByteBuffer(src);
}

ECode ReadWriteHeapByteBuffer::PutChar(
    /* [in] */ Char32 value)
{
    Int32 newPosition = mPosition + sizeof(Char32);
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    Memory::PokeInt32(mBackingArray, mOffset + mPosition, (Int32)value, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    FAIL_RETURN(CheckIndex(index, sizeof(Char32)));
    Memory::PokeInt32(mBackingArray, mOffset + index, (Int32)value, mOrder);
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutDouble(
    /* [in] */ Double value)
{
    return PutInt64(Elastos::Core::Math::DoubleToRawInt64Bits(value));
}

ECode ReadWriteHeapByteBuffer::PutDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    return PutInt64(index, Elastos::Core::Math::DoubleToRawInt64Bits(value));
}

ECode ReadWriteHeapByteBuffer::PutFloat(
    /* [in] */ Float value)
{
    return PutInt32(Elastos::Core::Math::FloatToRawInt32Bits(value));
}

ECode ReadWriteHeapByteBuffer::PutFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    return PutInt32(index, Elastos::Core::Math::FloatToRawInt32Bits(value));
}

ECode ReadWriteHeapByteBuffer::PutInt16(
    /* [in] */ Int16 value)
{
    Int32 newPosition = mPosition + sizeof(Int16);
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    Memory::PokeInt16(mBackingArray, mOffset + mPosition, value, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutInt16(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    FAIL_RETURN(CheckIndex(index, sizeof(Int16)));
    Memory::PokeInt16(mBackingArray, mOffset + index, value, mOrder);
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutInt32(
    /* [in] */ Int32 value)
{
    Int32 newPosition = mPosition + sizeof(Int32);
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    Memory::PokeInt32(mBackingArray, mOffset + mPosition, value, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    FAIL_RETURN(CheckIndex(index, sizeof(Int32)));
    Memory::PokeInt32(mBackingArray, mOffset + index, value, mOrder);
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutInt64(
    /* [in] */ Int64 value)
{
    Int32 newPosition = mPosition + sizeof(Int64);
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    Memory::PokeInt64(mBackingArray, mOffset + mPosition, value, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    FAIL_RETURN(CheckIndex(index, sizeof(Int64)));
    Memory::PokeInt64(mBackingArray, mOffset + index, value, mOrder);
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::Slice(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining;
    GetRemaining(&remaining);
    AutoPtr<ReadWriteHeapByteBuffer> slice = new ReadWriteHeapByteBuffer(mBackingArray,
            remaining, mOffset + mPosition);
    *buffer = (IByteBuffer*)slice.Get();
    REFCOUNT_ADD(*buffer);
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    VALIDATE_NOT_NULL(cap);
    return HeapByteBuffer::GetCapacity(cap);
}

ECode ReadWriteHeapByteBuffer::Clear()
{
    return HeapByteBuffer::Clear();
}

ECode ReadWriteHeapByteBuffer::Flip()
{
    return HeapByteBuffer::Flip();
}

ECode ReadWriteHeapByteBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    VALIDATE_NOT_NULL(hasArray);
    return HeapByteBuffer::HasArray(hasArray);
}

ECode ReadWriteHeapByteBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    VALIDATE_NOT_NULL(hasRemaining);
    return HeapByteBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteHeapByteBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    return HeapByteBuffer::IsDirect(isDirect);
}

ECode ReadWriteHeapByteBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    VALIDATE_NOT_NULL(limit);
    return HeapByteBuffer::GetLimit(limit);
}

ECode ReadWriteHeapByteBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return HeapByteBuffer::SetLimit(newLimit);
}

ECode ReadWriteHeapByteBuffer::Mark()
{
    return HeapByteBuffer::Mark();
}

ECode ReadWriteHeapByteBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    VALIDATE_NOT_NULL(pos);
    return HeapByteBuffer::GetPosition(pos);
}

ECode ReadWriteHeapByteBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return HeapByteBuffer::SetPosition(newPosition);
}

ECode ReadWriteHeapByteBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    VALIDATE_NOT_NULL(remaining);
    return HeapByteBuffer::GetRemaining(remaining);
}

ECode ReadWriteHeapByteBuffer::Reset()
{
    return HeapByteBuffer::Reset();
}

ECode ReadWriteHeapByteBuffer::Rewind()
{
    return HeapByteBuffer::Rewind();
}

ECode ReadWriteHeapByteBuffer::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* isEquals)
{
    VALIDATE_NOT_NULL(isEquals);
    ByteBuffer::Equals(other, isEquals);
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutChar32s(
    /* [in] */ const ArrayOf<Char32>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    Int32 byteCount = 0;
    FAIL_RETURN(CheckPutBounds(sizeof(Char32), src.GetLength(), srcOffset, charCount, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    FAIL_RETURN(Memory::UnsafeBulkPut(mBackingArray, mOffset + mPosition, byteCount, (Byte*)src.GetPayload(),
            srcOffset, sizeof(Char32), needsSwap))
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutInt16s(
    /* [in] */ const ArrayOf<Int16>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int16Count)
{
    Int32 byteCount = 0;
    FAIL_RETURN(CheckPutBounds(sizeof(Int16), src.GetLength(), srcOffset, int16Count, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    FAIL_RETURN(Memory::UnsafeBulkPut(mBackingArray, mOffset + mPosition, byteCount, (Byte*)src.GetPayload(),
            srcOffset, sizeof(Int16), needsSwap))
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutInt32s(
    /* [in] */ const ArrayOf<Int32>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int32Count)
{
    Int32 byteCount = 0;
    FAIL_RETURN(CheckPutBounds(sizeof(Int32), src.GetLength(), srcOffset, int32Count, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    FAIL_RETURN(Memory::UnsafeBulkPut(mBackingArray, mOffset + mPosition, byteCount, (Byte*)src.GetPayload(),
            srcOffset, sizeof(Int32), needsSwap))
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutInt64s(
    /* [in] */ const ArrayOf<Int64>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int64Count)
{
    Int32 byteCount = 0;
    FAIL_RETURN(CheckPutBounds(sizeof(Int64), src.GetLength(), srcOffset, int64Count, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    FAIL_RETURN(Memory::UnsafeBulkPut(mBackingArray, mOffset + mPosition, byteCount, (Byte*)src.GetPayload(),
            srcOffset, sizeof(Int64), needsSwap))
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutFloats(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 floatCount)
{
    Int32 byteCount = 0;
    FAIL_RETURN(CheckPutBounds(sizeof(Float), src.GetLength(), srcOffset, floatCount, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    FAIL_RETURN(Memory::UnsafeBulkPut(mBackingArray, mOffset + mPosition, byteCount, (Byte*)src.GetPayload(),
            srcOffset, sizeof(Float), needsSwap))
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutDoubles(
    /* [in] */ const ArrayOf<Double>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 doubleCount)
{
    Int32 byteCount = 0;
    FAIL_RETURN(CheckPutBounds(sizeof(Double), src.GetLength(), srcOffset, doubleCount, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    FAIL_RETURN(Memory::UnsafeBulkPut(mBackingArray, mOffset + mPosition, byteCount, (Byte*)src.GetPayload(),
            srcOffset, sizeof(Double), needsSwap))
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadWriteHeapByteBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadWriteHeapByteBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadWriteHeapByteBuffer";
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
