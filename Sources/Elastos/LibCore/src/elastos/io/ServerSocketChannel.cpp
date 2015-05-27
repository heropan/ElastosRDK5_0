
#include "coredef.h"
#include "ServerSocketChannel.h"

namespace Elastos {
namespace IO {
namespace Channels {

ServerSocketChannel::ServerSocketChannel(
    /* [in] */ ISelectorProvider* provider)
    : AbstractSelectableChannel(provider)
{
    assert(0 != provider);
}

ECode ServerSocketChannel::Open(
    /* [out] */ IServerSocketChannel** channel)
{
    return E_NOT_IMPLEMENTED;
}

ECode ServerSocketChannel::ValidOps(
    /* [out] */ Int32* ret)
{
    *ret = ISelectionKey::OP_ACCEPT;
    return NOERROR;
}

} // namespace Channels
} // namespace IO
} // namespace Elastos
