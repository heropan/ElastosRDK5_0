
#include "ByteBufferAsCharBuffer.h"
#include "DirectByteBuffer.h"
#include "ByteArrayBuffer.h"

using Libcore::IO::ISizeOf;

namespace Elastos {
namespace IO {

// {88daddf0-a8d1-4752-8a42-cb439858875f}
extern "C" const InterfaceID EIID_ByteBufferAsCharBuffer =
        { 0x88daddf0, 0xa8d1, 0x4752, { 0x8a, 0x42, 0xcb, 0x43, 0x98, 0x58, 0x87, 0x5f } };

ByteBufferAsCharBuffer::ByteBufferAsCharBuffer(
    /* [in] */ ByteBuffer* byteBuffer)
    : mCap(0)
    , CharBuffer((byteBuffer->GetCapacity(&mCap), mCap) / ISizeOf::CHAR, mByteBuffer->mEffectiveDirectAddress)
{
    mByteBuffer = byteBuffer;
    mByteBuffer->Clear();
}

AutoPtr<ICharBuffer> ByteBufferAsCharBuffer::AsCharBuffer(
    /* [in] */ ByteBuffer* byteBuffer)
{
    AutoPtr<IByteBuffer> slice;
    byteBuffer->Slice((IByteBuffer**)&slice);
    ByteOrder midorder;
    byteBuffer->GetOrder(&midorder);
    slice->SetOrder(midorder);
    AutoPtr<ICharBuffer> res = (ICharBuffer*) new ByteBufferAsCharBuffer((ByteBuffer*)slice.Get());
    return res;
}

ECode ByteBufferAsCharBuffer::AsReadOnlyBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    AutoPtr<IByteBuffer> robuffer;
    mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&robuffer);
    AutoPtr<ByteBufferAsCharBuffer> buf = new ByteBufferAsCharBuffer((ByteBuffer*)robuffer.Get());
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    buf->mByteBuffer->mOrder = mByteBuffer->mOrder;
    *buffer = (ICharBuffer*) buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Compact()
{
    Boolean isflag = FALSE;
    if (mByteBuffer->IsReadOnly(&isflag), isflag) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    mByteBuffer->SetLimit(mLimit * ISizeOf::CHAR);
    mByteBuffer->SetPosition(mPosition * ISizeOf::CHAR);
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = UNSET_MARK;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Duplicate(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    AutoPtr<IByteBuffer> bb;
    mByteBuffer->Duplicate((IByteBuffer**)&bb);
    ByteOrder midorder;
    mByteBuffer->GetOrder(&midorder);
    bb->SetOrder(midorder);
    AutoPtr<ByteBufferAsCharBuffer> buf = new ByteBufferAsCharBuffer((ByteBuffer*)bb.Get());
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    *buffer = buf;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Get(
    /* [out] */ Char32* value)
{
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDERFLOW_EXCEPTION;
    }
    return mByteBuffer->GetChar(mPosition++ * ISizeOf::CHAR, value);
}

ECode ByteBufferAsCharBuffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    FAIL_RETURN(CheckIndex(index));
    return mByteBuffer->GetChar(index * ISizeOf::CHAR, value);
}

ECode ByteBufferAsCharBuffer::Get(
    /* [out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    mByteBuffer->SetLimit(mLimit * ISizeOf::CHAR);
    mByteBuffer->SetPosition(mPosition * ISizeOf::CHAR);

    AutoPtr<DirectByteBuffer> res = static_cast<DirectByteBuffer*>(mByteBuffer.Get());
    if (res) {
        res->GetChars(dst, dstOffset, charCount);
    }
    else {
        ((ByteArrayBuffer*) mByteBuffer.Get())->GetChars(dst, dstOffset, charCount);
    }

    mPosition += charCount;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::IsDirect(
    /* [out] */ Boolean* rst)
{
    return mByteBuffer->IsDirect(rst);
}

ECode ByteBufferAsCharBuffer::IsReadOnly(
    /* [out] */ Boolean* rst)
{
    return mByteBuffer->IsReadOnly(rst);
}

ECode ByteBufferAsCharBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    return mByteBuffer->GetOrder(byteOrder);
}

ECode ByteBufferAsCharBuffer::Put(
    /* [in] */ Char32 c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVERFLOW_EXCEPTION;
    }
    return mByteBuffer->PutChar(mPosition++ * ISizeOf::CHAR, c);
}

ECode ByteBufferAsCharBuffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Char32 c)
{
    FAIL_RETURN(CheckIndex(index));
    return mByteBuffer->PutChar(index * ISizeOf::CHAR, c);
}

ECode ByteBufferAsCharBuffer::Put(
    /* [in] */ ArrayOf<Char32>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    mByteBuffer->SetLimit(mLimit * ISizeOf::CHAR);
    mByteBuffer->SetPosition(mPosition * ISizeOf::CHAR);
    AutoPtr<DirectByteBuffer> res = static_cast<DirectByteBuffer*>(mByteBuffer.Get());
    if (res) {
        res->PutChars(src, srcOffset, charCount);
    }
    else {
        ((ByteArrayBuffer*) mByteBuffer.Get())->PutChars(src, srcOffset, charCount);
    }
    mPosition += charCount;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Slice(
    /* [out] */ ICharBuffer** buffer)
{
    mByteBuffer->SetLimit(mLimit * ISizeOf::CHAR);
    mByteBuffer->SetPosition(mPosition * ISizeOf::CHAR);
    AutoPtr<IByteBuffer> bb;
    mByteBuffer->Slice((IByteBuffer**)&bb);
    ByteOrder midorder;
    mByteBuffer->GetOrder(&midorder);
    bb->SetOrder(midorder);
    AutoPtr<ICharBuffer> result = (ICharBuffer*) new ByteBufferAsCharBuffer((ByteBuffer*)bb.Get());
    mByteBuffer->Clear();
    *buffer = result;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    FAIL_RETURN(CheckStartEndRemaining(start, end));
    AutoPtr<ICharBuffer> result;
    Duplicate((ICharBuffer**)&result);
    IBuffer::Probe(result)->SetLimit(mPosition + end);
    IBuffer::Probe(result)->SetPosition(mPosition + start);
    *csq = ICharSequence::Probe(result);
    REFCOUNT_ADD(*csq)
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode ByteBufferAsCharBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode ByteBufferAsCharBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = FALSE;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
