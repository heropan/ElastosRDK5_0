
#ifndef __CDATAGRAMSOCKET_H__
#define __CDATAGRAMSOCKET_H__

#include "DatagramSocket.h"
#include "_CDatagramSocket.h"



using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Net {

CarClass(CDatagramSocket) , public DatagramSocket
{
public:
    CARAPI Close();

    CARAPI Connect(
        /* [in] */ ISocketAddress* peer);

    CARAPI Connect(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 aPort);

    CARAPI Disconnect();

    CARAPI GetInetAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetLocalPort(
        /* [out] */ Int32* port);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* size);

    CARAPI GetSendBufferSize(
        /* [out] */ Int32* size);

    CARAPI GetSoTimeout(
        /* [out] */ Int32* timeout);

    CARAPI Receive(
        /* [in] */ IDatagramPacket* pack);

    CARAPI Send(
        /* [in] */ IDatagramPacket* pack);

    CARAPI SetNetworkInterface(
        /* [out] */ INetworkInterface* netInterface);

    CARAPI SetSendBufferSize(
        /* [in] */ Int32 size);

    CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    CARAPI SetSoTimeout(
        /* [in] */ Int32 timeout);

    CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr);

    CARAPI IsBound(
        /* [out] */ Boolean* isBound);

    CARAPI IsConnected(
        /* [out] */ Boolean* isConnected);

    CARAPI GetRemoteSocketAddress(
        /* [out] */ ISocketAddress** address);

    CARAPI GetLocalSocketAddress(
        /* [out] */ ISocketAddress** address);

    CARAPI SetReuseAddress(
        /* [in] */ Boolean reuse);

    CARAPI GetReuseAddress(
        /* [out] */ Boolean* reuse);

    CARAPI SetBroadcast(
        /* [in] */ Boolean broadcast);

    CARAPI GetBroadcast(
        /* [out] */ Boolean* broadcast);

    CARAPI SetTrafficClass(
        /* [in] */ Int32 value);

    CARAPI GetTrafficClass(
        /* [out] */ Int32* value);

    CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    CARAPI GetChannel(
        /* [out] */ IDatagramChannel** channel);

    CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** fd);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 aPort);

    CARAPI constructor(
        /* [in] */ Int32 aport,
        /* [in] */ IInetAddress* addr);

    CARAPI constructor(
        /* [in] */ ISocketAddress* localAddr);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

} // namespace Net
} // namespace Elastos

#endif //__CDATAGRAMSOCKET_H__
