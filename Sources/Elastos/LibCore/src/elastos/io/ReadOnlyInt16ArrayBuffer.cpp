
#include "coredef.h"
#include "ReadOnlyInt16ArrayBuffer.h"
#include "elastos/core/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

ReadOnlyInt16ArrayBuffer::ReadOnlyInt16ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int16>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : Int16ArrayBuffer(capacity, backingArray, arrayOffset)
{}

AutoPtr<ReadOnlyInt16ArrayBuffer> ReadOnlyInt16ArrayBuffer::Copy(
    /* [in] */ Int16ArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    assert(0 && "TODO");
    AutoPtr<ReadOnlyInt16ArrayBuffer> buf; // = new ReadOnlyInt16ArrayBuffer(other->mCapacity,
            // other->mBackingArray, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

PInterface ReadOnlyInt16ArrayBuffer::Probe(
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


UInt32 ReadOnlyInt16ArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyInt16ArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyInt16ArrayBuffer::GetInterfaceID(
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

ECode ReadOnlyInt16ArrayBuffer::GetPrimitiveArray(
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

ECode ReadOnlyInt16ArrayBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    return Int16ArrayBuffer::GetArray(array);
}

ECode ReadOnlyInt16ArrayBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadOnlyInt16ArrayBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadOnlyInt16ArrayBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int16ArrayBuffer::GetArrayOffset(offset);
}

ECode ReadOnlyInt16ArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IInt16Buffer** buffer)
{
    return Duplicate(buffer);
}

ECode ReadOnlyInt16ArrayBuffer::Compact()
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt16ArrayBuffer::CompareTo(
    /* [in] */ IInt16Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int16ArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyInt16ArrayBuffer::Duplicate(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<ReadOnlyInt16ArrayBuffer> buf = Copy((Int16ArrayBuffer*)this, mMark);
    *buffer = (IInt16Buffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadOnlyInt16ArrayBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    return Int16ArrayBuffer::GetInt16(value);
}

ECode ReadOnlyInt16ArrayBuffer::GetInt16(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return Int16ArrayBuffer::GetInt16(index, value);
}

ECode ReadOnlyInt16ArrayBuffer::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst)
{
    return Int16ArrayBuffer::GetInt16s(dst, 0, dst->GetLength());
}

ECode ReadOnlyInt16ArrayBuffer::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int16Count)
{
    return Int16ArrayBuffer::GetInt16s(dst, dstOffset, int16Count);
}

ECode ReadOnlyInt16ArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return Int16ArrayBuffer::GetOrder(order);
}

ECode ReadOnlyInt16ArrayBuffer::PutInt16(
    /* [in] */ Int16 c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt16ArrayBuffer::PutInt16(
    /* [in] */ Int32 index,
    /* [in] */ Int16 c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt16ArrayBuffer::PutInt16s(
    /* [in] */ ArrayOf<Int16>* src)
{
    return Int16ArrayBuffer::Put(src);
}

ECode ReadOnlyInt16ArrayBuffer::PutInt16s(
    /* [in] */ ArrayOf<Int16>* src,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt16ArrayBuffer::PutInt16Buffer(
    /* [in] */ IInt16Buffer* src)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt16ArrayBuffer::Slice(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    assert(0 && "TODO");
    // *buffer = (IInt16Buffer*)new ReadOnlyInt16ArrayBuffer(remaining, mBackingArray,
            // mOffset + mPosition);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadOnlyInt16ArrayBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return Int16ArrayBuffer::GetCapacity(cap);
}

ECode ReadOnlyInt16ArrayBuffer::Clear()
{
    return Int16ArrayBuffer::Clear();
}

ECode ReadOnlyInt16ArrayBuffer::Flip()
{
    return Int16ArrayBuffer::Flip();
}

ECode ReadOnlyInt16ArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int16ArrayBuffer::HasArray(hasArray);
}

ECode ReadOnlyInt16ArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int16ArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyInt16ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return Int16ArrayBuffer::IsDirect(isDirect);
}

ECode ReadOnlyInt16ArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

CARAPI ReadOnlyInt16ArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

CARAPI ReadOnlyInt16ArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

CARAPI ReadOnlyInt16ArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode ReadOnlyInt16ArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int16ArrayBuffer::GetLimit(limit);
}

ECode ReadOnlyInt16ArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int16ArrayBuffer::SetLimit(newLimit);
}

ECode ReadOnlyInt16ArrayBuffer::Mark()
{
    return Int16ArrayBuffer::Mark();
}

ECode ReadOnlyInt16ArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int16ArrayBuffer::GetPosition(pos);
}

ECode ReadOnlyInt16ArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int16ArrayBuffer::SetPosition(newPosition);
}

ECode ReadOnlyInt16ArrayBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return Int16ArrayBuffer::GetRemaining(remaining);
}

ECode ReadOnlyInt16ArrayBuffer::Reset()
{
    return Int16ArrayBuffer::Reset();
}

ECode ReadOnlyInt16ArrayBuffer::Rewind()
{
    return Int16ArrayBuffer::Rewind();
}

ECode ReadOnlyInt16ArrayBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadOnlyInt16ArrayBuffer";
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
