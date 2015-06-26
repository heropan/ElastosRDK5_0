
#ifndef __ELASTOS_NET_DATAGRAMSOCKET_H__
#define __ELASTOS_NET_DATAGRAMSOCKET_H__

#include "Object.h"

using Elastos::Net::IInetAddress;
using Elastos::Net::IDatagramSocket;
using Elastos::IO::ICloseable;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::Channels::IDatagramChannel;
using Elastos::Net::IInetAddress;
using Elastos::Net::IDatagramSocket;

namespace Elastos {
namespace Net {

class DatagramSocket
    : public Object
    , public IDatagramSocket
    , public ICloseable
{
public:
    CAR_INTERFACE_DECL()

    virtual CARAPI Close();

    virtual CARAPI OnClose();

    virtual CARAPI Connect(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 aPort);

    virtual CARAPI OnConnect(
        /* [in] */ IInetAddress* remoteAddress,
        /* [in] */ Int32 remotePort);

    virtual CARAPI Disconnect();

    virtual CARAPI OnDisconnect();

    virtual CARAPI CreateSocket(
        /* [in] */ Int32 aPort,
        /* [in] */ IInetAddress* addr);

    virtual CARAPI GetInetAddress(
        /* [out] */ IInetAddress** address);

    virtual CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** address);

    virtual CARAPI GetLocalPort(
        /* [out] */ Int32* port);

    virtual CARAPI GetPort(
        /* [out] */ Int32* port);

    virtual CARAPI_(Boolean) IsMulticastSocket();

    virtual CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* size);

    virtual CARAPI GetSendBufferSize(
        /* [out] */ Int32* size);

    virtual CARAPI GetSoTimeout(
        /* [out] */ Int32* timeout);

    virtual CARAPI Receive(
        /* [in, out] */ IDatagramPacket* pack);

    virtual CARAPI Send(
        /* [in] */ IDatagramPacket* pack);

    virtual CARAPI SetSendBufferSize(
        /* [in] */ Int32 size);

    virtual CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    virtual CARAPI SetSoTimeout(
        /* [in] */ Int32 timeout);

    static CARAPI SetDatagramSocketImplFactory(
        /* [in] */ IDatagramSocketImplFactory* fac);

    virtual CARAPI CheckOpen();

    virtual CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr);

    virtual CARAPI OnBind(
        /* [in] */ IInetAddress* localAddress,
        /* [in] */ Int32 localPort);

    virtual CARAPI Connect(
        /* [in] */ ISocketAddress* peer);

    virtual CARAPI IsBound(
        /* [out] */ Boolean* isBound);

    virtual CARAPI IsConnected(
        /* [out] */ Boolean* isConnected);

    virtual CARAPI GetRemoteSocketAddress(
        /* [out] */ ISocketAddress** address);

    virtual CARAPI GetLocalSocketAddress(
        /* [out] */ ISocketAddress** address);

    virtual CARAPI SetReuseAddress(
        /* [in] */ Boolean reuse);

    virtual CARAPI GetReuseAddress(
        /* [out] */ Boolean* reuse);

    virtual CARAPI SetBroadcast(
        /* [in] */ Boolean broadcast);

    virtual CARAPI GetBroadcast(
        /* [out] */ Boolean* broadcast);

    virtual CARAPI SetTrafficClass(
        /* [in] */ Int32 value);

    virtual CARAPI GetTrafficClass(
        /* [out] */ Int32* value);

    virtual CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    virtual CARAPI GetChannel(
        /* [out] */ IDatagramChannel** channel);

    virtual CARAPI SetNetworkInterface(
        /* [in] */ INetworkInterface* netInterface);

    virtual CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** fd);

//protected:
    DatagramSocket();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 aPort);

    CARAPI constructor(
        /* [in] */ Int32 aport,
        /* [in] */ IInetAddress* addr);

    CARAPI constructor(
        /* [in] */ ISocketAddress* localAddr);

private:
    CARAPI CheckPort(
        /* [in] */ Int32 aPort);

    CARAPI EnsureBound();

public:
    static const String TAG;
    AutoPtr<IDatagramSocketImpl> mImpl;

    AutoPtr<IInetAddress> mAddress;

    Int32 mPort;

    static AutoPtr<IDatagramSocketImplFactory> mFactory;
    static Object sLock;

    Boolean mIsBound;

private:
    Boolean mIsConnected;

    ECode mPendingConnectException;

    Boolean mIsClosed;

    Object mLock;
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_DATAGRAMSOCKET_H__
