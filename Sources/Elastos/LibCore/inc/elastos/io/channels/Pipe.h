#ifndef __ELASTOS_IO_CHANNELS_SINKCHANNEL_H__
#define __ELASTOS_IO_CHANNELS_SINKCHANNEL_H__

#include <elastos/core/Thread.h>
#include <Object.h>
#include "AbstractSelectableChannel.h"

using Elastos::Core::IRunnable;
using Elastos::Core::Object;
using Elastos::IO::Channels::Spi::ISelectorProvider;
using Elastos::IO::Channels::Spi::AbstractSelectableChannel;

namespace Elastos {
namespace IO {
namespace Channels {

class Pipe
    : public Object
    , public IPipe
{
public:
    /**
     * Writable sink channel used to write to a pipe.
     */
    class SinkChannel
        : public AbstractSelectableChannel
        , public IWritableByteChannel
        , public IGatheringByteChannel
    {
    protected:
        /**
         * Constructs a new {@code SinkChannel}.
         *
         * @param provider
         *            the provider of the channel.
         */
        SinkChannel(
            /* [in] */ ISelectorProvider* provider);

    public:
        CAR_INTERFACE_DECL()

        /**
         * Indicates that this channel only supports writing.
         *
         * @return a static value of OP_WRITE.
         */
        CARAPI ValidOps(
            /* [out] */ Int32* value);
    };

    /**
     * Readable source channel used to read from a pipe.
     */
    class SourceChannel
        : public AbstractSelectableChannel
        , public IReadableByteChannel
        , public IScatteringByteChannel
    {
    protected:
        /**
         * Constructs a new {@code SourceChannel}.
         *
         * @param provider
         *            the provider of the channel.
         */
        SourceChannel(
            /* [in] */ ISelectorProvider* provider);

    public:
        CAR_INTERFACE_DECL()

        /**
         * Indicates that this channel only supports reading.
         *
         * @return a static value of OP_READ.
         */
        CARAPI ValidOps(
            /* [out] */ Int32* value);
    };

public:
    CAR_INTERFACE_DECL()

    /**
     * Returns a new pipe from the default {@see java.nio.channels.spi.SelectorProvider}.
     *
     * @throws IOException
     *             if an I/O error occurs.
     */
    static CARAPI Open(
        /* [out] */ IPipe** pipe);

    /**
     * Returns the sink channel of the pipe.
     *
     * @return a writable sink channel of the pipe.
     */
    virtual CARAPI Sink(
        /* [out] */ ISinkChannel** channel) = 0;

    /**
     * Returns the source channel of the pipe.
     *
     * @return a readable source channel of the pipe.
     */
    virtual CARAPI Source(
        /* [out] */ ISourceChannel** channel) = 0;

protected:
    Pipe();
};

} // namespace Channels
} // namespace IO
} // namespace Elastos

#endif

