#ifndef __ELASTOS_IO_CHANNELS_SOURCECHANNEL_H__
#define __ELASTOS_IO_CHANNELS_SOURCECHANNEL_H__

#include <Elastos.CoreLibrary_server.h>
#include "AbstractSelectableChannel.h"

using Elastos::IO::Channels::Spi::AbstractSelectableChannel;

namespace Elastos {
namespace IO {
namespace Channels {

class SourceChannel
    : public AbstractSelectableChannel
    , public IScatteringByteChannel
{
public:
    CARAPI ValidOps(
        /* [out] */ Int32* ret);

protected:
    SourceChannel(
        /* [in] */ ISelectorProvider* provider);
};

} // namespace Channels
} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CHANNELS_SOURCECHANNEL_H__
