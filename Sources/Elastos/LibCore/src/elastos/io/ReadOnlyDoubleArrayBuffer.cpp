
#include "coredef.h"
#include "ReadOnlyDoubleArrayBuffer.h"
#include "elastos/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

ReadOnlyDoubleArrayBuffer::ReadOnlyDoubleArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Double>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : DoubleArrayBuffer(capacity, backingArray, arrayOffset)
{}

AutoPtr<ReadOnlyDoubleArrayBuffer> ReadOnlyDoubleArrayBuffer::Copy(
    /* [in] */ DoubleArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    AutoPtr<ReadOnlyDoubleArrayBuffer> buf = new ReadOnlyDoubleArrayBuffer(other->mCapacity,
            other->mBackingArray, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

PInterface ReadOnlyDoubleArrayBuffer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
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


UInt32 ReadOnlyDoubleArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyDoubleArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyDoubleArrayBuffer::GetInterfaceID(
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

ECode ReadOnlyDoubleArrayBuffer::GetPrimitiveArray(
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

ECode ReadOnlyDoubleArrayBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    return DoubleArrayBuffer::GetArray(array);
}

ECode ReadOnlyDoubleArrayBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadOnlyDoubleArrayBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadOnlyDoubleArrayBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return DoubleArrayBuffer::GetArrayOffset(offset);
}

ECode ReadOnlyDoubleArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    return Duplicate(buffer);
}

ECode ReadOnlyDoubleArrayBuffer::Compact()
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyDoubleArrayBuffer::CompareTo(
    /* [in] */ IDoubleBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return DoubleArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyDoubleArrayBuffer::Duplicate(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<ReadOnlyDoubleArrayBuffer> buf = Copy((DoubleArrayBuffer*)this, mMark);
    *buffer = (IDoubleBuffer*)buf.Get();
    INTERFACE_ADDREF(*buffer)
    return NOERROR;
}

ECode ReadOnlyDoubleArrayBuffer::GetDouble(
    /* [out] */ Double* value)
{
    return DoubleArrayBuffer::GetDouble(value);
}

ECode ReadOnlyDoubleArrayBuffer::GetDouble(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return DoubleArrayBuffer::GetDoubleEx(index, value);
}

ECode ReadOnlyDoubleArrayBuffer::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst)
{
    return DoubleArrayBuffer::GetDoubles(dst);
}

ECode ReadOnlyDoubleArrayBuffer::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 doubleCount)
{
    return DoubleArrayBuffer::GetDoublesEx(dst, dstOffset, doubleCount);
}

ECode ReadOnlyDoubleArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return DoubleArrayBuffer::GetOrder(order);
}

ECode ReadOnlyDoubleArrayBuffer::PutDouble(
    /* [in] */ Double c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyDoubleArrayBuffer::PutDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyDoubleArrayBuffer::PutDoubles(
    /* [in] */ const ArrayOf<Double>& src)
{
    return DoubleArrayBuffer::PutDoubles(src);
}

ECode ReadOnlyDoubleArrayBuffer::PutDoubles(
    /* [in] */ const ArrayOf<Double>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 doubleCount)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyDoubleArrayBuffer::PutDoubleBuffer(
    /* [in] */ IDoubleBuffer* src)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyDoubleArrayBuffer::Slice(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    *buffer = (IDoubleBuffer*)new ReadOnlyDoubleArrayBuffer(remaining, mBackingArray, mOffset + mPosition);
    INTERFACE_ADDREF(*buffer)
    return NOERROR;
}

ECode ReadOnlyDoubleArrayBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return DoubleArrayBuffer::GetCapacity(cap);
}

ECode ReadOnlyDoubleArrayBuffer::Clear()
{
    return DoubleArrayBuffer::Clear();
}

ECode ReadOnlyDoubleArrayBuffer::Flip()
{
    return DoubleArrayBuffer::Flip();
}

ECode ReadOnlyDoubleArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return DoubleArrayBuffer::HasArray(hasArray);
}

ECode ReadOnlyDoubleArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return DoubleArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyDoubleArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return DoubleArrayBuffer::IsDirect(isDirect);
}

ECode ReadOnlyDoubleArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

CARAPI ReadOnlyDoubleArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

CARAPI ReadOnlyDoubleArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

CARAPI ReadOnlyDoubleArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode ReadOnlyDoubleArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return DoubleArrayBuffer::GetLimit(limit);
}

ECode ReadOnlyDoubleArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return DoubleArrayBuffer::SetLimit(newLimit);
}

ECode ReadOnlyDoubleArrayBuffer::Mark()
{
    return DoubleArrayBuffer::Mark();
}

ECode ReadOnlyDoubleArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return DoubleArrayBuffer::GetPosition(pos);
}

ECode ReadOnlyDoubleArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return DoubleArrayBuffer::SetPosition(newPosition);
}

ECode ReadOnlyDoubleArrayBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return DoubleArrayBuffer::GetRemaining(remaining);
}

ECode ReadOnlyDoubleArrayBuffer::Reset()
{
    return DoubleArrayBuffer::Reset();
}

ECode ReadOnlyDoubleArrayBuffer::Rewind()
{
    return DoubleArrayBuffer::Rewind();
}

ECode ReadOnlyDoubleArrayBuffer::Equals(
    /* [in] */  IInterface* other,
    /* [out] */ Boolean* rst)
{
    return DoubleArrayBuffer::Equals(other, rst);
}

ECode ReadOnlyDoubleArrayBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadOnlyDoubleArrayBuffer";
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
