
#include "CDefaultSocketFactory.h"
#include "CSocket.h"

using Elastos::Net::CSocket;

namespace Elastosx {
namespace Net {

CAR_OBJECT_IMPL(CDefaultSocketFactory)

ECode CDefaultSocketFactory::constructor()
{
    return NOERROR;
}

ECode CDefaultSocketFactory::CreateSocket(
    /* [out] */ ISocket** sock)
{
    VALIDATE_NOT_NULL(sock)

    return  CSocket::New(sock);
}

ECode CDefaultSocketFactory::CreateSocket(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [out] */ ISocket** sock)
{
    VALIDATE_NOT_NULL(sock)

    return  CSocket::New(host, port, sock);
}

ECode CDefaultSocketFactory::CreateSocket(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ IInetAddress* localHost,
    /* [in] */ Int32 localPort,
    /* [out] */ ISocket** sock)
{
    VALIDATE_NOT_NULL(sock)

    return  CSocket::New(host, port, localHost, localPort, sock);
}

ECode CDefaultSocketFactory::CreateSocket(
    /* [in] */ IInetAddress* host,
    /* [in] */ Int32 port,
    /* [out] */ ISocket** sock)
{
    VALIDATE_NOT_NULL(sock)

    return  CSocket::New(host, port, sock);
}

ECode CDefaultSocketFactory::CreateSocket(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port,
    /* [in] */ IInetAddress* localAddress,
    /* [in] */ Int32 localPort,
    /* [out] */ ISocket** sock)
{
    VALIDATE_NOT_NULL(sock)

    return  CSocket::New(address, port, localAddress, localPort, sock);
}

} // namespace Net
} // namespace Elastosx

