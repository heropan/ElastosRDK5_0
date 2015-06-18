#ifndef __ELASTOS_IO_SERVERSOCKET_CHANNEL_H__
#define __ELASTOS_IO_SERVERSOCKET_CHANNEL_H__

#include "AbstractSelectableChannel.h"
#include "Elastos.CoreLibrary_server.h"

using Elastos::IO::Channels::Spi::AbstractSelectableChannel;
using Elastos::IO::Channels::ISelectionKey;
using Elastos::IO::IByteBuffer;
using Elastos::Net::IDatagramSocket;
using Elastos::Net::ISocketAddress;
using Elastos::Net::IServerSocket;

namespace Elastos {
namespace IO {
namespace Channels {

class ServerSocketChannel : public AbstractSelectableChannel
{
public:
    ServerSocketChannel();

    ServerSocketChannel(
        /* [in] */ ISelectorProvider* provider);

    static CARAPI Open(
        /* [in] */ IServerSocketChannel** channel);

    CARAPI ValidOps(
        /* [out] */ Int32* ret);

    virtual CARAPI GetSocket(
        /* [in] */ IServerSocket** socket) = 0;

    virtual CARAPI Accept(
        /* [in] */ ISocketChannel** channel) = 0;
};

} // namespace Channels
} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_NET_SERVERSOCKET_CHANNEL_H__
