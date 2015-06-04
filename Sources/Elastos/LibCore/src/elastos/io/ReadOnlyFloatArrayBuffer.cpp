
#include "coredef.h"
#include "ReadOnlyFloatArrayBuffer.h"
#include "elastos/core/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

ReadOnlyFloatArrayBuffer::ReadOnlyFloatArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Float>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : FloatArrayBuffer(capacity, backingArray, arrayOffset)
{}

AutoPtr<ReadOnlyFloatArrayBuffer> ReadOnlyFloatArrayBuffer::Copy(
    /* [in] */ FloatArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    AutoPtr<ReadOnlyFloatArrayBuffer> buf = new ReadOnlyFloatArrayBuffer(other->mCapacity,
            other->mBackingArray, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

PInterface ReadOnlyFloatArrayBuffer::Probe(
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


UInt32 ReadOnlyFloatArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyFloatArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyFloatArrayBuffer::GetInterfaceID(
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

ECode ReadOnlyFloatArrayBuffer::GetPrimitiveArray(
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

ECode ReadOnlyFloatArrayBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    return FloatArrayBuffer::GetArray(array);
}

ECode ReadOnlyFloatArrayBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadOnlyFloatArrayBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadOnlyFloatArrayBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return FloatArrayBuffer::GetArrayOffset(offset);
}

ECode ReadOnlyFloatArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    return Duplicate(buffer);
}

ECode ReadOnlyFloatArrayBuffer::Compact()
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyFloatArrayBuffer::CompareTo(
    /* [in] */ IFloatBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return FloatArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyFloatArrayBuffer::Duplicate(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<ReadOnlyFloatArrayBuffer> buf = Copy((FloatArrayBuffer*)this, mMark);
    *buffer = (IFloatBuffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadOnlyFloatArrayBuffer::GetFloat(
    /* [out] */ Float* value)
{
    return FloatArrayBuffer::GetFloat(value);
}

ECode ReadOnlyFloatArrayBuffer::GetFloat(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return FloatArrayBuffer::GetFloat(index, value);
}

ECode ReadOnlyFloatArrayBuffer::GetFloats(
    /* [out] */ ArrayOf<Float>* dst)
{
    return FloatArrayBuffer::GetFloats(dst, 0, dst->GetLength());
}

ECode ReadOnlyFloatArrayBuffer::GetFloats(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 floatCount)
{
    return FloatArrayBuffer::GetFloats(dst, dstOffset, floatCount);
}

ECode ReadOnlyFloatArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return FloatArrayBuffer::GetOrder(order);
}

ECode ReadOnlyFloatArrayBuffer::PutFloat(
    /* [in] */ Float c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyFloatArrayBuffer::PutFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyFloatArrayBuffer::PutFloats(
    /* [in] */ const ArrayOf<Float>& src)
{
    return FloatArrayBuffer::PutFloats(src);
}

ECode ReadOnlyFloatArrayBuffer::PutFloats(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 floatCount)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyFloatArrayBuffer::PutFloatBuffer(
    /* [in] */ IFloatBuffer* src)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyFloatArrayBuffer::Slice(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    *buffer = (IFloatBuffer*)new ReadOnlyFloatArrayBuffer(remaining, mBackingArray, mOffset + mPosition);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadOnlyFloatArrayBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return FloatArrayBuffer::GetCapacity(cap);
}

ECode ReadOnlyFloatArrayBuffer::Clear()
{
    return FloatArrayBuffer::Clear();
}

ECode ReadOnlyFloatArrayBuffer::Flip()
{
    return FloatArrayBuffer::Flip();
}

ECode ReadOnlyFloatArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return FloatArrayBuffer::HasArray(hasArray);
}

ECode ReadOnlyFloatArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return FloatArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyFloatArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return FloatArrayBuffer::IsDirect(isDirect);
}

ECode ReadOnlyFloatArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

CARAPI ReadOnlyFloatArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

CARAPI ReadOnlyFloatArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

CARAPI ReadOnlyFloatArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode ReadOnlyFloatArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return FloatArrayBuffer::GetLimit(limit);
}

ECode ReadOnlyFloatArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return FloatArrayBuffer::SetLimit(newLimit);
}

ECode ReadOnlyFloatArrayBuffer::Mark()
{
    return FloatArrayBuffer::Mark();
}

ECode ReadOnlyFloatArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return FloatArrayBuffer::GetPosition(pos);
}

ECode ReadOnlyFloatArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return FloatArrayBuffer::SetPosition(newPosition);
}

ECode ReadOnlyFloatArrayBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return FloatArrayBuffer::GetRemaining(remaining);
}

ECode ReadOnlyFloatArrayBuffer::Reset()
{
    return FloatArrayBuffer::Reset();
}

ECode ReadOnlyFloatArrayBuffer::Rewind()
{
    return FloatArrayBuffer::Rewind();
}

ECode ReadOnlyFloatArrayBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "ReadOnlyFloatArrayBuffer";
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
