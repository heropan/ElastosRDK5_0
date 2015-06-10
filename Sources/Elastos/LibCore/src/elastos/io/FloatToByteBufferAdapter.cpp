
#include "FloatToByteBufferAdapter.h"
#include "ByteBuffer.h"
#include "DirectByteBuffer.h"
#include "HeapByteBuffer.h"
#include "ReadWriteDirectByteBuffer.h"
#include "ReadWriteHeapByteBuffer.h"
#include "elastos/core/StringBuilder.h"
#include "cutils/log.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

FloatToByteBufferAdapter::FloatToByteBufferAdapter(
    /* [in] */ IByteBuffer* byteBuffer)
    : FloatBuffer(((ByteBuffer*)byteBuffer)->mCapacity / sizeof(Float))
    , mByteBuffer(byteBuffer)
{
    IBuffer::Probe(mByteBuffer)->Clear();
    mEffectiveDirectAddress = (reinterpret_cast<ByteBuffer*>(byteBuffer->Probe(EIID_ByteBuffer)))->mEffectiveDirectAddress;
}

ECode FloatToByteBufferAdapter::AsFloatBuffer(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ IFloatBuffer** floatBuffer)
{
    VALIDATE_NOT_NULL(floatBuffer)
    VALIDATE_NOT_NULL(byteBuffer)
    AutoPtr<IByteBuffer> slice;
    FAIL_RETURN(byteBuffer->Slice((IByteBuffer**)&slice))
    slice->SetOrder((reinterpret_cast<ByteBuffer*>(byteBuffer->Probe(EIID_ByteBuffer)))->mOrder);
    *floatBuffer = (IFloatBuffer*) new FloatToByteBufferAdapter(slice);
    REFCOUNT_ADD(*floatBuffer);
    return NOERROR;
}

PInterface FloatToByteBufferAdapter::Probe(
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


UInt32 FloatToByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 FloatToByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode FloatToByteBufferAdapter::GetInterfaceID(
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

ECode FloatToByteBufferAdapter::GetPrimitiveArray(
    /* [out] */ Handle32* arrayHandle)
{
    mArrayTemp = NULL;
    GetArray((ArrayOf<Float>**)&mArrayTemp);
    if (mArrayTemp == NULL)
    {
        assert(0 && "TODO");
        // return IBuffer::Probe(mByteBuffer)->GetPrimitiveArray(arrayHandle);
        *arrayHandle = 0;
        return NOERROR;
    }
    Float* primitiveArray = mArrayTemp->GetPayload();
    *arrayHandle = reinterpret_cast<Handle32>(primitiveArray);
    return NOERROR;
}

ECode FloatToByteBufferAdapter::GetArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    return FloatBuffer::GetArray(array);
}

ECode FloatToByteBufferAdapter::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode FloatToByteBufferAdapter::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode FloatToByteBufferAdapter::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return FloatBuffer::GetArrayOffset(offset);
}

ECode FloatToByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> byteBuf;
    FAIL_RETURN(mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&byteBuf))
    AutoPtr<FloatToByteBufferAdapter> buf = new FloatToByteBufferAdapter(byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    assert(0 && "TODO");
    // buf->mByteBuffer->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (IFloatBuffer*)buf->Probe(EIID_IFloatBuffer);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode FloatToByteBufferAdapter::Compact()
{
    Boolean isReadOnly = FALSE;
    IBuffer::Probe(mByteBuffer)->IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    IBuffer::Probe(mByteBuffer)->SetLimit(mLimit * sizeof(Float));
    IBuffer::Probe(mByteBuffer)->SetPosition(mPosition * sizeof(Float));
    mByteBuffer->Compact();
    IBuffer::Probe(mByteBuffer)->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode FloatToByteBufferAdapter::CompareTo(
    /* [in] */ IFloatBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return FloatBuffer::CompareTo(otherBuffer, result);
}

ECode FloatToByteBufferAdapter::Duplicate(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Duplicate((IByteBuffer**)&bb))
    assert(0 && "TODO");
    // bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    AutoPtr<FloatToByteBufferAdapter> buf = new FloatToByteBufferAdapter(bb);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    *buffer = (IFloatBuffer*)buf->Probe(EIID_IFloatBuffer);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode FloatToByteBufferAdapter::GetFloat(
    /* [out] */ Float* value)
{
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    return mByteBuffer->GetFloat(mPosition++ * sizeof(Float), value);
}

ECode FloatToByteBufferAdapter::GetFloat(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->GetFloat(index * sizeof(Float), value);
}

ECode FloatToByteBufferAdapter::GetFloats(
    /* [out] */ ArrayOf<Float>* dst)
{
    return FloatBuffer::GetFloats(dst);
}

ECode FloatToByteBufferAdapter::GetFloats(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 floatCount)
{
    IBuffer::Probe(mByteBuffer)->SetLimit(mLimit * sizeof(Float));
    IBuffer::Probe(mByteBuffer)->SetPosition(mPosition * sizeof(Float));
    if (mByteBuffer->Probe(EIID_DirectByteBuffer) != NULL) {
        assert(0 && "TODO");
        // FAIL_RETURN(((DirectByteBuffer*)mByteBuffer->Probe(EIID_DirectByteBuffer))->GetFloats(dst, dstOffset, floatCount))
    }
    else {
        assert(0 && "TODO");
        // FAIL_RETURN(((HeapByteBuffer*)mByteBuffer->Probe(EIID_HeapByteBuffer))->GetFloats(dst, dstOffset, floatCount))
    }
    mPosition += floatCount;
    return NOERROR;
}

ECode FloatToByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return mByteBuffer->GetOrder(order);
}

ECode FloatToByteBufferAdapter::PutFloat(
    /* [in] */ Float c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    return mByteBuffer->PutFloat(mPosition++ * sizeof(Float), c);
}

ECode FloatToByteBufferAdapter::PutFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float c)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->PutFloat(index * sizeof(Float), c);
}

ECode FloatToByteBufferAdapter::PutFloats(
    /* [in] */ const ArrayOf<Float>& src)
{
    return FloatBuffer::PutFloats(src);
}

ECode FloatToByteBufferAdapter::PutFloats(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 floatCount)
{
    IBuffer::Probe(mByteBuffer)->SetLimit(mLimit * sizeof(Float));
    IBuffer::Probe(mByteBuffer)->SetPosition(mPosition * sizeof(Float));
    if (mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer) != NULL ) {
        ReadWriteDirectByteBuffer* tmp = reinterpret_cast<ReadWriteDirectByteBuffer*>(
            mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer));
        FAIL_RETURN(tmp->PutFloats(src, srcOffset, floatCount))
    }
    else {
        FAIL_RETURN(((ReadWriteHeapByteBuffer*)mByteBuffer.Get())->PutFloats(src, srcOffset, floatCount))
    }
    mPosition += floatCount;
    return NOERROR;
}

ECode FloatToByteBufferAdapter::PutFloatBuffer(
    /* [in] */ IFloatBuffer* src)
{
    return FloatBuffer::PutFloatBuffer(src);
}

ECode FloatToByteBufferAdapter::Slice(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    IBuffer::Probe(mByteBuffer)->SetLimit(mLimit * sizeof(Float));
    IBuffer::Probe(mByteBuffer)->SetPosition(mPosition * sizeof(Float));
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Slice((IByteBuffer**)&bb))
    assert(0 && "TODO");
    // bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (IFloatBuffer*) new FloatToByteBufferAdapter(bb);
    REFCOUNT_ADD(*buffer)
    IBuffer::Probe(mByteBuffer)->Clear();
    return NOERROR;
}

ECode FloatToByteBufferAdapter::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "FloatToByteBufferAdapter";
    buf += ", status: capacity=";
    buf += mCapacity;
    buf += " position=";
    buf += mPosition;
    buf += " limit=";
    buf += mLimit;
    return buf.ToString(str);
}

ECode FloatToByteBufferAdapter::GetCapacity(
    /* [out] */ Int32* cap)
{
    return FloatBuffer::GetCapacity(cap);
}

ECode FloatToByteBufferAdapter::Clear()
{
    return FloatBuffer::Clear();
}

ECode FloatToByteBufferAdapter::Flip()
{
    return FloatBuffer::Flip();
}

ECode FloatToByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return FloatBuffer::HasArray(hasArray);
}

ECode FloatToByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return FloatBuffer::HasRemaining(hasRemaining);
}

ECode FloatToByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return IBuffer::Probe(mByteBuffer)->IsDirect(isDirect);
}

ECode FloatToByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return IBuffer::Probe(mByteBuffer)->IsReadOnly(isReadOnly);
}

CARAPI FloatToByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

CARAPI FloatToByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

CARAPI FloatToByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode FloatToByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return FloatBuffer::GetLimit(limit);
}

ECode FloatToByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return FloatBuffer::SetLimit(newLimit);
}

ECode FloatToByteBufferAdapter::Mark()
{
    return FloatBuffer::Mark();
}

ECode FloatToByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return FloatBuffer::GetPosition(pos);
}

ECode FloatToByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return FloatBuffer::SetPosition(newPosition);
}

ECode FloatToByteBufferAdapter::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return FloatBuffer::GetRemaining(remaining);
}

ECode FloatToByteBufferAdapter::Reset()
{
    return FloatBuffer::Reset();
}

ECode FloatToByteBufferAdapter::Rewind()
{
    return FloatBuffer::Rewind();
}

} // namespace IO
} // namespace Elastos
