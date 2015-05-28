
#include "ReadOnlyDirectByteBuffer.h"
#include "coredef.h"
#include "elastos/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

PInterface ReadOnlyDirectByteBuffer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IByteBuffer) {
        return (IByteBuffer*)this;
    }
    else if (riid == EIID_ByteBuffer) {
        return reinterpret_cast<PInterface>((ByteBuffer*)this);
    }
    else if (riid == EIID_DirectByteBuffer) {
        return reinterpret_cast<PInterface>((DirectByteBuffer*)this);
    }

    return NULL;
}

UInt32 ReadOnlyDirectByteBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyDirectByteBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyDirectByteBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(IByteBuffer *)this) {
        *pIID = EIID_IByteBuffer ;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

AutoPtr<ReadOnlyDirectByteBuffer> ReadOnlyDirectByteBuffer::Copy(
    /* [in] */ DirectByteBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    assert(other != NULL);
    AutoPtr<ReadOnlyDirectByteBuffer> buf = new ReadOnlyDirectByteBuffer(
            other->mBlock, other->mCapacity, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

ReadOnlyDirectByteBuffer::ReadOnlyDirectByteBuffer(
    /* [in] */ MemoryBlock* block,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 offset)
    : DirectByteBuffer(block, capacity, offset)
{}

ECode ReadOnlyDirectByteBuffer::GetPrimitiveArray(
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

ECode ReadOnlyDirectByteBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return DirectByteBuffer::GetArray(array);
}

ECode ReadOnlyDirectByteBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadOnlyDirectByteBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadOnlyDirectByteBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return DirectByteBuffer::GetArrayOffset(offset);
}

ECode ReadOnlyDirectByteBuffer::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return DirectByteBuffer::AsCharBuffer(buffer);
}

ECode ReadOnlyDirectByteBuffer::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    return DirectByteBuffer::AsDoubleBuffer(buffer);
}

ECode ReadOnlyDirectByteBuffer::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    return DirectByteBuffer::AsFloatBuffer(buffer);
}

ECode ReadOnlyDirectByteBuffer::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    return DirectByteBuffer::AsInt16Buffer(buffer);
}

ECode ReadOnlyDirectByteBuffer::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    return DirectByteBuffer::AsInt32Buffer(buffer);
}

ECode ReadOnlyDirectByteBuffer::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    return DirectByteBuffer::AsInt64Buffer(buffer);
}

ECode ReadOnlyDirectByteBuffer::AsReadOnlyBuffer(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<ReadOnlyDirectByteBuffer> buf = Copy((DirectByteBuffer*)this, mMark);
    *buffer = (IByteBuffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadOnlyDirectByteBuffer::Compact()
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::CompareTo(
    /* [in] */ IByteBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return DirectByteBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyDirectByteBuffer::Duplicate(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> res = (IByteBuffer*)Copy(this, mMark);
    *buffer = res;
    REFCOUNT_ADD(*buffer);
    return NOERROR;
}

ECode ReadOnlyDirectByteBuffer::GetByte(
    /* [out] */ Byte* value)
{
    return DirectByteBuffer::GetByte(value);
}

ECode ReadOnlyDirectByteBuffer::GetByte(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    return DirectByteBuffer::GetByte(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst)
{
    return DirectByteBuffer::GetBytes(dst);
}

ECode ReadOnlyDirectByteBuffer::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 byteCount)
{
    return DirectByteBuffer::GetBytes(dst, dstOffset, byteCount);
}

ECode ReadOnlyDirectByteBuffer::GetChar(
    /* [out] */ Char32* value)
{
    return DirectByteBuffer::GetChar(value);
}

ECode ReadOnlyDirectByteBuffer::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return DirectByteBuffer::GetChar(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetDouble(
    /* [out] */ Double* value)
{
    return DirectByteBuffer::GetDouble(value);
}

ECode ReadOnlyDirectByteBuffer::GetDouble(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return DirectByteBuffer::GetDouble(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetFloat(
    /* [out] */ Float* value)
{
    return DirectByteBuffer::GetFloat(value);
}

ECode ReadOnlyDirectByteBuffer::GetFloat(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return DirectByteBuffer::GetFloat(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    return DirectByteBuffer::GetInt32(value);
}

ECode ReadOnlyDirectByteBuffer::GetInt32(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return DirectByteBuffer::GetInt32(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    return DirectByteBuffer::GetInt64(value);
}

ECode ReadOnlyDirectByteBuffer::GetInt64(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return DirectByteBuffer::GetInt64(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    return DirectByteBuffer::GetInt16(value);
}

ECode ReadOnlyDirectByteBuffer::GetInt16(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return DirectByteBuffer::GetInt16(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    return DirectByteBuffer::GetOrder(byteOrder);
}

ECode ReadOnlyDirectByteBuffer::SetOrder(
    /* [in] */ ByteOrder byteOrder)
{
    return DirectByteBuffer::SetOrder(byteOrder);
}

ECode ReadOnlyDirectByteBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyDirectByteBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyDirectByteBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    VALIDATE_NOT_NULL(hasArray)
    *hasArray = FALSE;
    return NOERROR;
}

ECode ReadOnlyDirectByteBuffer::PutByte(
    /* [in] */ Byte b)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutByte(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src)
{
    return DirectByteBuffer::PutBytes(src);
}

ECode ReadOnlyDirectByteBuffer::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 byteCount)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutByteBuffer(
    /* [in] */ IByteBuffer* src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutChar(
    /* [in] */ Char32 value)
{
    return DirectByteBuffer::PutChar(value);
}

ECode ReadOnlyDirectByteBuffer::PutChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    return DirectByteBuffer::PutChar(index, value);
}

ECode ReadOnlyDirectByteBuffer::PutDouble(
    /* [in] */ Double value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutFloat(
    /* [in] */ Float value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutInt16(
    /* [in] */ Int16 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutInt16(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutInt32(
    /* [in] */ Int32 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutInt64(
    /* [in] */ Int64 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::Slice(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    Int32 remaining = 0;
    GetRemaining(&remaining);
    *buffer = (IByteBuffer*)new ReadOnlyDirectByteBuffer(
            mBlock, remaining, mOffset + mPosition);
    REFCOUNT_ADD(*buffer);
    return NOERROR;
}

ECode ReadOnlyDirectByteBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return DirectByteBuffer::GetCapacity(cap);
}

ECode ReadOnlyDirectByteBuffer::Clear()
{
    return DirectByteBuffer::Clear();
}

ECode ReadOnlyDirectByteBuffer::Flip()
{
    return DirectByteBuffer::Flip();
}

ECode ReadOnlyDirectByteBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return DirectByteBuffer::HasArray(hasArray);
}

ECode ReadOnlyDirectByteBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return DirectByteBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyDirectByteBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return DirectByteBuffer::IsDirect(isDirect);
}

ECode ReadOnlyDirectByteBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

ECode ReadOnlyDirectByteBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return DirectByteBuffer::GetLimit(limit);
}

ECode ReadOnlyDirectByteBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return DirectByteBuffer::SetLimit(newLimit);
}

ECode ReadOnlyDirectByteBuffer::Mark()
{
    return DirectByteBuffer::Mark();
}

ECode ReadOnlyDirectByteBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return DirectByteBuffer::GetPosition(pos);
}

ECode ReadOnlyDirectByteBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return DirectByteBuffer::SetPosition(newPosition);
}

ECode ReadOnlyDirectByteBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return DirectByteBuffer::GetRemaining(remaining);
}

ECode ReadOnlyDirectByteBuffer::Reset()
{
    return DirectByteBuffer::Reset();
}

ECode ReadOnlyDirectByteBuffer::Rewind()
{
    return DirectByteBuffer::Rewind();
}

ECode ReadOnlyDirectByteBuffer::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* isEquals)
{
    ByteBuffer::Equals(other, isEquals);
    return NOERROR;
}

ECode ReadOnlyDirectByteBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadOnlyDirectByteBuffer";
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
