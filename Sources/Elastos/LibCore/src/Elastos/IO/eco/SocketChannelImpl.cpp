#include "SocketChannelImpl.h"

using Libcore::IO::IIoBridge;
using Libcore::IO::CIoBridge;

namespace Elastos{
namespace IO{

Int32 SocketChannelImpl::SOCKET_STATUS_UNINITITLIZED = -1;

// Status before connect.
Int32 SocketChannelImpl::SOCKET_STATUS_UNCONNECTED = 0;

// Status connection pending.
Int32 SocketChannelImpl::SOCKET_STATUS_PENDING = 1;

// Status after connection success.
Int32 SocketChannelImpl::SOCKET_STATUS_CONNECTED = 2;

// Status closed.
Int32 SocketChannelImpl::SOCKET_STATUS_CLOSED = 3;

SocketChannelImpl::SocketChannelImpl(
    /* [in] */ ISelectorProvider* provider)
    : mStatus(SOCKET_STATUS_UNINITITLIZED)
    , mIsBound(FALSE)
{
    assert(NULL != provider);
    SocketChannelImpl(provider, TRUE);
}

SocketChannelImpl::SocketChannelImpl(
    /* [in] */ ISelectorProvider* provider,
    /* [in] */ Boolean connect)
    : SocketChannel(provider)
    , mStatus(SOCKET_STATUS_UNCONNECTED)
    , mIsBound(FALSE)
{
}

} // namespace IO
} // namespace Elastos
