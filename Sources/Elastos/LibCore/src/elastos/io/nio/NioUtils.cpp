
#include "NioUtils.h"
#include "DirectByteBuffer.h"
#include "MappedByteBuffer.h"
#include "MappedByteBufferAdapter.h"
#include "FileChannelImpl.h"
#include "CByteArrayBuffer.h"

namespace Elastos {
namespace IO {

ECode NioUtils::FreeDirectBuffer(
    /* [in] */ IByteBuffer* buffer)
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

AutoPtr<IFileDescriptor> NioUtils::GetFD(
    /* [in] */ IFileChannel* fc)
{
    AutoPtr<IFileDescriptor> outfd;
    IFileChannelImpl::Probe(fc)->GetFD((IFileDescriptor**)&outfd);
    return outfd;
}

AutoPtr<IFileChannel> NioUtils::NewFileChannel(
    /* [in] */ ICloseable *stream,
    /* [in] */ IFileDescriptor *fd,
    /* [in] */ Int32 mode)
{
    AutoPtr<IFileChannel> res = (IFileChannel *) new FileChannelImpl(stream, fd, mode);
    return res;
}

AutoPtr<ArrayOf<Byte> > NioUtils::GetUnsafeArray(
    /* [in] */ IByteBuffer* b)
{
    return ((CByteArrayBuffer*)b)->mBackingArray;
}

Int32 NioUtils::GetUnsafeArrayOffset(
    /* [in] */ IByteBuffer* b)
{
    return ((CByteArrayBuffer*)b)->mArrayOffset;
}

} // namespace IO
} // namespace Elastos
