
#include "ReadWriteDoubleArrayBuffer.h"
#include "ReadOnlyDoubleArrayBuffer.h"
#include "elastos/core/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

ReadWriteDoubleArrayBuffer::ReadWriteDoubleArrayBuffer(
    /* [in] */ ArrayOf<Double>* array)
    : DoubleArrayBuffer(array)
{}

ReadWriteDoubleArrayBuffer::ReadWriteDoubleArrayBuffer(
    /* [in] */ Int32 capacity)
    : DoubleArrayBuffer(capacity)
{}

ReadWriteDoubleArrayBuffer::ReadWriteDoubleArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Double>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : DoubleArrayBuffer(capacity, backingArray, arrayOffset)
{}

AutoPtr<ReadWriteDoubleArrayBuffer> ReadWriteDoubleArrayBuffer::Copy(
    /* [in] */ DoubleArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    assert(0 && "TODO");
    AutoPtr<ReadWriteDoubleArrayBuffer> buf; // = new ReadWriteDoubleArrayBuffer(other->mCapacity,
            // other->mBackingArray, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

PInterface ReadWriteDoubleArrayBuffer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        assert(0 && "TODO");
        // return (PInterface)this;
    }
    else if (riid == EIID_IDoubleBuffer) {
        return (IDoubleBuffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_DoubleBuffer) {
        return reinterpret_cast<PInterface>((DoubleBuffer*)this);
    }

    return NULL;
}

UInt32 ReadWriteDoubleArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteDoubleArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteDoubleArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(IDoubleBuffer*)this) {
        *pIID = EIID_IDoubleBuffer;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::GetPrimitiveArray(
    /* [out] */ Handle32* arrayHandle)
{
    mArrayTemp = NULL;
    GetArray((ArrayOf<Double>**)&mArrayTemp);
    if (mArrayTemp == NULL)
    {
        *arrayHandle = 0;
        return NOERROR;
    }
    Double* primitiveArray = mArrayTemp->GetPayload();
    *arrayHandle = reinterpret_cast<Handle32>(primitiveArray);
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    return DoubleArrayBuffer::GetArray(array);
}

ECode ReadWriteDoubleArrayBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadWriteDoubleArrayBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadWriteDoubleArrayBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return DoubleArrayBuffer::GetArrayOffset(offset);
}

ECode ReadWriteDoubleArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<ReadOnlyDoubleArrayBuffer> buf = ReadOnlyDoubleArrayBuffer::Copy((DoubleArrayBuffer*)this, mMark);
    *buffer = (IDoubleBuffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::Compact()
{
    Int32 remaining = 0;
    GetRemaining(&remaining);
    mBackingArray->Copy(mOffset, mBackingArray, mPosition + mOffset, remaining);
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::CompareTo(
    /* [in] */ IDoubleBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return DoubleArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteDoubleArrayBuffer::Duplicate(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    *buffer = (IDoubleBuffer*)Copy((DoubleArrayBuffer*)this, mMark);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::GetDouble(
    /* [out] */ Double* value)
{
    return DoubleArrayBuffer::GetDouble(value);
}

ECode ReadWriteDoubleArrayBuffer::GetDouble(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return DoubleArrayBuffer::GetDouble(index, value);
}

ECode ReadWriteDoubleArrayBuffer::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst)
{
    return DoubleArrayBuffer::GetDoubles(dst, 0, dst->GetLength());
}

ECode ReadWriteDoubleArrayBuffer::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 doubleCount)
{
    return DoubleArrayBuffer::GetDoubles(dst, dstOffset, doubleCount);
}

ECode ReadWriteDoubleArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return DoubleArrayBuffer::GetOrder(order);
}

ECode ReadWriteDoubleArrayBuffer::PutDouble(
    /* [in] */ Double c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    (*mBackingArray)[mOffset + mPosition++] = c;
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::PutDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double c)
{
    FAIL_RETURN(CheckIndex(index))
    (*mBackingArray)[mOffset + index] = c;
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::PutDoubles(
    /* [in] */ const ArrayOf<Double>& src)
{
    return DoubleArrayBuffer::PutDoubles(src);
}

ECode ReadWriteDoubleArrayBuffer::PutDoubles(
    /* [in] */ const ArrayOf<Double>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 doubleCount)
{
    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (doubleCount > remaining) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    mBackingArray->Copy(mOffset + mPosition, &src, srcOffset, doubleCount);
    mPosition += doubleCount;
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::PutDoubleBuffer(
    /* [in] */ IDoubleBuffer* src)
{
    return DoubleArrayBuffer::PutDoubleBuffer(src);
}

ECode ReadWriteDoubleArrayBuffer::Slice(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    // *buffer = (IDoubleBuffer*)new ReadWriteDoubleArrayBuffer(remaining, mBackingArray,
            // mOffset + mPosition);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return DoubleArrayBuffer::GetCapacity(cap);
}

ECode ReadWriteDoubleArrayBuffer::Clear()
{
    return DoubleArrayBuffer::Clear();
}

ECode ReadWriteDoubleArrayBuffer::Flip()
{
    return DoubleArrayBuffer::Flip();
}

ECode ReadWriteDoubleArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return DoubleArrayBuffer::HasArray(hasArray);
}

ECode ReadWriteDoubleArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return DoubleArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteDoubleArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return DoubleArrayBuffer::IsDirect(isDirect);
}

ECode ReadWriteDoubleArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

CARAPI ReadWriteDoubleArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

CARAPI ReadWriteDoubleArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

CARAPI ReadWriteDoubleArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return DoubleArrayBuffer::GetLimit(limit);
}

ECode ReadWriteDoubleArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return DoubleArrayBuffer::SetLimit(newLimit);
}

ECode ReadWriteDoubleArrayBuffer::Mark()
{
    return DoubleArrayBuffer::Mark();
}

ECode ReadWriteDoubleArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return DoubleArrayBuffer::GetPosition(pos);
}

ECode ReadWriteDoubleArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return DoubleArrayBuffer::SetPosition(newPosition);
}

ECode ReadWriteDoubleArrayBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return DoubleArrayBuffer::GetRemaining(remaining);
}

ECode ReadWriteDoubleArrayBuffer::Reset()
{
    return DoubleArrayBuffer::Reset();
}

ECode ReadWriteDoubleArrayBuffer::Rewind()
{
    return DoubleArrayBuffer::Rewind();
}

ECode ReadWriteDoubleArrayBuffer::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* rst)
{
    return DoubleArrayBuffer::Equals(other, rst);
}

ECode ReadWriteDoubleArrayBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadWriteDoubleArrayBuffer";
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
