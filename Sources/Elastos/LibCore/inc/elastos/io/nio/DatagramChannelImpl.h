#ifndef __ELASTOS_IO_DATAGRAMCHANNELIMPL_H__
#define __ELASTOS_IO_DATAGRAMCHANNELIMPL_H__

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
    {
    public:
        DatagramSocketAdapter();

        CARAPI constructor(
            /* [in] */ IDatagramSocketImpl* socketimpl,
            /* [in] */ DatagramChannelImpl* channelimpl);

        CARAPI Bind(
            /* [in] */ ISocketAddress* localAddr);

        CARAPI GetChannel(
            /* [out] */ IDatagramChannel** channel);

        CARAPI Connect(
            /* [in] */ ISocketAddress* peer);

        CARAPI Connect(
            /* [in] */ IInetAddress* address,
            /* [in] */ Int32 aPort);

        CARAPI Receive(
            /* [in] */ IDatagramPacket* pack);

        CARAPI Send(
            /* [in] */ IDatagramPacket* pack);

        CARAPI Close();

        CARAPI Disconnect();

    private:
        AutoPtr<DatagramChannelImpl> mChannelImpl;
    };

public:
    CAR_INTERFACE_DECL()

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
    Boolean mConnected;

    // whether the socket is bound
    Boolean mIsBound;

    AutoPtr<IInetAddress> mLocalAddress;

private:
    // The fd to interact with native code
    AutoPtr<IFileDescriptor> mFd;

    // Our internal DatagramSocket.
    AutoPtr<IDatagramSocket> mSocket;

    // local port
    Int32 mLocalPort;

    Object mReadLock;
    Object mWriteLock;
    AutoPtr<ISelectorProvider> mSelprovider;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_DATAGRAMCHANNELIMPL_H__
