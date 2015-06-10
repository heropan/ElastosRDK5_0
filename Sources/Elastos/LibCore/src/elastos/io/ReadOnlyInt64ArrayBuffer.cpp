
#include "ReadOnlyInt64ArrayBuffer.h"
#include "elastos/core/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

ReadOnlyInt64ArrayBuffer::ReadOnlyInt64ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int64>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : Int64ArrayBuffer(capacity, backingArray, arrayOffset)
{}

AutoPtr<ReadOnlyInt64ArrayBuffer> ReadOnlyInt64ArrayBuffer::Copy(
    /* [in] */ Int64ArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    assert(0 && "TODO");
    AutoPtr<ReadOnlyInt64ArrayBuffer> buf; // = new ReadOnlyInt64ArrayBuffer(other->mCapacity,
            // other->mBackingArray, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

PInterface ReadOnlyInt64ArrayBuffer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        assert(0 && "TODO");
        // return (PInterface)this;
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

UInt32 ReadOnlyInt64ArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyInt64ArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyInt64ArrayBuffer::GetInterfaceID(
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

ECode ReadOnlyInt64ArrayBuffer::GetPrimitiveArray(
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

ECode ReadOnlyInt64ArrayBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    return Int64ArrayBuffer::GetArray(array);
}

ECode ReadOnlyInt64ArrayBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadOnlyInt64ArrayBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadOnlyInt64ArrayBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int64ArrayBuffer::GetArrayOffset(offset);
}

ECode ReadOnlyInt64ArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IInt64Buffer** buffer)
{
    return Duplicate(buffer);
}

ECode ReadOnlyInt64ArrayBuffer::Compact()
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt64ArrayBuffer::CompareTo(
    /* [in] */ IInt64Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int64ArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyInt64ArrayBuffer::Duplicate(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<ReadOnlyInt64ArrayBuffer> buf = Copy((Int64ArrayBuffer*)this, mMark);
    *buffer = (IInt64Buffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadOnlyInt64ArrayBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    return Int64ArrayBuffer::GetInt64(value);
}

ECode ReadOnlyInt64ArrayBuffer::GetInt64(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return Int64ArrayBuffer::GetInt64(index, value);
}

ECode ReadOnlyInt64ArrayBuffer::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst)
{
    return Int64ArrayBuffer::GetInt64s(dst, 0, dst->GetLength());
}

ECode ReadOnlyInt64ArrayBuffer::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int64Count)
{
    return Int64ArrayBuffer::GetInt64s(dst, dstOffset, int64Count);
}

ECode ReadOnlyInt64ArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return Int64ArrayBuffer::GetOrder(order);
}

ECode ReadOnlyInt64ArrayBuffer::PutInt64(
    /* [in] */ Int64 c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt64ArrayBuffer::PutInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt64ArrayBuffer::PutInt64s(
    /* [in] */ const ArrayOf<Int64>& src)
{
    return Int64ArrayBuffer::PutInt64s(src);
}

ECode ReadOnlyInt64ArrayBuffer::PutInt64s(
    /* [in] */ const ArrayOf<Int64>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int64Count)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt64ArrayBuffer::PutInt64Buffer(
    /* [in] */ IInt64Buffer* src)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyInt64ArrayBuffer::Slice(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    assert(0 && "TODO");
    // *buffer = (IInt64Buffer*)new ReadOnlyInt64ArrayBuffer(remaining, mBackingArray,
            // mOffset + mPosition);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadOnlyInt64ArrayBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return Int64ArrayBuffer::GetCapacity(cap);
}

ECode ReadOnlyInt64ArrayBuffer::Clear()
{
    return Int64ArrayBuffer::Clear();
}

ECode ReadOnlyInt64ArrayBuffer::Flip()
{
    return Int64ArrayBuffer::Flip();
}

ECode ReadOnlyInt64ArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int64ArrayBuffer::HasArray(hasArray);
}

ECode ReadOnlyInt64ArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int64ArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyInt64ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return Int64ArrayBuffer::IsDirect(isDirect);
}

ECode ReadOnlyInt64ArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

CARAPI ReadOnlyInt64ArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

CARAPI ReadOnlyInt64ArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

CARAPI ReadOnlyInt64ArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode ReadOnlyInt64ArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int64ArrayBuffer::GetLimit(limit);
}

ECode ReadOnlyInt64ArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int64ArrayBuffer::SetLimit(newLimit);
}

ECode ReadOnlyInt64ArrayBuffer::Mark()
{
    return Int64ArrayBuffer::Mark();
}

ECode ReadOnlyInt64ArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int64ArrayBuffer::GetPosition(pos);
}

ECode ReadOnlyInt64ArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int64ArrayBuffer::SetPosition(newPosition);
}

ECode ReadOnlyInt64ArrayBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return Int64ArrayBuffer::GetRemaining(remaining);
}

ECode ReadOnlyInt64ArrayBuffer::Reset()
{
    return Int64ArrayBuffer::Reset();
}

ECode ReadOnlyInt64ArrayBuffer::Rewind()
{
    return Int64ArrayBuffer::Rewind();
}

ECode ReadOnlyInt64ArrayBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadOnlyInt64ArrayBuffer";
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
