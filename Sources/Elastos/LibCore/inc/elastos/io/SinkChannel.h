#ifndef __ELASTOS_IO_CHANNELS_SINKCHANNEL_H__
#define __ELASTOS_IO_CHANNELS_SINKCHANNEL_H__

#include <Elastos.CoreLibrary_server.h>
#include <elastos/Thread.h>
#include <elastos/Mutex.h>
#include "AbstractSelectableChannel.h"

using Elastos::Core::IRunnable;
using Elastos::Core::Mutex;
using Elastos::IO::Channels::Spi::ISelectorProvider;
using Elastos::IO::Channels::Spi::AbstractSelectableChannel;

namespace Elastos {
namespace IO {
namespace Channels {

class SinkChannel
    : public AbstractSelectableChannel
    , public IGatheringByteChannel
{
public:
    CARAPI ValidOps(
        /* [out] */ Int32* ret);

protected:
    SinkChannel(
        /* [in] */ ISelectorProvider* provider);
};

} // namespace Channels
} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CHANNELS_SINKCHANNEL_H__
