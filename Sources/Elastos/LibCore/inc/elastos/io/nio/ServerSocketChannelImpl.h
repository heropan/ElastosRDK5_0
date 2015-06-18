#ifndef __ELASTOS_IO_SERVERSOCKETCHANNELIMPL_H__
#define __ELASTOS_IO_SERVERSOCKETCHANNELIMPL_H__

#include "ServerSocketChannel.h"
#include "SocketChannelImpl.h"
#include "SelectorProvider.h"
// #include "ServerSocket.h"

using Elastos::IO::Channels::IServerSocketChannel;
using Elastos::IO::Channels::ServerSocketChannel;
using Elastos::IO::Channels::ISocketChannel;
using Elastos::IO::Channels::Spi::SelectorProvider;
using Elastos::Net::ISocket;

namespace Elastos {
namespace IO {

class ServerSocketChannelImpl
    : public ServerSocketChannel
    , public IFileDescriptorChannel
{
private:
    // extends ServerSocket
    class ServerSocketAdapter : public Object
    {
    protected:
        ServerSocketAdapter(
            /* [in] */ ServerSocketChannelImpl* aChannelImpl);

        CARAPI Accept(
            /* [out] */ ISocket** socket);

        CARAPI ImplAccept(
            /* [in] */ SocketChannelImpl* clientSocketChannel,
            /* [out] */ ISocket** aSocket);

        CARAPI GetChannel(
            /* [out] */ IServerSocketChannel** channel);

        CARAPI Close();

    private:
        AutoPtr<IFileDescriptor> GetFD();

    private:
        AutoPtr<ServerSocketChannelImpl> mChannelImpl;
    };

public:
    ServerSocketChannelImpl(
        /* [in] */ SelectorProvider* sp);

    CAR_INTERFACE_DECL()

    CARAPI Socket(
        /* [out] */ IServerSocket** outsock);

    CARAPI Accept(
        /* [in] */ ISocketChannel** channel);

    CARAPI GetFD(
        /* [out] */ IFileDescriptor** outfd);

private:
    CARAPI_(Boolean) ShouldThrowSocketTimeoutExceptionFromAccept(
        /* [in] */ ECode e);

protected:
    CARAPI ImplConfigureBlocking(
        /* [in] */ Boolean blocking);

    CARAPI ImplCloseSelectableChannel();

private:
    AutoPtr<ServerSocketAdapter> socket;
    Object mAcceptLock;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_SERVERSOCKETCHANNELIMPL_H__
