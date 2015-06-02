
#include "coredef.h"
#include "Int32ToByteBufferAdapter.h"
#include "ByteBuffer.h"
#include "DirectByteBuffer.h"
#include "HeapByteBuffer.h"
#include "ReadWriteDirectByteBuffer.h"
#include "ReadWriteHeapByteBuffer.h"
#include "elastos/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

Int32ToByteBufferAdapter::Int32ToByteBufferAdapter(
    /* [in] */ IByteBuffer* byteBuffer)
    : Int32Buffer(((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mCapacity / sizeof(Int32))
    , mByteBuffer(byteBuffer)
{
    mByteBuffer->Clear();
    mEffectiveDirectAddress = ((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mEffectiveDirectAddress;
}

ECode Int32ToByteBufferAdapter::AsInt32Buffer(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ IInt32Buffer** int32Buffer)
{
    VALIDATE_NOT_NULL(int32Buffer)
    VALIDATE_NOT_NULL(byteBuffer)
    AutoPtr<IByteBuffer> slice;
    FAIL_RETURN(byteBuffer->Slice((IByteBuffer**)&slice))
    slice->SetOrder(((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *int32Buffer = (IInt32Buffer*) new Int32ToByteBufferAdapter(slice);
    REFCOUNT_ADD(*int32Buffer);
    return NOERROR;
}

PInterface Int32ToByteBufferAdapter::Probe(
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


UInt32 Int32ToByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Int32ToByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode Int32ToByteBufferAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(Int32Buffer*)this) {
        *pIID = EIID_Int32Buffer;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode Int32ToByteBufferAdapter::GetPrimitiveArray(
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

ECode Int32ToByteBufferAdapter::GetArray(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    return Int32Buffer::GetArray(array);
}

ECode Int32ToByteBufferAdapter::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode Int32ToByteBufferAdapter::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode Int32ToByteBufferAdapter::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int32Buffer::GetArrayOffset(offset);
}

ECode Int32ToByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> byteBuf;
    FAIL_RETURN(mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&byteBuf))
    AutoPtr<Int32ToByteBufferAdapter> buf = new Int32ToByteBufferAdapter(byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    buf->mByteBuffer->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (IInt32Buffer*)buf->Probe(EIID_IInt32Buffer);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode Int32ToByteBufferAdapter::Compact()
{
    Boolean isReadOnly = FALSE;
    mByteBuffer->IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    mByteBuffer->SetLimit(mLimit * sizeof(Int32));
    mByteBuffer->SetPosition(mPosition * sizeof(Int32));
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode Int32ToByteBufferAdapter::CompareTo(
    /* [in] */ IInt32Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int32Buffer::CompareTo(otherBuffer, result);
}

ECode Int32ToByteBufferAdapter::Duplicate(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Duplicate((IByteBuffer**)&bb))
    bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    AutoPtr<Int32ToByteBufferAdapter> buf = new Int32ToByteBufferAdapter(bb);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    *buffer = (IInt32Buffer*)buf->Probe(EIID_IInt32Buffer);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode Int32ToByteBufferAdapter::GetInt32(
    /* [out] */ Int32* value)
{
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    return mByteBuffer->GetInt32(mPosition++ * sizeof(Int32), value);
}

ECode Int32ToByteBufferAdapter::GetInt32(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->GetInt32(index * sizeof(Int32), value);
}

ECode Int32ToByteBufferAdapter::GetInt32s(
    /* [out] */ ArrayOf<Int32>* dst)
{
    return Int32Buffer::GetInt32s(dst);
}

ECode Int32ToByteBufferAdapter::GetInt32s(
    /* [out] */ ArrayOf<Int32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int32Count)
{
    mByteBuffer->SetLimit(mLimit * sizeof(Int32));
    mByteBuffer->SetPosition(mPosition * sizeof(Int32));
    if (mByteBuffer->Probe(EIID_DirectByteBuffer) != NULL) {
        FAIL_RETURN(((DirectByteBuffer*)mByteBuffer->Probe(EIID_DirectByteBuffer))->GetInt32s(dst, dstOffset, int32Count))
    }
    else {
        FAIL_RETURN(((HeapByteBuffer*)mByteBuffer->Probe(EIID_HeapByteBuffer))->GetInt32s(dst, dstOffset, int32Count))
    }
    mPosition += int32Count;
    return NOERROR;
}

ECode Int32ToByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return mByteBuffer->GetOrder(order);
}

ECode Int32ToByteBufferAdapter::PutInt32(
    /* [in] */ Int32 c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    return mByteBuffer->PutInt32(mPosition++ * sizeof(Int32), c);
}

ECode Int32ToByteBufferAdapter::PutInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32 c)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->PutInt32(index * sizeof(Int32), c);
}

ECode Int32ToByteBufferAdapter::PutInt32s(
    /* [in] */ const ArrayOf<Int32>& src)
{
    return Int32Buffer::PutInt32s(src);
}

ECode Int32ToByteBufferAdapter::PutInt32s(
    /* [in] */ const ArrayOf<Int32>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int32Count)
{
    mByteBuffer->SetLimit(mLimit * sizeof(Int32));
    mByteBuffer->SetPosition(mPosition * sizeof(Int32));
    if (mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer) != NULL ) {
        FAIL_RETURN(((ReadWriteDirectByteBuffer*)mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer))->PutInt32s(
                src, srcOffset, int32Count))
    }
    else {
        FAIL_RETURN(((ReadWriteHeapByteBuffer*)mByteBuffer.Get())->PutInt32s(src, srcOffset, int32Count))
    }
    mPosition += int32Count;
    return NOERROR;
}

ECode Int32ToByteBufferAdapter::PutInt32Buffer(
    /* [in] */ IInt32Buffer* src)
{
    return Int32Buffer::PutInt32Buffer(src);
}

ECode Int32ToByteBufferAdapter::Slice(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    mByteBuffer->SetLimit(mLimit * sizeof(Int32));
    mByteBuffer->SetPosition(mPosition * sizeof(Int32));
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Slice((IByteBuffer**)&bb))
    bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (IInt32Buffer*) new Int32ToByteBufferAdapter(bb);
    REFCOUNT_ADD(*buffer)
    mByteBuffer->Clear();
    return NOERROR;
}

ECode Int32ToByteBufferAdapter::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "Int32ToByteBufferAdapter";
    buf += ", status: capacity=";
    buf += mCapacity;
    buf += " position=";
    buf += mPosition;
    buf += " limit=";
    buf += mLimit;
    return buf.ToString(str);
}

ECode Int32ToByteBufferAdapter::GetCapacity(
    /* [out] */ Int32* cap)
{
    return Int32Buffer::GetCapacity(cap);
}

ECode Int32ToByteBufferAdapter::Clear()
{
    return Int32Buffer::Clear();
}

ECode Int32ToByteBufferAdapter::Flip()
{
    return Int32Buffer::Flip();
}

ECode Int32ToByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int32Buffer::HasArray(hasArray);
}

ECode Int32ToByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int32Buffer::HasRemaining(hasRemaining);
}

ECode Int32ToByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return mByteBuffer->IsDirect(isDirect);
}

ECode Int32ToByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return mByteBuffer->IsReadOnly(isReadOnly);
}

CARAPI Int32ToByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

CARAPI Int32ToByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

CARAPI Int32ToByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode Int32ToByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int32Buffer::GetLimit(limit);
}

ECode Int32ToByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int32Buffer::SetLimit(newLimit);
}

ECode Int32ToByteBufferAdapter::Mark()
{
    return Int32Buffer::Mark();
}

ECode Int32ToByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int32Buffer::GetPosition(pos);
}

ECode Int32ToByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int32Buffer::SetPosition(newPosition);
}

ECode Int32ToByteBufferAdapter::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return Int32Buffer::GetRemaining(remaining);
}

ECode Int32ToByteBufferAdapter::Reset()
{
    return Int32Buffer::Reset();
}

ECode Int32ToByteBufferAdapter::Rewind()
{
    return Int32Buffer::Rewind();
}

} // namespace IO
} // namespace Elastos
