
#include "SinkChannel.h"

namespace Elastos {
namespace IO {
namespace Channels {

CAR_INTERFACE_DECL(SinkChannel, AbstractSelectableChannel, IIGatheringByteChannel)

SinkChannel::SinkChannel(
    /* [in] */ ISelectorProvider* provider)
    : AbstractSelectableChannel(provider)
{
    assert(NULL != provider);
}

ECode SinkChannel::ValidOps(
    /* [out] */ Int32* ret)
{
	VALIDATE_NOT_NULL(ret)
    *ret = ISelectionKey::OP_WRITE;
    return NOERROR;
}

} // namespace Channels
} // namespace IO
} // namespace Elastoss
