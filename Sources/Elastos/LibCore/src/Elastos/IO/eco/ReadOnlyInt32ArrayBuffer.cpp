
#include "cmdef.h"
#include "ReadOnlyInt32ArrayBuffer.h"
#include "elastos/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

ReadOnlyInt32ArrayBuffer::ReadOnlyInt32ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int32>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : Int32ArrayBuffer(capacity, backingArray, arrayOffset)
{}

AutoPtr<ReadOnlyInt32ArrayBuffer> ReadOnlyInt32ArrayBuffer::Copy(
    /* [in] */ Int32ArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    AutoPtr<ReadOnlyInt32ArrayBuffer> buf = new ReadOnlyInt32ArrayBuffer(other->mCapacity,
            other->mBackingArray, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

PInterface ReadOnlyInt32ArrayBuffer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IInt32Buffer) {
        return (IInt32Buffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_Int32Buffer) {
        return reinterpret_cast<PInterface>((Int32Buffer*)this);
    }

    return NULL;
}


UInt32 ReadOnlyInt32ArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyInt32ArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyInt32ArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(IInt32Buffer*)this) {
        *pIID = EIID_IInt32Buffer;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode ReadOnlyInt32ArrayBuffer::GetPrimitiveArray(
    /* [out] */ Handle32* arrayHandle)
{
    mArrayTemp = NULL;
    GetArray((ArrayOf<Int32>**)&mArrayTemp);
    if (mArrayTemp == NULL)
    {
        *arrayHandle = 0;
        return NOERROR;
    }
    Int32* primitiveArray = mArrayTemp->GetPayload();
    *arrayHandle = reinterpret_cast<Handle32>(primitiveArray);
    return NOERROR;
}

ECode ReadOnlyInt32ArrayBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    return Int32ArrayBuffer::GetArray(array);
}

ECode ReadOnlyInt32ArrayBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadOnlyInt32ArrayBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadOnlyInt32ArrayBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int32ArrayBuffer::GetArrayOffset(offset);
}

ECode ReadOnlyInt32ArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IInt32Buffer** buffer)
{
    return Duplicate(buffer);
}

ECode ReadOnlyInt32ArrayBuffer::Compact()
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt32ArrayBuffer::CompareTo(
    /* [in] */ IInt32Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int32ArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyInt32ArrayBuffer::Duplicate(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<ReadOnlyInt32ArrayBuffer> buf = Copy((Int32ArrayBuffer*)this, mMark);
    *buffer = (IInt32Buffer*)buf.Get();
    INTERFACE_ADDREF(*buffer)
    return NOERROR;
}

ECode ReadOnlyInt32ArrayBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    return Int32ArrayBuffer::GetInt32(value);
}

ECode ReadOnlyInt32ArrayBuffer::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return Int32ArrayBuffer::GetInt32Ex(index, value);
}

ECode ReadOnlyInt32ArrayBuffer::GetInt32s(
    /* [out] */ ArrayOf<Int32>* dst)
{
    return Int32ArrayBuffer::GetInt32s(dst);
}

ECode ReadOnlyInt32ArrayBuffer::GetInt32sEx(
    /* [out] */ ArrayOf<Int32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int32Count)
{
    return Int32ArrayBuffer::GetInt32sEx(dst, dstOffset, int32Count);
}

ECode ReadOnlyInt32ArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return Int32ArrayBuffer::GetOrder(order);
}

ECode ReadOnlyInt32ArrayBuffer::PutInt32(
    /* [in] */ Int32 c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt32ArrayBuffer::PutInt32Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int32 c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt32ArrayBuffer::PutInt32s(
    /* [in] */ const ArrayOf<Int32>& src)
{
    return Int32ArrayBuffer::PutInt32s(src);
}

ECode ReadOnlyInt32ArrayBuffer::PutInt32sEx(
    /* [in] */ const ArrayOf<Int32>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int32Count)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt32ArrayBuffer::PutInt32Buffer(
    /* [in] */ IInt32Buffer* src)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt32ArrayBuffer::Slice(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    *buffer = (IInt32Buffer*)new ReadOnlyInt32ArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    INTERFACE_ADDREF(*buffer)
    return NOERROR;
}

ECode ReadOnlyInt32ArrayBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return Int32ArrayBuffer::GetCapacity(cap);
}

ECode ReadOnlyInt32ArrayBuffer::Clear()
{
    return Int32ArrayBuffer::Clear();
}

ECode ReadOnlyInt32ArrayBuffer::Flip()
{
    return Int32ArrayBuffer::Flip();
}

ECode ReadOnlyInt32ArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int32ArrayBuffer::HasArray(hasArray);
}

ECode ReadOnlyInt32ArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int32ArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyInt32ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return Int32ArrayBuffer::IsDirect(isDirect);
}

ECode ReadOnlyInt32ArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

CARAPI ReadOnlyInt32ArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

CARAPI ReadOnlyInt32ArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

CARAPI ReadOnlyInt32ArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode ReadOnlyInt32ArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int32ArrayBuffer::GetLimit(limit);
}

ECode ReadOnlyInt32ArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int32ArrayBuffer::SetLimit(newLimit);
}

ECode ReadOnlyInt32ArrayBuffer::Mark()
{
    return Int32ArrayBuffer::Mark();
}

ECode ReadOnlyInt32ArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int32ArrayBuffer::GetPosition(pos);
}

ECode ReadOnlyInt32ArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int32ArrayBuffer::SetPosition(newPosition);
}

ECode ReadOnlyInt32ArrayBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return Int32ArrayBuffer::GetRemaining(remaining);
}

ECode ReadOnlyInt32ArrayBuffer::Reset()
{
    return Int32ArrayBuffer::Reset();
}

ECode ReadOnlyInt32ArrayBuffer::Rewind()
{
    return Int32ArrayBuffer::Rewind();
}

ECode ReadOnlyInt32ArrayBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadOnlyInt32ArrayBuffer";
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
