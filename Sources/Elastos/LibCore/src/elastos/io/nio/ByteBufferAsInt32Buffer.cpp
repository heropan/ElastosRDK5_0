
#include "ByteBufferAsInt32Buffer.h"
#include "DirectByteBuffer.h"
#include "ByteArrayBuffer.h"

using Libcore::IO::ISizeOf;

namespace Elastos {
namespace IO {

// {b19aa0ae-9913-4cf6-8c33-852d2a6946bf}
extern "C" const InterfaceID EIID_ByteBufferAsInt32Buffer =
        { 0xb19aa0ae, 0x9913, 0x4cf6, { 0x8c, 0x33, 0x85, 0x2d, 0x2a, 0x69, 0x46, 0xbf } };

ByteBufferAsInt32Buffer::ByteBufferAsInt32Buffer(
    /* [in] */ ByteBuffer* byteBuffer)
    : mCap(0)
    , Int32Buffer((byteBuffer->GetCapacity(&mCap), &mCap), byteBuffer->mEffectiveDirectAddress)
{
    mByteBuffer = byteBuffer;
    mByteBuffer->Clear();
}

AutoPtr<IInt32Buffer> ByteBufferAsInt32Buffer::AsInt32Buffer(
    /* [in] */ ByteBuffer* mByteBuffer)
{
    AutoPtr<IByteBuffer> slice;
    mByteBuffer->Slice((IByteBuffer**)&slice);
    ByteOrder midorder;
    mByteBuffer->GetOrder(&midorder);
    slice->SetOrder(midorder);
    AutoPtr<IInt32Buffer> res = (IInt32Buffer*) new ByteBufferAsInt32Buffer((ByteBuffer*)slice.Get());
    return res;
}

ECode ByteBufferAsInt32Buffer::AsReadOnlyBuffer(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    AutoPtr<IByteBuffer> resbb;
    mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&resbb);
    AutoPtr<ByteBufferAsInt32Buffer> buf = new ByteBufferAsInt32Buffer((ByteBuffer*)resbb.Get());
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    buf->mByteBuffer->mOrder = mByteBuffer->mOrder;
    *buffer = buf;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::Compact()
{
    Boolean isflag = FALSE;
    if (mByteBuffer->IsReadOnly(&isflag), isflag) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    mByteBuffer->SetLimit(mLimit * ISizeOf::INT);
    mByteBuffer->SetPosition(mPosition * ISizeOf::INT);
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = UNSET_MARK;
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::Duplicate(
    /* [out] */ IInt32Buffer** buffer)
{
    AutoPtr<IByteBuffer> bb;
    mByteBuffer->Duplicate((IByteBuffer**)&bb);
    ByteOrder midorder;
    mByteBuffer->GetOrder(&midorder);
    bb->SetOrder(midorder);
    AutoPtr<ByteBufferAsInt32Buffer> buf = new ByteBufferAsInt32Buffer((ByteBuffer*)bb.Get());
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    *buffer = buf;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::Get(
    /* [out] */ Int32* value)
{
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDERFLOW_EXCEPTION;
    }
    return mByteBuffer->GetInt32(mPosition++ * ISizeOf::INT, value);
}

ECode ByteBufferAsInt32Buffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    FAIL_RETURN(CheckIndex(index));
    return mByteBuffer->GetInt32(index * ISizeOf::INT, value);
}

ECode ByteBufferAsInt32Buffer::Get(
    /* [out] */ ArrayOf<Int32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    mByteBuffer->SetLimit(mLimit * ISizeOf::INT);
    mByteBuffer->SetPosition(mPosition * ISizeOf::INT);
    AutoPtr<DirectByteBuffer> res = static_cast<DirectByteBuffer*>(mByteBuffer.Get());
    if (res) {
        res->GetInt32s(dst, dstOffset, charCount);
    }
    else {
        ((ByteArrayBuffer*) mByteBuffer.Get())->GetInt32s(dst, dstOffset, charCount);
    }
    mPosition += charCount;
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::IsDirect(
    /* [out] */ Boolean* rst)
{
    return mByteBuffer->IsDirect(rst);
}

ECode ByteBufferAsInt32Buffer::IsReadOnly(
    /* [out] */ Boolean* rst)
{
    return mByteBuffer->IsReadOnly(rst);
}

ECode ByteBufferAsInt32Buffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    return mByteBuffer->GetOrder(byteOrder);
}

ECode ByteBufferAsInt32Buffer::Put(
    /* [in] */ Int32 c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_UNDERFLOW_EXCEPTION;
    }
    return mByteBuffer->PutInt32(mPosition++ * ISizeOf::INT, c);
}

ECode ByteBufferAsInt32Buffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Int32 c)
{
    FAIL_RETURN(CheckIndex(index));
    return mByteBuffer->PutInt32(index * ISizeOf::INT, c);
}

ECode ByteBufferAsInt32Buffer::Put(
    /* [in] */ ArrayOf<Int32>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    mByteBuffer->SetLimit(mLimit * ISizeOf::INT);
    mByteBuffer->SetPosition(mPosition * ISizeOf::INT);
    AutoPtr<DirectByteBuffer> res = static_cast<DirectByteBuffer*>(mByteBuffer.Get());
    if (res) {
        res->PutInt32s(src, srcOffset, charCount);
    } else {
        ((ByteArrayBuffer*) mByteBuffer.Get())->PutInt32s(src, srcOffset, charCount);
    }
    mPosition += charCount;
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::Slice(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    mByteBuffer->SetLimit(mLimit * ISizeOf::INT);
    mByteBuffer->SetPosition(mPosition * ISizeOf::INT);
    AutoPtr<IByteBuffer> bb;
    mByteBuffer->Slice((IByteBuffer**)&bb);
    ByteOrder midorder;
    mByteBuffer->GetOrder(&midorder);
    bb->SetOrder(midorder);
    AutoPtr<IInt32Buffer> result = (IInt32Buffer*) new ByteBufferAsInt32Buffer((ByteBuffer*)bb.Get());
    mByteBuffer->Clear();
    *buffer = result;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode ByteBufferAsInt32Buffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode ByteBufferAsInt32Buffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    *result = FALSE;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
