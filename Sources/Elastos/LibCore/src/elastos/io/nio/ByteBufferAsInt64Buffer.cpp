
#include "ByteBufferAsInt64Buffer.h"
#include "DirectByteBuffer.h"
#include "ByteArrayBuffer.h"

using Libcore::IO::ISizeOf;

namespace Elastos {
namespace IO {

ByteBufferAsInt64Buffer::ByteBufferAsInt64Buffer(
    /* [in] */ IByteBuffer* byteBuffer)
{
    // super(mByteBuffer->capacity() / ISizeOf::LONG, mByteBuffer->effectiveDirectAddress);
    mByteBuffer = byteBuffer;
    IBuffer::Probe(mByteBuffer)->Clear();
}

AutoPtr<IInt64Buffer> ByteBufferAsInt64Buffer::AsInt64Buffer(
    /* [in] */ IByteBuffer* byteBuffer)
{
    AutoPtr<IByteBuffer> slice;
    byteBuffer->Slice((IByteBuffer**)&slice);

    ByteOrder outorder;
    byteBuffer->GetOrder(&outorder);
    slice->SetOrder(outorder);
    AutoPtr<IInt64Buffer> res = (IInt64Buffer*) new ByteBufferAsInt64Buffer(slice);
    return res;
}

ECode ByteBufferAsInt64Buffer::AsReadOnlyBuffer(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    AutoPtr<IByteBuffer> res;
    mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&res);
    AutoPtr<ByteBufferAsInt64Buffer> buf = new ByteBufferAsInt64Buffer(res);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    buf->mByteBuffer->SetOrder(((ByteBuffer*)mByteBuffer.Get())->mOrder);
    *buffer = (IInt64Buffer*) buf->Probe(EIID_IInt64Buffer);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Compact()
{
    Boolean isflag = FALSE;
    if (IBuffer::Probe(mByteBuffer)->IsReadOnly(&isflag), isflag) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    IBuffer::Probe(mByteBuffer)->SetLimit(mLimit * ISizeOf::LONG);
    IBuffer::Probe(mByteBuffer)->SetPosition(mPosition * ISizeOf::LONG);
    mByteBuffer->Compact();
    IBuffer::Probe(mByteBuffer)->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = UNSET_MARK;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Duplicate(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    ByteOrder outorder;
    mByteBuffer->GetOrder(&outorder);
    AutoPtr<IByteBuffer> bb;
    mByteBuffer->Duplicate((IByteBuffer**)&bb);
    bb->SetOrder(outorder);
    AutoPtr<ByteBufferAsInt64Buffer> buf = new ByteBufferAsInt64Buffer(bb);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    *buffer = (IInt64Buffer*) buf->Probe(EIID_IInt64Buffer);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Get(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)

    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDERFLOW_EXCEPTION;
    }
    return mByteBuffer->GetInt64(mPosition++ * ISizeOf::LONG, value);
}

ECode ByteBufferAsInt64Buffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)

    FAIL_RETURN(CheckIndex(index));
    return mByteBuffer->GetInt64(index * ISizeOf::LONG, value);
}

ECode ByteBufferAsInt64Buffer::Get(
    /* [out] */ ArrayOf<Int64>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    VALIDATE_NOT_NULL(dst)

    IBuffer::Probe(mByteBuffer)->SetLimit(mLimit * ISizeOf::LONG);
    IBuffer::Probe(mByteBuffer)->SetPosition(mPosition * ISizeOf::LONG);
    AutoPtr<DirectByteBuffer> res = static_cast<DirectByteBuffer*>(mByteBuffer.Get());
    if (res) {
        res->GetInt64s(dst, dstOffset, charCount);
    }
    else {
        ((ByteArrayBuffer*) mByteBuffer.Get())->GetInt64s(dst, dstOffset, charCount);
    }
    mPosition += charCount;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::IsDirect(
    /* [out] */ Boolean* rst)
{
    // return mByteBuffer->isDirect();
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::IsReadOnly(
    /* [out] */ Boolean* rst)
{
    // return mByteBuffer->isReadOnly();
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    // return mByteBuffer->order();
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Put(
    /* [in] */ Int64 c)
{
    // if (mPosition == mLimit) {
    //     throw new BufferOverflowException();
    // }
    // mByteBuffer->putLong(mPosition++ * ISizeOf::LONG, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Int64 c)
{
    // checkIndex(index);
    // mByteBuffer->putLong(index * ISizeOf::LONG, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Put(
    /* [in] */ ArrayOf<Int64>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    // mByteBuffer->mLimit(mLimit * ISizeOf::LONG);
    // mByteBuffer->mPosition(mPosition * ISizeOf::LONG);
    // if (byteBuffer instanceof DirectByteBuffer) {
    //     ((DirectByteBuffer) byteBuffer).put(src, srcOffset, longCount);
    // } else {
    //     ((ByteArrayBuffer) byteBuffer).put(src, srcOffset, longCount);
    // }
    // this.mPosition += longCount;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Slice(
    /* [out] */ IInt64Buffer** buffer)
{
    // mByteBuffer->mLimit(mLimit * ISizeOf::LONG);
    // mByteBuffer->mPosition(mPosition * ISizeOf::LONG);
    // ByteBuffer bb = mByteBuffer->slice().order(mByteBuffer->order());
    // LongBuffer result = new ByteBufferAsInt64Buffer(bb);
    // mByteBuffer->clear();
    // return result;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    // return false;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
