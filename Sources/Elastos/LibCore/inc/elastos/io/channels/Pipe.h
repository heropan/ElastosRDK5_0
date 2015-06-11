#ifndef __ELASTOS_IO_CHANNELS_SINKCHANNEL_H__
#define __ELASTOS_IO_CHANNELS_SINKCHANNEL_H__

#include <Elastos.CoreLibrary_server.h>
#include <elastos/core/Thread.h>

using Elastos::Core::IRunnable;
using Elastos::IO::Channels::Spi::ISelectorProvider;

namespace Elastos {
namespace IO {
namespace Channels {

class Pipe
{
public:
    /**
     * Returns a new pipe from the default {@see java.nio.channels.spi.SelectorProvider}.
     *
     * @throws IOException
     *             if an I/O error occurs.
     */
    static CARAPI Open(IPipe** pipe);

    /**
     * Returns the sink channel of the pipe.
     *
     * @return a writable sink channel of the pipe.
     */
    virtual CARAPI Sink(ISinkChannel** channel) = 0;

    /**
     * Returns the source channel of the pipe.
     *
     * @return a readable source channel of the pipe.
     */
    virtual CARAPI Source(ISourceChannel** channel) = 0;

protected:
    Pipe();
};

} // namespace Channels
} // namespace IO
} // namespace Elastos

#endif

