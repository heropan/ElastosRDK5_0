
#include "CSocket.h"

namespace Elastos {
namespace Net {

ECode CSocket::constructor()
{
    return Socket::Init();
}

ECode CSocket::constructor(
    /* [in] */ IProxy* proxy)
{
    return Socket::Init(proxy);
}

ECode CSocket::constructor(
    /* [in] */ const String& dstName,
    /* [in] */ Int32 dstPort)
{
    return Socket::Init(dstName, dstPort);
}

ECode CSocket::constructor(
    /* [in] */ const String& dstName,
    /* [in] */ Int32 dstPort,
    /* [in] */ IInetAddress* localAddress,
    /* [in] */ Int32 localPort)
{
    return Socket::Init(dstName, dstPort, localAddress, localPort);
}

ECode CSocket::constructor(
    /* [in] */ const String& hostName,
    /* [in] */ Int32 port,
    /* [in] */ Boolean streaming)
{
    return Socket::Init(hostName, port, streaming);
}

ECode CSocket::constructor(
    /* [in] */ IInetAddress* dstAddress,
    /* [in] */ Int32 dstPort)
{
    return Socket::Init(dstAddress, dstPort);
}

ECode CSocket::constructor(
    /* [in] */ IInetAddress* dstAddress,
    /* [in] */ Int32 dstPort,
    /* [in] */ IInetAddress* localAddress,
    /* [in] */ Int32 localPort)
{
    return Socket::Init(dstAddress, dstPort, localAddress, localPort);
}

ECode CSocket::constructor(
    /* [in] */ IInetAddress* addr,
    /* [in] */ Int32 port,
    /* [in] */ Boolean streaming)
{
    return Socket::Init(addr, port, streaming);
}

ECode CSocket::Close()
{
    return Socket::Close();
}

ECode CSocket::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    return Socket::GetInetAddress(address);
}

ECode CSocket::GetInputStream(
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is);
    return Socket::GetInputStream(is);
}

ECode CSocket::GetKeepAlive(
    /* [out] */ Boolean* keepAlive)
{
    VALIDATE_NOT_NULL(keepAlive);

    return Socket::GetKeepAlive(keepAlive);
}

ECode CSocket::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    return Socket::GetLocalAddress(address);
}

ECode CSocket::GetLocalPort(
    /* [out] */ Int32* localPort)
{
    VALIDATE_NOT_NULL(localPort);

    return Socket::GetLocalPort(localPort);
}

ECode CSocket::GetOutputStream(
    /* [out] */ IOutputStream** os)
{
    VALIDATE_NOT_NULL(os);

    return Socket::GetOutputStream(os);
}

ECode CSocket::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    return Socket::GetPort(port);
}

ECode CSocket::GetSoLinger(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return Socket::GetSoLinger(value);
}

ECode CSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    return Socket::GetReceiveBufferSize(size);
}

ECode CSocket::GetSendBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    return Socket::GetSendBufferSize(size);
}

ECode CSocket::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout);

    return Socket::GetSoTimeout(timeout);
}

ECode CSocket::GetTcpNoDelay(
    /* [out] */ Boolean* noDelay)
{
    VALIDATE_NOT_NULL(noDelay);

    return Socket::GetTcpNoDelay(noDelay);
}

ECode CSocket::SetKeepAlive(
    /* [in] */ Boolean keepAlive)
{
    return Socket::SetKeepAlive(keepAlive);
}

ECode CSocket::SetSendBufferSize(
    /* [in] */ Int32 size)
{
    return Socket::SetSendBufferSize(size);
}

ECode CSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    return Socket::SetReceiveBufferSize(size);
}

ECode CSocket::SetSoLinger(
    /* [in] */ Boolean on,
    /* [in] */ Int32 timeout)
{
    return Socket::SetSoLinger(on, timeout);
}

ECode CSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    return Socket::SetSoTimeout(timeout);
}

ECode CSocket::SetTcpNoDelay(
    /* [in] */ Boolean on)
{
    return Socket::SetTcpNoDelay(on);
}

ECode CSocket::ShutdownInput()
{
    return Socket::ShutdownInput();
}

ECode CSocket::ShutdownOutput()
{
    return Socket::ShutdownOutput();
}

ECode CSocket::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);

    return Socket::GetLocalSocketAddress(address);
}

ECode CSocket::GetRemoteSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);

    return Socket::GetRemoteSocketAddress(address);
}

ECode CSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    VALIDATE_NOT_NULL(isBound);

    return Socket::IsBound(isBound);
}

ECode CSocket::IsConnected(
    /* [out] */ Boolean* isConnected)
{
    VALIDATE_NOT_NULL(isConnected);

    return Socket::IsConnected(isConnected);
}

ECode CSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    VALIDATE_NOT_NULL(isClosed);

    return Socket::IsClosed(isClosed);
}

ECode CSocket::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    return Socket::Bind(localAddr);
}

ECode CSocket::Connect(
    /* [in] */ ISocketAddress* remoteAddr)
{
    return Socket::Connect(remoteAddr);
}

ECode CSocket::Connect(
    /* [in] */ ISocketAddress* remoteAddr,
    /* [in] */ Int32 timeout)
{
    return Socket::Connect(remoteAddr, timeout);
}

ECode CSocket::IsInputShutdown(
    /* [out] */ Boolean* isInputShutdown)
{
    VALIDATE_NOT_NULL(isInputShutdown);

    return Socket::IsInputShutdown(isInputShutdown);
}

ECode CSocket::IsOutputShutdown(
    /* [out] */ Boolean* isOutputShutdown)
{
    VALIDATE_NOT_NULL(isOutputShutdown);

    return Socket::IsOutputShutdown(isOutputShutdown);
}

ECode CSocket::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    return Socket::SetReuseAddress(reuse);
}

ECode CSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    VALIDATE_NOT_NULL(reuse);

    return Socket::GetReuseAddress(reuse);
}

ECode CSocket::SetOOBInline(
    /* [in] */ Boolean oobinline)
{
    return Socket::SetOOBInline(oobinline);
}

ECode CSocket::GetOOBInline(
    /* [out] */ Boolean* oobinline)
{
    VALIDATE_NOT_NULL(oobinline);

    return Socket::GetOOBInline(oobinline);
}

ECode CSocket::SetTrafficClass(
    /* [in] */ Int32 value)
{
    return Socket::SetTrafficClass(value);
}

ECode CSocket::GetTrafficClass(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return Socket::GetTrafficClass(value);
}

ECode CSocket::SendUrgentData(
    /* [in] */ Int32 value)
{
    return Socket::SendUrgentData(value);
}

ECode CSocket::GetChannel(
    /* [out] */ ISocketChannel** channel)
{
    VALIDATE_NOT_NULL(channel);
    return Socket::GetChannel(channel);
}

ECode CSocket::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);
    return Socket::GetFileDescriptor(fd);
}

ECode CSocket::SetPerformancePreferences(
    /* [in] */ Int32 connectionTime,
    /* [in] */ Int32 latency,
    /* [in] */ Int32 bandwidth)
{
    return Socket::SetPerformancePreferences(connectionTime, latency, bandwidth);
}

Mutex* CSocket::GetSelfLock()
{
    return &_m_syncLock;
}

} // namespace Net
} // namespace Elastos
