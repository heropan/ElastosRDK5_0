
#include "coredef.h"
#include "DoubleToByteBufferAdapter.h"
#include "ByteBuffer.h"
#include "DirectByteBuffer.h"
#include "HeapByteBuffer.h"
#include "ReadWriteDirectByteBuffer.h"
#include "ReadWriteHeapByteBuffer.h"
#include "elastos/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

DoubleToByteBufferAdapter::DoubleToByteBufferAdapter(
    /* [in] */ IByteBuffer* byteBuffer)
    : DoubleBuffer(((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mCapacity / sizeof(Double))
    , mByteBuffer(byteBuffer)
{
    mByteBuffer->Clear();
    mEffectiveDirectAddress = ((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mEffectiveDirectAddress;
}

ECode DoubleToByteBufferAdapter::AsDoubleBuffer(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ IDoubleBuffer** doubleBuffer)
{
    VALIDATE_NOT_NULL(doubleBuffer)
    VALIDATE_NOT_NULL(byteBuffer)
    AutoPtr<IByteBuffer> slice;
    FAIL_RETURN(byteBuffer->Slice((IByteBuffer**)&slice))
    slice->SetOrder(((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *doubleBuffer = (IDoubleBuffer*) new DoubleToByteBufferAdapter(slice);
    REFCOUNT_ADD(*doubleBuffer);
    return NOERROR;
}

PInterface DoubleToByteBufferAdapter::Probe(
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


UInt32 DoubleToByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 DoubleToByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode DoubleToByteBufferAdapter::GetInterfaceID(
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

ECode DoubleToByteBufferAdapter::GetPrimitiveArray(
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

ECode DoubleToByteBufferAdapter::GetArray(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    return DoubleBuffer::GetArray(array);
}

ECode DoubleToByteBufferAdapter::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode DoubleToByteBufferAdapter::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode DoubleToByteBufferAdapter::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return DoubleBuffer::GetArrayOffset(offset);
}

ECode DoubleToByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> byteBuf;
    FAIL_RETURN(mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&byteBuf))
    AutoPtr<DoubleToByteBufferAdapter> buf = new DoubleToByteBufferAdapter(byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    buf->mByteBuffer->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (IDoubleBuffer*)buf->Probe(EIID_IDoubleBuffer);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DoubleToByteBufferAdapter::Compact()
{
    Boolean isReadOnly = FALSE;
    mByteBuffer->IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    mByteBuffer->SetLimit(mLimit * sizeof(Double));
    mByteBuffer->SetPosition(mPosition * sizeof(Double));
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode DoubleToByteBufferAdapter::CompareTo(
    /* [in] */ IDoubleBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return DoubleBuffer::CompareTo(otherBuffer, result);
}

ECode DoubleToByteBufferAdapter::Duplicate(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Duplicate((IByteBuffer**)&bb))
    bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    AutoPtr<DoubleToByteBufferAdapter> buf = new DoubleToByteBufferAdapter(bb);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    *buffer = (IDoubleBuffer*)buf->Probe(EIID_IDoubleBuffer);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DoubleToByteBufferAdapter::GetDouble(
    /* [out] */ Double* value)
{
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    return mByteBuffer->GetDouble(mPosition++ * sizeof(Double), value);
}

ECode DoubleToByteBufferAdapter::GetDouble(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->GetDouble(index * sizeof(Double), value);
}

ECode DoubleToByteBufferAdapter::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst)
{
    return DoubleBuffer::GetDoubles(dst);
}

ECode DoubleToByteBufferAdapter::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 doubleCount)
{
    mByteBuffer->SetLimit(mLimit * sizeof(Double));
    mByteBuffer->SetPosition(mPosition * sizeof(Double));
    if (mByteBuffer->Probe(EIID_DirectByteBuffer) != NULL) {
        FAIL_RETURN(((DirectByteBuffer*)mByteBuffer->Probe(EIID_DirectByteBuffer))->GetDoubles(dst, dstOffset, doubleCount))
    }
    else {
        FAIL_RETURN(((HeapByteBuffer*)mByteBuffer->Probe(EIID_HeapByteBuffer))->GetDoubles(dst, dstOffset, doubleCount))
    }
    mPosition += doubleCount;
    return NOERROR;
}

ECode DoubleToByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return mByteBuffer->GetOrder(order);
}

ECode DoubleToByteBufferAdapter::PutDouble(
    /* [in] */ Double c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    return mByteBuffer->PutDouble(mPosition++ * sizeof(Double), c);
}

ECode DoubleToByteBufferAdapter::PutDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double c)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->PutDouble(index * sizeof(Double), c);
}

ECode DoubleToByteBufferAdapter::PutDoubles(
    /* [in] */ const ArrayOf<Double>& src)
{
    return DoubleBuffer::PutDoubles(src);
}

ECode DoubleToByteBufferAdapter::PutDoubles(
    /* [in] */ const ArrayOf<Double>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 doubleCount)
{
    mByteBuffer->SetLimit(mLimit * sizeof(Double));
    mByteBuffer->SetPosition(mPosition * sizeof(Double));
    if (mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer) != NULL ) {
        FAIL_RETURN(((ReadWriteDirectByteBuffer*)mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer))->PutDoubles(
                src, srcOffset, doubleCount))
    }
    else {
        FAIL_RETURN(((ReadWriteHeapByteBuffer*)mByteBuffer.Get())->PutDoubles(src, srcOffset, doubleCount))
    }
    mPosition += doubleCount;
    return NOERROR;
}

ECode DoubleToByteBufferAdapter::PutDoubleBuffer(
    /* [in] */ IDoubleBuffer* src)
{
    return DoubleBuffer::PutDoubleBuffer(src);
}

ECode DoubleToByteBufferAdapter::Slice(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    mByteBuffer->SetLimit(mLimit * sizeof(Double));
    mByteBuffer->SetPosition(mPosition * sizeof(Double));
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Slice((IByteBuffer**)&bb))
    bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (IDoubleBuffer*) new DoubleToByteBufferAdapter(bb);
    REFCOUNT_ADD(*buffer)
    mByteBuffer->Clear();
    return NOERROR;
}

ECode DoubleToByteBufferAdapter::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "DoubleToByteBufferAdapter";
    buf += ", status: capacity=";
    buf += mCapacity;
    buf += " position=";
    buf += mPosition;
    buf += " limit=";
    buf += mLimit;
    return buf.ToString(str);
}

ECode DoubleToByteBufferAdapter::GetCapacity(
    /* [out] */ Int32* cap)
{
    return DoubleBuffer::GetCapacity(cap);
}

ECode DoubleToByteBufferAdapter::Clear()
{
    return DoubleBuffer::Clear();
}

ECode DoubleToByteBufferAdapter::Flip()
{
    return DoubleBuffer::Flip();
}

ECode DoubleToByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return DoubleBuffer::HasArray(hasArray);
}

ECode DoubleToByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return DoubleBuffer::HasRemaining(hasRemaining);
}

ECode DoubleToByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return mByteBuffer->IsDirect(isDirect);
}

ECode DoubleToByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return mByteBuffer->IsReadOnly(isReadOnly);
}

CARAPI DoubleToByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

CARAPI DoubleToByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

CARAPI DoubleToByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode DoubleToByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return DoubleBuffer::GetLimit(limit);
}

ECode DoubleToByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return DoubleBuffer::SetLimit(newLimit);
}

ECode DoubleToByteBufferAdapter::Mark()
{
    return DoubleBuffer::Mark();
}

ECode DoubleToByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return DoubleBuffer::GetPosition(pos);
}

ECode DoubleToByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return DoubleBuffer::SetPosition(newPosition);
}

ECode DoubleToByteBufferAdapter::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return DoubleBuffer::GetRemaining(remaining);
}

ECode DoubleToByteBufferAdapter::Reset()
{
    return DoubleBuffer::Reset();
}

ECode DoubleToByteBufferAdapter::Rewind()
{
    return DoubleBuffer::Rewind();
}

ECode DoubleToByteBufferAdapter::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* rst)
{
    return DoubleBuffer::Equals(other, rst);
}

} // namespace IO
} // namespace Elastos
