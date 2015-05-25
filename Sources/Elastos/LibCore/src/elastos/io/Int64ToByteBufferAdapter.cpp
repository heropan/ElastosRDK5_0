
#include "cmdef.h"
#include "Int64ToByteBufferAdapter.h"
#include "ByteBuffer.h"
#include "DirectByteBuffer.h"
#include "HeapByteBuffer.h"
#include "ReadWriteDirectByteBuffer.h"
#include "ReadWriteHeapByteBuffer.h"
#include "elastos/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

Int64ToByteBufferAdapter::Int64ToByteBufferAdapter(
    /* [in] */ IByteBuffer* byteBuffer)
    : Int64Buffer(((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mCapacity / sizeof(Int64))
    , mByteBuffer(byteBuffer)
{
    mByteBuffer->Clear();
    mEffectiveDirectAddress = ((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mEffectiveDirectAddress;
}

ECode Int64ToByteBufferAdapter::AsInt64Buffer(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ IInt64Buffer** int64Buffer)
{
    VALIDATE_NOT_NULL(int64Buffer)
    assert(byteBuffer != NULL);
    AutoPtr<IByteBuffer> slice;
    FAIL_RETURN(byteBuffer->Slice((IByteBuffer**)&slice))
    slice->SetOrder(((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *int64Buffer = (IInt64Buffer*) new Int64ToByteBufferAdapter(slice);
    INTERFACE_ADDREF(*int64Buffer);
    return NOERROR;
}

PInterface Int64ToByteBufferAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
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


UInt32 Int64ToByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Int64ToByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode Int64ToByteBufferAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(Int64Buffer*)this) {
        *pIID = EIID_Int64Buffer;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode Int64ToByteBufferAdapter::GetPrimitiveArray(
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

ECode Int64ToByteBufferAdapter::GetArray(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    return Int64Buffer::GetArray(array);
}

ECode Int64ToByteBufferAdapter::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode Int64ToByteBufferAdapter::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode Int64ToByteBufferAdapter::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int64Buffer::GetArrayOffset(offset);
}

ECode Int64ToByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> byteBuf;
    FAIL_RETURN(mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&byteBuf))
    AutoPtr<Int64ToByteBufferAdapter> buf = new Int64ToByteBufferAdapter(byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    buf->mByteBuffer->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (IInt64Buffer*)buf->Probe(EIID_IInt64Buffer);
    INTERFACE_ADDREF(*buffer)
    return NOERROR;
}

ECode Int64ToByteBufferAdapter::Compact()
{
    Boolean isReadOnly = FALSE;
    mByteBuffer->IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    mByteBuffer->SetLimit(mLimit * sizeof(Int64));
    mByteBuffer->SetPosition(mPosition * sizeof(Int64));
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode Int64ToByteBufferAdapter::CompareTo(
    /* [in] */ IInt64Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int64Buffer::CompareTo(otherBuffer, result);
}

ECode Int64ToByteBufferAdapter::Duplicate(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Duplicate((IByteBuffer**)&bb))
    bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    AutoPtr<Int64ToByteBufferAdapter> buf = new Int64ToByteBufferAdapter(bb);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    *buffer = (IInt64Buffer*)buf->Probe(EIID_IInt64Buffer);
    INTERFACE_ADDREF(*buffer)
    return NOERROR;
}

ECode Int64ToByteBufferAdapter::GetInt64(
    /* [out] */ Int64* value)
{
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    return mByteBuffer->GetInt64Ex(mPosition++ * sizeof(Int64), value);
}

ECode Int64ToByteBufferAdapter::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->GetInt64Ex(index * sizeof(Int64), value);
}

ECode Int64ToByteBufferAdapter::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst)
{
    return Int64Buffer::GetInt64s(dst);
}

ECode Int64ToByteBufferAdapter::GetInt64sEx(
    /* [out] */ ArrayOf<Int64>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int64Count)
{
    mByteBuffer->SetLimit(mLimit * sizeof(Int64));
    mByteBuffer->SetPosition(mPosition * sizeof(Int64));
    if (mByteBuffer->Probe(EIID_DirectByteBuffer) != NULL) {
        FAIL_RETURN(((DirectByteBuffer*)mByteBuffer->Probe(EIID_DirectByteBuffer))->GetInt64sEx(dst, dstOffset, int64Count))
    }
    else {
        FAIL_RETURN(((HeapByteBuffer*)mByteBuffer->Probe(EIID_HeapByteBuffer))->GetInt64sEx(dst, dstOffset, int64Count))
    }
    mPosition += int64Count;
    return NOERROR;
}

ECode Int64ToByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return mByteBuffer->GetOrder(order);
}

ECode Int64ToByteBufferAdapter::PutInt64(
    /* [in] */ Int64 c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    return mByteBuffer->PutInt64Ex(mPosition++ * sizeof(Int64), c);
}

ECode Int64ToByteBufferAdapter::PutInt64Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int64 c)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->PutInt64Ex(index * sizeof(Int64), c);
}

ECode Int64ToByteBufferAdapter::PutInt64s(
    /* [in] */ const ArrayOf<Int64>& src)
{
    return Int64Buffer::PutInt64s(src);
}

ECode Int64ToByteBufferAdapter::PutInt64sEx(
    /* [in] */ const ArrayOf<Int64>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int64Count)
{
    mByteBuffer->SetLimit(mLimit * sizeof(Int64));
    mByteBuffer->SetPosition(mPosition * sizeof(Int64));
    if (mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer) != NULL ) {
        FAIL_RETURN(((ReadWriteDirectByteBuffer*)mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer))->PutInt64s(
                src, srcOffset, int64Count))
    }
    else {
        FAIL_RETURN(((ReadWriteHeapByteBuffer*)mByteBuffer.Get())->PutInt64s(src, srcOffset, int64Count))
    }
    mPosition += int64Count;
    return NOERROR;
}

ECode Int64ToByteBufferAdapter::PutInt64Buffer(
    /* [in] */ IInt64Buffer* src)
{
    return Int64Buffer::PutInt64Buffer(src);
}

ECode Int64ToByteBufferAdapter::Slice(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    mByteBuffer->SetLimit(mLimit * sizeof(Int64));
    mByteBuffer->SetPosition(mPosition * sizeof(Int64));
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Slice((IByteBuffer**)&bb))
    bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (IInt64Buffer*) new Int64ToByteBufferAdapter(bb);
    INTERFACE_ADDREF(*buffer)
    mByteBuffer->Clear();
    return NOERROR;
}

ECode Int64ToByteBufferAdapter::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "Int64ToByteBufferAdapter";
    buf += ", status: capacity=";
    buf += mCapacity;
    buf += " position=";
    buf += mPosition;
    buf += " limit=";
    buf += mLimit;
    return buf.ToString(str);
}

ECode Int64ToByteBufferAdapter::GetCapacity(
    /* [out] */ Int32* cap)
{
    return Int64Buffer::GetCapacity(cap);
}

ECode Int64ToByteBufferAdapter::Clear()
{
    return Int64Buffer::Clear();
}

ECode Int64ToByteBufferAdapter::Flip()
{
    return Int64Buffer::Flip();
}

ECode Int64ToByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int64Buffer::HasArray(hasArray);
}

ECode Int64ToByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int64Buffer::HasRemaining(hasRemaining);
}

ECode Int64ToByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return mByteBuffer->IsDirect(isDirect);
}

ECode Int64ToByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return mByteBuffer->IsReadOnly(isReadOnly);
}

CARAPI Int64ToByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

CARAPI Int64ToByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

CARAPI Int64ToByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode Int64ToByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int64Buffer::GetLimit(limit);
}

ECode Int64ToByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int64Buffer::SetLimit(newLimit);
}

ECode Int64ToByteBufferAdapter::Mark()
{
    return Int64Buffer::Mark();
}

ECode Int64ToByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int64Buffer::GetPosition(pos);
}

ECode Int64ToByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int64Buffer::SetPosition(newPosition);
}

ECode Int64ToByteBufferAdapter::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return Int64Buffer::GetRemaining(remaining);
}

ECode Int64ToByteBufferAdapter::Reset()
{
    return Int64Buffer::Reset();
}

ECode Int64ToByteBufferAdapter::Rewind()
{
    return Int64Buffer::Rewind();
}

} // namespace IO
} // namespace Elastos
