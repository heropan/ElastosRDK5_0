#include "SocketChannelImpl.h"
#include "CIoBridge.h"

using Libcore::IO::IIoBridge;
using Libcore::IO::CIoBridge;

namespace Elastos{
namespace IO{

//==========================================================
//       SocketChannelImpl::SocketAdapter
//==========================================================
SocketChannelImpl::SocketAdapter::SocketAdapter(
    /* [in] */ IPlainSocketImpl* socketImpl,
    /* [in] */ SocketChannelImpl* channel)
{
    // super(socketImpl);
    // this.socketImpl = socketImpl;
    // this.channel = channel;
    // SocketUtils.setCreated(this);

    // // Sync state socket state with the channel it is being created from
    // if (channel.isBound) {
    //     onBind(channel.localAddress, channel.localPort);
    // }
    // if (channel.isConnected()) {
    //     onConnect(channel.connectAddress.getAddress(), channel.connectAddress.getPort());
    // }
    // if (!channel.isOpen()) {
    //     onClose();
    // }
}

AutoPtr<SocketChannel> SocketChannelImpl::SocketAdapter::GetChannel()
{
    // return channel;
    return NULL;
}

ECode SocketChannelImpl::SocketAdapter::Connect(
    /* [in] */ ISocketAddress* remoteAddr,
    /* [in] */ Int32 timeout)
{
    // if (!channel.isBlocking()) {
    //     throw new IllegalBlockingModeException();
    // }
    // if (isConnected()) {
    //     throw new AlreadyConnectedException();
    // }
    // super.connect(remoteAddr, timeout);
    // channel.onBind(false);
    // if (super.isConnected()) {
    //     InetSocketAddress remoteInetAddress = (InetSocketAddress) remoteAddr;
    //     channel.onConnectStatusChanged(
    //             remoteInetAddress, SOCKET_STATUS_CONNECTED, false /* updateSocketState */);
    // }
    return NOERROR;
}

ECode SocketChannelImpl::SocketAdapter::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    // if (channel.isConnected()) {
    //     throw new AlreadyConnectedException();
    // }
    // if (SocketChannelImpl.SOCKET_STATUS_PENDING == channel.status) {
    //     throw new ConnectionPendingException();
    // }
    // super.bind(localAddr);
    // channel.onBind(false);
    return NOERROR;
}

ECode SocketChannelImpl::SocketAdapter::Close()
{
    // synchronized (channel) {
    //     super.close();
    //     if (channel.isOpen()) {
    //         // channel.close() recognizes the socket is closed and avoids recursion. There
    //         // is no channel.onClose() because the "closed" field is private.
    //         channel.close();
    //     }
    // }
    return NOERROR;
}

ECode SocketChannelImpl::SocketAdapter::GetInputStream(
    /* [out] */ IInputStream** inputStream)
{
    // return new BlockingCheckInputStream(super.getInputStream(), channel);
    return NOERROR;
}

ECode SocketChannelImpl::SocketAdapter::GetOutputStream(
    /* [out] */ IOutputStream** outstream)
{
    // return new BlockingCheckOutputStream(super.getOutputStream(), channel);
    return NOERROR;
}

ECode SocketChannelImpl::SocketAdapter::GetFileDescriptor(
    /* [out] */ IFileDescriptor** outfd)
{
    // return socketImpl.getFD$();
    return NOERROR;
}

//==========================================================
//       SocketChannelImpl::BlockingCheckOutputStream
//==========================================================
SocketChannelImpl::BlockingCheckOutputStream::BlockingCheckOutputStream(
    /* [in] */ IOutputStream* out,
    /* [in] */ SocketChannel* channel)
{
    // super(out);
    // this.channel = channel;
}

ECode SocketChannelImpl::BlockingCheckOutputStream::Write(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    // checkBlocking();
    // out.write(buffer, offset, byteCount);
    return NOERROR;
}

ECode SocketChannelImpl::BlockingCheckOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    // checkBlocking();
    // out.write(oneByte);
    return NOERROR;
}

ECode SocketChannelImpl::BlockingCheckOutputStream::Write(
    /* [in] */ ArrayOf<Byte>* buffer)
{
    // checkBlocking();
    // out.write(buffer);
    return NOERROR;
}

ECode SocketChannelImpl::BlockingCheckOutputStream::Close()
{
    // super.close();
    // // channel.close() recognizes the socket is closed and avoids recursion. There is no
    // // channel.onClose() because the "closed" field is private.
    // channel.close();
    return NOERROR;
}

ECode SocketChannelImpl::BlockingCheckOutputStream::CheckBlocking()
{
    // if (!channel.isBlocking()) {
    //     throw new IllegalBlockingModeException();
    // }
    return NOERROR;
}

//==========================================================
//       SocketChannelImpl::BlockingCheckInputStream
//==========================================================

SocketChannelImpl::BlockingCheckInputStream::BlockingCheckInputStream(
    /* [in] */ IInputStream* in,
    /* [in] */ SocketChannel* channel)
{
    // super(in);
    // this.channel = channel;
}

ECode SocketChannelImpl::BlockingCheckInputStream::Read(
    /* [out] */ Int32* value)
{
    // checkBlocking();
    // return in.read();
    return NOERROR;
}

ECode SocketChannelImpl::BlockingCheckInputStream::Read(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    // checkBlocking();
    // return in.read(buffer, byteOffset, byteCount);
    return NOERROR;
}

ECode SocketChannelImpl::BlockingCheckInputStream::Read(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    // checkBlocking();
    // return in.read(buffer);
    return NOERROR;
}

ECode SocketChannelImpl::BlockingCheckInputStream::Close()
{
    // super.close();
    // // channel.close() recognizes the socket is closed and avoids recursion. There is no
    // // channel.onClose() because the "closed" field is private.
    // channel.close();
    return NOERROR;
}

ECode SocketChannelImpl::BlockingCheckInputStream::CheckBlocking()
{
    // if (!channel.isBlocking()) {
    //     throw new IllegalBlockingModeException();
    // }
    return NOERROR;
}

const Int32 SocketChannelImpl::SOCKET_STATUS_UNINITITLIZED = -1;

const Int32 SocketChannelImpl::SOCKET_STATUS_UNCONNECTED = 0;

const Int32 SocketChannelImpl::SOCKET_STATUS_PENDING = 1;

const Int32 SocketChannelImpl::SOCKET_STATUS_CONNECTED = 2;

const Int32 SocketChannelImpl::SOCKET_STATUS_CLOSED = 3;

SocketChannelImpl::SocketChannelImpl(
    /* [in] */ ISelectorProvider* provider)
{
    // this(selectorProvider, true);
}

SocketChannelImpl::SocketChannelImpl(
    /* [in] */ ISelectorProvider* provider,
    /* [in] */ Boolean connect)
{
    // super(selectorProvider);
    // status = SOCKET_STATUS_UNCONNECTED;
    // fd = (connect ? IoBridge.socket(true) : new FileDescriptor());
}

SocketChannelImpl::SocketChannelImpl(
    /* [in] */ ISelectorProvider* provider,
    /* [in] */ IFileDescriptor* existingFd)
{
    // super(selectorProvider);
    // status = SOCKET_STATUS_CONNECTED;
    // fd = existingFd;
}

CAR_INTERFACE_IMPL(SocketChannelImpl, Object, IFileDescriptorChannel)

ECode SocketChannelImpl::GetSocket(
    /* [out] */ ISocket** socket)
{
    // if (socket == null) {
    //     try {
    //         InetAddress addr = null;
    //         int port = 0;
    //         if (connectAddress != null) {
    //             addr = connectAddress.getAddress();
    //             port = connectAddress.getPort();
    //         }
    //         socket = new SocketAdapter(new PlainSocketImpl(fd, localPort, addr, port), this);
    //     } catch (SocketException e) {
    //         return null;
    //     }
    // }
    // return socket;
    return NOERROR;
}

ECode SocketChannelImpl::IsConnected(
    /* [out] */ Boolean* ret)
{
    // return status == SOCKET_STATUS_CONNECTED;
    return NOERROR;
}

ECode SocketChannelImpl::IsConnectionPending(
    /* [out] */ Boolean* ret)
{
    // return status == SOCKET_STATUS_PENDING;
    return NOERROR;
}

ECode SocketChannelImpl::Connect(
    /* [in] */ ISocketAddress* addr,
    /* [out] */ Boolean* ret)
{
    // // status must be open and unconnected
    // checkUnconnected();

    // // check the address
    // InetSocketAddress inetSocketAddress = validateAddress(socketAddress);
    // InetAddress normalAddr = inetSocketAddress.getAddress();
    // int port = inetSocketAddress.getPort();

    // // When connecting, map ANY address to localhost
    // if (normalAddr.isAnyLocalAddress()) {
    //     normalAddr = InetAddress.getLocalHost();
    // }

    // boolean isBlocking = isBlocking();
    // boolean finished = false;
    // int newStatus;
    // try {
    //     if (isBlocking) {
    //         begin();
    //     }
    //     // When in blocking mode, IoBridge.connect() will return without an exception when the
    //     // socket is connected. When in non-blocking mode it will return without an exception
    //     // without knowing the result of the connection attempt, which could still be going on.
    //     IoBridge.connect(fd, normalAddr, port);
    //     newStatus = isBlocking ? SOCKET_STATUS_CONNECTED : SOCKET_STATUS_PENDING;
    //     finished = true;
    // } catch (IOException e) {
    //     if (isEINPROGRESS(e)) {
    //         newStatus = SOCKET_STATUS_PENDING;
    //     } else {
    //         if (isOpen()) {
    //             close();
    //             finished = true;
    //         }
    //         throw e;
    //     }
    // } finally {
    //     if (isBlocking) {
    //         end(finished);
    //     }
    // }

    // // If the channel was not bound, a connection attempt will have caused an implicit bind() to
    // // take place. Keep the local address state held by the channel and the socket up to date.
    // if (!isBound) {
    //     onBind(true /* updateSocketState */);
    // }

    // // Keep the connected state held by the channel and the socket up to date.
    // onConnectStatusChanged(inetSocketAddress, newStatus, true /* updateSocketState */);

    // return status == SOCKET_STATUS_CONNECTED;
    return NOERROR;
}

ECode SocketChannelImpl::FinishConnect(
    /* [out] */ Boolean* ret)
{
    // synchronized (this) {
    //     if (!isOpen()) {
    //         throw new ClosedChannelException();
    //     }
    //     if (status == SOCKET_STATUS_CONNECTED) {
    //         return true;
    //     }
    //     if (status != SOCKET_STATUS_PENDING) {
    //         throw new NoConnectionPendingException();
    //     }
    // }

    // boolean finished = false;
    // try {
    //     begin();
    //     InetAddress inetAddress = connectAddress.getAddress();
    //     int port = connectAddress.getPort();
    //     finished = IoBridge.isConnected(fd, inetAddress, port, 0, 0); // Return immediately.
    // } catch (ConnectException e) {
    //     if (isOpen()) {
    //         close();
    //         finished = true;
    //     }
    //     throw e;
    // } finally {
    //     end(finished);
    // }

    // synchronized (this) {
    //     status = (finished ? SOCKET_STATUS_CONNECTED : status);
    //     if (finished && socket != null) {
    //         socket.onConnect(connectAddress.getAddress(), connectAddress.getPort());
    //     }
    // }
    // return finished;
    return NOERROR;
}

ECode SocketChannelImpl::Read(
    /* [in] */ IByteBuffer* dst,
    /* [out] */ Int32* ret)
{
    // dst.checkWritable();
    // checkOpenConnected();
    // if (!dst.hasRemaining()) {
    //     return 0;
    // }
    // return readImpl(dst);
    return NOERROR;
}

ECode SocketChannelImpl::Read(
    /* [in] */ ArrayOf<IByteBuffer*>* target,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 len,
    /* [out] */ Int64* ret)
{
    // Arrays.checkOffsetAndCount(targets.length, offset, length);
    // checkOpenConnected();
    // int totalCount = FileChannelImpl.calculateTotalRemaining(targets, offset, length, true);
    // if (totalCount == 0) {
    //     return 0;
    // }
    // byte[] readArray = new byte[totalCount];
    // ByteBuffer readBuffer = ByteBuffer.wrap(readArray);
    // int readCount;
    // // read data to readBuffer, and then transfer data from readBuffer to targets.
    // readCount = readImpl(readBuffer);
    // readBuffer.flip();
    // if (readCount > 0) {
    //     int left = readCount;
    //     int index = offset;
    //     // transfer data from readArray to targets
    //     while (left > 0) {
    //         int putLength = Math.min(targets[index].remaining(), left);
    //         targets[index].put(readArray, readCount - left, putLength);
    //         index++;
    //         left -= putLength;
    //     }
    // }
    // return readCount;
    return NOERROR;
}

ECode SocketChannelImpl::Write(
    /* [in] */ IByteBuffer* src,
    /* [out] */ Int32* number)
{
    // if (src == null) {
    //     throw new NullPointerException("src == null");
    // }
    // checkOpenConnected();
    // if (!src.hasRemaining()) {
    //     return 0;
    // }
    // return writeImpl(src);
    return NOERROR;
}

ECode SocketChannelImpl::Write(
    /* [in] */ ArrayOf<IByteBuffer*>* source,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 len,
    /* [out] */ Int64* ret)
{
    // Arrays.checkOffsetAndCount(sources.length, offset, length);
    // checkOpenConnected();
    // int count = FileChannelImpl.calculateTotalRemaining(sources, offset, length, false);
    // if (count == 0) {
    //     return 0;
    // }
    // ByteBuffer writeBuf = ByteBuffer.allocate(count);
    // for (int val = offset; val < length + offset; val++) {
    //     ByteBuffer source = sources[val];
    //     int oldPosition = source.position();
    //     writeBuf.put(source);
    //     source.position(oldPosition);
    // }
    // writeBuf.flip();
    // int result = writeImpl(writeBuf);
    // int val = offset;
    // int written = result;
    // while (result > 0) {
    //     ByteBuffer source = sources[val];
    //     int gap = Math.min(result, source.remaining());
    //     source.position(source.position() + gap);
    //     val++;
    //     result -= gap;
    // }
    // return written;
    return NOERROR;
}

ECode SocketChannelImpl::GetLocalAddress(
    /* [out] */ IInetAddress** addr)
{
    return NOERROR;
}

ECode SocketChannelImpl::GetFD(
    /* [out] */ IFileDescriptor** dstcriptor)
{
    // return fd;
    return NOERROR;
}

ECode SocketChannelImpl::OnAccept(
    /* [in] */ IInetSocketAddress* remoteAddress,
    /* [in] */ Boolean updateSocketState)
{
    // onBind(updateSocketState);
    // onConnectStatusChanged(remoteAddress, SOCKET_STATUS_CONNECTED, updateSocketState);
    return NOERROR;
}

void SocketChannelImpl::OnBind(
    /* [in] */ Boolean updateSocketState)
{
    // SocketAddress sa;
    // try {
    //     sa = Libcore.os.getsockname(fd);
    // } catch (ErrnoException errnoException) {
    //     throw new AssertionError(errnoException);
    // }
    // isBound = true;
    // InetSocketAddress localSocketAddress = (InetSocketAddress) sa;
    // localAddress = localSocketAddress.getAddress();
    // localPort = localSocketAddress.getPort();
    // if (updateSocketState && socket != null) {
    //     socket.onBind(localAddress, localPort);
    // }
}

void SocketChannelImpl::OnConnectStatusChanged(
    /* [in] */ IInetSocketAddress* address,
    /* [in] */ Int32 status,
    /* [in] */ Boolean updateSocketState)
{
    // this.status = status;
    // connectAddress = address;
    // if (status == SOCKET_STATUS_CONNECTED && updateSocketState && socket != null) {
    //     socket.onConnect(connectAddress.getAddress(), connectAddress.getPort());
    // }
}

void SocketChannelImpl::SetConnected()
{
}

void SocketChannelImpl::SetBound(
    /* [in] */ Boolean flag)
{
}

void SocketChannelImpl::FinishAccept()
{
}

ECode SocketChannelImpl::ValidateAddress(
    /* [in] */ ISocketAddress* addr,
    /* [out] */ IInetSocketAddress** sockAddr)
{
    // if (socketAddress == null) {
    //     throw new IllegalArgumentException("socketAddress == null");
    // }
    // if (!(socketAddress instanceof InetSocketAddress)) {
    //     throw new UnsupportedAddressTypeException();
    // }
    // InetSocketAddress inetSocketAddress = (InetSocketAddress) socketAddress;
    // if (inetSocketAddress.isUnresolved()) {
    //     throw new UnresolvedAddressException();
    // }
    // return inetSocketAddress;
    return NOERROR;
}

ECode SocketChannelImpl::ImplCloseSelectableChannel()
{
    // if (status != SOCKET_STATUS_CLOSED) {
    //     status = SOCKET_STATUS_CLOSED;
    //     // IoBridge.closeAndSignalBlockedThreads(fd) is idempotent: It is safe to call on an
    //     // already-closed file descriptor.
    //     IoBridge.closeAndSignalBlockedThreads(fd);
    //     if (socket != null && !socket.isClosed()) {
    //         socket.onClose();
    //     }
    // }
    return NOERROR;
}

ECode SocketChannelImpl::ImplConfigureBlocking(
    /* [in] */ Boolean block)
{
    // IoUtils.setBlocking(fd, blocking);
    return NOERROR;
}

ECode SocketChannelImpl::InitLocalAddressAndPort()
{
    return NOERROR;
}

ECode SocketChannelImpl::ReadImpl(
    /* [in] */ IByteBuffer* dst,
    /* [out] */ Int32* ret)
{
    // synchronized (readLock) {
    //     int readCount = 0;
    //     try {
    //         if (isBlocking()) {
    //             begin();
    //         }
    //         readCount = IoBridge.recvfrom(true, fd, dst, 0, null, false);
    //         if (readCount > 0) {
    //             dst.position(dst.position() + readCount);
    //         }
    //     } finally {
    //         if (isBlocking()) {
    //             end(readCount > 0);
    //         }
    //     }
    //     return readCount;
    // }
    return NOERROR;
}

ECode SocketChannelImpl::WriteImpl(
    /* [in] */ IByteBuffer* src,
    /* [out] */ Int32* ret)
{
    // synchronized (writeLock) {
    //     if (!src.hasRemaining()) {
    //         return 0;
    //     }
    //     int writeCount = 0;
    //     try {
    //         if (isBlocking()) {
    //             begin();
    //         }
    //         writeCount = IoBridge.sendto(fd, src, 0, null, 0);
    //         if (writeCount > 0) {
    //             src.position(src.position() + writeCount);
    //         }
    //     } finally {
    //         if (isBlocking()) {
    //             end(writeCount >= 0);
    //         }
    //     }
    //     return writeCount;
    // }
    return NOERROR;
}

ECode SocketChannelImpl::CheckOpenConnected()
{
    // if (!isOpen()) {
    //     throw new ClosedChannelException();
    // }
    // if (!isConnected()) {
    //     throw new NotYetConnectedException();
    // }
    return NOERROR;
}

ECode SocketChannelImpl::CheckUnconnected()
{
    // if (!isOpen()) {
    //     throw new ClosedChannelException();
    // }
    // if (status == SOCKET_STATUS_CONNECTED) {
    //     throw new AlreadyConnectedException();
    // }
    // if (status == SOCKET_STATUS_PENDING) {
    //     throw new ConnectionPendingException();
    // }
    return NOERROR;
}

Boolean SocketChannelImpl::IsEINPROGRESS(
    /* [in] */ ECode e)
{
    // if (isBlocking()) {
    //     return false;
    // }
    // if (e instanceof ConnectException) {
    //     Throwable cause = e.getCause();
    //     if (cause instanceof ErrnoException) {
    //         return ((ErrnoException) cause).errno == EINPROGRESS;
    //     }
    // }
    // return false;
    return FALSE;
}

} // namespace IO
} // namespace Elastos
