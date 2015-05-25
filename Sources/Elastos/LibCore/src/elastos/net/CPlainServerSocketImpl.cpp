
#include "CPlainServerSocketImpl.h"

namespace Elastos {
namespace Net {

ECode CPlainServerSocketImpl::InitLocalPort(
    /* [in] */ Int32 localPort)
{
    return PlainServerSocketImpl::InitLocalPort(localPort);
}

ECode CPlainServerSocketImpl::InitRemoteAddressAndPort(
    /* [in] */ IInetAddress* remoteAddress,
    /* [in] */ Int32 remotePort)
{
    return PlainServerSocketImpl::InitRemoteAddressAndPort(remoteAddress, remotePort);
}

ECode CPlainServerSocketImpl::SocksAccept()
{
    return PlainServerSocketImpl::SocksAccept();
}

ECode CPlainServerSocketImpl::GetOption(
    /* [in] */ Int32 optID,
    /* [out] */ IInterface ** ppOptVal)
{
    return PlainServerSocketImpl::GetOption(optID, ppOptVal);
}

ECode CPlainServerSocketImpl::SetOption(
    /* [in] */ Int32 optID,
    /* [in] */ IInterface * pOptVal)
{
    return PlainServerSocketImpl::SetOption(optID, pOptVal);
}

ECode CPlainServerSocketImpl::Accept(
    /* [in] */ ISocketImpl * pNewSocket)
{
    return PlainServerSocketImpl::Accept(pNewSocket);
}

ECode CPlainServerSocketImpl::Available(
    /* [out] */ Int32 * pNum)
{
    return PlainServerSocketImpl::Available(pNum);
}

ECode CPlainServerSocketImpl::Bind(
    /* [in] */ IInetAddress * pAddress,
    /* [in] */ Int32 port)
{
    return PlainServerSocketImpl::Bind(pAddress, port);
}

ECode CPlainServerSocketImpl::Close()
{
    return PlainServerSocketImpl::Close();
}

ECode CPlainServerSocketImpl::Connect(
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    return PlainServerSocketImpl::Connect(host, port);
}

ECode CPlainServerSocketImpl::ConnectEx(
    /* [in] */ IInetAddress * pAddress,
    /* [in] */ Int32 port)
{
    return PlainServerSocketImpl::Connect(pAddress, port);
}

ECode CPlainServerSocketImpl::Create(
    /* [in] */ Boolean isStreaming)
{
    return PlainServerSocketImpl::Create(isStreaming);
}

ECode CPlainServerSocketImpl::GetFileDescriptor(
    /* [out] */ IFileDescriptor ** ppFileDescriptor)
{
    return PlainServerSocketImpl::GetFD(ppFileDescriptor);
}

ECode CPlainServerSocketImpl::GetInetAddress(
    /* [out] */ IInetAddress ** ppNetAddress)
{
    return PlainServerSocketImpl::GetInetAddress(ppNetAddress);
}

ECode CPlainServerSocketImpl::GetInputStream(
    /* [out] */ IInputStream ** ppInputStream)
{
    return PlainServerSocketImpl::GetInputStream(ppInputStream);
}

ECode CPlainServerSocketImpl::GetLocalPort(
    /* [out] */ Int32 * pNum)
{
    return PlainServerSocketImpl::GetLocalPort(pNum);
}

ECode CPlainServerSocketImpl::GetOutputStream(
    /* [out] */ IOutputStream ** ppOutputStream)
{
    return PlainServerSocketImpl::GetOutputStream(ppOutputStream);
}

ECode CPlainServerSocketImpl::GetPort(
    /* [out] */ Int32 * pPort)
{
    return PlainServerSocketImpl::GetPort(pPort);
}

ECode CPlainServerSocketImpl::Listen(
    /* [in] */ Int32 backlog)
{
    return PlainServerSocketImpl::Listen(backlog);
}

ECode CPlainServerSocketImpl::ShutdownInput()
{
    return PlainServerSocketImpl::ShutdownInput();
}

ECode CPlainServerSocketImpl::ShutdownOutput()
{
    return PlainServerSocketImpl::ShutdownOutput();
}

ECode CPlainServerSocketImpl::ConnectEx2(
    /* [in] */ ISocketAddress * pRemoteAddr,
    /* [in] */ Int32 timeout)
{
    return PlainServerSocketImpl::Connect(pRemoteAddr, timeout);
}

ECode CPlainServerSocketImpl::SupportsUrgentData(
    /* [out] */ Boolean * pIsSupports)
{
    return PlainServerSocketImpl::SupportsUrgentData(pIsSupports);
}

ECode CPlainServerSocketImpl::SendUrgentData(
    /* [in] */ Int32 value)
{
    return PlainServerSocketImpl::SendUrgentData(value);
}

ECode CPlainServerSocketImpl::SetPerformancePreferences(
    /* [in] */ Int32 connectionTime,
    /* [in] */ Int32 latency,
    /* [in] */ Int32 bandwidth)
{
    return PlainServerSocketImpl::SetPerformancePreferences(connectionTime, latency, bandwidth);
}

ECode CPlainServerSocketImpl::constructor()
{
    return PlainServerSocketImpl::Init();
}

ECode CPlainServerSocketImpl::constructor(
    /* [in] */ IFileDescriptor * pFd)
{
    return PlainServerSocketImpl::Init(pFd);
}

} // namespace Net
} // namespace Elastos
