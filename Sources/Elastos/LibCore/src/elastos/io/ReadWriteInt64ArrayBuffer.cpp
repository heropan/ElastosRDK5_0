
#include "coredef.h"
#include "ReadWriteInt64ArrayBuffer.h"
#include "ReadOnlyInt64ArrayBuffer.h"
#include "elastos/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

ReadWriteInt64ArrayBuffer::ReadWriteInt64ArrayBuffer(
    /* [in] */ ArrayOf<Int64>* array)
    : Int64ArrayBuffer(array)
{}

ReadWriteInt64ArrayBuffer::ReadWriteInt64ArrayBuffer(
    /* [in] */ Int32 capacity)
    : Int64ArrayBuffer(capacity)
{}

ReadWriteInt64ArrayBuffer::ReadWriteInt64ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int64>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : Int64ArrayBuffer(capacity, backingArray, arrayOffset)
{}

AutoPtr<ReadWriteInt64ArrayBuffer> ReadWriteInt64ArrayBuffer::Copy(
    /* [in] */ Int64ArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    AutoPtr<ReadWriteInt64ArrayBuffer> buf = new ReadWriteInt64ArrayBuffer(other->mCapacity,
            other->mBackingArray, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

PInterface ReadWriteInt64ArrayBuffer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IInt64Buffer) {
        return (IInt64Buffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_Int64Buffer) {
        return reinterpret_cast<PInterface>((Int64Buffer*)this);
    }

    return NULL;
}

UInt32 ReadWriteInt64ArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteInt64ArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteInt64ArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(IInt64Buffer*)this) {
        *pIID = EIID_IInt64Buffer;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::GetPrimitiveArray(
    /* [out] */ Handle32* arrayHandle)
{
    mArrayTemp = NULL;
    GetArray((ArrayOf<Int64>**)&mArrayTemp);
    if (mArrayTemp == NULL)
    {
        *arrayHandle = 0;
        return NOERROR;
    }
    Int64* primitiveArray = mArrayTemp->GetPayload();
    *arrayHandle = reinterpret_cast<Handle32>(primitiveArray);
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    return Int64ArrayBuffer::GetArray(array);
}

ECode ReadWriteInt64ArrayBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadWriteInt64ArrayBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadWriteInt64ArrayBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int64ArrayBuffer::GetArrayOffset(offset);
}

ECode ReadWriteInt64ArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<ReadOnlyInt64ArrayBuffer> buf = ReadOnlyInt64ArrayBuffer::Copy((Int64ArrayBuffer*)this, mMark);
    *buffer = (IInt64Buffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::Compact()
{
    Int32 remaining = 0;
    GetRemaining(&remaining);
    mBackingArray->Copy(mOffset, mBackingArray, mPosition + mOffset, remaining);
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::CompareTo(
    /* [in] */ IInt64Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int64ArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteInt64ArrayBuffer::Duplicate(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    *buffer = (IInt64Buffer*)Copy((Int64ArrayBuffer*)this, mMark);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    return Int64ArrayBuffer::GetInt64(value);
}

ECode ReadWriteInt64ArrayBuffer::GetInt64(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return Int64ArrayBuffer::GetInt64(index, value);
}

ECode ReadWriteInt64ArrayBuffer::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst)
{
    return Int64ArrayBuffer::GetInt64s(dst);
}

ECode ReadWriteInt64ArrayBuffer::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int64Count)
{
    return Int64ArrayBuffer::GetInt64s(dst, dstOffset, int64Count);
}

ECode ReadWriteInt64ArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return Int64ArrayBuffer::GetOrder(order);
}

ECode ReadWriteInt64ArrayBuffer::PutInt64(
    /* [in] */ Int64 c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    (*mBackingArray)[mOffset + mPosition++] = c;
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::PutInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 c)
{
    FAIL_RETURN(CheckIndex(index))
    (*mBackingArray)[mOffset + index] = c;
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::PutInt64s(
    /* [in] */ const ArrayOf<Int64>& src)
{
    return Int64ArrayBuffer::PutInt64s(src);
}

ECode ReadWriteInt64ArrayBuffer::PutInt64s(
    /* [in] */ const ArrayOf<Int64>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int64Count)
{
    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (int64Count > remaining) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    mBackingArray->Copy(mOffset + mPosition, &src, srcOffset, int64Count);
    mPosition += int64Count;
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::PutInt64Buffer(
    /* [in] */ IInt64Buffer* src)
{
    return Int64ArrayBuffer::PutInt64Buffer(src);
}

ECode ReadWriteInt64ArrayBuffer::Slice(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    *buffer = (IInt64Buffer*)new ReadWriteInt64ArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return Int64ArrayBuffer::GetCapacity(cap);
}

ECode ReadWriteInt64ArrayBuffer::Clear()
{
    return Int64ArrayBuffer::Clear();
}

ECode ReadWriteInt64ArrayBuffer::Flip()
{
    return Int64ArrayBuffer::Flip();
}

ECode ReadWriteInt64ArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int64ArrayBuffer::HasArray(hasArray);
}

ECode ReadWriteInt64ArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int64ArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteInt64ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return Int64ArrayBuffer::IsDirect(isDirect);
}

ECode ReadWriteInt64ArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

CARAPI ReadWriteInt64ArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

CARAPI ReadWriteInt64ArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

CARAPI ReadWriteInt64ArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int64ArrayBuffer::GetLimit(limit);
}

ECode ReadWriteInt64ArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int64ArrayBuffer::SetLimit(newLimit);
}

ECode ReadWriteInt64ArrayBuffer::Mark()
{
    return Int64ArrayBuffer::Mark();
}

ECode ReadWriteInt64ArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int64ArrayBuffer::GetPosition(pos);
}

ECode ReadWriteInt64ArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int64ArrayBuffer::SetPosition(newPosition);
}

ECode ReadWriteInt64ArrayBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return Int64ArrayBuffer::GetRemaining(remaining);
}

ECode ReadWriteInt64ArrayBuffer::Reset()
{
    return Int64ArrayBuffer::Reset();
}

ECode ReadWriteInt64ArrayBuffer::Rewind()
{
    return Int64ArrayBuffer::Rewind();
}

ECode ReadWriteInt64ArrayBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadWriteInt64ArrayBuffer";
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
