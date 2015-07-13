
#include "ByteBufferAsInt16Buffer.h"
#include "DirectByteBuffer.h"
#include "ByteArrayBuffer.h"

using Libcore::IO::ISizeOf;

namespace Elastos {
namespace IO {

// {82131487-1a59-4024-89de-850c64b9d84d}
extern "C" const InterfaceID EIID_ByteBufferAsInt16Buffer =
        { 0x82131487, 0x1a59, 0x4024, { 0x89, 0xde, 0x85, 0x0c, 0x64, 0xb9, 0xd8, 0x4d } };

ByteBufferAsInt16Buffer::ByteBufferAsInt16Buffer(
    /* [in] */ ByteBuffer* byteBuffer)
    : mCap(0)
    , Int16Buffer((byteBuffer->GetCapacity(&mCap), &mCap), byteBuffer->mEffectiveDirectAddress)
{
    mByteBuffer = byteBuffer;
    mByteBuffer->Clear();
}

AutoPtr<IInt16Buffer> ByteBufferAsInt16Buffer::AsInt16Buffer(
    /* [in] */ ByteBuffer* mByteBuffer)
{
    AutoPtr<IByteBuffer> slice;
    mByteBuffer->Slice((IByteBuffer**)&slice);
    ByteOrder midorder;
    mByteBuffer->GetOrder(&midorder);
    slice->SetOrder(midorder);
    AutoPtr<IInt16Buffer> res = (IInt16Buffer*) new ByteBufferAsInt16Buffer((ByteBuffer*)slice.Get());
    return res;
}

ECode ByteBufferAsInt16Buffer::AsReadOnlyBuffer(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    AutoPtr<IByteBuffer> resbb;
    mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&resbb);
    AutoPtr<ByteBufferAsInt16Buffer> buf = new ByteBufferAsInt16Buffer((ByteBuffer*)resbb.Get());
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    buf->mByteBuffer->mOrder = mByteBuffer->mOrder;
    *buffer = buf;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::Compact()
{
    Boolean isflag = FALSE;
    if (mByteBuffer->IsReadOnly(&isflag), isflag) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    mByteBuffer->SetLimit(mLimit * ISizeOf::SHORT);
    mByteBuffer->SetPosition(mPosition * ISizeOf::SHORT);
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = UNSET_MARK;
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::Duplicate(
    /* [out] */ IInt16Buffer** buffer)
{
    AutoPtr<IByteBuffer> bb;
    mByteBuffer->Duplicate((IByteBuffer**)&bb);
    ByteOrder midorder;
    mByteBuffer->GetOrder(&midorder);
    bb->SetOrder(midorder);
    AutoPtr<ByteBufferAsInt16Buffer> buf = new ByteBufferAsInt16Buffer((ByteBuffer*)bb.Get());
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    *buffer = buf;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::Get(
    /* [out] */ Int16* value)
{
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDERFLOW_EXCEPTION;
    }
    return mByteBuffer->GetInt16(mPosition++ * ISizeOf::SHORT, value);
}

ECode ByteBufferAsInt16Buffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    FAIL_RETURN(CheckIndex(index));
    return mByteBuffer->GetInt16(index * ISizeOf::SHORT, value);
}

ECode ByteBufferAsInt16Buffer::Get(
    /* [out] */ ArrayOf<Int16>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    mByteBuffer->SetLimit(mLimit * ISizeOf::SHORT);
    mByteBuffer->SetPosition(mPosition * ISizeOf::SHORT);
    AutoPtr<DirectByteBuffer> res = static_cast<DirectByteBuffer*>(mByteBuffer.Get());
    if (res) {
        res->GetInt16s(dst, dstOffset, charCount);
    }
    else {
        ((ByteArrayBuffer*) mByteBuffer.Get())->GetInt16s(dst, dstOffset, charCount);
    }
    mPosition += charCount;
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::IsDirect(
    /* [out] */ Boolean* rst)
{
    return mByteBuffer->IsDirect(rst);
}

ECode ByteBufferAsInt16Buffer::IsReadOnly(
    /* [out] */ Boolean* rst)
{
    return mByteBuffer->IsReadOnly(rst);
}

ECode ByteBufferAsInt16Buffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    return mByteBuffer->GetOrder(byteOrder);
}

ECode ByteBufferAsInt16Buffer::Put(
    /* [in] */ Int16 c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_UNDERFLOW_EXCEPTION;
    }
    return mByteBuffer->PutInt16(mPosition++ * ISizeOf::SHORT, c);
}

ECode ByteBufferAsInt16Buffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Int16 c)
{
    FAIL_RETURN(CheckIndex(index));
    return mByteBuffer->PutInt16(index * ISizeOf::SHORT, c);
}

ECode ByteBufferAsInt16Buffer::Put(
    /* [in] */ ArrayOf<Int16>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    mByteBuffer->SetLimit(mLimit * ISizeOf::SHORT);
    mByteBuffer->SetPosition(mPosition * ISizeOf::SHORT);
    AutoPtr<DirectByteBuffer> res = static_cast<DirectByteBuffer*>(mByteBuffer.Get());
    if (res) {
        res->PutInt16s(src, srcOffset, charCount);
    } else {
        ((ByteArrayBuffer*) mByteBuffer.Get())->PutInt16s(src, srcOffset, charCount);
    }
    mPosition += charCount;
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::Slice(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    mByteBuffer->SetLimit(mLimit * ISizeOf::SHORT);
    mByteBuffer->SetPosition(mPosition * ISizeOf::SHORT);
    AutoPtr<IByteBuffer> bb;
    mByteBuffer->Slice((IByteBuffer**)&bb);
    ByteOrder midorder;
    mByteBuffer->GetOrder(&midorder);
    bb->SetOrder(midorder);
    AutoPtr<IInt16Buffer> result = (IInt16Buffer*) new ByteBufferAsInt16Buffer((ByteBuffer*)bb.Get());
    mByteBuffer->Clear();
    *buffer = result;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode ByteBufferAsInt16Buffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode ByteBufferAsInt16Buffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    *result = FALSE;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
