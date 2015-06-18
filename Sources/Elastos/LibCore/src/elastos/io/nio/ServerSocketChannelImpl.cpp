
#include "ServerSocketChannelImpl.h"

namespace Elastos {
namespace IO {

//==========================================================
//       ServerSocketChannelImpl::ServerSocketAdapter
//==========================================================
ServerSocketChannelImpl::ServerSocketAdapter::ServerSocketAdapter(
    /* [in] */ ServerSocketChannelImpl* aChannelImpl)
{
    // this.channelImpl = aChannelImpl;
}

CAR_INTERFACE_IMPL(ServerSocketChannelImpl, Object, IFileDescriptorChannel)

ECode ServerSocketChannelImpl::ServerSocketAdapter::Accept(
    /* [out] */ ISocket** socket)
{
    // if (!isBound()) {
    //     throw new IllegalBlockingModeException();
    // }
    // SocketChannel sc = channelImpl.accept();
    // if (sc == null) {
    //     throw new IllegalBlockingModeException();
    // }
    // return sc.socket();
    return NOERROR;
}

ECode ServerSocketChannelImpl::ServerSocketAdapter::ImplAccept(
    /* [in] */ SocketChannelImpl* clientSocketChannel,
    /* [out] */ ISocket** aSocket)
{
    // Socket clientSocket = clientSocketChannel.socket();
    // boolean connectOK = false;
    // try {
    //     synchronized (this) {
    //         super.implAccept(clientSocket);

    //         // Sync the client socket's associated channel state with the Socket and OS.
    //         InetSocketAddress remoteAddress =
    //                 new InetSocketAddress(
    //                         clientSocket.getInetAddress(), clientSocket.getPort());
    //         clientSocketChannel.onAccept(remoteAddress, false /* updateSocketState */);
    //     }
    //     connectOK = true;
    // } finally {
    //     if (!connectOK) {
    //         clientSocket.close();
    //     }
    // }
    // return clientSocket;
    return NOERROR;
}

ECode ServerSocketChannelImpl::ServerSocketAdapter::GetChannel(
    /* [out] */ IServerSocketChannel** channel)
{
    // return channelImpl;
    return NOERROR;
}

ECode ServerSocketChannelImpl::ServerSocketAdapter::Close()
{
    // synchronized (channelImpl) {
    //     super.close();
    //     if (channelImpl.isOpen()) {
    //         channelImpl.close();
    //     }
    // }
    return NOERROR;
}

AutoPtr<IFileDescriptor> ServerSocketChannelImpl::ServerSocketAdapter::GetFD()
{
    // return super.getImpl$().getFD$();
    return NOERROR;
}

//==========================================================
//       ServerSocketChannelImpl
//==========================================================

ServerSocketChannelImpl::ServerSocketChannelImpl(
    /* [in] */ SelectorProvider* sp)
{
    // super(sp);
    // this.socket = new ServerSocketAdapter(this);
}

ECode ServerSocketChannelImpl::Socket(
    /* [out] */ IServerSocket** outsock)
{
    // return socket;
    return NOERROR;
}

ECode ServerSocketChannelImpl::Accept(
    /* [in] */ ISocketChannel** channel)
{
    // if (!isOpen()) {
    //     throw new ClosedChannelException();
    // }
    // if (!socket.isBound()) {
    //     throw new NotYetBoundException();
    // }

    // // Create an empty socket channel. This will be populated by ServerSocketAdapter.implAccept.
    // SocketChannelImpl result = new SocketChannelImpl(provider(), false);
    // try {
    //     begin();
    //     synchronized (acceptLock) {
    //         try {
    //             socket.implAccept(result);
    //         } catch (SocketTimeoutException e) {
    //             if (shouldThrowSocketTimeoutExceptionFromAccept(e)) {
    //                 throw e;
    //             }
    //             // Otherwise, this is a non-blocking socket and there's nothing ready, so we'll
    //             // fall through and return null.
    //         }
    //     }
    // } finally {
    //     end(result.isConnected());
    // }
    // return result.isConnected() ? result : null;
    return NOERROR;
}

ECode ServerSocketChannelImpl::GetFD(
    /* [out] */ IFileDescriptor** outfd)
{
    // return socket.getFD$();
    return NOERROR;
}

Boolean ServerSocketChannelImpl::ShouldThrowSocketTimeoutExceptionFromAccept(
    /* [in] */ ECode e)
{
    // if (isBlocking()) {
    //     return true;
    // }
    // Throwable cause = e.getCause();
    // if (cause instanceof ErrnoException) {
    //     if (((ErrnoException) cause).errno == EAGAIN) {
    //         return false;
    //     }
    // }
    // return true;
    return FALSE;
}

ECode ServerSocketChannelImpl::ImplConfigureBlocking(
    /* [in] */ Boolean blocking)
{
    // IoUtils.setBlocking(socket.getFD$(), blocking);
    return NOERROR;
}

ECode ServerSocketChannelImpl::ImplCloseSelectableChannel()
{
    // if (!socket.isClosed()) {
    //     socket.close();
    // }
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
