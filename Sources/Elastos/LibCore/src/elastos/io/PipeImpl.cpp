
#include "coredef.h"
#include "PipeImpl.h"
#include "IoUtils.h"

namespace Elastos{
namespace IO{

// PipeSoucreChannel
PipeSourceChannel::PipeSourceChannel(
    /* [in] */ ISelectorProvider* provider,
    /* [in] */ IFileDescriptor* fd)
    : SourceChannel(provider), mFd(fd)
{
    assert(NULL != provider);
    assert(NULL != fd);
//    mChannel = new SocketChannelImpl(provider, fd);
    printf("ERROR:SocketChannelImpl Not Implemented Yet\n");
    assert(false);
}

ECode PipeSourceChannel::ImplCloseSelectableChannel()
{
    return ICloseable::Probe(mChannel)->Close();
}

ECode PipeSourceChannel::ImplConfigureBlocking(Boolean blocking)
{
    AutoPtr<IFileDescriptor> descriptor;
    ECode ecRet = GetFD((IFileDescriptor**)&descriptor);
    if (NOERROR != ecRet)
    {
        return ecRet;
    }

    return IoUtils::SetBlocking(descriptor, blocking);
}

ECode PipeSourceChannel::ReadByteBuffer(IByteBuffer* buffer, Int32* nRead)
{
    return mChannel->ReadByteBuffer(buffer, nRead);
}

ECode PipeSourceChannel::ReadByteBuffers(const ArrayOf<IByteBuffer*> & buffers,
        Int64* nRead)
{
    return mChannel->ReadByteBuffers(buffers, nRead);
}

ECode PipeSourceChannel::ReadByteBuffers(
        const ArrayOf<IByteBuffer*> & buffers, Int32 offset, Int32 length,
        Int64* nRead)
{
    return mChannel->ReadByteBuffers(buffers, offset, length, nRead);
}

ECode PipeSourceChannel::GetFD(IFileDescriptor** descriptor)
{
    *descriptor = mFd.Get();
    REFCOUNT_ADD(*descriptor);
    return NOERROR;
}
// end PipeSourceChannel;

// PipeSinkChannel
PipeSinkChannel::PipeSinkChannel(ISelectorProvider* provider, IFileDescriptor* fd) :
        SinkChannel(provider), mFd(fd)
{
    assert(NULL != provider);
    assert(NULL != fd);
  //  mChannel = new SocketChannnelImpl();
    printf("ERROR: SocketChannelImpl Not Implemented Yet\n");
    assert(false);
}

ECode PipeSinkChannel::ImplCloseSelectableChannel()
{
    return ICloseable::Probe(mChannel)->Close();
}

ECode PipeSinkChannel::ImplConfigureBlocking(Boolean blocking)
{
    AutoPtr<IFileDescriptor> desc;
    ECode ecRet = GetFD((IFileDescriptor**)&desc);
    if(NOERROR != ecRet)
        return ecRet;

    return IoUtils::SetBlocking(desc, blocking);
}

ECode PipeSinkChannel::WriteBuffer(IByteBuffer* buffer, Int32* nWrite)
{
    return mChannel->WriteByteBuffer(buffer, nWrite);
}

ECode PipeSinkChannel::WriteBuffers(ArrayOf<IByteBuffer*> & buffers, Int64* nWrite)
{
    return mChannel->WriteByteBuffers(buffers, nWrite);
}

ECode PipeSinkChannel::WriteBuffers(ArrayOf<IByteBuffer*> & buffers,
        Int32 offset, Int32 length, Int64* nWrite)
{
    return mChannel->WriteByteBuffers(buffers, offset, length, nWrite);
}

ECode PipeSinkChannel::GetFD(IFileDescriptor** descriptor)
{
    *descriptor = mFd.Get();
    REFCOUNT_ADD(*descriptor);

    return NOERROR;
}

} // namespace IO
} // namespace Elastoss
