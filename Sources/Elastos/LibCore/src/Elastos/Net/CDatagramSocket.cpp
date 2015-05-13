
#include "cmdef.h"
#include "CDatagramSocket.h"

namespace Elastos {
namespace Net {

ECode CDatagramSocket::Close()
{
    return DatagramSocket::Close();
}

ECode CDatagramSocket::ConnectEx(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 aPort)
{
    return DatagramSocket::Connect(address, aPort);
}

ECode CDatagramSocket::Connect(
    /* [in] */ ISocketAddress* peer)
{
    return DatagramSocket::Connect(peer);
}
ECode CDatagramSocket::Disconnect()
{
    return DatagramSocket::Disconnect();
}

ECode CDatagramSocket::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    return DatagramSocket::GetInetAddress(address);
}

ECode CDatagramSocket::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    return DatagramSocket::GetLocalAddress(address);
}

ECode CDatagramSocket::GetLocalPort(
    /* [out] */ Int32* port)
{
    return DatagramSocket::GetLocalPort(port);
}

ECode CDatagramSocket::GetPort(
    /* [out] */ Int32* port)
{
    return DatagramSocket::GetPort(port);
}

ECode CDatagramSocket::SetNetworkInterface(
    /* [out] */ INetworkInterface* netInterface)
{
    return DatagramSocket::SetNetworkInterface(netInterface);
}

ECode CDatagramSocket::GetSendBufferSize(
    /* [out] */ Int32* size)
{
    return DatagramSocket::GetSendBufferSize(size);
}

ECode CDatagramSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    return DatagramSocket::GetReceiveBufferSize(size);
}

ECode CDatagramSocket::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    return DatagramSocket::GetSoTimeout(timeout);
}

ECode CDatagramSocket::Receive(
    /* [in] */ IDatagramPacket* pack)
{
    return DatagramSocket::Receive(pack);
}

ECode CDatagramSocket::Send(
    /* [in] */ IDatagramPacket* pack)
{
    return DatagramSocket::Send(pack);
}

ECode CDatagramSocket::SetSendBufferSize(
    /* [in] */ Int32 size)
{
    return DatagramSocket::SetSendBufferSize(size);
}

ECode CDatagramSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    return DatagramSocket::SetReceiveBufferSize(size);
}

ECode CDatagramSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    return DatagramSocket::SetSoTimeout(timeout);
}

ECode CDatagramSocket::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    return DatagramSocket::Bind(localAddr);
}

ECode CDatagramSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    return DatagramSocket::IsBound(isBound);
}

ECode CDatagramSocket::IsConnected(
    /* [out] */ Boolean* isConnected)
{
    return DatagramSocket::IsConnected(isConnected);
}

ECode CDatagramSocket::GetRemoteSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    return DatagramSocket::GetRemoteSocketAddress(address);
}

ECode CDatagramSocket::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    return DatagramSocket::GetLocalSocketAddress(address);
}

ECode CDatagramSocket::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    return DatagramSocket::SetReuseAddress(reuse);
}

ECode CDatagramSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    return DatagramSocket::GetReuseAddress(reuse);
}

ECode CDatagramSocket::SetBroadcast(
    /* [in] */ Boolean broadcast)
{
    return DatagramSocket::SetBroadcast(broadcast);
}

ECode CDatagramSocket::GetBroadcast(
    /* [out] */ Boolean* broadcast)
{
    return DatagramSocket::GetBroadcast(broadcast);
}

ECode CDatagramSocket::SetTrafficClass(
    /* [in] */ Int32 value)
{
    return DatagramSocket::SetTrafficClass(value);
}

ECode CDatagramSocket::GetTrafficClass(
    /* [out] */ Int32* value)
{
    return DatagramSocket::GetTrafficClass(value);
}

ECode CDatagramSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    return DatagramSocket::IsClosed(isClosed);
}

ECode CDatagramSocket::GetChannel(
    /* [out] */ IDatagramChannel** channel)
{
    return DatagramSocket::GetChannel(channel);
}

ECode CDatagramSocket::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fd)
{
    return DatagramSocket::GetFileDescriptor(fd);
}

ECode CDatagramSocket::constructor()
{
    return DatagramSocket::Init();
}

ECode CDatagramSocket::constructor(
    /* [in] */ Int32 aPort)
{
    return DatagramSocket::Init(aPort);
}

ECode CDatagramSocket::constructor(
    /* [in] */ Int32 aport,
    /* [in] */ IInetAddress* addr)
{
    return DatagramSocket::Init(aport, addr);
}

ECode CDatagramSocket::constructor(
    /* [in] */ ISocketAddress* localAddr)
{
    return DatagramSocket::Init(localAddr);
}

Mutex* CDatagramSocket::GetSelfLock()
{
    return &_m_syncLock;
}

} // namespace Net
} // namespace Elastos
