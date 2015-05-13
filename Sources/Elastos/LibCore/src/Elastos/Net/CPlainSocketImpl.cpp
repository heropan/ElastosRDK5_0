
#include "CPlainSocketImpl.h"
#include <stdio.h>

namespace Elastos {
namespace Net {

ECode CPlainSocketImpl::constructor()
{
    return PlainSocketImpl::Init();
}

ECode CPlainSocketImpl::constructor(
    /* [in] */ IFileDescriptor * pFd)
{
    return PlainSocketImpl::Init(pFd);
}

ECode CPlainSocketImpl::constructor(
    /* [in] */ IProxy * pProxy)
{
    return PlainSocketImpl::Init(pProxy);
}

ECode CPlainSocketImpl::constructor(
    /* [in] */ IFileDescriptor * pFd,
    /* [in] */ Int32 localport,
    /* [in] */ IInetAddress * pAddr,
    /* [in] */ Int32 port)
{
    return PlainSocketImpl::Init(pFd, localport, pAddr, port);
}

PInterface CPlainSocketImpl::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_PlainSocketImpl) {
        return reinterpret_cast<PInterface>((PlainSocketImpl*)this);
    }
    else if (riid == EIID_SocketImpl) {
        return reinterpret_cast<PInterface>((SocketImpl*)this);
    }

    return _CPlainSocketImpl::Probe(riid);
}

ECode CPlainSocketImpl::InitLocalPort(
    /* [in] */ Int32 localPort)
{
    return PlainSocketImpl::InitLocalPort(localPort);
}

ECode CPlainSocketImpl::InitRemoteAddressAndPort(
    /* [in] */ IInetAddress* remoteAddress,
    /* [in] */ Int32 remotePort)
{
    return PlainSocketImpl::InitRemoteAddressAndPort(remoteAddress, remotePort);
}

ECode CPlainSocketImpl::SocksAccept()
{
    return PlainSocketImpl::SocksAccept();
}

ECode CPlainSocketImpl::GetOption(
    /* [in] */ Int32 optID,
    /* [out] */ IInterface ** ppOptVal)
{
    return PlainSocketImpl::GetOption(optID, ppOptVal);
}

ECode CPlainSocketImpl::SetOption(
    /* [in] */ Int32 optID,
    /* [in] */ IInterface * pOptVal)
{
    return PlainSocketImpl::SetOption(optID, pOptVal);
}

ECode CPlainSocketImpl::Accept(
    /* [in] */ ISocketImpl * pNewSocket)
{
    return PlainSocketImpl::Accept(pNewSocket);
}

ECode CPlainSocketImpl::Available(
    /* [out] */ Int32 * pNum)
{
    return PlainSocketImpl::Available(pNum);
}

ECode CPlainSocketImpl::Bind(
    /* [in] */ IInetAddress * pAddress,
    /* [in] */ Int32 port)
{
    return PlainSocketImpl::Bind(pAddress, port);
}

ECode CPlainSocketImpl::Close()
{
    return PlainSocketImpl::Close();
}

ECode CPlainSocketImpl::Connect(
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    return PlainSocketImpl::Connect(host, port);
}

ECode CPlainSocketImpl::ConnectEx(
    /* [in] */ IInetAddress * pAddress,
    /* [in] */ Int32 port)
{
    return PlainSocketImpl::Connect(pAddress, port);
}

ECode CPlainSocketImpl::Create(
    /* [in] */ Boolean isStreaming)
{
    return PlainSocketImpl::Create(isStreaming);
}

ECode CPlainSocketImpl::GetFileDescriptor(
    /* [out] */ IFileDescriptor ** ppFileDescriptor)
{
    return PlainSocketImpl::GetFileDescriptor(ppFileDescriptor);
}

ECode CPlainSocketImpl::GetInetAddress(
    /* [out] */ IInetAddress ** ppNetAddress)
{
    return PlainSocketImpl::GetInetAddress(ppNetAddress);
}

ECode CPlainSocketImpl::GetInputStream(
    /* [out] */ IInputStream ** ppInputStream)
{
    return PlainSocketImpl::GetInputStream(ppInputStream);
}

ECode CPlainSocketImpl::GetLocalPort(
    /* [out] */ Int32 * pNum)
{
    return PlainSocketImpl::GetLocalPort(pNum);
}

ECode CPlainSocketImpl::GetOutputStream(
    /* [out] */ IOutputStream ** ppOutputStream)
{
    return PlainSocketImpl::GetOutputStream(ppOutputStream);
}

ECode CPlainSocketImpl::GetPort(
    /* [out] */ Int32 * pPort)
{
    return PlainSocketImpl::GetPort(pPort);
}

ECode CPlainSocketImpl::Listen(
    /* [in] */ Int32 backlog)
{
    return PlainSocketImpl::Listen(backlog);
}

ECode CPlainSocketImpl::ShutdownInput()
{
    return PlainSocketImpl::ShutdownInput();
}

ECode CPlainSocketImpl::ShutdownOutput()
{
    return PlainSocketImpl::ShutdownOutput();
}

ECode CPlainSocketImpl::ConnectEx2(
    /* [in] */ ISocketAddress * pRemoteAddr,
    /* [in] */ Int32 timeout)
{
    return PlainSocketImpl::Connect(pRemoteAddr, timeout);
}

ECode CPlainSocketImpl::SupportsUrgentData(
    /* [out] */ Boolean * pIsSupports)
{
    return PlainSocketImpl::SupportsUrgentData(pIsSupports);
}

ECode CPlainSocketImpl::SendUrgentData(
    /* [in] */ Int32 value)
{
    return PlainSocketImpl::SendUrgentData(value);
}

ECode CPlainSocketImpl::SetPerformancePreferences(
    /* [in] */ Int32 connectionTime,
    /* [in] */ Int32 latency,
    /* [in] */ Int32 bandwidth)
{
    return PlainSocketImpl::SetPerformancePreferences(connectionTime, latency, bandwidth);
}

} // namespace Net
} // namespace Elastos
