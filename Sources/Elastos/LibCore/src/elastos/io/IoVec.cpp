
#include "coredef.h"
#include "IoVec.h"
#include "NioUtils.h"
#include "CLibcore.h"
#include "CInteger32.h"

using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Libcore::IO::CLibcore;

namespace Elastos {
namespace IO {

IoVec::IoVec(
    /* [in] */ ArrayOf<ByteBuffer*>* byteBuffers,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 bufferCount,
    /* [in] */ Direction direction)
    : mOffset(offset)
    , mBufferCount(bufferCount)
    , mDirection(direction)
{
    mByteBuffers = byteBuffers;
    mIoBuffers = ArrayOf<IInterface*>::Alloc(bufferCount);
    mOffsets = ArrayOf<Int32>::Alloc(mBufferCount);
    mByteCounts = ArrayOf<Int32>::Alloc(mBufferCount);
}

ECode IoVec::Init(
    /* [out] */ Int32* totalRemaining)
{
    VALIDATE_NOT_NULL(totalRemaining)
    *totalRemaining = 0;
    for (Int32 i = 0; i < mBufferCount; ++i) {
        AutoPtr<ByteBuffer> b = (*mByteBuffers)[i + mOffset];
        if (mDirection == Direction_READV) {
            FAIL_RETURN(b->CheckWritable())
        }
        Int32 remaining = 0;
        b->GetRemaining(&remaining);
        Boolean isDirect = FALSE;
        if ((b->IsDirect(&isDirect), isDirect)) {
            mIoBuffers->Set(i, b->Probe(EIID_IInterface));
            Int32 pos = 0;
            b->GetPosition(&pos);
            (*mOffsets)[i] = pos;
        }
        else {
            Int32 safevalue = NioUtils::GetUnsafeArrayOffset(reinterpret_cast<HeapByteBuffer*>(b->Probe(EIID_HeapByteBuffer)));
            AutoPtr<IInteger32> int32;
            CInteger32::New(safevalue, (IInteger32**)&int32);
            mIoBuffers->Set(i, int32);
            Int32 pos = 0;
            b->GetPosition(&pos);
            (*mOffsets)[i] = safevalue + pos;
        }
        (*mByteCounts)[i] = remaining;
        *totalRemaining += remaining;
    }
    return NOERROR;
}

ECode IoVec::DoTransfer(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    // try {
    *result = 0;
    Int32 fdvalue = 0;
    fd->GetDescriptor(&fdvalue);
    if (mDirection == Direction_READV) {
        FAIL_RETURN(CLibcore::sOs->Readv(fdvalue, *mIoBuffers, *mOffsets, *mByteCounts, result));
        if (*result == 0) {
            *result = -1;
        }
        return NOERROR;
    }
    else {
        FAIL_RETURN(CLibcore::sOs->Writev(fdvalue, *mIoBuffers, *mOffsets, *mByteCounts, result));
        return NOERROR;
    }
    // } catch (ErrnoException errnoException) {
    //     throw errnoException.rethrowAsIOException();
    // }
}

ECode IoVec::DidTransfer(
    /* [in] */ Int32 byteCount)
{
    for (Int32 i = 0; byteCount > 0 && i < mBufferCount; ++i) {
        AutoPtr<ByteBuffer> b = (*mByteBuffers)[i + mOffset];
        if ((*mByteCounts)[i] < byteCount) {
            Int32 limit = 0;
            b->GetLimit(&limit);
            b->SetPosition(limit);
            byteCount -= (*mByteCounts)[i];
        }
        else {
            Int32 pos = 0;
            b->SetPosition((mDirection == Direction_WRITEV ? (b->GetPosition(&pos), pos) : 0) + byteCount);
            byteCount = 0;
        }
    }
    return NOERROR;
}

} // namespace IO
} // namespace Elastos