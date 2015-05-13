#ifndef __DATAGRAMCHANNELIMPL_H__
#define __DATAGRAMCHANNELIMPL_H__

#include "DatagramChannel.h"
#include "DatagramSocket.h"

using Elastos::IO::Channels::DatagramChannel;
using Elastos::IO::IFileDescriptorChannel;
using Elastos::IO::IFileDescriptor;
using Elastos::Net::IDatagramSocket;
using Elastos::Net::ISocketAddress;
using Elastos::Net::IInetSocketAddress;
using Elastos::Net::IDatagramPacket;
using Elastos::Net::DatagramSocket;
using Elastos::Net::IDatagramSocketImpl;
using Elastos::Net::INetworkInterface;

namespace Elastos {
namespace IO {

class DatagramChannelImpl
    : public DatagramChannel
    , public IFileDescriptorChannel
{
private:
    class DatagramSocketAdapter
        : public DatagramSocket
        , public IDatagramSocket
        , public ElRefBase
    {
    public:
        DatagramSocketAdapter(
            /* [in] */ IDatagramSocketImpl* socketimpl,
            /* [in] */ DatagramChannelImpl* channelimpl);

        CAR_INTERFACE_DECL();

        /**
         * Closes this UDP datagram socket and all possibly associated channels.
         */
        // In the documentation jdk1.1.7a/guide/net/miscNet.html, this method is
        // noted as not being synchronized.
        CARAPI Close();

        /**
         * Connects this datagram socket to the address and port specified by {peer}.
         * Future calls to send will use this as the default target, and receive
         *
         * @sa #send
         * @sa #receive
         * will only accept packets from this source.
         *
         * @return ECode = NOERROR(0) if success, else as follows:
         * Value | Meaning |
         * :-|:------------|
         * SocketException | if an error occurs.
         */
        CARAPI Connect(
            /* [in] */ ISocketAddress* peer);

        /**
         * Connects this datagram socket to the specific {address} and {port}.
         * Future calls to send will use this as the default target, and receive
         *
         * @sa #send
         * @sa #receive
         * will only accept packets from this source.
         *
         * <p>Beware: because it can't throw, this method silently ignores failures.
         * Use connect(SocketAddress) instead.
         *
         * @sa #connect(SocketAddress)
         */
        CARAPI ConnectEx(
            /* [in] */ IInetAddress* address,
            /* [in] */ Int32 aPort);

        /**
         * Disconnects this UDP datagram socket from the remote host. This method
         * called on an unconnected socket does nothing.
         */
        CARAPI Disconnect();

        /**
         * Gets the {InetAddress} instance representing the remote address to
         * which this UDP datagram socket is connected.
         *
         * @return the remote address this socket is connected to or {null} if
         *         this socket is not connected.
         */
        CARAPI GetInetAddress(
            /* [out] */ IInetAddress** address);

        /**
         * Gets the {InetAddress} instance representing the bound local
         * address of this UDP datagram socket.
         *
         * @return the local address to which this socket is bound to or
         *         {null} if this socket is closed.
         */
        CARAPI GetLocalAddress(
            /* [out] */ IInetAddress** address);

        /**
         * Gets the local port which this socket is bound to.
         *
         * @return the local port of this socket or {-1} if this socket is
         *         closed and {0} if it is unbound.
         */
        CARAPI GetLocalPort(
            /* [out] */ Int32* port);

        /**
         * Gets the remote port which this socket is connected to.
         *
         * @return the remote port of this socket. The return value {-1}
         *         indicates that this socket is not connected.
         */
        CARAPI GetPort(
            /* [out] */ Int32* port);

        /**
         * Returns this socket's SocketOptions#SO_RCVBUF receive buffer size.
         *
         * @sa SocketOptions#SO_RCVBUF receive buffer size
         */
        CARAPI GetReceiveBufferSize(
            /* [out] */ Int32* size);

        /**
         * Returns this socket's SocketOptions#SO_SNDBUF send buffer size.
         * @sa SocketOptions#SO_RCVBUF receive buffer size
         */
        CARAPI GetSendBufferSize(
            /* [out] */ Int32* size);

        /**
         * Gets the socket SocketOptions#SO_TIMEOUT receive timeout.
         *
         * @sa SocketOptions#SO_RCVBUF receive buffer size
         *
         * @return ECode = NOERROR(0) if success, else as follows:
         * Value | Meaning |
         * :-|:------------|
         * SocketException | if an error occurs while getting the option value.
         */
        CARAPI GetSoTimeout(
            /* [out] */ Int32* timeout);

        /**
         * Receives a packet from this socket and stores it in the argument
         * {pack}. All fields of {pack} must be set according to the data
         * received. If the received data is longer than the packet buffer size it
         * is truncated. This method blocks until a packet is received or a timeout
         * has expired.
         *
         * @param pack
         *            the {DatagramPacket} to store the received data.
         * @return ECode = NOERROR(0) if success, else as follows:
         * Value | Meaning |
         * :-|:------------|
         * IOException | if an error occurs while receiving the packet.
         */
        CARAPI Receive(
            /* [in] */ IDatagramPacket* pack);

        /**
         * Sends a packet over this socket.
         *
         * @param pack
         *            the {DatagramPacket} which has to be sent.
         * @return ECode = NOERROR(0) if success, else as follows:
         * Value | Meaning |
         * :-|:------------|
         * IOException | if an error occurs while sending the packet.
         */
        CARAPI Send(
            /* [in] */ IDatagramPacket* pack);

        /**
         * Sets the network interface used by this socket.  Any packets sent
         * via this socket are transmitted via the specified interface.  Any
         * packets received by this socket will come from the specified
         * interface.  Broadcast datagrams received on this interface will
         * be processed by this socket. This corresponds to Linux's SO_BINDTODEVICE.
         *
         * @hide used by GoogleTV for DHCP
         */
        CARAPI SetNetworkInterface(
            /* [in] */ INetworkInterface* netInterface);

        /**
         * Sets this socket's SocketOptions#SO_SNDBUF send buffer size.
         *
         * @sa SocketOptions#SO_SNDBUF send buffer size
         */
        CARAPI SetSendBufferSize(
            /* [in] */ Int32 size);

        /**
         * Sets this socket's SocketOptions#SO_SNDBUF receive buffer size.
         *
         * @sa SocketOptions#SO_SNDBUF send buffer size
         */
        CARAPI SetReceiveBufferSize(
            /* [in] */ Int32 size);

        /**
         * Sets the SocketOptions#SO_TIMEOUT read timeout in milliseconds for this socket.
         *
         * @sa SocketOptions#SO_TIMEOUT read timeout
         * This receive timeout defines the period the socket will block waiting to
         * receive data before throwing an {InterruptedIOException}. The value
         * {0} (default) is used to set an infinite timeout. To have effect
         * this option must be set before the blocking method was called.
         *
         * @param timeout the timeout in milliseconds or 0 for no timeout.
         * @return ECode = NOERROR(0) if success, else as follows:
         * Value | Meaning |
         * :-|:------------|
         * SocketException | if an error occurs while setting the option.
         */
        CARAPI SetSoTimeout(
            /* [in] */ Int32 timeout);

        /**
         * Binds this socket to the local address and port specified by
         * {localAddr}. If this value is {null} any free port on a valid local
         * address is used.
         *
         * @param localAddr
         *            the local machine address and port to bind on.
         * @return ECode = NOERROR(0) if success, else as follows:
         * Value | Meaning |
         * :-|:------------|
         * IllegalArgumentException | if the SocketAddress is not supported
         * SocketException | if the socket is already bound or a problem occurs during binding.
         */
        CARAPI Bind(
            /* [in] */ ISocketAddress* localAddr);

        /**
         * Returns true if this socket is bound to a local address. See bind.
         *
         * @sa #bind
         */
        CARAPI IsBound(
            /* [out] */ Boolean* isBound);

        /**
         * Returns true if this datagram socket is connected to a remote address.
         * See connect.
         *
         * @sa #connect
         */
        CARAPI IsConnected(
            /* [out] */ Boolean* isConnected);

        /**
         * Returns the {SocketAddress} this socket is connected to, or null for an unconnected
         * socket.
         */
        CARAPI GetRemoteSocketAddress(
            /* [out] */ ISocketAddress** address);

        /**
         * Returns the {SocketAddress} this socket is bound to, or null for an unbound socket.
         */
        CARAPI GetLocalSocketAddress(
            /* [out] */ ISocketAddress** address);

        /**
         * Sets the socket option {SocketOptions.SO_REUSEADDR}. This option
         * has to be enabled if more than one UDP socket wants to be bound to the
         * same address. That could be needed for receiving multicast packets.
         * <p>
         * There is an undefined behavior if this option is set after the socket is
         * already bound.
         *
         * @param reuse
         *            the socket option value to enable or disable this option.
         * @return ECode = NOERROR(0) if success, else as follows:
         * Value | Meaning |
         * :-|:------------|
         * SocketException | if the socket is closed or the option could not be set.
         */
        CARAPI SetReuseAddress(
            /* [in] */ Boolean reuse);

        /**
         * Gets the state of the socket option {SocketOptions.SO_REUSEADDR}.
         *
         * @return {true} if the option is enabled, {false} otherwise.
         * @return ECode = NOERROR(0) if success, else as follows:
         * Value | Meaning |
         * :-|:------------|
         * SocketException | if the socket is closed or the option is invalid.
         */
        CARAPI GetReuseAddress(
            /* [out] */ Boolean* reuse);

        /**
         * Sets the socket option {SocketOptions.SO_BROADCAST}. This option
         * must be enabled to send broadcast messages.
         *
         * @param broadcast
         *            the socket option value to enable or disable this option.
         * @return ECode = NOERROR(0) if success, else as follows:
         * Value | Meaning |
         * :-|:------------|
         * SocketException | if the socket is closed or the option could not be set.
         */
        CARAPI SetBroadcast(
            /* [in] */ Boolean broadcast);

        /**
         * Gets the state of the socket option {SocketOptions.SO_BROADCAST}.
         *
         * @return {true} if the option is enabled, {false} otherwise.
         * @return ECode = NOERROR(0) if success, else as follows:
         * Value | Meaning |
         * :-|:------------|
         * SocketException | if the socket is closed or the option is invalid.
         */
        CARAPI GetBroadcast(
            /* [out] */ Boolean* broadcast);

        /**
         * Sets the {@see SocketOptions#IP_TOS} value for every packet sent by this socket.
         *
         * SocketException | if the socket is closed or the option could not be set.
         */
        CARAPI SetTrafficClass(
            /* [in] */ Int32 value);

        /**
         * Returns this socket's {@see SocketOptions#IP_TOS} setting.
         *
         * @return ECode = NOERROR(0) if success, else as follows:
         * Value | Meaning |
         * :-|:------------|
         * SocketException | if the socket is closed or the option is invalid.
         */
        CARAPI GetTrafficClass(
            /* [out] */ Int32* value);

        /**
         * Gets the state of this socket.
         *
         * @return {true} if the socket is closed, {false} otherwise.
         */
        CARAPI IsClosed(
            /* [out] */ Boolean* isClosed);

        /**
         * Returns this socket's {DatagramChannel}, if one exists. A channel is
         * available only if this socket wraps a channel. (That is, you can go from a
         * channel to a socket and back again, but you can't go from an arbitrary socket to a channel.)
         * In practice, this means that the socket must have been created by
         * java.nio.channels.DatagramChannel#open.
         *
         * @sa java.nio.channels.DatagramChannel#open
         */
        CARAPI GetChannel(
            /* [out] */ IDatagramChannel** channel);

        CARAPI GetFileDescriptor(
            /* [out] */ IFileDescriptor** fd);

    protected:
        CARAPI_(Mutex*) GetSelfLock();

    private:
        DatagramChannelImpl* mChannelImpl;
        Mutex mMlock;
    };

public:
    CARAPI IsOpen(
        /* [out] */ Boolean* value);

    CARAPI Close();

    /*
     * Getting the internal DatagramSocket If we have not the socket, we create
     * a new one.
     */
     CARAPI GetSocket(
        /* [out] */ IDatagramSocket** socket);

     /**
      * @see java.nio.channels.DatagramChannel#isConnected()
      */
     Boolean IsConnected();

     /**
       * @see java.nio.channels.DatagramChannel#connect(java.net.SocketAddress)
       */
     CARAPI Connect(
        /* [in] */ ISocketAddress* address,
        /* [out] */ IDatagramChannel** channel);

     /**
       * @see java.nio.channels.DatagramChannel#disconnect()
       */
     CARAPI Disconnect();

     virtual CARAPI Receive(
        /* [in] */ IByteBuffer* target,
        /* [out] */ ISocketAddress** address);

protected:
    /*
     * Constructor
     */
    DatagramChannelImpl(
        /* [in] */ ISelectorProvider* provider);

    /**
     * Returns the local address to which the socket is bound.
     */
    CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** addr);

private:
    DatagramChannelImpl();

    CARAPI ReceiveImpl(
        /* [in] */ IByteBuffer* target,
        /* [in] */ Boolean loop,
        /* [out] */ ISocketAddress** addr);

protected:
    // The address to be connected.
    AutoPtr<IInetSocketAddress> mConnectAddress;

    // At first, uninitialized.
    Boolean mConnected /* = FALSE */;

    // whether the socket is bound
    Boolean mIsBound /* = FALSE */;

private:
    // The fd to interact with native code
    AutoPtr<IFileDescriptor> mFd;

    // Our internal DatagramSocket.
    AutoPtr<IDatagramSocket> mSocket;

    // local port
    Int32 mLocalPort;

    Object* mReadLock;
    Object* mWriteLock;
    AutoPtr<ISelectorProvider> mSelprovider;
};

} // namespace IO
} // namespace Elastos

#endif // __DATAGRAMCHANNELIMPL_H__
