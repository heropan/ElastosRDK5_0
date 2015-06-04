
#include "coredef.h"
#include "ReadWriteInt16ArrayBuffer.h"
#include "ReadOnlyInt16ArrayBuffer.h"
#include "elastos/core/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

ReadWriteInt16ArrayBuffer::ReadWriteInt16ArrayBuffer(
    /* [in] */ ArrayOf<Int16>* array)
    : Int16ArrayBuffer(array)
{}

ReadWriteInt16ArrayBuffer::ReadWriteInt16ArrayBuffer(
    /* [in] */ Int32 capacity)
    : Int16ArrayBuffer(capacity)
{}

ReadWriteInt16ArrayBuffer::ReadWriteInt16ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int16>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : Int16ArrayBuffer(capacity, backingArray, arrayOffset)
{}

AutoPtr<ReadWriteInt16ArrayBuffer> ReadWriteInt16ArrayBuffer::Copy(
    /* [in] */ Int16ArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    assert(0 && "TODO");
    AutoPtr<ReadWriteInt16ArrayBuffer> buf; // = new ReadWriteInt16ArrayBuffer(other->mCapacity,
            // other->mBackingArray, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

PInterface ReadWriteInt16ArrayBuffer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        assert(0 && "TODO");
        // return (PInterface)this;
    }
    else if (riid == EIID_IInt16Buffer) {
        return (IInt16Buffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_Int16Buffer) {
        return reinterpret_cast<PInterface>((Int16Buffer*)this);
    }

    return NULL;
}


UInt32 ReadWriteInt16ArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteInt16ArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteInt16ArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(IInt16Buffer*)this) {
        *pIID = EIID_IInt16Buffer;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::GetPrimitiveArray(
    /* [out] */ Handle32* arrayHandle)
{
    mArrayTemp = NULL;
    GetArray((ArrayOf<Int16>**)&mArrayTemp);
    if (mArrayTemp == NULL)
    {
        *arrayHandle = 0;
        return NOERROR;
    }
    Int16* primitiveArray = mArrayTemp->GetPayload();
    *arrayHandle = reinterpret_cast<Handle32>(primitiveArray);
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    return Int16ArrayBuffer::GetArray(array);
}

ECode ReadWriteInt16ArrayBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadWriteInt16ArrayBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadWriteInt16ArrayBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int16ArrayBuffer::GetArrayOffset(offset);
}

ECode ReadWriteInt16ArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<ReadOnlyInt16ArrayBuffer> buf = ReadOnlyInt16ArrayBuffer::Copy((Int16ArrayBuffer*)this, mMark);
    *buffer = (IInt16Buffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::Compact()
{
    Int32 remaining = 0;
    GetRemaining(&remaining);
    mBackingArray->Copy(mOffset, mBackingArray, mPosition + mOffset, remaining);
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::CompareTo(
    /* [in] */ IInt16Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int16ArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteInt16ArrayBuffer::Duplicate(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    *buffer = (IInt16Buffer*)Copy((Int16ArrayBuffer*)this, mMark);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    return Int16ArrayBuffer::GetInt16(value);
}

ECode ReadWriteInt16ArrayBuffer::GetInt16(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return Int16ArrayBuffer::GetInt16(index, value);
}

ECode ReadWriteInt16ArrayBuffer::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst)
{
    return Int16ArrayBuffer::GetInt16s(dst, 0, dst->GetLength());
}

ECode ReadWriteInt16ArrayBuffer::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int16Count)
{
    return Int16ArrayBuffer::GetInt16s(dst, dstOffset, int16Count);
}

ECode ReadWriteInt16ArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return Int16ArrayBuffer::GetOrder(order);
}

ECode ReadWriteInt16ArrayBuffer::PutInt16(
    /* [in] */ Int16 c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    (*mBackingArray)[mOffset + mPosition++] = c;
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::PutInt16(
    /* [in] */ Int32 index,
    /* [in] */ Int16 c)
{
    FAIL_RETURN(CheckIndex(index))
    (*mBackingArray)[mOffset + index] = c;
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::PutInt16s(
    /* [in] */ ArrayOf<Int16>* src)
{
    return Int16ArrayBuffer::Put(src);
}

ECode ReadWriteInt16ArrayBuffer::PutInt16s(
    /* [in] */ ArrayOf<Int16>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int16Count)
{
    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (int16Count > remaining) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    mBackingArray->Copy(mOffset + mPosition, src, srcOffset, int16Count);
    mPosition += int16Count;
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::PutInt16Buffer(
    /* [in] */ IInt16Buffer* src)
{
    return Int16ArrayBuffer::PutInt16Buffer(src);
}

ECode ReadWriteInt16ArrayBuffer::Slice(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    assert(0 && "TODO");
    // *buffer = (IInt16Buffer*)new ReadWriteInt16ArrayBuffer(remaining, mBackingArray,
            // mOffset + mPosition);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return Int16ArrayBuffer::GetCapacity(cap);
}

ECode ReadWriteInt16ArrayBuffer::Clear()
{
    return Int16ArrayBuffer::Clear();
}

ECode ReadWriteInt16ArrayBuffer::Flip()
{
    return Int16ArrayBuffer::Flip();
}

ECode ReadWriteInt16ArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int16ArrayBuffer::HasArray(hasArray);
}

ECode ReadWriteInt16ArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int16ArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteInt16ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return Int16ArrayBuffer::IsDirect(isDirect);
}

ECode ReadWriteInt16ArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

CARAPI ReadWriteInt16ArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

CARAPI ReadWriteInt16ArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

CARAPI ReadWriteInt16ArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int16ArrayBuffer::GetLimit(limit);
}

ECode ReadWriteInt16ArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int16ArrayBuffer::SetLimit(newLimit);
}

ECode ReadWriteInt16ArrayBuffer::Mark()
{
    return Int16ArrayBuffer::Mark();
}

ECode ReadWriteInt16ArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int16ArrayBuffer::GetPosition(pos);
}

ECode ReadWriteInt16ArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int16ArrayBuffer::SetPosition(newPosition);
}

ECode ReadWriteInt16ArrayBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return Int16ArrayBuffer::GetRemaining(remaining);
}

ECode ReadWriteInt16ArrayBuffer::Reset()
{
    return Int16ArrayBuffer::Reset();
}

ECode ReadWriteInt16ArrayBuffer::Rewind()
{
    return Int16ArrayBuffer::Rewind();
}

ECode ReadWriteInt16ArrayBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadWriteInt16ArrayBuffer";
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
