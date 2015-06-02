
#include "coredef.h"
#include "SourceChannel.h"

namespace Elastos {
namespace IO {
namespace Channels {

SourceChannel::SourceChannel(
    /* [in] */ ISelectorProvider* provider)
    : AbstractSelectableChannel(provider)
{
    assert(NULL != provider);
}

ECode SourceChannel::ValidOps(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)
    *ret = ISelectionKey::OP_READ;
    return NOERROR;
}

} // namespace Channels
} // namespace IO
} // namespace Elastoss
