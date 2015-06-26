#ifndef __ELASTOS_IO_PIPEIMPL_H__
#define __ELASTOS_IO_PIPEIMPL_H__

// #include "SourceChannel.h"
// #include "SinkChannel.h"
#include "Pipe.h"

using Elastos::IO::IFileDescriptorChannel;
using Elastos::IO::Channels::ISocketChannel;
using Elastos::IO::Channels::Pipe;
using Elastos::IO::Channels::ISinkChannel;
using Elastos::IO::Channels::ISourceChannel;
// using Elastos::IO::Channels::SourceChannel;
// using Elastos::IO::Channels::SinkChannel;

namespace Elastos {
namespace IO {

class PipeImpl : public Pipe
{
private:
    class PipeSourceChannel
        : public Pipe::SourceChannel
        , public IFileDescriptorChannel
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI ReadByteBuffer(
            /* [in] */ IByteBuffer* buffer,
            /* [out] */ Int32* nRead);

        CARAPI ReadByteBuffers(
            /* [in] */ const ArrayOf<IByteBuffer*>& buffers,
            /* [out] */ Int64* nRead);

        CARAPI ReadByteBuffers(
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
        AutoPtr<IFileDescriptor> mFd;
        AutoPtr<ISocketChannel> mChannel;
    };

    class PipeSinkChannel
        : public Pipe::SinkChannel
        , public IFileDescriptorChannel
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI WriteBuffer(
            /* [in] */ IByteBuffer* buffer,
            /* [out] */ Int32* nWrite);

        CARAPI WriteBuffers(
            /* [in] */ ArrayOf<IByteBuffer*>& buffers,
            /* [out] */ Int64* nWrite);

        CARAPI WriteBuffers(
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

public:
    PipeImpl(
        /* [in] */ ISelectorProvider* selectorProvider);

    CARAPI Sink(
        /* [out] */ ISinkChannel** channel);

    CARAPI Source(
        /* [out] */ ISourceChannel** channel);

private:
    AutoPtr<PipeSinkChannel> mSink;
    AutoPtr<PipeSourceChannel> mSource;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_PIPEIMPL_H__
