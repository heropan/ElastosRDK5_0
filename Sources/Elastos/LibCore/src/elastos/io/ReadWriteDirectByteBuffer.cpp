
#include "coredef.h"
#include "ReadWriteDirectByteBuffer.h"
#include "ReadOnlyDirectByteBuffer.h"
#include "CByteOrderHelper.h"
#include "Memory.h"
#include <elastos/Math.h>
#include "elastos/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

// {D9F5D1A4-E93E-4512-BF39-05E76DF67EA2}
extern "C" const InterfaceID EIID_ReadWriteDirectByteBuffer =
    { 0xd9f5d1a4, 0xe93e, 0x4512, { 0xbf, 0x39, 0x5, 0xe7, 0x6d, 0xf6, 0x7e, 0xa2 } };

PInterface ReadWriteDirectByteBuffer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
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
    else if (riid == EIID_DirectByteBuffer) {
        return reinterpret_cast<PInterface>((DirectByteBuffer*)this);
    }
    else if (riid == EIID_ReadWriteDirectByteBuffer) {
        return reinterpret_cast<PInterface>(this);
    }

    return NULL;
}

UInt32 ReadWriteDirectByteBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteDirectByteBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteDirectByteBuffer::GetInterfaceID(
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

AutoPtr<ReadWriteDirectByteBuffer> ReadWriteDirectByteBuffer::Copy(
    /* [in] */ DirectByteBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    VALIDATE_NOT_NULL(other)
    AutoPtr<ReadWriteDirectByteBuffer> buf = new ReadWriteDirectByteBuffer(
            other->mBlock, other->mCapacity, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

ReadWriteDirectByteBuffer::ReadWriteDirectByteBuffer(
    /* [in] */ Int32 capacity)
    : DirectByteBuffer(MemoryBlock::Allocate(capacity), capacity, 0)
{}

ReadWriteDirectByteBuffer::ReadWriteDirectByteBuffer(
    /* [in] */ Int32 pointer,
    /* [in] */ Int32 capacity)
    : DirectByteBuffer(MemoryBlock::WrapFromNative(pointer, capacity), capacity, 0)
{}

ReadWriteDirectByteBuffer::ReadWriteDirectByteBuffer(
    /* [in] */ MemoryBlock* block,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 offset)
    : DirectByteBuffer(block, capacity, offset)
{}

ECode ReadWriteDirectByteBuffer::GetPrimitiveArray(
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

ECode ReadWriteDirectByteBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return DirectByteBuffer::GetArray(array);
}

ECode ReadWriteDirectByteBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadWriteDirectByteBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadWriteDirectByteBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return DirectByteBuffer::GetArrayOffset(offset);
}

ECode ReadWriteDirectByteBuffer::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return DirectByteBuffer::AsCharBuffer(buffer);
}

ECode ReadWriteDirectByteBuffer::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    return DirectByteBuffer::AsDoubleBuffer(buffer);
}

ECode ReadWriteDirectByteBuffer::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    return DirectByteBuffer::AsFloatBuffer(buffer);
}

ECode ReadWriteDirectByteBuffer::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    return DirectByteBuffer::AsInt16Buffer(buffer);
}

ECode ReadWriteDirectByteBuffer::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    return DirectByteBuffer::AsInt32Buffer(buffer);
}

ECode ReadWriteDirectByteBuffer::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    return DirectByteBuffer::AsInt64Buffer(buffer);
}

ECode ReadWriteDirectByteBuffer::AsReadOnlyBuffer(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<ReadOnlyDirectByteBuffer> buf = ReadOnlyDirectByteBuffer::Copy((DirectByteBuffer*)this, mMark);
    *buffer = (IByteBuffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::Compact()
{
    Int32 remaining = 0;
    GetRemaining(&remaining);
    AutoPtr< ArrayOf<Byte> > tmpByteArray;
    GetArray((ArrayOf<Byte>**)&tmpByteArray);
    ASSERT_SUCCEEDED(Memory::Memmove(tmpByteArray, 0, *tmpByteArray, mPosition, remaining))
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::CompareTo(
    /* [in] */ IByteBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return DirectByteBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteDirectByteBuffer::Duplicate(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    *buffer = (IByteBuffer*)Copy((DirectByteBuffer*)this, mMark);
    REFCOUNT_ADD(*buffer);
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::GetByte(
    /* [out] */ Byte* value)
{
    return DirectByteBuffer::GetByte(value);
}

ECode ReadWriteDirectByteBuffer::GetByte(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    return DirectByteBuffer::GetByte(index, value);
}

ECode ReadWriteDirectByteBuffer::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst)
{
    return DirectByteBuffer::GetBytes(dst);
}

ECode ReadWriteDirectByteBuffer::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    return DirectByteBuffer::GetBytes(dst, off, len);
}

ECode ReadWriteDirectByteBuffer::GetChar(
    /* [out] */ Char32* value)
{
    return DirectByteBuffer::GetChar(value);
}

ECode ReadWriteDirectByteBuffer::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return DirectByteBuffer::GetChar(index, value);
}

ECode ReadWriteDirectByteBuffer::GetDouble(
    /* [out] */ Double* value)
{
    return DirectByteBuffer::GetDouble(value);
}

ECode ReadWriteDirectByteBuffer::GetDouble(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return DirectByteBuffer::GetDouble(index, value);
}

ECode ReadWriteDirectByteBuffer::GetFloat(
    /* [out] */ Float* value)
{
    return DirectByteBuffer::GetFloat(value);
}

ECode ReadWriteDirectByteBuffer::GetFloat(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return DirectByteBuffer::GetFloat(index, value);
}

ECode ReadWriteDirectByteBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    return DirectByteBuffer::GetInt32(value);
}

ECode ReadWriteDirectByteBuffer::GetInt32(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return DirectByteBuffer::GetInt32(index, value);
}

ECode ReadWriteDirectByteBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    return DirectByteBuffer::GetInt64(value);
}

ECode ReadWriteDirectByteBuffer::GetInt64(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return DirectByteBuffer::GetInt64(index, value);
}

ECode ReadWriteDirectByteBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    return DirectByteBuffer::GetInt16(value);
}

ECode ReadWriteDirectByteBuffer::GetInt16(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return DirectByteBuffer::GetInt16(index, value);
}

ECode ReadWriteDirectByteBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    return DirectByteBuffer::GetOrder(byteOrder);
}

ECode ReadWriteDirectByteBuffer::SetOrder(
    /* [in] */ ByteOrder byteOrder)
{
    return DirectByteBuffer::SetOrder(byteOrder);
}

ECode ReadWriteDirectByteBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return DirectByteBuffer::ProtectedArray(array);
}

ECode ReadWriteDirectByteBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return DirectByteBuffer::ProtectedArrayOffset(offset);
}

ECode ReadWriteDirectByteBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    return DirectByteBuffer::ProtectedHasArray(hasArray);
}

ECode ReadWriteDirectByteBuffer::PutByte(
    /* [in] */ Byte b)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    mBlock->PokeByte(mOffset + mPosition++, b);
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutByte(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    FAIL_RETURN(CheckIndex(index))
    mBlock->PokeByte(mOffset + index, b);
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src)
{
    return DirectByteBuffer::PutBytes(src);
}

ECode ReadWriteDirectByteBuffer::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 byteCount)
{
    Int32 result = 0;
    FAIL_RETURN(CheckPutBounds(1, src.GetLength(), srcOffset, byteCount, &result))
    mBlock->PokeByteArray(mOffset + mPosition, const_cast<ArrayOf<Byte>*>(&src), srcOffset, byteCount);
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutByteBuffer(
    /* [in] */ IByteBuffer* src)
{
    return DirectByteBuffer::PutByteBuffer(src);
}

ECode ReadWriteDirectByteBuffer::PutChar32s(
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
    mBlock->PokeCharArray(mOffset + mPosition, const_cast<ArrayOf<Char32>*>(&src), srcOffset, charCount, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutInt16s(
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
    mBlock->PokeInt16Array(mOffset + mPosition, const_cast<ArrayOf<Int16>*>(&src), srcOffset, int16Count, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutInt32s(
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
    mBlock->PokeInt32Array(mOffset + mPosition, const_cast<ArrayOf<Int32>*>(&src), srcOffset, int32Count, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutInt64s(
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
    mBlock->PokeInt64Array(mOffset + mPosition, const_cast<ArrayOf<Int64>*>(&src), srcOffset, int64Count, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutFloats(
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
    mBlock->PokeFloatArray(mOffset + mPosition, const_cast<ArrayOf<Float>*>(&src), srcOffset, floatCount, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutDoubles(
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
    mBlock->PokeDoubleArray(mOffset + mPosition, const_cast<ArrayOf<Double>*>(&src), srcOffset, doubleCount, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutChar(
    /* [in] */ Char32 value)
{
    Int32 newPosition = mPosition + sizeof(Char32);
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    mBlock->PokeInt16(mOffset + mPosition, (Int16) value, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    FAIL_RETURN(CheckIndex(index, sizeof(Char32)))
    mBlock->PokeInt16(mOffset + index, (Int16) value, mOrder);
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutDouble(
    /* [in] */ Double value)
{
    Int32 newPosition = mPosition + sizeof(Double);
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }

    mBlock->PokeInt64(mOffset + mPosition, Elastos::Core::Math::DoubleToInt64Bits(value), mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    FAIL_RETURN(CheckIndex(index, sizeof(Double)))
    mBlock->PokeInt64(mOffset + index, Elastos::Core::Math::DoubleToInt64Bits(value), mOrder);
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutFloat(
    /* [in] */ Float value)
{
    Int32 newPosition = mPosition + sizeof(Float);
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    mBlock->PokeInt32(mOffset + mPosition, Elastos::Core::Math::FloatToRawInt32Bits(value), mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    FAIL_RETURN(CheckIndex(index, sizeof(Float)))
    mBlock->PokeInt32(mOffset + index, Elastos::Core::Math::FloatToRawInt32Bits(value), mOrder);
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutInt16(
    /* [in] */ Int16 value)
{
    Int32 newPosition = mPosition + sizeof(Int16);
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    mBlock->PokeInt16(mOffset + mPosition, value, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutInt16(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    FAIL_RETURN(CheckIndex(index, sizeof(Int16)))
    mBlock->PokeInt16(mOffset + index, value, mOrder);
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutInt32(
    /* [in] */ Int32 value)
{
    Int32 newPosition = mPosition + sizeof(Int32);
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    mBlock->PokeInt32(mOffset + mPosition, value, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    FAIL_RETURN(CheckIndex(index, sizeof(Int32)))
    mBlock->PokeInt32(mOffset + index, value, mOrder);
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutInt64(
    /* [in] */ Int64 value)
{
    Int32 newPosition = mPosition + sizeof(Int64);
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    mBlock->PokeInt64(mOffset + mPosition, value, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::PutInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    FAIL_RETURN(CheckIndex(index, sizeof(Int64)))
    mBlock->PokeInt64(mOffset + index, value, mOrder);
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::Slice(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 num = 0;
    GetRemaining(&num);
    *buffer = (IByteBuffer*)new ReadWriteDirectByteBuffer(mBlock, num, mOffset + mPosition);
    REFCOUNT_ADD(*buffer);
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return DirectByteBuffer::GetCapacity(cap);
}

ECode ReadWriteDirectByteBuffer::Clear()
{
    return DirectByteBuffer::Clear();
}

ECode ReadWriteDirectByteBuffer::Flip()
{
    return DirectByteBuffer::Flip();
}

ECode ReadWriteDirectByteBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return DirectByteBuffer::HasArray(hasArray);
}

ECode ReadWriteDirectByteBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return DirectByteBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteDirectByteBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return DirectByteBuffer::IsDirect(isDirect);
}

ECode ReadWriteDirectByteBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return DirectByteBuffer::GetLimit(limit);
}

ECode ReadWriteDirectByteBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return DirectByteBuffer::SetLimit(newLimit);
}

ECode ReadWriteDirectByteBuffer::Mark()
{
    return DirectByteBuffer::Mark();
}

ECode ReadWriteDirectByteBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return DirectByteBuffer::GetPosition(pos);
}

ECode ReadWriteDirectByteBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return DirectByteBuffer::SetPosition(newPosition);
}

ECode ReadWriteDirectByteBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return DirectByteBuffer::GetRemaining(remaining);
}

ECode ReadWriteDirectByteBuffer::Reset()
{
    return DirectByteBuffer::Reset();
}

ECode ReadWriteDirectByteBuffer::Rewind()
{
    return DirectByteBuffer::Rewind();
}

ECode ReadWriteDirectByteBuffer::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* isEquals)
{
    ByteBuffer::Equals(other, isEquals);
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadWriteDirectByteBuffer";
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
