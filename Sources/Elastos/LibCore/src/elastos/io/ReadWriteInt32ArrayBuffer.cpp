
#include "coredef.h"
#include "ReadWriteInt32ArrayBuffer.h"
#include "ReadOnlyInt32ArrayBuffer.h"
#include "elastos/core/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

ReadWriteInt32ArrayBuffer::ReadWriteInt32ArrayBuffer(
    /* [in] */ ArrayOf<Int32>* array)
    : Int32ArrayBuffer(array)
{}

ReadWriteInt32ArrayBuffer::ReadWriteInt32ArrayBuffer(
    /* [in] */ Int32 capacity)
    : Int32ArrayBuffer(capacity)
{}

ReadWriteInt32ArrayBuffer::ReadWriteInt32ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int32>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : Int32ArrayBuffer(capacity, backingArray, arrayOffset)
{}

AutoPtr<ReadWriteInt32ArrayBuffer> ReadWriteInt32ArrayBuffer::Copy(
    /* [in] */ Int32ArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    assert(0 && "TODO");
    AutoPtr<ReadWriteInt32ArrayBuffer> buf; // = new ReadWriteInt32ArrayBuffer(other->mCapacity,
            // other->mBackingArray, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

PInterface ReadWriteInt32ArrayBuffer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        assert(0 && "TODO");
        // return (PInterface)this;
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

UInt32 ReadWriteInt32ArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteInt32ArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteInt32ArrayBuffer::GetInterfaceID(
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

ECode ReadWriteInt32ArrayBuffer::GetPrimitiveArray(
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

ECode ReadWriteInt32ArrayBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    return Int32ArrayBuffer::GetArray(array);
}

ECode ReadWriteInt32ArrayBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadWriteInt32ArrayBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadWriteInt32ArrayBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int32ArrayBuffer::GetArrayOffset(offset);
}

ECode ReadWriteInt32ArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<ReadOnlyInt32ArrayBuffer> buf = ReadOnlyInt32ArrayBuffer::Copy((Int32ArrayBuffer*)this, mMark);
    *buffer = (IInt32Buffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::Compact()
{
    Int32 remaining = 0;
    GetRemaining(&remaining);
    mBackingArray->Copy(mOffset, mBackingArray, mPosition + mOffset, remaining);
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::CompareTo(
    /* [in] */ IInt32Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int32ArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteInt32ArrayBuffer::Duplicate(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    *buffer = (IInt32Buffer*)Copy((Int32ArrayBuffer*)this, mMark);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    return Int32ArrayBuffer::GetInt32(value);
}

ECode ReadWriteInt32ArrayBuffer::GetInt32(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return Int32ArrayBuffer::GetInt32(index, value);
}

ECode ReadWriteInt32ArrayBuffer::GetInt32s(
    /* [out] */ ArrayOf<Int32>* dst)
{
    return Int32ArrayBuffer::GetInt32s(dst, 0, dst->GetLength());
}

ECode ReadWriteInt32ArrayBuffer::GetInt32s(
    /* [out] */ ArrayOf<Int32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int32Count)
{
    return Int32ArrayBuffer::GetInt32s(dst, dstOffset, int32Count);
}

ECode ReadWriteInt32ArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return Int32ArrayBuffer::GetOrder(order);
}

ECode ReadWriteInt32ArrayBuffer::PutInt32(
    /* [in] */ Int32 c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    (*mBackingArray)[mOffset + mPosition++] = c;
    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::PutInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32 c)
{
    FAIL_RETURN(CheckIndex(index))
    (*mBackingArray)[mOffset + index] = c;
    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::PutInt32s(
    /* [in] */ const ArrayOf<Int32>& src)
{
    return Int32ArrayBuffer::PutInt32s(src);
}

ECode ReadWriteInt32ArrayBuffer::PutInt32s(
    /* [in] */ const ArrayOf<Int32>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int32Count)
{
    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (int32Count > remaining) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    mBackingArray->Copy(mOffset + mPosition, &src, srcOffset, int32Count);
    mPosition += int32Count;
    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::PutInt32Buffer(
    /* [in] */ IInt32Buffer* src)
{
    return Int32ArrayBuffer::PutInt32Buffer(src);
}

ECode ReadWriteInt32ArrayBuffer::Slice(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    assert(0 && "TODO");
    // *buffer = (IInt32Buffer*)new ReadWriteInt32ArrayBuffer(remaining, mBackingArray,
            // mOffset + mPosition);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return Int32ArrayBuffer::GetCapacity(cap);
}

ECode ReadWriteInt32ArrayBuffer::Clear()
{
    return Int32ArrayBuffer::Clear();
}

ECode ReadWriteInt32ArrayBuffer::Flip()
{
    return Int32ArrayBuffer::Flip();
}

ECode ReadWriteInt32ArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int32ArrayBuffer::HasArray(hasArray);
}

ECode ReadWriteInt32ArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int32ArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteInt32ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return Int32ArrayBuffer::IsDirect(isDirect);
}

ECode ReadWriteInt32ArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

CARAPI ReadWriteInt32ArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

CARAPI ReadWriteInt32ArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

CARAPI ReadWriteInt32ArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int32ArrayBuffer::GetLimit(limit);
}

ECode ReadWriteInt32ArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int32ArrayBuffer::SetLimit(newLimit);
}

ECode ReadWriteInt32ArrayBuffer::Mark()
{
    return Int32ArrayBuffer::Mark();
}

ECode ReadWriteInt32ArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int32ArrayBuffer::GetPosition(pos);
}

ECode ReadWriteInt32ArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int32ArrayBuffer::SetPosition(newPosition);
}

ECode ReadWriteInt32ArrayBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return Int32ArrayBuffer::GetRemaining(remaining);
}

ECode ReadWriteInt32ArrayBuffer::Reset()
{
    return Int32ArrayBuffer::Reset();
}

ECode ReadWriteInt32ArrayBuffer::Rewind()
{
    return Int32ArrayBuffer::Rewind();
}

ECode ReadWriteInt32ArrayBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadWriteInt32ArrayBuffer";
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
