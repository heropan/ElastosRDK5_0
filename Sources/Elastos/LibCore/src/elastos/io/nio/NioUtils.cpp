
#include "NioUtils.h"
#include "DirectByteBuffer.h"
#include "MappedByteBuffer.h"
#include "MappedByteBufferAdapter.h"
#include "FileChannelImpl.h"

namespace Elastos {
namespace IO {

Int32 NioUtils::GetDirectBufferAddress(
    /* [in] */ Buffer* buffer)
{
    return buffer->mEffectiveDirectAddress;
}

ECode NioUtils::FreeDirectBuffer(
    /* [in] */ ByteBuffer* buffer)
{
    if (NULL == buffer)
        return NOERROR;

    AutoPtr<DirectByteBuffer> directBuf;
    AutoPtr<MappedByteBuffer> mappedBuf;
    if ((directBuf = reinterpret_cast<DirectByteBuffer*>(buffer->Probe(EIID_DirectByteBuffer))) != NULL) {
        directBuf->Free();
        return NOERROR;
    }
    else if ((mappedBuf = reinterpret_cast<MappedByteBuffer*>(buffer->Probe(EIID_MappedByteBuffer))) != NULL) {
        (reinterpret_cast<MappedByteBufferAdapter*>(mappedBuf.Get()))->Free();
        return NOERROR;
    }
    else {
        // assert(0 != 0);
        return E_ASSERTION_ERROR;
    }
}

AutoPtr<IFileChannel> NioUtils::NewFileChannel(
    /* [in] */ IObject *stream,
    /* [in] */ IFileDescriptor *fd,
    /* [in] */ Int32 mode)
{
    AutoPtr<IFileChannel> res = (IFileChannel *) new FileChannelImpl(stream, fd, mode);
    return res;
}

ArrayOf<Byte>* NioUtils::GetUnsafeArray(
    /* [in] */ ByteBuffer* b)
{
    // return b->mBackingArray;
}

Int32 NioUtils::GetUnsafeArrayOffset(
    /* [in] */ ByteBuffer* b)
{
    // return b->mOffset;
}

} // namespace IO
} // namespace Elastos
