
#include "coredef.h"
#include "ReadWriteFloatArrayBuffer.h"
#include "ReadOnlyFloatArrayBuffer.h"
#include "elastos/core/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

ReadWriteFloatArrayBuffer::ReadWriteFloatArrayBuffer(
    /* [in] */ ArrayOf<Float>* array)
    : FloatArrayBuffer(array)
{}

ReadWriteFloatArrayBuffer::ReadWriteFloatArrayBuffer(
    /* [in] */ Int32 capacity)
    : FloatArrayBuffer(capacity)
{}

ReadWriteFloatArrayBuffer::ReadWriteFloatArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Float>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : FloatArrayBuffer(capacity, backingArray, arrayOffset)
{}

AutoPtr<ReadWriteFloatArrayBuffer> ReadWriteFloatArrayBuffer::Copy(
    /* [in] */ FloatArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    AutoPtr<ReadWriteFloatArrayBuffer> buf = new ReadWriteFloatArrayBuffer(other->mCapacity,
            other->mBackingArray, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

PInterface ReadWriteFloatArrayBuffer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        assert(0 && "TODO");
        // return (PInterface)this;
    }
    else if (riid == EIID_IFloatBuffer) {
        return (IFloatBuffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_FloatBuffer) {
        return reinterpret_cast<PInterface>((FloatBuffer*)this);
    }

    return NULL;
}

UInt32 ReadWriteFloatArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteFloatArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteFloatArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(IFloatBuffer*)this) {
        *pIID = EIID_IFloatBuffer;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::GetPrimitiveArray(
    /* [out] */ Handle32* arrayHandle)
{
    mArrayTemp = NULL;
    GetArray((ArrayOf<Float>**)&mArrayTemp);
    if (mArrayTemp == NULL)
    {
        *arrayHandle = 0;
        return NOERROR;
    }
    Float* primitiveArray = mArrayTemp->GetPayload();
    *arrayHandle = reinterpret_cast<Handle32>(primitiveArray);
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    return FloatArrayBuffer::GetArray(array);
}

ECode ReadWriteFloatArrayBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadWriteFloatArrayBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadWriteFloatArrayBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return FloatArrayBuffer::GetArrayOffset(offset);
}

ECode ReadWriteFloatArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<ReadOnlyFloatArrayBuffer> buf = ReadOnlyFloatArrayBuffer::Copy((FloatArrayBuffer*)this, mMark);
    *buffer = (IFloatBuffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::Compact()
{
    Int32 remaining = 0;
    GetRemaining(&remaining);
    mBackingArray->Copy(mOffset, mBackingArray, mPosition + mOffset, remaining);
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::CompareTo(
    /* [in] */ IFloatBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return FloatArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteFloatArrayBuffer::Duplicate(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    *buffer = (IFloatBuffer*)Copy((FloatArrayBuffer*)this, mMark);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::GetFloat(
    /* [out] */ Float* value)
{
    return FloatArrayBuffer::GetFloat(value);
}

ECode ReadWriteFloatArrayBuffer::GetFloat(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return FloatArrayBuffer::GetFloat(index, value);
}

ECode ReadWriteFloatArrayBuffer::GetFloats(
    /* [out] */ ArrayOf<Float>* dst)
{
    return FloatArrayBuffer::GetFloats(dst, 0, dst->GetLength());
}

ECode ReadWriteFloatArrayBuffer::GetFloats(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 floatCount)
{
    return FloatArrayBuffer::GetFloats(dst, dstOffset, floatCount);
}

ECode ReadWriteFloatArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return FloatArrayBuffer::GetOrder(order);
}

ECode ReadWriteFloatArrayBuffer::PutFloat(
    /* [in] */ Float c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    (*mBackingArray)[mOffset + mPosition++] = c;
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::PutFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float c)
{
    FAIL_RETURN(CheckIndex(index))
    (*mBackingArray)[mOffset + index] = c;
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::PutFloats(
    /* [in] */ const ArrayOf<Float>& src)
{
    return FloatArrayBuffer::PutFloats(src);
}

ECode ReadWriteFloatArrayBuffer::PutFloats(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 floatCount)
{
    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (floatCount > remaining) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    mBackingArray->Copy(mOffset + mPosition, &src, srcOffset, floatCount);
    mPosition += floatCount;
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::PutFloatBuffer(
    /* [in] */ IFloatBuffer* src)
{
    return FloatArrayBuffer::PutFloatBuffer(src);
}

ECode ReadWriteFloatArrayBuffer::Slice(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    *buffer = (IFloatBuffer*)new ReadWriteFloatArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return FloatArrayBuffer::GetCapacity(cap);
}

ECode ReadWriteFloatArrayBuffer::Clear()
{
    return FloatArrayBuffer::Clear();
}

ECode ReadWriteFloatArrayBuffer::Flip()
{
    return FloatArrayBuffer::Flip();
}

ECode ReadWriteFloatArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return FloatArrayBuffer::HasArray(hasArray);
}

ECode ReadWriteFloatArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return FloatArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteFloatArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return FloatArrayBuffer::IsDirect(isDirect);
}

ECode ReadWriteFloatArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

CARAPI ReadWriteFloatArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

CARAPI ReadWriteFloatArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

CARAPI ReadWriteFloatArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return FloatArrayBuffer::GetLimit(limit);
}

ECode ReadWriteFloatArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return FloatArrayBuffer::SetLimit(newLimit);
}

ECode ReadWriteFloatArrayBuffer::Mark()
{
    return FloatArrayBuffer::Mark();
}

ECode ReadWriteFloatArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return FloatArrayBuffer::GetPosition(pos);
}

ECode ReadWriteFloatArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return FloatArrayBuffer::SetPosition(newPosition);
}

ECode ReadWriteFloatArrayBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return FloatArrayBuffer::GetRemaining(remaining);
}

ECode ReadWriteFloatArrayBuffer::Reset()
{
    return FloatArrayBuffer::Reset();
}

ECode ReadWriteFloatArrayBuffer::Rewind()
{
    return FloatArrayBuffer::Rewind();
}

ECode ReadWriteFloatArrayBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadWriteFloatArrayBuffer";
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
