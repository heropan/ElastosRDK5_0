
#include "cmdef.h"
#include "CServerSocket.h"

namespace Elastos {
namespace Net {

ECode CServerSocket::constructor()
{
    return ServerSocket::Init();
}

ECode CServerSocket::constructor(
    /* [in] */ Int32 aPort)
{
    return ServerSocket::Init(aPort);
}

ECode CServerSocket::constructor(
    /* [in] */ Int32 aport,
    /* [in] */ Int32 backlog)
{
    return ServerSocket::Init(aport, backlog);
}

ECode CServerSocket::constructor(
    /* [in] */ Int32 aport,
    /* [in] */ Int32 backlog,
    /* [in] */ IInetAddress* localAddr)
{
    return ServerSocket::Init(aport, backlog, localAddr);
}

ECode CServerSocket::GetImpl(
    /* [out] */ ISocketImpl** impl)
{
    return ServerSocket::GetImpl(impl);
}

ECode CServerSocket::Accept(
    /* [out] */ ISocket** socket)
{
    VALIDATE_NOT_NULL(socket);
    return ServerSocket::Accept(socket);
}

ECode CServerSocket::Close()
{
    return ServerSocket::Close();
}

ECode CServerSocket::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    return ServerSocket::GetInetAddress(address);
}

ECode CServerSocket::GetLocalPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    return ServerSocket::GetLocalPort(port);
}

ECode CServerSocket::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout);
    return ServerSocket::GetSoTimeout(timeout);
}

ECode CServerSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    return ServerSocket::SetSoTimeout(timeout);
}

ECode CServerSocket::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    return ServerSocket::Bind(localAddr);
}

ECode CServerSocket::BindEx(
    /* [in] */ ISocketAddress* localAddr,
    /* [in] */ Int32 backlog)
{
    return ServerSocket::BindEx(localAddr, backlog);
}

ECode CServerSocket::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);
    return ServerSocket::GetLocalSocketAddress(address);
}

ECode CServerSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    VALIDATE_NOT_NULL(isBound);
    return ServerSocket::IsBound(isBound);
}

ECode CServerSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    VALIDATE_NOT_NULL(isClosed);
    return ServerSocket::IsClosed(isClosed);
}

ECode CServerSocket::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    return ServerSocket::SetReuseAddress(reuse);
}

ECode CServerSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    VALIDATE_NOT_NULL(reuse);
    return ServerSocket::GetReuseAddress(reuse);
}

ECode CServerSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    return ServerSocket::SetReceiveBufferSize(size);
}

ECode CServerSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return ServerSocket::GetReceiveBufferSize(size);
}

ECode CServerSocket::GetChannel(
    /* [out] */ IServerSocketChannel** channel)
{
    VALIDATE_NOT_NULL(channel);
    return ServerSocket::GetChannel(channel);
}

ECode CServerSocket::SetPerformancePreferences(
    /* [in] */ Int32 connectionTime,
    /* [in] */ Int32 latency,
    /* [in] */ Int32 bandwidth)
{
    return ServerSocket::SetPerformancePreferences(connectionTime, latency, bandwidth);
}

} // namespace Net
} // namespace Elastos
