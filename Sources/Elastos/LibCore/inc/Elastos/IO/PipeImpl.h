#ifndef __ELASTOS_IO_PIPEIMPL_H__
#define __ELASTOS_IO_PIPEIMPL_H__

#include "Elastos.Core_server.h"
#include "SourceChannel.h"
#include "SinkChannel.h"

using Elastos::IO::IFileDescriptorChannel;
using Elastos::IO::Channels::ISocketChannel;
using Elastos::IO::Channels::SourceChannel;
using Elastos::IO::Channels::SinkChannel;

namespace Elastos {
namespace IO {

class PipeSourceChannel
    : public SourceChannel
    , public IFileDescriptorChannel
{
public:
    CARAPI ReadByteBuffer(
        /* [in] */ IByteBuffer* buffer,
        /* [out] */ Int32* nRead);

    CARAPI ReadByteBuffers(
        /* [in] */ const ArrayOf<IByteBuffer*>& buffers,
        /* [out] */ Int64* nRead);

    CARAPI ReadByteBuffersEx(
        /* [in] */ const ArrayOf<IByteBuffer*>& buffers,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int64* nRead);

    CARAPI GetFD(
        /* [out] */ IFileDescriptor** descriptor);

protected:
    CARAPI ImplCloseSelectableChannel();

    CARAPI ImplConfigureBlocking(
        /* [in] */ Boolean blocking);
private:
    PipeSourceChannel(
        /* [in] */ ISelectorProvider* provider,
        /* [in] */ IFileDescriptor* fd);

private:
    AutoPtr<IFileDescriptor> mFd; // If NOT Use AutoPtr, Remember ADD_REF in construcotr and DEC_REF in destructor
    AutoPtr<ISocketChannel> mChannel;
};
// PipeSourceChannel;

class PipeSinkChannel
    : public SinkChannel
    , public IFileDescriptorChannel
{
public:
    CARAPI WriteBuffer(
        /* [in] */ IByteBuffer* buffer,
        /* [out] */ Int32* nWrite);

    CARAPI WriteBuffers(
        /* [in] */ ArrayOf<IByteBuffer*>& buffers,
        /* [out] */ Int64* nWrite);

    CARAPI WriteBuffersEx(
        /* [in] */ ArrayOf<IByteBuffer*>& buffers,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int64* nWrite);

    CARAPI GetFD(
        /* [out] */ IFileDescriptor** descriptor);

protected:
    CARAPI ImplCloseSelectableChannel();

    CARAPI ImplConfigureBlocking(
        /* [out] */ Boolean blocking);

private:
    PipeSinkChannel(
        /* [in] */ ISelectorProvider* provider,
        /* [in] */ IFileDescriptor* fd);

private:
    AutoPtr<IFileDescriptor> mFd;
    AutoPtr<ISocketChannel> mChannel;
};
// PipeSinkChannel

} // namespace IO
} // namespace Elastos

#endif

