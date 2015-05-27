
#include "coredef.h"
#include "Int16ToByteBufferAdapter.h"
#include "ByteBuffer.h"
#include "DirectByteBuffer.h"
#include "HeapByteBuffer.h"
#include "ReadWriteDirectByteBuffer.h"
#include "ReadWriteHeapByteBuffer.h"
#include "elastos/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

Int16ToByteBufferAdapter::Int16ToByteBufferAdapter(
    /* [in] */ IByteBuffer* byteBuffer)
    : Int16Buffer(((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mCapacity / sizeof(Int16))
    , mByteBuffer(byteBuffer)
{
    mByteBuffer = byteBuffer;
    mByteBuffer->Clear();
    mEffectiveDirectAddress = ((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mEffectiveDirectAddress;
}

ECode Int16ToByteBufferAdapter::AsInt16Buffer(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ IInt16Buffer** int16Buffer)
{
    VALIDATE_NOT_NULL(int16Buffer)
    assert(byteBuffer != NULL);
    AutoPtr<IByteBuffer> slice;
    FAIL_RETURN(byteBuffer->Slice((IByteBuffer**)&slice))
    slice->SetOrder(((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *int16Buffer = (IInt16Buffer*) new Int16ToByteBufferAdapter(slice);
    INTERFACE_ADDREF(*int16Buffer);
    return NOERROR;
}

PInterface Int16ToByteBufferAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
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


UInt32 Int16ToByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Int16ToByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode Int16ToByteBufferAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(Int16Buffer*)this) {
        *pIID = EIID_Int16Buffer;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode Int16ToByteBufferAdapter::GetPrimitiveArray(
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

ECode Int16ToByteBufferAdapter::GetArray(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    return Int16Buffer::GetArray(array);
}

ECode Int16ToByteBufferAdapter::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode Int16ToByteBufferAdapter::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode Int16ToByteBufferAdapter::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int16Buffer::GetArrayOffset(offset);
}

ECode Int16ToByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> byteBuf;
    FAIL_RETURN(mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&byteBuf))
    AutoPtr<Int16ToByteBufferAdapter> buf = new Int16ToByteBufferAdapter(byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    buf->mByteBuffer->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (IInt16Buffer*)buf->Probe(EIID_IInt16Buffer);
    INTERFACE_ADDREF(*buffer)
    return NOERROR;
}

ECode Int16ToByteBufferAdapter::Compact()
{
    Boolean isReadOnly = FALSE;
    mByteBuffer->IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    mByteBuffer->SetLimit(mLimit * sizeof(Int16));
    mByteBuffer->SetPosition(mPosition * sizeof(Int16));
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode Int16ToByteBufferAdapter::CompareTo(
    /* [in] */ IInt16Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int16Buffer::CompareTo(otherBuffer, result);
}

ECode Int16ToByteBufferAdapter::Duplicate(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Duplicate((IByteBuffer**)&bb))
    bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    AutoPtr<Int16ToByteBufferAdapter> buf = new Int16ToByteBufferAdapter(bb);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    *buffer = (IInt16Buffer*)buf->Probe(EIID_IInt16Buffer);
    INTERFACE_ADDREF(*buffer)
    return NOERROR;
}

ECode Int16ToByteBufferAdapter::GetInt16(
    /* [out] */ Int16* value)
{
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    return mByteBuffer->GetInt16Ex(mPosition++ * sizeof(Int16), value);
}

ECode Int16ToByteBufferAdapter::GetInt16(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->GetInt16Ex(index * sizeof(Int16), value);
}

ECode Int16ToByteBufferAdapter::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst)
{
    return Int16Buffer::GetInt16s(dst);
}

ECode Int16ToByteBufferAdapter::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int16Count)
{
    mByteBuffer->SetLimit(mLimit * sizeof(Int16));
    mByteBuffer->SetPosition(mPosition * sizeof(Int16));
    if (mByteBuffer->Probe(EIID_DirectByteBuffer) != NULL) {
        FAIL_RETURN(((DirectByteBuffer*)mByteBuffer->Probe(EIID_DirectByteBuffer))->GetInt16sEx(dst, dstOffset, int16Count))
    }
    else {
        FAIL_RETURN(((HeapByteBuffer*)mByteBuffer->Probe(EIID_HeapByteBuffer))->GetInt16sEx(dst, dstOffset, int16Count))
    }
    mPosition += int16Count;
    return NOERROR;
}

ECode Int16ToByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return mByteBuffer->GetOrder(order);
}

ECode Int16ToByteBufferAdapter::PutInt16(
    /* [in] */ Int16 c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    return mByteBuffer->PutInt16Ex(mPosition++ * sizeof(Int16), c);
}

ECode Int16ToByteBufferAdapter::PutInt16(
    /* [in] */ Int32 index,
    /* [in] */ Int16 c)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->PutInt16Ex(index * sizeof(Int16), c);
}

ECode Int16ToByteBufferAdapter::PutInt16s(
    /* [in] */ const ArrayOf<Int16>& src)
{
    return Int16Buffer::PutInt16s(src);
}

ECode Int16ToByteBufferAdapter::PutInt16s(
    /* [in] */ const ArrayOf<Int16>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int16Count)
{
    mByteBuffer->SetLimit(mLimit * sizeof(Int16));
    mByteBuffer->SetPosition(mPosition * sizeof(Int16));
    if (mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer) != NULL ) {
        FAIL_RETURN(((ReadWriteDirectByteBuffer*)mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer))->PutInt16s(
                src, srcOffset, int16Count))
    }
    else {
        FAIL_RETURN(((ReadWriteHeapByteBuffer*)mByteBuffer.Get())->PutInt16s(src, srcOffset, int16Count))
    }
    mPosition += int16Count;
    return NOERROR;
}

ECode Int16ToByteBufferAdapter::PutInt16Buffer(
    /* [in] */ IInt16Buffer* src)
{
    return Int16Buffer::PutInt16Buffer(src);
}

ECode Int16ToByteBufferAdapter::Slice(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    mByteBuffer->SetLimit(mLimit * sizeof(Int16));
    mByteBuffer->SetPosition(mPosition * sizeof(Int16));
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Slice((IByteBuffer**)&bb))
    bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (IInt16Buffer*) new Int16ToByteBufferAdapter(bb);
    INTERFACE_ADDREF(*buffer)
    mByteBuffer->Clear();
    return NOERROR;
}

ECode Int16ToByteBufferAdapter::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "Int16ToByteBufferAdapter";
    buf += ", status: capacity=";
    buf += mCapacity;
    buf += " position=";
    buf += mPosition;
    buf += " limit=";
    buf += mLimit;
    return buf.ToString(str);
}

ECode Int16ToByteBufferAdapter::GetCapacity(
    /* [out] */ Int32* cap)
{
    return Int16Buffer::GetCapacity(cap);
}

ECode Int16ToByteBufferAdapter::Clear()
{
    return Int16Buffer::Clear();
}

ECode Int16ToByteBufferAdapter::Flip()
{
    return Int16Buffer::Flip();
}

ECode Int16ToByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int16Buffer::HasArray(hasArray);
}

ECode Int16ToByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int16Buffer::HasRemaining(hasRemaining);
}

ECode Int16ToByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return mByteBuffer->IsDirect(isDirect);
}

ECode Int16ToByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return mByteBuffer->IsReadOnly(isReadOnly);
}

CARAPI Int16ToByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

CARAPI Int16ToByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

CARAPI Int16ToByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode Int16ToByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int16Buffer::GetLimit(limit);
}

ECode Int16ToByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int16Buffer::SetLimit(newLimit);
}

ECode Int16ToByteBufferAdapter::Mark()
{
    return Int16Buffer::Mark();
}

ECode Int16ToByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int16Buffer::GetPosition(pos);
}

ECode Int16ToByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int16Buffer::SetPosition(newPosition);
}

ECode Int16ToByteBufferAdapter::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return Int16Buffer::GetRemaining(remaining);
}

ECode Int16ToByteBufferAdapter::Reset()
{
    return Int16Buffer::Reset();
}

ECode Int16ToByteBufferAdapter::Rewind()
{
    return Int16Buffer::Rewind();
}

} // namespace IO
} // namespace Elastos
