
#include "ByteBufferAsDoubleBuffer.h"
#include "DirectByteBuffer.h"
#include "ByteArrayBuffer.h"

using Libcore::IO::ISizeOf;

namespace Elastos {
namespace IO {

// {b4b1403f-82a0-4b7e-943c-fe094c26c067}
extern "C" const InterfaceID EIID_ByteBufferAsDoubleBuffer =
        { 0xb4b1403f, 0x82a0, 0x4b7e, { 0x94, 0x3c, 0xfe, 0x09, 0x4c, 0x26, 0xc0, 0x67 } };

ByteBufferAsDoubleBuffer::ByteBufferAsDoubleBuffer(
    /* [in] */ ByteBuffer* byteBuffer)
    : DoubleBuffer((byteBuffer->GetCapacity(&mCap), (Double)mCap) / ISizeOf::DOUBLE, byteBuffer->mEffectiveDirectAddress)
{
    mByteBuffer = byteBuffer;
    mByteBuffer->Clear();
}

AutoPtr<IDoubleBuffer> ByteBufferAsDoubleBuffer::AsDoubleBuffer(
    /* [in] */ ByteBuffer* mByteBuffer)
{
    AutoPtr<IByteBuffer> slice;
    mByteBuffer->Slice((IByteBuffer**)&slice);
    ByteOrder midorder;
    mByteBuffer->GetOrder(&midorder);
    slice->SetOrder(midorder);
    AutoPtr<IDoubleBuffer> res = (IDoubleBuffer*) new ByteBufferAsDoubleBuffer((ByteBuffer*)slice.Get());
    return res;
}

ECode ByteBufferAsDoubleBuffer::AsReadOnlyBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    AutoPtr<IByteBuffer> resbb;
    mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&resbb);
    AutoPtr<ByteBufferAsDoubleBuffer> buf = new ByteBufferAsDoubleBuffer((ByteBuffer*)resbb.Get());
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    buf->mByteBuffer->mOrder = mByteBuffer->mOrder;
    *buffer = buf;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::Compact()
{
    Boolean isflag = FALSE;
    if (mByteBuffer->IsReadOnly(&isflag), isflag) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    mByteBuffer->SetLimit(mLimit * ISizeOf::DOUBLE);
    mByteBuffer->SetPosition(mPosition * ISizeOf::DOUBLE);
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = UNSET_MARK;
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::Duplicate(
    /* [out] */ IDoubleBuffer** buffer)
{
    AutoPtr<IByteBuffer> bb;
    mByteBuffer->Duplicate((IByteBuffer**)&bb);
    ByteOrder midorder;
    mByteBuffer->GetOrder(&midorder);
    bb->SetOrder(midorder);
    AutoPtr<ByteBufferAsDoubleBuffer> buf = new ByteBufferAsDoubleBuffer((ByteBuffer*)bb.Get());
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    *buffer = buf;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::Get(
    /* [out] */ Double* value)
{
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDERFLOW_EXCEPTION;
    }
    return mByteBuffer->GetDouble(mPosition++ * ISizeOf::DOUBLE, value);
}

ECode ByteBufferAsDoubleBuffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    FAIL_RETURN(CheckIndex(index));
    return mByteBuffer->GetDouble(index * ISizeOf::DOUBLE, value);
}

ECode ByteBufferAsDoubleBuffer::Get(
    /* [out] */ ArrayOf<Double>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 doubleCount)
{
    mByteBuffer->SetLimit(mLimit * ISizeOf::DOUBLE);
    mByteBuffer->SetPosition(mPosition * ISizeOf::DOUBLE);
    AutoPtr<DirectByteBuffer> res = static_cast<DirectByteBuffer*>(mByteBuffer.Get());
    if (res) {
        res->GetDoubles(dst, dstOffset, doubleCount);
    } else {
        ((ByteArrayBuffer*) mByteBuffer.Get())->GetDoubles(dst, dstOffset, doubleCount);
    }
    mPosition += doubleCount;
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::IsDirect(
    /* [out] */ Boolean* rst)
{
    return mByteBuffer->IsDirect(rst);
}

ECode ByteBufferAsDoubleBuffer::IsReadOnly(
    /* [out] */ Boolean* rst)
{
    return mByteBuffer->IsReadOnly(rst);
}

ECode ByteBufferAsDoubleBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    return mByteBuffer->GetOrder(byteOrder);
}

ECode ByteBufferAsDoubleBuffer::Put(
    /* [in] */ Double c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_UNDERFLOW_EXCEPTION;
    }
    return mByteBuffer->PutDouble(mPosition++ * ISizeOf::DOUBLE, c);
}

ECode ByteBufferAsDoubleBuffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Double c)
{
    FAIL_RETURN(CheckIndex(index));
    return mByteBuffer->PutDouble(index * ISizeOf::DOUBLE, c);
}

ECode ByteBufferAsDoubleBuffer::Put(
    /* [in] */ ArrayOf<Double>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 doubleCount)
{
    mByteBuffer->SetLimit(mLimit * ISizeOf::DOUBLE);
    mByteBuffer->SetPosition(mPosition * ISizeOf::DOUBLE);
    AutoPtr<DirectByteBuffer> res = static_cast<DirectByteBuffer*>(mByteBuffer.Get());
    if (res) {
        res->PutDoubles(src, srcOffset, doubleCount);
    } else {
        ((ByteArrayBuffer*) mByteBuffer.Get())->PutDoubles(src, srcOffset, doubleCount);
    }
    mPosition += doubleCount;
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::Slice(
    /* [out] */ IDoubleBuffer** buffer)
{
    mByteBuffer->SetLimit(mLimit * ISizeOf::DOUBLE);
    mByteBuffer->SetPosition(mPosition * ISizeOf::DOUBLE);
    AutoPtr<IByteBuffer> bb;
    mByteBuffer->Slice((IByteBuffer**)&bb);
    ByteOrder midorder;
    mByteBuffer->GetOrder(&midorder);
    bb->SetOrder(midorder);
    AutoPtr<IDoubleBuffer> result = (IDoubleBuffer*) new ByteBufferAsDoubleBuffer((ByteBuffer*)bb.Get());
    mByteBuffer->Clear();
    *buffer = result;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode ByteBufferAsDoubleBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode ByteBufferAsDoubleBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = FALSE;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
